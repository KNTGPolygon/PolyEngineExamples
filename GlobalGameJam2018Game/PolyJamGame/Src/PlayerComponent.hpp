#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT PlayerComponent : public ComponentBase
	{
	public:
		PlayerComponent() = default;

		float TransmitterRepairSpeed = 2;
		float MaxHpPoints = 100;
		float ScorePoints = 0;
	};
}