#pragma once
#include "MovingObjects.h"
#include "Pacman.h"
#include <string>
#include <map>
#include<vector>

class Ghosts ://abstract class
	public MovingObjects
{
public:
	Ghosts();
	virtual ~Ghosts() = 0;

	bool CheckPacman(const Pacman&) const;

	int GetDirection() const;

	void SetDirection(std::string);
	
	int GetStatus() const;

	void SetStatus(std::string);

	bool GetDoorLock() const;

	void SetDoorLock(bool);

	std::pair<int, int> GetTargetField() const;

	void SetTargetField(std::pair<int, int>);

	std::pair <int, int> GetStartPos() const;

	void SetStartPos(int, int);

	std::pair <char, int> GetLowField() const;

	void SetLowField(std::pair <char, int>);

	std::pair<int, int> FindTargetLee(std::pair<int, int>);

	std::pair<int, int> FindTargetDirect(std::pair<int, int>);

	std::pair<int, int> Go(const Pacman&);

protected:
	std::map<std::string, int> directions;
	std::map<std::string, int> status;

private:
	std::string current_direction_;
	std::string current_status_;
	std::vector<std::vector<int>> land_;
	std::pair<int, int> targetfield_;
	std::pair<int, int> startpos_;
	std::pair<char, int> lowfield_;
	bool door_;
};

