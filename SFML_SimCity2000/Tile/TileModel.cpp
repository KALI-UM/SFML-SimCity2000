#include "pch.h"
#include "TileModel.h"
#include "TileResTable.h"
#include "TileView.h"

TileModel::TileModel(const sf::Vector2u& cellCnt, const sf::Vector2f& cellSize)
	:m_CellCount(cellCnt), m_CellSize(cellSize)
{
	m_TileInfos.resize(m_MaxDepth);
}

TileModel::~TileModel()
{
}

bool TileModel::Initialize()
{
	m_TileInfos[(int)TileDepth::Terrain] = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));
	rapidcsv::Document doc_d0("datatables/tileInfo_depth0.csv", rapidcsv::LabelParams(-1, -1));
	int row = doc_d0.GetRowCount();
	int col = doc_d0.GetColumnCount();
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileInfos[(int)TileDepth::Terrain][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.type = GetTypeToEnum(doc_d0.GetCell<std::string>(i * 2, j));
			currTileInfo.name = doc_d0.GetCell<std::string>(i * 2 + 1, j);
			currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(doc_d0.GetCell<std::string>(i * 2, j), currTileInfo.name);
		}
	}

	m_TileInfos[(int)TileDepth::OnGround] = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));
	rapidcsv::Document doc_d1("datatables/tileInfo_depth1.csv", rapidcsv::LabelParams(-1, -1));
	row = doc_d1.GetRowCount();
	col = doc_d1.GetColumnCount();
	for (int j = 0; j < row; j++)
	{
		for (int i = 0; i < col/2; i++)
		{
			auto& currTileInfo = m_TileInfos[(int)TileDepth::OnGround][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.type = GetTypeToEnum(doc_d1.GetCell<std::string>(i * 2, j));
			currTileInfo.name = doc_d1.GetCell<std::string>(i * 2 + 1, j);
			currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(doc_d1.GetCell<std::string>(i * 2, j), currTileInfo.name);
		}
	}

	return row && col;
}

void TileModel::Reset()
{
}

void TileModel::Update(float dt)
{
}

const TileInfo& TileModel::GetTileInfo(const TileDepth& depth, const CellIndex& tileIndex) const
{
	return m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];
}

bool TileModel::IsValidTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x > 0 && tileIndex.x < m_CellCount.x - 1 && tileIndex.y > 0 && tileIndex.y < m_CellCount.y - 1;
}

void TileModel::SetTiles(std::list<CellIndex>& tiles, TileType type)
{
	TileDepth depth = GetTileDepth(type);
	for (auto& currIndex : tiles)
	{
		auto& currTileInfo = m_TileInfos[(int)depth][currIndex.y][currIndex.x];
		currTileInfo.type = type;
	}

	std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>(m_CellCount.y, std::vector<bool>(m_CellCount.x, false));

	for (auto& currIndex : tiles)
	{
		if (visited[currIndex.y][currIndex.x])continue;
		visited[currIndex.y][currIndex.x] = true;

		auto& currTileInfo = m_TileInfos[(int)depth][currIndex.y][currIndex.x];
		int connection = 0;
		if (currIndex == *tiles.begin() || currIndex == *(--tiles.end()))
		{
			connection = GetConnectVaule(depth, currIndex, type);
			//시작지점이거나 끝지점이면 한면만 이어져 있더라도 두면이 이어져 있는 타일로
			if (connection == 1 || connection == 2)
			{
				connection = 3;
				currTileInfo.name = "road_" + GetConnectedTileName(connection);
				currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(GetTypeToString(currTileInfo.type), currTileInfo.name);
				RequestUpdateTile(depth, currIndex);
				continue;
			}
			else if (connection == 4 || connection == 8)
			{
				connection = 12;
				currTileInfo.name = "road_" + GetConnectedTileName(connection);
				currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(GetTypeToString(currTileInfo.type), currTileInfo.name);
				RequestUpdateTile(depth, currIndex);
				continue;
			}
		}

		connection = 0;
		if (currTileInfo.type == m_TileInfos[(int)depth][currIndex.y][currIndex.x - 1].type)
		{
			connection |= 1 << 0;
			if (!visited[currIndex.y][currIndex.x - 1])
			{
				SetTile({ currIndex.x - 1, currIndex.y }, type);
				visited[currIndex.y][currIndex.x - 1] = true;
			}
		}
		if (currTileInfo.type == m_TileInfos[(int)depth][currIndex.y][currIndex.x + 1].type)
		{
			connection |= 1 << 1;
			if (!visited[currIndex.y][currIndex.x + 1])
			{
				SetTile({ currIndex.x + 1, currIndex.y }, type);
				visited[currIndex.y][currIndex.x + 1] = true;
			}
		}
		if (currTileInfo.type == m_TileInfos[(int)depth][currIndex.y - 1][currIndex.x].type)
		{
			connection |= 1 << 2;
			if (!visited[currIndex.y - 1][currIndex.x])
			{
				SetTile({ currIndex.x, currIndex.y - 1 }, type);
				visited[currIndex.y - 1][currIndex.x] = true;
			}
		}
		if (currTileInfo.type == m_TileInfos[(int)depth][currIndex.y + 1][currIndex.x].type)
		{
			connection |= 1 << 3;
			if (!visited[currIndex.y + 1][currIndex.x])
			{
				SetTile({ currIndex.x, currIndex.y + 1 }, type);
				visited[currIndex.y + 1][currIndex.x] = true;
			}
		}

		currTileInfo.name = "road_" + GetConnectedTileName(connection);
		currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(GetTypeToString(currTileInfo.type), currTileInfo.name);
		RequestUpdateTile(depth, currIndex);
	}
}

