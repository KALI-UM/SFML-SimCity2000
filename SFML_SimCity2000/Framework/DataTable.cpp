#include "pch.h"
#include "DataTable.h"

bool DataTable::GetBooleanFromNY(const std::string& ny)
{
	return ny == "Y";
}

void DataTable::SetStringToVectorElements(const std::string& str, std::vector<std::string>& vec)
{
	vec.clear();
	int curr = 0;
	while (true)
	{
		auto it = str.find(',', curr);
		if (it == std::string::npos)
		{
			vec.push_back(str.substr(curr, str.length() - curr));
			return;
		}
		vec.push_back(str.substr(curr, it - curr));
		curr = it + 1;
	}
}
