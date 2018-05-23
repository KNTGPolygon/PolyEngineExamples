#pragma once

#include <functional>

#include <ECS/World.hpp>
#include <Collections/OrderedMap.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT EnemyAIBase : public BaseObject<>
	{
	public:
		using actionSignature = std::function<bool(World* world, Entity* selfEntity)>;

		EnemyAIBase() : actions(OrderedMap<int, actionSignature>())
		{ }

		void AddAction(int priority, actionSignature action)
		{
			actions.Insert(priority, action);
		}

		void IterateActions(World* world, Entity* selfEntity)
		{
			for(auto action : actions)
			{
				// if action has been executed
				if(action.value(world, selfEntity))
					break;
			}

			RecreateActionsScript();
		}

	private:
		virtual void InitActions() = 0;

		void RecreateActionsScript()
		{
			actions.Clear();

			InitActions();
		}
		OrderedMap<int, actionSignature> actions;
	};

	class GAME_DLLEXPORT EnemyAIEngineer : public EnemyAIBase
	{
	public:
		EnemyAIEngineer()
		{
			InitActions();
		}
	private:
		virtual void InitActions() override final;
	};

	class GAME_DLLEXPORT EnemyAIAssailant : public EnemyAIBase
	{
	public:
		EnemyAIAssailant()
		{
			InitActions();
		}
	private:
		virtual void InitActions() override final;
	};
}