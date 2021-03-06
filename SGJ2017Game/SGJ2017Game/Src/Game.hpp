#pragma once

#include <Engine.hpp>
#include <ECS/Scene.hpp>

DECLARE_GAME()

#pragma warning(disable: 4251)

namespace Poly { class SoundResource; }

class GAME_DLLEXPORT SGJGame : public Poly::IGame {
public:
	void RegisterEngine(Poly::Engine* /*engine*/) override { }

	void Init() override;
	void Deinit() override;

private:
	Poly::Dynarray<Poly::SoundResource*> SoundsPreload;
};
