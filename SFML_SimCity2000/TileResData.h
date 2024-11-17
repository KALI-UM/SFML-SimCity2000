#pragma once

enum class ConnectedType
{
	C0110 = 0,
	C1001 = 1,
	C0101 = 2,
	C0011 = 3,
	C1010 = 4,
	C1100 = 5,
	C1

};




struct TileResData
{
	int id;
	std::string filename;
	std::string type;
	std::string subtype;
	std::string name;

	sf::Vector2u	lotSize;
	bool			animated;
};

