#pragma once

class DataTableManager :
    public Singleton<DataTableManager>
{
    friend class Singleton<DataTableManager>;

protected:
	DataTableManager() = default;
	virtual ~DataTableManager();

	DataTableManager(const DataTableManager& other) = delete;
	DataTableManager& operator=(const DataTableManager& other) = delete;

public:
	std::unordered_map<DataTable::DataId, DataTable*> m_DataTables;

public:
	bool Initialize();
	void Release();

	template<typename T>
	T* Get(DataTable::DataId id);
};

template<typename T>
inline T* DataTableManager::Get(DataTable::DataId id)
{
	auto find = m_DataTables.find(id);
	if (find == m_DataTables.end())
		return nullptr;
	return dynamic_cast<T*>(find->second);
}

#define DATATABLE_MGR (DataTableManager::GetInstance())
#define DATATABLE_TILERES (DATATABLE_MGR->Get<TileResTable>(DataTable::DataId::TileRes))
#define DATATABLE_TILEATT (DATATABLE_MGR->Get<TileAttTable>(DataTable::DataId::TileAtt))
