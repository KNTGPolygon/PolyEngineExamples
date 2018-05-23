#include "ReceiverSystem.hpp"

#include "ReceiverComponent.hpp"
#include "TransmitterComponent.hpp"

void GGJGame::ReceiverSystem::Update(World* world)
{
	for (auto tuple : world->IterateComponents<ReceiverComponent>())
	{
		int transmitterCount = 0;
		int activeTransmitterCount = 0;

		for (auto tuple : world->IterateComponents<TransmitterComponent>())
		{
			if (std::get<TransmitterComponent*>(tuple)->Transmitting)
				++activeTransmitterCount;

			++transmitterCount;
		}

		ReceiverComponent* receiver = std::get<ReceiverComponent*>(tuple);
		receiver->ActiveTransmitters = activeTransmitterCount;
		receiver->SignalStrength = (float)activeTransmitterCount / (float)transmitterCount;
	}
}
