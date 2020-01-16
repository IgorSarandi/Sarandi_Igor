#pragma once
#include "Objects.h"
#include <chrono>

class MovingObjects :
	public Objects
{
public:
	MovingObjects();

	virtual ~MovingObjects() = 0;

	void SetSpeed(int);

	bool Update();

	void UpdateDeltaTime();

private:
	int speed_;
	float delta_;
	float deltatime_;
	std::chrono::time_point<std::chrono::steady_clock> oldtime_;
};

