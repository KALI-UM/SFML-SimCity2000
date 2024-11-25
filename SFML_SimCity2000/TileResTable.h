#pragma once
#include "DataTable.h"
#include "Tile.h"

namespace std {
	template <typename T1, typename T2>
	struct hash<std::pair<T1, T2>> {
		std::size_t operator()(const std::pair<T1, T2>& p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);
			return h1 ^ (h2 << 1);
		}
	};
}

class TileResTable :
	public DataTable
{
	static TileResData m_Empty;
public:
	TileResTable() : DataTable(DataId::TileRes, "datatables/tileres.csv") { };
	~TileResTable() = default;

	bool Load() override;
	void Release() override;

	std::string GetTileFilePath(const TYPE& type, const SUBTYPE& subtype, const NAME& name );
	std::string GetTileFilePath(const TileType& type, const SUBTYPE& subtype, const NAME& name);
	const TileResData& GetTileRes(const TYPE& type, const SUBTYPE& subtype, const NAME& name) const;
	const TileResData& GetTileRes(const TileType& type, const SUBTYPE& subtype, const NAME& name) const;
	const TileResData& GetTileRes(const ID& id);
	void SetTileInfo(TileInfo& info, const TileType& type, const SUBTYPE& subtype, const NAME& name);
	void SetTileInfo(TileInfo& info, const ID& id);

	const TileResData& GetTileRes(const ZONE& zone, const sf::Vector2u& lotSize, int index) const;
	const TileResData& GetTileRes(const ZoneType& zone, const sf::Vector2u& lotSize, int index) const;

protected:
	std::string GetTileFilePath(const TileResData& data);
	
protected:
	std::unordered_map<std::pair<TYPE,SUBTYPE>, std::unordered_map<NAME, ID>>		m_TileResDataByType;
	std::unordered_map<ZONE, std::unordered_multimap<int, ID>>						m_TileResDataByZone;
	std::vector<TileResData> m_TileResDataById;
	std::string m_TilePngPath = "tiles/";
private:
	sf::Vector2u GetLotSizeFromNXM(const std::string& NxM);
};

