#pragma once

#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Core.hpp>
#include <Engine.hpp>
#include <ECS/World.hpp>

using namespace Poly;

DECLARE_GAME()

class GAME_DLLEXPORT RenderingSandbox : public IGame {
public:
	void RegisterEngine(Engine* engine) override { Engine = engine; }
	
	void Init() override;
	void Deinit() override;

private:
	
	Engine* Engine;
};
