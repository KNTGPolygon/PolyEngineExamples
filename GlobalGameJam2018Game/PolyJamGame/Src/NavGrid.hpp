#pragma once

#include <functional>

#include <ECS/World.hpp>
#include <Collections/OrderedMap.hpp>
#include <Math/Vector2f.hpp>
#include <AI/PathfindingComponent.hpp>
#include <Collections/Dynarray.hpp>


namespace GGJGame
{
	/*class ENGINE_DLLEXPORT NavNode : public BaseObject<>
	{
	public:
		virtual Vector GetWorldPosition() const = 0;
	};

	class ENGINE_DLLEXPORT NavGraph : public BaseObject<>
	{
	public:
		virtual bool IsPositionValid(const Vector& pos, float radius = 0.f) const = 0;
		virtual bool IsLineValid(const Vector& start, const Vector& end, float radius = 0.f) const = 0;
		virtual const NavNode* GetNodeForPosition(const Vector& pos) const = 0;
		virtual Dynarray<const NavNode*> GetConnections() = 0;
	};*/

	struct NavGridCell : public Poly::NavNode
	{
		NavGridCell() = default;
		NavGridCell(bool occupied) : Occupied(occupied) {}
		bool Occupied = false;
	};

	class GAME_DLLEXPORT NavGrid : public Poly::NavGraph
	{
	public:
		NavGrid(const Poly::Vector2f& origin, const Poly::Vector2i& gridSize, float cellSize = 1.0f);
		NavGrid(const Poly::Vector2f& origin, const Poly::Vector2i& gridSize, const Poly::Dynarray<bool>& occpanceMap, float cellSize = 1.0f);

		// Interface methods
		float GetTravelCost(const Poly::NavNode* from, const Poly::NavNode* to) const override;
		float GetHeuristicCost(const Poly::NavNode* from, const Poly::NavNode* to) const override;
		Poly::Vector GetNodeWorldPosition(const Poly::NavNode* node) const override;
		const Poly::NavNode* GetNodeFromWorldPosition(const Poly::Vector& pos) const override;
		void GetConnections(const Poly::NavNode* node, Poly::Dynarray<const Poly::NavNode*>& connections) const override;
		bool CanConnectDirectly(const Poly::NavNode* n1, const Poly::NavNode* n2) const override;

		// Helper methods
		Poly::Vector2i GetNodeGridPosition(const Poly::NavNode* node) const;
		inline const NavGridCell* GetNodeFromGridPosition(const Poly::Vector2i& pos) const { return &Cells[GetArrayIdx(pos)]; }
		inline NavGridCell* GetNodeFromGridPosition(const Poly::Vector2i& pos) { return &Cells[GetArrayIdx(pos)]; }

		const Poly::Vector2i& GetGridSize() const { return GridSize; };
		const float GetCellSize() const { return CellSize; };
	private:
		// Private helpers
		Poly::Vector2f GetCellMiddlePos(const Poly::Vector2i& cell) const;
		Poly::Optional<Poly::Vector2i> GetIdxFromPos(const Poly::Vector2f& pos) const;
		inline size_t GetArrayIdx(const Poly::Vector2i& v) const { return GetArrayIdx(v.X, v.Y); }
		inline size_t GetArrayIdx(size_t x, size_t y) const { return y * GridSize.X + x; }
		bool IsPointInsideGrid(const Poly::Vector2f& point) const;

		Poly::Vector2f GetCellOrigin(const Poly::Vector2i& cell) const;

		void TryAddCandidate(const Poly::Vector2i& cell, Poly::Dynarray<const Poly::NavNode*>& connections) const;

		Poly::Vector2f GridOrigin;
		float CellSize = 1.0f;
		Poly::Vector2i GridSize;
		Poly::Dynarray<NavGridCell> Cells;
	};
}