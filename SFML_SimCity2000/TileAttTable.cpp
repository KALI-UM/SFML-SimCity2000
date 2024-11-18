#include "pch.h"
#include "TileAttTable.h"

TileTypeAttribute TileAttTable::m_Empty = TileTypeAttribute();

bool TileAttTable::Load()
{
	Release();

	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(0, -1));
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		TileTypeAttribute currAtt;
		currAtt.type = doc.GetCell<std::string>("type", i);

		TileType eType = Tile::GetTypeToEnum(currAtt.type);
		if (eType == TileType::None)continue;
		auto it = m_TileTypeAtrributes.find(currAtt.type);
		if (it != m_TileTypeAtrributes.end())continue;
		currAtt.depth = doc.GetCell<int>("depth", i);
		currAtt.connectable = GetBooleanFromNY(doc.GetCell<std::string>("connectable", i));
		SetCanBeSub(currAtt.canbesub, doc.GetCell<std::string>("canbesub", i));
		m_TileTypeAtrributes.insert({ currAtt.type , currAtt });
	}

	return true;
}

void TileAttTable::Release()
{
	m_TileTypeAtrributes.clear();
}

const TileTypeAttribute& TileAttTable::GetTileAttribute(const TYPE& type) const
{
	auto it = m_TileTypeAtrributes.find(type);
	if (it == m_TileTypeAtrributes.end())
		return m_Empty;
	return it->second;
}


const TileTypeAttribute& TileAttTable::GetTileAttribute(const TileType& type) const
{
	if (type == TileType::None)
		return m_Empty;
	return GetTileAttribute(Tile::GetTypeToString(type));
}

bool TileAttTable::CanBeSub(const TYPE& type1, const SUBTYPE& type2) const
{
	auto att = GetTileAttribute(type1);
	if (&att == &m_Empty)return false;
	auto it = att.canbesub.find(type2);
	if (it == att.canbesub.end())
		return false;
	return true;
}

bool TileAttTable::CanBeSub(const TileType& type, const TileType& sub) const
{
	return CanBeSub(Tile::GetTypeToString(type), Tile::GetTypeToString(sub));
}

std::pair<TileType, SUBTYPE> TileAttTable::GetTypeAndSubtype(const TYPE& type1, const TYPE& type2) const
{
	if (type1 == type2)
		return { Tile::GetTypeToEnum(type1), type2 };
	bool t2IsSub = CanBeSub(type1, type2);
	bool t1IsSub = CanBeSub(type2, type1);

	if (!t2IsSub && !t1IsSub)return { TileType::None, "" };

	if (t2IsSub)
		return { Tile::GetTypeToEnum(type1), type2 };
	if (t1IsSub)
		return { Tile::GetTypeToEnum(type2), type1 };

	return  { TileType::None, "" };
}

std::pair<TileType, SUBTYPE> TileAttTable::GetTypeAndSubtype(const TileType& type1, const TileType& type2) const
{
	return GetTypeAndSubtype(Tile::GetTypeToString(type1), Tile::GetTypeToString(type2));
}

void TileAttTable::SetCanBeSub(std::unordered_set<SUBTYPE>& att, const std::string& types)
{
	if (types == "" )return;

	std::vector<std::string> types_vec;
	SetStringToVectorElements(types, types_vec);
	for (auto& typestring : types_vec)
	{
		att.insert(typestring);
	}
}

