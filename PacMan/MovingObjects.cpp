#include "MovingObjects.h"



MovingObjects::MovingObjects() : speed_(0), delta_(0), deltatime_(0)
{
	oldtime_ = std::chrono::steady_clock::now();
}


MovingObjects::~MovingObjects()
{
}

void MovingObjects::SetSpeed(int speed)
{
	speed_ = speed;
}


void MovingObjects::UpdateDeltaTime()
{
	auto currenttime_ = std::chrono::steady_clock::now();
	deltatime_ = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(currenttime_ - oldtime_).count());
	oldtime_ = currenttime_;
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