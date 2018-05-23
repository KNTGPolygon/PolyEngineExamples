#pragma once

#include "ECS/ComponentBase.hpp"
#include <Collections/Dynarray.hpp>

namespace BT
{
	class GAME_DLLEXPORT GameManagerWorldComponent : public Poly::ComponentBase
	{
	public:
		GameManagerWorldComponent() = default;

		Poly::UniqueID Camera;
		Poly::UniqueID DirectionalLight;

		Poly::UniqueID Ground;
		Poly::Dynarray<Poly::UniqueID> Bricks;
		Poly::Dynarray<Poly::UniqueID> Bullets;
	};
}