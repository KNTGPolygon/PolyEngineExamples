#include "EnginePCH.hpp"

#include "NavGrid.hpp"

using namespace Poly;

GGJGame::NavGrid::NavGrid(const Vector2f& origin, const Vector2i& gridSize, float cellSize)
	: GridOrigin(origin),  CellSize(cellSize), GridSize(gridSize)
{
	Cells.Resize(gridSize.X * gridSize.Y);
}

GGJGame::NavGrid::NavGrid(const Vector2f& origin, const Vector2i& gridSize, const Dynarray<bool>& occpanceMap, float cellSize)
	: GridOrigin(origin), CellSize(cellSize), GridSize(gridSize)
{
	ASSERTE(gridSize.X * gridSize.Y == (int)occpanceMap.GetSize(), "Invalid occupance map size!");
	Cells.Reserve(occpanceMap.GetSize());
	for (bool v : occpanceMap)
		Cells.PushBack(NavGridCell{ v });
}

Poly::Vector2i GGJGame::NavGrid::GetNodeGridPosition(const Poly::NavNode* node) const
{
	ASSERTE(node, "Node is null!");
	ASSERTE(node >= Cells.GetData(), "Invalid node pointer");
	const size_t idx = static_cast<const NavGridCell*>(node) - Cells.GetData();
	ASSERTE(idx < Cells.GetSize(), "Invalid index");
	return Vector2i(idx % GridSize.X, idx / GridSize.X);
}

float GGJGame::NavGrid::GetTravelCost(const Poly::NavNode* from, const Poly::NavNode* to) const
{
	float len = GetHeuristicCost(from, to);
	ASSERTE(len <= sqrt(2), "Cannot travel between not adjacent cells");
	return len;
}

float GGJGame::NavGrid::GetHeuristicCost(const Poly::NavNode* from, const Poly::NavNode* to) const
{
	Vector2f a = GetNodeGridPosition(from).ToVector2f();
	Vector2f b = GetNodeGridPosition(to).ToVector2f();
	return (a - b).Length();
}

Poly::Vector GGJGame::NavGrid::GetNodeWorldPosition(const Poly::NavNode* node) const
{
	ASSERTE(node, "Node is null!");
	Vector2i pos = GetNodeGridPosition(node);
	Vector2f realPos = GetCellMiddlePos(pos);
	return Vector(realPos.X, 0.f, realPos.Y);
}

const Poly::NavNode* GGJGame::NavGrid::GetNodeFromWorldPosition(const Poly::Vector& pos) const
{
	Optional<Vector2i> idx = GetIdxFromPos(Vector2f(pos.X, pos.Z));
	if (idx.HasValue())
	{
		const NavGridCell* cell = static_cast<const NavGridCell*>(GetNodeFromGridPosition(idx.Value()));
		if (cell->Occupied)
			return nullptr;
		return cell;
	}
	return nullptr;
}

void GGJGame::NavGrid::GetConnections(const Poly::NavNode* node, Poly::Dynarray<const Poly::NavNode*>& connections) const
{
	ASSERTE(node, "Node is null!");
	ASSERTE(!static_cast<const NavGridCell*>(node)->Occupied, "Invalid node");
	connections.Reserve(connections.GetSize() + 8);
	const Vector2i cell = GetNodeGridPosition(node);

	TryAddCandidate(Vector2i(cell.X - 1, cell.Y - 1), connections);
	TryAddCandidate(Vector2i(cell.X - 1, cell.Y), connections);
	TryAddCandidate(Vector2i(cell.X, cell.Y - 1), connections);
	TryAddCandidate(Vector2i(cell.X + 1, cell.Y + 1), connections);
	TryAddCandidate(Vector2i(cell.X + 1, cell.Y), connections);
	TryAddCandidate(Vector2i(cell.X, cell.Y + 1), connections);
	TryAddCandidate(Vector2i(cell.X + 1, cell.Y - 1), connections);
	TryAddCandidate(Vector2i(cell.X - 1, cell.Y + 1), connections);
}

void GGJGame::NavGrid::TryAddCandidate(const Poly::Vector2i& candidate, Poly::Dynarray<const Poly::NavNode*>& connections) const
{
	if (candidate.X < 0
		|| candidate.X >= GridSize.X
		|| candidate.Y < 0
		|| candidate.Y >= GridSize.Y)
		return;

	const NavGridCell* cell = GetNodeFromGridPosition(candidate);
	if (!cell->Occupied)
		connections.PushBack(cell);
}

bool GGJGame::NavGrid::CanConnectDirectly(const NavNode * n1, const NavNode * n2) const
{
	//TODO implement this to enable path smoothing.
	return false;
}

Vector2f GGJGame::NavGrid::GetCellMiddlePos(const Vector2i& cell) const
{
	return GetCellOrigin(cell) + Vector2f(CellSize, CellSize) * 0.5f;
}

bool GGJGame::NavGrid::IsPointInsideGrid(const Vector2f& point) const
{
	if (point.X < GridOrigin.X || point.Y < GridOrigin.Y)
		return false;

	Vector2f gridEnd = GridOrigin + GridSize.ToVector2f() * CellSize;

	if (point.X >= gridEnd.X || point.Y >= gridEnd.Y)
		return false;

	return true;
}

Vector2f GGJGame::NavGrid::GetCellOrigin(const Vector2i& cell) const
{
	return Vector2f(GridOrigin + cell.ToVector2f() * CellSize);
}

Optional<Vector2i> GGJGame::NavGrid::GetIdxFromPos(const Vector2f& pos) const
{
	if (!IsPointInsideGrid(pos))
		return Optional<Vector2i>();

	Vector2i idx;
	idx.X = (pos.X - GridOrigin.X) / CellSize;
	idx.Y = (pos.Y - GridOrigin.Y) / CellSize;
	return idx;
}