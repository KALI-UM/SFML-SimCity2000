#include "pch.h"
#include "TileResTable.h"
#include "rapidcsv.h"

TileResData TileResTable::m_Empty = TileResData();

bool TileResTable::Load()
{
	Release();
	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(0, -1));
	m_TileResDataById.resize(doc.GetRowCount());
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		auto& currRes = m_TileResDataById[doc.GetCell<int>("id", i)];
		currRes.id = doc.GetCell<int>("id", i);
		currRes.type = doc.GetCell<std::string>("type", i);
		currRes.subtype = doc.GetCell<std::string>("subtype", i);
		currRes.filename = m_TilePngPath + doc.GetCell<std::string>("filename", i);
		currRes.name = doc.GetCell<std::string>("name", i);
		currRes.lotSize = GetLotSizeFromNXM(doc.GetCell<std::string>("lot_size", i));
		currRes.animated = GetBooleanFromNY(doc.GetCell<std::string>("animated", i));
		currRes.zone = doc.GetCell<std::string>("zone", i);
		auto it = m_TileResDataByType.find({ currRes.type,currRes.subtype });
		if (it == m_TileResDataByType.end())
		{
			m_TileResDataByType.insert({ { currRes.type,currRes.subtype }, std::unordered_map<NAME, ID>() });
			it = m_TileResDataByType.find({ currRes.type,currRes.subtype });
		}
		it->second.insert({ currRes.name, currRes.id });

		auto it2 = m_TileResDataByZone.find(currRes.zone);
		if (it2 == m_TileResDataByZone.end())
		{
			m_TileResDataByZone.insert({ currRes.zone, std::unordered_multimap<int, ID>() });
			it2 = m_TileResDataByZone.find(currRes.zone);
		}
		it2->second.insert({ currRes.lotSize.x, currRes.id });
	}

	return true;
}

void TileResTable::Release()
{
	m_TileResDataByType.clear();
	m_TileResDataById.clear();
}

std::string TileResTable::GetTileFilePath(const TYPE& type, const SUBTYPE& subtype, const NAME& name)
{
	if (type == "")
		return "";

	auto& tileres = GetTileRes(type, subtype, name);
	if (&tileres == &m_Empty)
		return "";

	std::string filename = tileres.filename;
	if (tileres.animated)
		filename.insert(filename.length() - 4, "-0");
	return filename;
}

std::string TileResTable::GetTileFilePath(const TileType& type, const SUBTYPE& subtype, const NAME& name)
{
	if (type == TileType::None)return "";
	return GetTileFilePath(Tile::GetTypeToString(type), subtype, name);
}

std::string TileResTable::GetTileFilePath(const TileResData& data)
{
	std::string filename = data.filename;
	if (data.animated)
		filename.insert(filename.length() - 4, "-0");
	return filename;
}

const TileResData& TileResTable::GetTileRes(const TYPE& type, const SUBTYPE& subtype, const NAME& name) const
{
	auto byType = m_TileResDataByType.find({ type, subtype });
	if (byType == m_TileResDataByType.end())
		return m_Empty;

	auto byName = byType->second.find(name);
	if (byName == byType->second.end())
		return m_Empty;

	return m_TileResDataById[byName->second];
}

const TileResData& TileResTable::GetTileRes(const TileType& type, const SUBTYPE& subtype, const NAME& name) const
{
	if (type == TileType::None)
		return m_Empty;
	return GetTileRes(Tile::GetTypeToString(type), subtype, name);
}

const TileResData& TileResTable::GetTileRes(const ID& id)
{
	if (id > m_TileResDataById.size() || id < 0)return m_Empty;
	return m_TileResDataById[id];
}

void TileResTable::SetTileInfo(TileInfo& info, const TileType& type, const SUBTYPE& subtype, const NAME& name)
{
	auto tileres = GetTileRes(type, subtype, name);
	info.lotSize = tileres.lotSize;
	info.filepath = GetTileFilePath(tileres);
}

void TileResTable::SetTileInfo(TileInfo& info, const ID& id)
{
	auto tileres = GetTileRes(id);
	info.type = Tile::GetTypeToEnum(tileres.type);
	info.subtype = tileres.subtype;
	info.name = tileres.name;
	info.lotSize = tileres.lotSize;
	info.filepath = GetTileFilePath(tileres);
}

const TileResData& TileResTable::GetTileRes(const ZONE& zone, const sf::Vector2u& lotSize, int index) const
{
	auto range = m_TileResDataByZone.find(zone)->second.equal_range(lotSize.x);

	int curr = 0;
	for (auto it = range.first; it != range.second; ++it)
	{
		curr++;
		if (curr == index)return m_TileResDataById[it->second];
	}
	if (curr == 0)return m_Empty;

	index %= curr;
	curr = 0;
	for (auto it = range.first; it != range.second; ++it)
	{
		curr++;
		if (curr == index)return m_TileResDataById[it->second];
	}
	return m_TileResDataById[range.first->second];
}

const TileResData& TileResTable::GetTileRes(const ZoneType& zone, const sf::Vector2u& lotSize, int index) const
{
	return GetTileRes(Tile::GetZoneToString(zone), lotSize, index);
}

sf::Vector2u TileResTable::GetLotSizeFromNXM(const std::string& NxM)
{
	auto xIndex = NxM.find_first_of('x');
	if (xIndex == std::string::npos)
		return sf::Vector2u(1, 1);
	return sf::Vector2u(std::stoul(NxM.substr(0, xIndex)), std::stoul(NxM.substr(xIndex + 1, NxM.length() - xIndex + 1)));
}
