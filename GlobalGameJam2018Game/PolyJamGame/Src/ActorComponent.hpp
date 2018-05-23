#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT ActorComponent : public ComponentBase
	{
	public:
		ActorComponent() = default;

		void SetMovement(const Vector& movement);
		const Vector& GetMovement() const { return Movement; }
		
		void SetJumpRequested(bool val);
		bool IsJumpRequested() { return JumpRequested; }
		void ResetJumpState() { InAir = false; }
		
		float GetHitPoints() const { return HitPoints; }
		void SetHitPoints(float val) { HitPoints = val; }
		bool IsDead() const { return HitPoints <= 0; }

		bool CanShoot() const { return TimeSinceLastShot >= ShotCooldown ; }
		void ResetShootTime() { TimeSinceLastShot = 0.f; }
		void SetShotCooldown(float cooldown) { ShotCooldown = cooldown; }
		
		void IncrementTime(float dt) 
		{ 
			TimeSinceLastShot += dt; 
			if(Movement.LengthSquared() > 0) 
				ElapsedMovementTime += dt; 
		}
		float GetElapsedMoveTime() const { return ElapsedMovementTime; }
	private:
		Vector Movement = Vector(0.f, 0.f, 0.f);
		bool JumpRequested = false;
		bool InAir = false;
		float TimeSinceLastShot = 0.f;
		float ShotCooldown = 0.2f;
		float HitPoints = 100.f;
		float ElapsedMovementTime = 0.f;
	};
}
