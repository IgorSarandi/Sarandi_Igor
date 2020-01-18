#pragma once
#include "Objects.h"

#include <chrono>
#include <map>
#include <vector>
class MovingObjects :
	public Objects
{
public:
	MovingObjects();
	virtual ~MovingObjects() = 0;

	void setSpeed(int);

	bool Update();

	void UpdateDeltaTime();

	std::pair<int, int> getStartPosition() const; //get start object position

	void setStartPosition(int _x, int _y); //set start object position

	int getDirection() const;

	void setDirection(std::string);

	std::chrono::time_point<std::chrono::steady_clock> getPresentTime() const;

	void UpdateLand(std::vector<std::vector<int>>);

protected:
	const std::map<std::string, int> directions;
	std::vector<std::vector<int>> land_;

private:
	int speed_;
	float delta_;
	float deltatime_;
	std::chrono::time_point<std::chrono::steady_clock> oldtime_;
	std::pair<int, int> startposition_;
	std::string current_direction_;
};

