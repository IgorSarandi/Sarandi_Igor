#pragma once
#include "MovingObjects.h"
#include "MotionlessObjects.h"

#include <string>
#include <map>
#include <vector>

class Ghosts ://abstract class
	public MovingObjects
{
public:
	Ghosts();
	virtual ~Ghosts() = 0;

	bool CheckPacman(std::pair<int, int>);

	int getStatus() const;

	void setStatus(std::string);

	bool getDoorPassed() const;

	void setDoorPassed(bool);

	std::pair<int, int> getTargetField() const;

	void setTargetField(std::pair<int, int>);

	std::pair <char, int> getLowField() const;

	void setLowField(std::pair<char, int>);

	std::pair<int, int> FindTargetDirect(std::pair<int, int>);

	std::pair<int, int> Go(std::pair<int, int>);

	virtual void Character() = 0;

protected:
	const std::map<std::string, int> status;
	const std::pair<int, int> doorCoord_;

private:
	std::string current_status_;
	std::pair<int, int> targetfield_;
	std::pair<int, int> startpos_;
	std::pair<char, int> lowfield_;
	bool doorPassed_;
};

