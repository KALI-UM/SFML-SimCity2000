#pragma once
#include "DataTable.h"
#include "Tile.h"


class TileAttTable :
    public DataTable
{
	static TileTypeAttribute m_Empty;
public:
	TileAttTable() : DataTable(DataId::TileAtt, "datatables/tileattribute.csv") { };
	~TileAttTable() = default;

	bool Load() override;
	void Release() override;

	const TileTypeAttribute& GetTileAttribute(const TYPE& type) const;
	const TileTypeAttribute& GetTileAttribute(const TileType& type) const;
	bool CanBeSub(const TYPE& type1, const SUBTYPE& type2) const;
	bool CanBeSub(const TileType& type, const TileType& sub) const;

	std::pair<TileType, SUBTYPE> GetTypeAndSubtype(const TYPE& type1, const TYPE& type2) const;
	std::pair<TileType, SUBTYPE> GetTypeAndSubtype(const TileType& type1, const TileType& type2) const;

protected:
	std::unordered_map<TYPE, TileTypeAttribute> m_TileTypeAtrributes;

	void SetCanBeSub(std::unordered_set<SUBTYPE>& att, const std::string& types);
};

