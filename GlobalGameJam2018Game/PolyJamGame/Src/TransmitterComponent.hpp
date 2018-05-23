#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT TransmitterComponent : public ComponentBase
	{
	public:
		TransmitterComponent() = default;

		bool Transmitting = false;
		float TurnTime = 0.0f;
	};
}