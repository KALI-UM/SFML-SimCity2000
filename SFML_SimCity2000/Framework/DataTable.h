#pragma once

class DataTable
{
public:
public:
	enum class DataId
	{
		TileRes,
	};

	DataTable(DataId id, const std::string& filepath) :m_DataTableId(id), m_FilePath(filepath) {}
	~DataTable() = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;

	const DataId m_DataTableId;
	const std::string m_FilePath;
};

