#pragma once

#include <ECS/ComponentBase.hpp>
#include <Math/Vector.hpp>

namespace SGJ
{
	class CameraMovementComponent : public Poly::ComponentBase
	{
		RTTI_DECLARE_COMPONENT(CameraMovementComponent) { NO_RTTI_PROPERTY(); }
	public:
		void SetTargetTranslation(Poly::Vector Value) { TargetTranslation = Value; }
		const Poly::Vector& GetTargetTranslation() const { return TargetTranslation; }

	private:
		Poly::Vector TargetTranslation;
	};
}