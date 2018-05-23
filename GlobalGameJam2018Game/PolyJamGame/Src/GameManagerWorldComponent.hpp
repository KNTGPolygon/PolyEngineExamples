#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <AI/PathfindingComponent.hpp>

using namespace Poly;

class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase {
public:
	SafePtr<Entity> Player;
	Entity* Level;
		// TODO: make this safe ptr
	MeshResource* BulletMesh;
	MeshResource* EnemyMesh;

	Poly::Dynarray<Poly::SafePtr<Poly::Entity>> SoundSampleEntities;

	bool IsWon = false;
};