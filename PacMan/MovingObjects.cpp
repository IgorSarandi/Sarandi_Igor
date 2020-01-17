#include "MovingObjects.h"


MovingObjects::MovingObjects() : 
	directions{ {"UP", 0}, {"DOWN", 1}, {"LEFT", 2}, {"RIGHT", 3} },
	speed_(0), delta_(0), deltatime_(0)
{
	oldtime_ = std::chrono::steady_clock::now();
}


MovingObjects::~MovingObjects()
{
}

void MovingObjects::setSpeed(int speed)
{
	speed_ = speed;
}


void MovingObjects::UpdateDeltaTime()
{
	auto currenttime_ = std::chrono::steady_clock::now();
	deltatime_ = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(currenttime_ - oldtime_).count());
	oldtime_ = currenttime_;
}

std::pair<int, int> MovingObjects::getStartPosition() const
{
	return startposition_;
}

void MovingObjects::setStartPosition(int x, int y)
{
	startposition_ = std::make_pair(x, y);
}

int MovingObjects::getDirection() const
{
	return directions.at(current_direction_);
}

void MovingObjects::setDirection(std::string current_direction)
{
	current_direction_ = current_direction;
}

void MovingObjects::UpdateLand(std::vector<std::vector<int>> crMap)
{
	land_ = crMap;
}

bool MovingObjects::Update()
{
	delta_ += deltatime_ * speed_;
	if (delta_ > 1)
	{
		delta_ = 0;
		return true;
	}
	return false;
}