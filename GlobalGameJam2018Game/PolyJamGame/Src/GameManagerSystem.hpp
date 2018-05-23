#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Utils/EnumUtils.hpp>
#include "TerrainComponent.hpp"
#include <Configs/ConfigBase.hpp>
#include <Rendering/MeshRenderingComponent.hpp>

enum class eLevels
{
	MOVEMENT_TEST = 0,
	RELEASE_LEVEL,
	_COUNT
};
namespace GGJGame
{
	namespace GameManagerSystem
	{
		void InitializeScene(Poly::World* world);
		void DenitializeScene(Poly::World* world);
		void Update(Poly::World* world);

		Poly::Entity* SpawnPlayer(Poly::World* world, const Poly::Vector& position);
		void DespawnEnemy(Poly::World* world);
		void DespawnPlayer(Poly::World* world);
		void Restart(Poly::World* world);

		Poly::Entity* SpawnLevel(Poly::World* world, eLevels level);
		void DespawnLevel(Poly::World* world);

		void PlaySample(Poly::World * world, const Poly::String& file, const Poly::Vector& position, float pitch = 1.0f, float gain = 1.0f, bool loop = false);

		void PrepareNonlevelObjects(Poly::World* world);
		void Cleanup(Poly::World* world);

		// levels spawn

		Poly::Entity* SpawnMovementTestLevel(Poly::World* world);
		Poly::Entity* SpawnReleaseLevel(Poly::World* world);

		// objects spawn

		Poly::Entity* SpawnPlaneGround(Poly::World* world, Poly::Entity* parent, 
			const String& meshPath = "Models/Ground_200x200.fbx", 
			eResourceSource meshResource = eResourceSource::GAME,
			PhongMaterial material = PhongMaterial(Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), 8.0f),
			const Vector& colliderHalfExtents = Vector(400, 400, 0),
			eTerrainPassability passability = eTerrainPassability::PASSABLE);

		Poly::Entity* SpawnLevelMesh(Poly::World* world, Poly::Entity* parent,
			const String& meshPath = "Models/Ground_200x200.fbx",
			eResourceSource meshResource = eResourceSource::GAME,
			PhongMaterial material = PhongMaterial(Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), 8.0f));

		Poly::Entity* SpawnCustomCollider(Poly::World* world, Poly::Entity* parent,
			const String& meshPath = "Models/Ground_200x200.fbx",
			eResourceSource meshResource = eResourceSource::GAME,
			eTerrainPassability passability = eTerrainPassability::PASSABLE);		

		//enemies spawn
		void SpawnEnemies(Poly::World* world, Poly::Entity* parent);
		template <typename T>
		Poly::Entity* SpawnEnemy(Poly::World* world, Poly::Entity* parent, const Poly::Vector& position);
		Poly::Entity* SpawnBeacon(Poly::World* world, Poly::Entity* parent, const Poly::Vector& position);


		void SpawnAmbientFX(World* world);
		void SpawnParticleAmbientSmall(World* world);
		void SpawnParticleAmbientBig(World* world);

		float Random();
		float Random(float min, float max);
		Vector RandomVector(float min, float max);
	}
}
