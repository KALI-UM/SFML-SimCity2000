#include "pch.h"
#include "TileModel.h"
#include "TileResTable.h"
#include "TileAttTable.h"
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
	InitializeTerrainDepth();
	InitializeOnGroundDepth();
	InitilaizeEffectDepth();

	return true;
}

void TileModel::Reset()
{
}

void TileModel::Update(float dt)
{
	TileInfo& info = m_TileInfos[(int)TileDepth::OnGround][2][2];
	i = info.connection;
}

const TileInfo& TileModel::GetTileInfo(const TileDepth& depth, const CellIndex& tileIndex) const
{
	return m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];
}

TileShapeType TileModel::GetTileShapeType(const TileDepth& depth, const CellIndex& tileIndex) const
{
	return (depth == TileDepth::Terrain ? TileShapeType::Diamond : TileShapeType::Convex);
}

bool TileModel::IsValidTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x > 0 && tileIndex.x < (int)m_CellCount.x - 1 && tileIndex.y > 0 && tileIndex.y < (int)m_CellCount.y - 1;
}

void TileModel::InitializeTerrainDepth()
{
	m_TileInfos[(int)TileDepth::Terrain] = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileInfos[(int)TileDepth::Terrain][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.type = TileType::Terrain;
			currTileInfo.subtype = "terrain";
			currTileInfo.name = "land";
			currTileInfo.ower = currTileInfo.index;
		}
	}
}

void TileModel::InitializeOnGroundDepth()
{
	m_TileInfos[(int)TileDepth::OnGround] = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileInfos[(int)TileDepth::OnGround][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.type = TileType::None;
			currTileInfo.ower = currTileInfo.index;
		}
	}
}

void TileModel::InitilaizeEffectDepth()
{
	m_TileInfos[(int)TileDepth::Effect] = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileInfos[(int)TileDepth::Effect][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.type = TileType::None;
			currTileInfo.ower = currTileInfo.index;
		}
	}
}

void TileModel::SetTiles(std::list<CellIndex>& tiles, TileType type, const std::string& subtype, const std::string& name, bool isNot1x1)
{
	TileDepth depth = (TileDepth)DATATABLE_TILEATT->GetTileAttribute(type).depth;
	for (auto& currIndex : tiles)
	{
		auto& currTileInfo = m_TileInfos[(int)depth][currIndex.y][currIndex.x];
		currTileInfo.prevtype = currTileInfo.type;
		currTileInfo.type = type;
	}

	if (DATATABLE_TILEATT->GetTileAttribute(type).connectable)
	{
		std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>(m_CellCount.y, std::vector<bool>(m_CellCount.x, false));
		for (auto& currIndex : tiles)
		{
			if (visited[currIndex.y][currIndex.x])
				continue;

			//bfs
			std::queue<CellIndex> updateQue;
			updateQue.push(tiles.front());

			while (!updateQue.empty())
			{
				CellIndex currIndex_currIndex = updateQue.front();
				updateQue.pop();

				if (visited[currIndex_currIndex.y][currIndex_currIndex.x])
					continue;
				visited[currIndex_currIndex.y][currIndex_currIndex.x] = true;
				int prevConnection = m_TileInfos[(int)depth][currIndex_currIndex.y][currIndex_currIndex.x].connection;
				SetConnection(currIndex_currIndex, depth, type);
				if (prevConnection == m_TileInfos[(int)depth][currIndex_currIndex.y][currIndex_currIndex.x].connection)
					continue;

				SetTile(currIndex_currIndex, depth, type, subtype, name, true);
				auto& updateInfo = m_TileInfos[(int)depth][currIndex_currIndex.y][currIndex_currIndex.x];
				CellIndex d[4] = { {-1,0},{1,0},{0,-1},{0,1} };
				for (int i = 0; i < 4; i++)
				{
					CellIndex dIndex = currIndex_currIndex + d[i];
					auto& dInfo = m_TileInfos[(int)depth][dIndex.y][dIndex.x];
					if (type == dInfo.type && !visited[dIndex.y][dIndex.x])
					{
						updateQue.push(dIndex);
					}
				}
			}
		}
	}
	else
	{
		if (!isNot1x1)
		{
			for (auto& currIndex : tiles)
			{
				SetTile(currIndex, depth, type, subtype, name);
			}
		}
		else
		{
			auto truetile = CellIndex(m_CellCount.x, 0);
			for (auto& currIndex : tiles)
			{
				truetile.x = std::min(truetile.x, currIndex.x);
				truetile.y = std::max(truetile.y, currIndex.y);
			}
			for (auto& currIndex : tiles)
			{
				SetTile(currIndex, depth, type, subtype, name, false, currIndex == truetile);
				m_TileInfos[(int)depth][currIndex.y][currIndex.x].ower = truetile;
			}
		}
	}
}