void TileModel::SetTile(const CellIndex& tileIndex, TileType type)
{
	TileDepth depth = GetTileDepth(type);
	auto& currTileInfo = m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];
	currTileInfo.type = type;
	currTileInfo.name = "road_" + GetConnectedTileName(GetConnectVaule(depth, tileIndex, type));
	currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(GetTypeToString(currTileInfo.type), currTileInfo.name);
	RequestUpdateTile(depth, tileIndex);
}

TileDepth TileModel::GetTileDepth(const TileType& type)
{
	switch (type)
	{
	case TileType::Terrain:
	case TileType::Zone:
		return TileDepth::Terrain;
		break;
	case TileType::Building:
	case TileType::Road:
	case TileType::Rail:
	case TileType::Highway:
	case TileType::Other:
		return TileDepth::OnGround;
		break;
	case TileType::Underground:
		return TileDepth::UnderGround;
		break;
	}
	return TileDepth::Terrain;
}

bool TileModel::IsPossibleToBuild(const CellIndex& tileIndex, TileType type)
{
	//수정보완 필
	return m_TileInfos[(int)TileDepth::Terrain][tileIndex.y][tileIndex.x].type == TileType::Terrain || m_TileInfos[(int)GetTileDepth(type)][tileIndex.y][tileIndex.x].type == type;
}

std::string TileModel::GetTypeToString(TileType type) const
{
	switch (type)
	{
	case TileType::Terrain:
		return "terrain";
		break;
	case TileType::Building:
		return "building";
		break;
	case TileType::Zone:
		return "zone";
		break;
	case TileType::Road:
		return "road";
		break;
	case TileType::Rail:
		return "rail";
		break;
	case TileType::Highway:
		return "highway";
		break;
	case TileType::Underground:
		return "underground";
		break;
	case TileType::Other:
		return "other";
		break;
	}
}

TileType TileModel::GetTypeToEnum(const std::string& type) const
{
	if (type == "")
		return TileType::None;
	if (type == "terrain")
		return TileType::Terrain;
	if (type == "building")
		return TileType::Building;
	if (type == "zone")
		return TileType::Zone;
	if (type == "road")
		return TileType::Road;
	if (type == "rail")
		return TileType::Rail;
	if (type == "highway")
		return TileType::Highway;
	if (type == "underground")
		return TileType::Underground;
	if (type == "other")
		return TileType::Other;

	return TileType::None;
}

int TileModel::GetConnectVaule(const TileDepth& depth, const CellIndex& tileIndex, TileType type)
{
	auto& currTileInfo = m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];

	int connection = 0;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y][tileIndex.x - 1].type)
		connection |= 1 << 0;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y][tileIndex.x + 1].type)
		connection |= 1 << 1;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y - 1][tileIndex.x].type)
		connection |= 1 << 2;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y + 1][tileIndex.x].type)
		connection |= 1 << 3;

	return connection;
}

std::string TileModel::GetConnectedTileName(int connection)
{
	if (connection == 3)return "1";
	if (connection == 12)return "2";
	if (connection == 72)return "3";
	if (connection == 33)return "4";
	if (connection == 132)return "5";
	if (connection == 18)return "6";
	if (connection == 10)return "7";
	if (connection == 9)return "8";
	if (connection == 5)return "9";
	if (connection == 6)return "10";
	if (connection == 14)return "11";
	if (connection == 11)return "12";
	if (connection == 13)return "13";
	if (connection == 7)return "14";
	if (connection == 15)return "15";

	if (connection == 1 || connection == 2)return "1";
	if (connection == 4 || connection == 8)return "2";
	return "";
}

void TileModel::RequestUpdateTile(const TileDepth& depth, const CellIndex& tileIndex)
{
	if (m_WhenNeedsToUpdateTileFunc)
		m_WhenNeedsToUpdateTileFunc(depth, tileIndex);
}
