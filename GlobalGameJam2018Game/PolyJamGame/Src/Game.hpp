#pragma once

#include <Core.hpp>
#include <Engine.hpp>
#include <ECS/World.hpp>
#include <Memory/SafePtr.hpp>

DECLARE_GAME()

class GAME_DLLEXPORT Game : public Poly::IGame {
public:
	void RegisterEngine(Poly::Engine* /*engine*/) override { }
	void Init() override;
	void Deinit() override;

private:
	Poly::SafePtr<Poly::Entity> Camera;
	Poly::Dynarray<Poly::SoundResource*> SoundsPreload;
	Poly::Dynarray<Poly::MeshResource*> MeshPreload;
};
