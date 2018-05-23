#include "PlayerSystem.hpp"

#include <Time/TimeSystem.hpp>
#include <Input/InputWorldComponent.hpp>
#include <Physics3D/Rigidbody3DComponent.hpp>
#include <Audio/SoundEmitterComponent.hpp>

#include "PlayerComponent.hpp"
#include "ReceiverComponent.hpp"
#include "ActorComponent.hpp"
#include "GameManagerWorldComponent.hpp"
#include "GunComponent.hpp"
#include "Rendering/Particles/ParticleComponent.hpp"
#include "Rendering/Particles/ParticleEmitter.hpp"
#include "GameManagerSystem.hpp"

#include "ActorSystem.hpp"

const float MOUSE_SENSITIVITY = 0.001f;

Entity* GGJGame::PlayerSystem::GetPlayerCamera(Entity* player)
{
	PlayerComponent* playerCmp = player->GetComponent<PlayerComponent>();
	for (Entity* e : playerCmp->GetOwner()->GetChildren())
		if (e->GetComponent<CameraComponent>())
			return e;
	return nullptr;
}

Entity * GGJGame::PlayerSystem::GetPlayerGun(Entity * player)
{
	PlayerComponent* playerCmp = player->GetComponent<PlayerComponent>();
	Entity* camera = GetPlayerCamera(playerCmp->GetOwner());
	if (!camera)
		return nullptr;

	for (Entity* e : camera->GetChildren())
		if (e->GetComponent<GunComponent>())
			return e;
	return nullptr;
}

Entity * GGJGame::PlayerSystem::GetPlayerSoundEmmiter(Entity * player)
{
	PlayerComponent* playerCmp = player->GetComponent<PlayerComponent>();
	for (Entity* e : playerCmp->GetOwner()->GetChildren())
		if (e->GetComponent<Poly::SoundEmitterComponent>())
			return e;

	return nullptr;
}

void GGJGame::PlayerSystem::Update(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, eEngineTimer::GAMEPLAY));
	//float time = (float)(TimeSystem::GetTimerElapsedTime(world, eEngineTimer::GAMEPLAY));
	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	//GameManagerWorldComponent* gmComp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (auto tuple : world->IterateComponents<PlayerComponent, ReceiverComponent>())
	{
		PlayerComponent* playerCmp = std::get<PlayerComponent*>(tuple);
		ReceiverComponent* receiverCmp = std::get<ReceiverComponent*>(tuple);

		playerCmp->ScorePoints += receiverCmp->SignalStrength * deltaTime;
	}

	// Player movement update
	for (auto tuple : world->IterateComponents<PlayerComponent, ActorComponent>())
	{
		PlayerComponent* playerCmp = std::get<PlayerComponent*>(tuple);
		ActorComponent* actorCmp = std::get<ActorComponent*>(tuple);
		Entity* player = playerCmp->GetOwner();
		Entity* camera = GetPlayerCamera(player);
		EntityTransform& playerTransform = player->GetTransform();
		EntityTransform& cameraTransform = camera->GetTransform();

		// Rotate
		Vector2i delta = inputCmp->GetMousePosDelta();
		Quaternion cameraRot = cameraTransform.GetLocalRotation() * Quaternion(Vector::UNIT_X, Angle::FromRadians(-delta.Y * MOUSE_SENSITIVITY));
		Quaternion bodyRot = Quaternion(Vector::UNIT_Y, Angle::FromRadians(-delta.X * MOUSE_SENSITIVITY)) * playerTransform.GetLocalRotation();

		auto clampingRot = cameraRot.ToEulerAngles();
		clampingRot.X = Clamp(clampingRot.X, -90_deg, 90_deg);
		cameraTransform.SetLocalRotation(clampingRot);
		playerTransform.SetLocalRotation(bodyRot.Normalize());

		// Check if dead
		Entity* gun = GetPlayerGun(player);
		if (actorCmp->IsDead() || !gun)
		{
			if (gun)
			{
				GameManagerSystem::PlaySample(world, "Audio/bomb.ogg", playerTransform.GetGlobalTranslation(), 1.0f, 1.0f, true);
				DeferredTaskSystem::DestroyEntity(world, gun);
			}
				
			continue;
		}

		EntityTransform& gunTransform = gun->GetTransform();

		// Shoot
		if (inputCmp->IsPressed(eMouseButton::LEFT))
		{
			Vector cameraGlobFwd = MovementSystem::GetGlobalForward(cameraTransform);
			ActorSystem::Shoot(world, player, cameraTransform.GetLocalTranslation(), cameraGlobFwd);

			SpawnMuzzleFlash(world, gunTransform, cameraGlobFwd);
		}

		// Jump
		if (inputCmp->IsClicked(eKey::SPACE))
		{
			ActorSystem::Jump(player);
		}

		// Walk
		Vector cameraLocFwd = MovementSystem::GetGlobalForward(cameraTransform);
		Vector cameraLocRight = MovementSystem::GetGlobalRight(cameraTransform);

		Vector moveDirection;
		if (inputCmp->IsPressed(eKey::KEY_W))
			moveDirection += cameraLocFwd;
		else if (inputCmp->IsPressed(eKey::KEY_S))
			moveDirection -= cameraLocFwd;
		
		if (inputCmp->IsPressed(eKey::KEY_A))
			moveDirection -= cameraLocRight;
		else if (inputCmp->IsPressed(eKey::KEY_D))
			moveDirection += cameraLocRight;

		Entity* soundemiter = GetPlayerSoundEmmiter(player);
		if (soundemiter)
		{
			if (moveDirection.LengthSquared() > 0 && !SoundSystem::IsEmmiterActive(world, soundemiter))
				SoundSystem::PlayEmitter(world, soundemiter);

			if (moveDirection.LengthSquared() == 0 && SoundSystem::IsEmmiterActive(world, soundemiter))
				SoundSystem::StopEmitter(world, soundemiter);

			if (moveDirection.LengthSquared() > 0)
			{
				float moveTime = actorCmp->GetElapsedMoveTime();

				const float shift1Speed = 10.0f;
				const float shift2Speed = 15.0f;
				const float aFactor = 0.1f;
				const float bFactor = 0.1f;
				const float cFactor = 0.005f;


				float shift1 = sin(shift1Speed * (moveTime + deltaTime)) - sin(shift1Speed * moveTime);
				float shift2 = sin(shift2Speed * (moveTime + deltaTime) + M_PI / 2) - sin(shift2Speed * moveTime + M_PI / 2);

				shift1 *= aFactor;
				shift2 *= cFactor;

				cameraTransform.SetLocalTranslation(cameraTransform.GetLocalTranslation() + Vector(0, shift1, 0));
				gunTransform.SetLocalTranslation(gunTransform.GetLocalTranslation() + Vector(shift1 * bFactor, shift2, 0));
			}
			
		}
		

		ActorSystem::Move(player, moveDirection);
	}
}

