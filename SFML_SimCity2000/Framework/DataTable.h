#pragma once

class DataTable
{
public:
	enum class DataId
	{
		TileRes,
		TileAtt,
	};

	DataTable(DataId id, const std::string& filepath) :m_DataTableId(id), m_FilePath(filepath) {}
	~DataTable() = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;

	const DataId m_DataTableId;
	const std::string m_FilePath;

protected:
	static bool GetBooleanFromNY(const std::string& ny);
	static void SetStringToVectorElements(const std::string& str, std::vector<std::string>& vec);
};

