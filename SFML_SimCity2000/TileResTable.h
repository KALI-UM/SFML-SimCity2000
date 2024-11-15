#pragma once
#include "DataTable.h"
#include "TileResData.h"

typedef std::string TYPE;
typedef std::string NAME;
typedef int ID;

class TileResTable :
	public DataTable
{
	static TileResData m_Empty;
public:
	TileResTable() : DataTable(DataId::TileRes, "datatables/tileres.csv") { };
	~TileResTable() = default;

	bool Load() override;
	void Release() override;

	std::string GetTileFilePath(const TYPE& type, const NAME& name) ;
protected:
	TileResData& GetTileRes(const TYPE& type, const NAME& name);
protected:
	std::unordered_map<TYPE, std::unordered_map<NAME, ID>> m_TileResDataByType;
	std::vector<TileResData> m_TileResDataById;
	std::string m_TilePngPath = "tiles/";
private:
	sf::Vector2u GetLotSizeFromNXM(const std::string& NxM);
};