void TileModel::SetTempEffectTiles(const CellIndex& tileIndex, TileType type, const std::string& subtype, const std::string& name)
{
	auto& currTileInfo = m_TileInfos[(int)TileDepth::Effect][tileIndex.y][tileIndex.x];
	currTileInfo.type = type;
	currTileInfo.subtype = subtype;
	currTileInfo.name = name;
	currTileInfo.lotSize = DATATABLE_TILERES->GetTileRes(currTileInfo.type, currTileInfo.subtype, currTileInfo.name).lotSize;
	currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(currTileInfo.type, currTileInfo.subtype, currTileInfo.name);
	RequestTempEffectTile(tileIndex);
}

void TileModel::SetTile(const CellIndex& tileIndex, const TileDepth& depth, TileType type, const std::string& subtype, const std::string& name, bool isConnectable, bool truetile)
{
	auto& currTileInfo = m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];

	if (currTileInfo.prevtype != TileType::None && currTileInfo.prevtype != type &&
		(DATATABLE_TILEATT->CanBeSub(currTileInfo.prevtype, type) || DATATABLE_TILEATT->CanBeSub(type, currTileInfo.prevtype)))
	{
		auto typesubtype = DATATABLE_TILEATT->GetTypeAndSubtype(currTileInfo.prevtype, type);
		currTileInfo.type = typesubtype.first;
		currTileInfo.subtype = typesubtype.second;
		currTileInfo.name = Tile::GetTypeToString(currTileInfo.type) + "_" + currTileInfo.subtype;
	}
	else if (currTileInfo.subtype == subtype && subtype == "rubble")
	{
		int rubble = std::atoi(&currTileInfo.name[currTileInfo.name.length() - 1]);
		rubble++;
		if (rubble == 5)
		{
			currTileInfo.type = TileType::None;
			currTileInfo.subtype = "";
			currTileInfo.name = "";
		}
		else
		{
			currTileInfo.name[currTileInfo.name.length() - 1] = '0' + rubble;
		}
	}
	else
	{
		currTileInfo.type = type;
		currTileInfo.subtype = subtype;
		currTileInfo.name = name;
	}

	if (DATATABLE_TILEATT->GetTileAttribute(currTileInfo.type).connectable)
	{
		if (currTileInfo.type != type)
		{
			SetConnection(tileIndex, depth, currTileInfo.type);
		}
		currTileInfo.name += GetConnectedTileName(currTileInfo.name, currTileInfo.connection);
	}
	else
	{
		currTileInfo.connection = -1;
	}

	const auto& tileres = DATATABLE_TILERES->GetTileRes(currTileInfo.type, currTileInfo.subtype, currTileInfo.name);
	currTileInfo.lotSize = ((name == "" || truetile) ? tileres.lotSize : sf::Vector2u(0, 0));
	currTileInfo.filepath = DATATABLE_TILERES->GetTileFilePath(currTileInfo.type, currTileInfo.subtype, currTileInfo.name);

	RequestUpdateTile(depth, tileIndex);
}

void TileModel::SetConnection(const CellIndex& tileIndex, const TileDepth& depth, TileType type)
{
	auto& currTileInfo = m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];
	currTileInfo.connection = 0;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y][tileIndex.x - 1].type
		|| m_TileInfos[(int)depth][tileIndex.y][tileIndex.x - 1].subtype == Tile::GetTypeToString(currTileInfo.type))
		currTileInfo.connection |= 1 << 0;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y][tileIndex.x + 1].type
		|| m_TileInfos[(int)depth][tileIndex.y][tileIndex.x + 1].subtype == Tile::GetTypeToString(currTileInfo.type))
		currTileInfo.connection |= 1 << 1;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y - 1][tileIndex.x].type
		|| m_TileInfos[(int)depth][tileIndex.y - 1][tileIndex.x].subtype == Tile::GetTypeToString(currTileInfo.type))
		currTileInfo.connection |= 1 << 2;
	if (currTileInfo.type == m_TileInfos[(int)depth][tileIndex.y + 1][tileIndex.x].type
		|| m_TileInfos[(int)depth][tileIndex.y + 1][tileIndex.x].subtype == Tile::GetTypeToString(currTileInfo.type))
		currTileInfo.connection |= 1 << 3;

	if (type == TileType::Powerline)
	{
		SetConnectionPowerline(tileIndex, depth);
	}
}

