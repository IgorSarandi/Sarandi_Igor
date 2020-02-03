#pragma once
#include <vector>
class CreateMap
{
public:
	CreateMap(int, int);
	~CreateMap();

	 std::vector<std::vector<int>> globalMap();

private:
	const int sizeX;
	const int sizeY;
};

