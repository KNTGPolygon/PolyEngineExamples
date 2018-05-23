#include "ActorComponent.hpp"

void GGJGame::ActorComponent::SetMovement(const Vector& movement)
{
	Movement = movement;
}

void GGJGame::ActorComponent::SetJumpRequested(bool val)
{
	if (!InAir || !val)
		JumpRequested = val;

	if (val)
		InAir = true;
}
