#pragma once
#include <iostream>
class Objects // general abstract class
{
public:
	Objects();
	virtual ~Objects() = 0;

	void Destroy();

	std::pair<int, int> getPosition() const;

	void setPosition(int, int);

	virtual std::pair<char, int> Figure() = 0; //object picture, where CHAR is display symbol, INT is color
private:
	std::pair<int, int> position_;
};

