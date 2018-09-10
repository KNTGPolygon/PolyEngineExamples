#pragma once

#include "Common.hpp"
#include <UniqueID.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include "Memory/SafePtr.hpp"
#include <AnimKeys.hpp>

using namespace Poly;

class ModelCheckWorldComponent : public ComponentBase
{
public:
	RTTI_DECLARE_COMPONENT(ModelCheckWorldComponent) { NO_RTTI_PROPERTY(); }

	
};