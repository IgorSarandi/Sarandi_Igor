#include "Objects.h"



Objects::Objects()
{
}

Objects::~Objects()
{
}

void Objects::Destroy()
{
	this->~Objects();
}

std::pair <int, int> Objects::getPosition() const
{
	return position_;
}

void Objects::setPosition(int x, int y)
{
	position_.first = x;
	position_.second = y;
}