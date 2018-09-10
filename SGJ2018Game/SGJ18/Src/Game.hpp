#pragma once

#include <Common.hpp>
#include <Engine.hpp>
#include <ECS/Scene.hpp>
#include <Memory/SafePtr.hpp>

DECLARE_GAME()

class GAME_DLLEXPORT Game : public Poly::IGame
{
public:
	void RegisterEngine(Poly::Engine* /*engine*/) override {}
	void Init() override;
	void InitModelCheck();
	void InitDebugAnimation();
	void SpawnParticles(Poly::Scene* world, Poly::Entity* ship);
	void Deinit() override;

private:
};