void TileModel::SetConnectionPowerline(const CellIndex& tileIndex, const TileDepth& depth)
{
	auto& currTileInfo = m_TileInfos[(int)depth][tileIndex.y][tileIndex.x];
	if (m_TileInfos[(int)depth][tileIndex.y][tileIndex.x - 1].subtype == "power_plant")
		currTileInfo.connection |= 1 << 0;
	if (m_TileInfos[(int)depth][tileIndex.y][tileIndex.x + 1].subtype == "power_plant")
		currTileInfo.connection |= 1 << 1;
	if (m_TileInfos[(int)depth][tileIndex.y - 1][tileIndex.x].subtype == "power_plant")
		currTileInfo.connection |= 1 << 2;
	if (m_TileInfos[(int)depth][tileIndex.y + 1][tileIndex.x].subtype == "power_plant")
		currTileInfo.connection |= 1 << 3;
}

std::string TileModel::GetConnectedTileName(std::string& name, int connection)
{
	if (name[name.length() - 2] == '_')
		name = name.substr(0, name.length() - 2);
	if (name[name.length() - 3] == '_')
		name = name.substr(0, name.length() - 3);

	if (connection == 3)return "_1";
	if (connection == 12)return "_2";
	if (connection == 72)return "_3";
	if (connection == 33)return "_4";
	if (connection == 132)return "_5";
	if (connection == 18)return "_6";
	if (connection == 10)return "_7";
	if (connection == 9)return "_8";
	if (connection == 5)return "_9";
	if (connection == 6)return "_10";
	if (connection == 14)return "_11";
	if (connection == 11)return "_12";
	if (connection == 13)return "_13";
	if (connection == 7)return "_14";
	if (connection == 15)return "_15";

	if (connection == 0 || connection == 1 || connection == 2)return "_1";
	if (connection == 4 || connection == 8)return "_2";
	return "";
}

bool TileModel::IsPossibleToBuild(const CellIndex& tileIndex, const TileType& type, const SUBTYPE& subtype)
{
	auto& currTileAtt = DATATABLE_TILEATT->GetTileAttribute(type);
	auto& originTileInfo = m_TileInfos[currTileAtt.depth][tileIndex.y][tileIndex.x];

	//도로, 레일, 전선이고 같은 타일이면 가능
	bool isSame = (originTileInfo.type == TileType::Road || originTileInfo.type == TileType::Rail || originTileInfo.type == TileType::Powerline)
		&& ((type == TileType::Road || type == TileType::Rail || type == TileType::Powerline));
	if (isSame)
	{
		////원래 타일이 overlay가능한 커넥션 값이고
		////원래 타일과 현재 타일이 오버레이 가능(canBeSub)하면 가능
		//bool isOverlayConnection = originTileInfo.connection == 0 || originTileInfo.connection == 1 || originTileInfo.connection == 2 || originTileInfo.connection == 3 || originTileInfo.connection == 4 || originTileInfo.connection == 8 || originTileInfo.connection == 12;
		//if (isOverlayConnection)
		//	return DATATABLE_TILEATT->CanBeSub(originTileInfo.type, type) || DATATABLE_TILEATT->CanBeSub(type, originTileInfo.type);
		return true;
	}

	//원래 타일이 나무거나 none 타일이면 가능
	bool isNone = originTileInfo.type == TileType::None || originTileInfo.subtype == "trees" || originTileInfo.type == TileType::Zone;
	//철거 타일이면
	bool isRubble = subtype == "rubble";
	if (isRubble)
	{
		return !isNone;
	}
	else
	{
		return isNone;
	}

	return originTileInfo.type != TileType::Building;
}

void TileModel::RequestUpdateTile(const TileDepth& depth, const CellIndex& tileIndex)
{
	if (m_WhenNeedsToUpdateTileFunc)
		m_WhenNeedsToUpdateTileFunc(depth, tileIndex);
}

void TileModel::RequestTempEffectTile(const CellIndex& tileIndex)
{
	if (m_WhenNeedsToUpdateTempEffectTileFunc)
		m_WhenNeedsToUpdateTempEffectTileFunc(tileIndex);
}
