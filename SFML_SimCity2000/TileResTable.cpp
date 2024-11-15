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

		auto it = m_TileResDataByType.find(currRes.type);
		if (it == m_TileResDataByType.end())
		{
			m_TileResDataByType.insert({ currRes.type, std::unordered_map<NAME, ID>() });
			it = m_TileResDataByType.find(currRes.type);
		}

		it->second.insert({ currRes.name, currRes.id });
	}

	return true;
}

void TileResTable::Release()
{
	m_TileResDataByType.clear();
	m_TileResDataById.clear();
}

std::string TileResTable::GetTileFilePath(const TYPE& type, const NAME& name) 
{
	return GetTileRes(type, name).filename;
}

TileResData& TileResTable::GetTileRes(const TYPE& type, const NAME& name) 
{
	auto byType = m_TileResDataByType.find(type);
	if (byType == m_TileResDataByType.end())return m_Empty;

	auto byName = byType->second.find(name);
	if (byName == byType->second.end())return m_Empty;

	return m_TileResDataById[byName->second];
}

sf::Vector2u TileResTable::GetLotSizeFromNXM(const std::string& NxM)
{
	auto xIndex = NxM.find_first_of('x');
	if (xIndex == std::string::npos)
		return sf::Vector2u(1, 1);
	return sf::Vector2u(std::stoul(NxM.substr(0, xIndex)), std::stoul(NxM.substr(xIndex + 1, NxM.length() - xIndex + 1)));
}
