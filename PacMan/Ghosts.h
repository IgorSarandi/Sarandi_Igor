#pragma once
#include "MovingObjects.h"
#include "MotionlessObjects.h"
#include "Pacman.h"
#include "Info.h"

#include <string>
#include <map>
#include <vector>
#include <chrono>

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

	std::pair<int, int> Go();

	void CheckMode();

	void setChaseCount(int);

	int getPrevState() const;

	std::pair<int, int> getDoorPosition() const;

	std::pair<int, int> getDoorFigure() const;

	void HomeMode();

	virtual void setTarget(Pacman*) = 0;

	virtual void Character();

	virtual bool StartCondition(const Info&);//exit from house

	virtual void setCoins(const Info &);

	virtual std::pair<int, int> getTarget() const = 0;

protected:
	const std::map<std::string, int> status;
	const std::pair<int, int> doorCoord_;
	const std::pair<char, int> doorFigure_;
	std::pair<int, int> pacmanTarget;

private:
	std::string current_status_;
	std::pair<int, int> targetfield_;
	std::pair<int, int> startpos_;
	std::pair<char, int> lowfield_;
	int chasecount_;
	bool doorPassed_;
	float deltatime_;
	int prevstate_;
	std::chrono::time_point<std::chrono::steady_clock> oldtime_;

};

