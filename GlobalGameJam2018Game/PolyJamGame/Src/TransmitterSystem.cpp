#include "TransmitterSystem.hpp"

#include <Time/TimeSystem.hpp>

#include "TransmitterComponent.hpp"
#include "PlayerComponent.hpp"
#include "EnemyComponent.hpp"
#include "TransmissionStatusWorldComponent.hpp"
#include "ActorComponent.hpp"

const float TIME_TO_TURN = 10.0f;
const float TRANSMISSION_TIME = 180.0f;
const float TRIGGER_RADIUS = 20.0f;
const float TRANSMISSION_MUL = 2.0f;

void GGJGame::TransmitterSystem::Update(World * world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, eEngineTimer::GAMEPLAY));
	TransmissionStatusWorldComponent* com = world->GetWorldComponent<TransmissionStatusWorldComponent>();
	if (!com) { return; }
	int currentTime = com->PercentData.TimeLeft;
	int transCount = 0;
	float toSubstract = 0;

	for (auto tuple : world->IterateComponents<TransmitterComponent>())
	{
		TransmitterComponent* transmitter = std::get<TransmitterComponent*>(tuple);
		EntityTransform& transmitterTransform = transmitter->GetTransform();

		for (auto tuple : world->IterateComponents<ActorComponent>())
		{
			ActorComponent* actor = std::get<ActorComponent*>(tuple);
			EnemyComponent* enemy = actor->GetSibling<EnemyComponent>();
			PlayerComponent* player = actor->GetSibling<PlayerComponent>(); 
			bool enemyNear = false;
			bool playerNear = false;
			if (enemy)
			{
				EntityTransform& enemyTransform = actor->GetTransform();
				if ((transmitterTransform.GetGlobalTranslation() - enemyTransform.GetGlobalTranslation()).Length()
					<= TRIGGER_RADIUS)
				{
					enemyNear = true;
				}
			}
			else if (player)
			{
				EntityTransform& playerTransform = actor->GetTransform();
				if ((transmitterTransform.GetGlobalTranslation() - playerTransform.GetGlobalTranslation()).Length()
					<= TRIGGER_RADIUS)
				{
					playerNear = true;
				}
			}

			if (enemyNear && !playerNear)
			{
				if (transmitter->Transmitting)
				{
					if ((transmitter->TurnTime - deltaTime) <= 0.0f)
					{
						transmitter->Transmitting = false;
						transmitter->TurnTime = 0;
					}
					transmitter->TurnTime -= deltaTime;
				}
			}
			else if (!enemyNear && playerNear)
			{
				if (!transmitter->Transmitting)
				{
					if ((transmitter->TurnTime + deltaTime) >= 1.0f)
					{
						transmitter->Transmitting = true;
						transmitter->TurnTime = TIME_TO_TURN;
					}
					transmitter->TurnTime += deltaTime;
				}
			}
			if (transmitter->Transmitting)
			{
				++transCount;
			}
		}
	}
	toSubstract = transCount * TRANSMISSION_MUL * deltaTime; //for now we do not punish player for allowing enemies to retake beacons
	
	currentTime = currentTime - toSubstract;
	if (currentTime <= 0.0f)
	{

	}

	if (!com->PercentData.TextEnt)
	{
		Entity* ent = DeferredTaskSystem::SpawnEntityImmediate(world);
		DeferredTaskSystem::AddComponentImmediate<ScreenSpaceTextComponent>(world, ent, Vector2i(100, 100), "Fonts/Raleway/Raleway-Regular.ttf", eResourceSource::ENGINE, 32);
		com->PercentData.TextEnt = ent;
	}
		float toDisplay = (currentTime / TRANSMISSION_TIME) * 100;
		com->PercentData.TimeLeft = currentTime;
		ScreenSpaceTextComponent* textCom = world->GetComponent<ScreenSpaceTextComponent>(com->PercentData.TextEnt.Get());
		StringBuilder scoreValue;
		scoreValue.Append("Transmission status: ");
		scoreValue.Append(toDisplay, 1);
		scoreValue.Append("%");
		textCom->SetText(scoreValue.GetString());
}



int GGJGame::TransmitterSystem::GetPercent(World* world)
{
	return world->GetWorldComponent<TransmissionStatusWorldComponent>()->PercentData.TimeLeft;
}

