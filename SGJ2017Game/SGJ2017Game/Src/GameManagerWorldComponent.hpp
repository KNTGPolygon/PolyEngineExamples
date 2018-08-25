#pragma once

#include <ECS/ComponentBase.hpp>
#include <Memory/SafePtr.hpp>
#include <Collections/Dynarray.hpp>
#include <UniqueID.hpp>
#include "Level.hpp"

namespace SGJ
{
	class GAME_DLLEXPORT GameManagerWorldComponent : public Poly::ComponentBase
	{
		RTTI_DECLARE_COMPONENT(GameManagerWorldComponent) { NO_RTTI_PROPERTY(); }
	public:
		Poly::SafePtr<Poly::Entity> Player;
		Poly::SafePtr<Poly::Entity> Camera;

		Poly::Dynarray<Poly::SafePtr<Poly::Entity>> LevelEntities;
		Poly::Dynarray<Poly::SafePtr<Poly::Entity>> OtherEntities;

		Poly::Dynarray<Poly::SafePtr<Poly::Entity>> SoundSampleEntities;

		Poly::Dynarray<Level*> Levels;
		size_t CurrentLevelID = 0;
		float MinLevelWidth, MaxLevelWidth;
		float MinLevelHeight, MaxLevelHeight;
	};
}