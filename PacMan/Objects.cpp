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

std::pair <int, int> Objects::GetPosition() const
{
	return position_;
}

void Objects::SetPosition(int x, int y)
{
	position_.first = x;
	position_.second = y;
}