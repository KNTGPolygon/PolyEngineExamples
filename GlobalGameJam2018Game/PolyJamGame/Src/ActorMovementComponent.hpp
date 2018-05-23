#pragma once

#include <ECS/ComponentBase.hpp>

class GAME_DLLEXPORT ActorMovementComponent : public Poly::ComponentBase 
{
public:
	float GetSpeed() const { return Speed; }
	float GetDrag() const { return Drag; }
	Poly::Vector GetMoveVector() const { return MoveVector; }

	void SetSpeed(float speed) { Speed = speed; }


private:
	float Speed;
	float Drag;
	Poly::Vector MoveVector;

};