void GGJGame::PlayerSystem::SpawnMuzzleFlash(Poly::World * world, Poly::EntityTransform & gunTransform, const Poly::Vector &cameraGlobFwd)
{
	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	// EntityTransform& ParticlesEnt1Trans = ParticlesEnt->GetTransform();
	// ParticlesEnt1Trans.SetLocalTranslation(Vector(-20.0f, 0.0f, 0.0f));
	ParticleEmitter::Settings settings;
	settings.InitialSize = 1;
	settings.Speed = 10.0f;
	settings.BurstSizeMin = 0;
	settings.BurstSizeMax = 0;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BaseColor = Color(2.0f, 1.0f, 0.5f, 0.1f);
	settings.ParticleInitFunc = [&](ParticleEmitter::Particle* p) {
		// p->Position = Vector(0.0f, 0.0f, 0.0f) + GameManagerSystem::RandomVector(0.2f, 0.2f);
		p->Position = gunTransform.GetGlobalTranslation() + cameraGlobFwd * 2.0f + Vector(0.0f, -0.2f, 0.0f);
		// ector accel = GameManagerSystem::RandomVector(0.1f, 1.0f);
		p->Acceleration = Vector(0.01f, 0.0f, 0.0f); // Vector(0.05f * accel.X, 1.0f * accel.Y, 0.05f * accel.Z);
		p->LifeTime = 0.1f; // Random(0.25f, 0.5f);
		p->Scale = Vector(1.0f, 1.0f, 1.0f) * 0.6f;
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Position += p->Acceleration;
		// float life = Lerp(p->Age / p->LifeTime, 0.1f, 1.0f);
		// p->Scale = Vector(1.0f, 1.0f, 1.0f) * (1.0f - life * life);
	};

    settings.SprsheetSettings.SpritePath = "Textures/muzzleflash_512.png";
    settings.SprsheetSettings.Source = eResourceSource::GAME;
	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	//ParticleComponent* ParticleComp = world->GetComponent<ParticleComponent>(ParticlesEnt);
}
