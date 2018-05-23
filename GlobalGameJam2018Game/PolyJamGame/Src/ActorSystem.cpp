#include "ActorSystem.hpp"

#include <Time/TimeSystem.hpp>
#include <Physics3D/Rigidbody3DComponent.hpp>

#include "ActorComponent.hpp"
#include "GameManagerSystem.hpp"
#include "GameManagerWorldComponent.hpp"

static const float PROJECTILE_SPAWN_DISTANCE = 0.1f;
static const float MOVEMENT_IMPULSE_SCALE = 0.6f;
static const float JUMP_IMPULSE_SCALE = 20.0f;

void GGJGame::ActorSystem::Update(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, eEngineTimer::GAMEPLAY));
	//InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
		
	for (auto tuple : world->IterateComponents<ActorComponent, Rigidbody3DComponent>())
	{
		ActorComponent* actorCmp = std::get<ActorComponent*>(tuple);
		Rigidbody3DComponent* rigidbodyCmp = std::get<Rigidbody3DComponent*>(tuple);
		//EntityTransform& transform = actorCmp->GetTransform();
		const float actorMass = actorCmp->GetSibling<Rigidbody3DComponent>()->GetMass();

		rigidbodyCmp->ApplyImpulseToCenter(actorCmp->GetMovement() * actorMass * MOVEMENT_IMPULSE_SCALE);

		actorCmp->IncrementTime(deltaTime);

		if (actorCmp->IsJumpRequested())
		{
			actorCmp->SetJumpRequested(false);
			rigidbodyCmp->ApplyImpulseToCenter(Vector::UNIT_Y * actorMass * JUMP_IMPULSE_SCALE);
		}
	}
}

void GGJGame::ActorSystem::Move(Poly::Entity* actor, Vector direction, float speedMult)
{
	direction.Y = 0.f;
	if (direction.LengthSquared() > 0)
	{
		direction.Normalize();
	}
		
	actor->GetComponent<ActorComponent>()->SetMovement(direction * speedMult);
}

void GGJGame::ActorSystem::GiveDamage(Poly::Entity* actor, float damage)
{
	ActorComponent* actorCmp = actor->GetComponent<ActorComponent>();
	actorCmp->SetHitPoints(actorCmp->GetHitPoints() - damage);
}

void GGJGame::ActorSystem::Jump(Poly::Entity* actor)
{
	actor->GetComponent<ActorComponent>()->SetJumpRequested(true);
}

void GGJGame::ActorSystem::Shoot(Poly::World* world, Poly::Entity* shooter, const Vector& localOffset, const Vector& direction)
{
	Vector position = shooter->GetTransform().GetGlobalTranslation() + localOffset + direction * PROJECTILE_SPAWN_DISTANCE;
	//GameManagerWorldComponent* gmComp = world->GetWorldComponent<GameManagerWorldComponent>();
	ActorComponent* actorCmp = shooter->GetComponent<ActorComponent>();
	DebugDrawSystem::DrawSphere(world, position + direction*5, 0.05f);
	if (actorCmp->CanShoot())
	{
		actorCmp->ResetShootTime();
		GameManagerSystem::PlaySample(world, "Audio/gunfire.ogg", position, 1.0f, 1.0f);
		auto rayCastRes = Physics3DSystem::ClosestHitRaycast(world, position, position + direction * 1000, eCollisionGroup::RIGIDBODY_GREEN, eCollisionGroup::RIGIDBODY_GREEN);
		
		for (auto hit : rayCastRes.Hits)
		{
			Entity* ent = hit.HitEntity;
			if (ActorComponent* actorCmp = ent->GetComponent<ActorComponent>())
			{
				actorCmp->SetHitPoints(actorCmp->GetHitPoints() - 50);
				gConsole.LogDebug("Collision! HP: {}", actorCmp->GetHitPoints());
				break;
			}
		}
	}
}

