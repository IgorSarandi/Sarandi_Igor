#include "Ghosts.h"
#include <math.h>
#include <random>


Ghosts::Ghosts() : 
	status { {"CHASE", 0}, {"SCATTER", 1}, {"FRIGHTENED", 2} }, doorCoord_(12, 14), doorFigure_(char(65), 31), current_status_("SCATTER"),
	lowfield_(' ', 1), chasecount_(4), doorPassed_(false), deltatime_(0), prevstate_(0)
{
	this->setDirection("LEFT");
	oldtime_ = this->getPresentTime();
}


Ghosts::~Ghosts()
{
}

bool Ghosts::CheckPacman(std::pair<int, int> pac)
{
	return this->getPosition() == pac ? true : false;
}

int Ghosts::getStatus() const
{
	return status.at(current_status_);
}

void Ghosts::setStatus(std::string current_status)
{
	prevstate_ = this->getStatus();
	oldtime_ = this->getPresentTime();

	current_status_ = current_status;
	int x = this->getPosition().first;
	int y = this->getPosition().second;
	int dir = this->getDirection();
	std::vector<int> temp;

	if (&land_[x - 1][y] != 0 && dir != 0)
	{
		temp.push_back(0);
	}
	if (&land_[x + 1][y] != 0 && dir != 1)
	{
		temp.push_back(1);
	}
	if (&land_[x][y - 1] != 0 && dir != 2)
	{
		temp.push_back(2);
	}
	if (&land_[x][y + 1] != 0 && dir != 3)
	{
		temp.push_back(3);
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(0, (int)(temp.size() - 1));
	int q = dis(gen);
	for (auto a : directions)
	{
		if (a.second == temp[q])
		{
			this->setDirection(a.first);
			return;
		}
	}
}

bool Ghosts::getDoorPassed() const
{
	return doorPassed_;
}

void Ghosts::setDoorPassed(bool door)
{
	doorPassed_ = door;
}

std::pair<int, int> Ghosts::getTargetField() const
{
	return targetfield_;
}

void Ghosts::setTargetField(std::pair<int, int> targetfield)
{
	targetfield_ = targetfield;
}

std::pair<char, int> Ghosts::getLowField() const
{
	return lowfield_;
}

void Ghosts::setLowField(std::pair<char, int> lowfield)
{
	lowfield_ = lowfield;
}

std::pair<int, int> Ghosts::FindTargetDirect(std::pair<int, int> target)
{
	int x = target.first;
	int y = target.second;

	int k = this->getPosition().first;
	int m = this->getPosition().second;
	int dir = this->getDirection();
	int new_dir;
	int x1, y1;
	int i;
	std::vector<int> dirs;

	double pow1, pow2;
	double dist = INT32_MAX;
	double ch = 0;

	int dx[4] = { -1, 1, 0, 0 };
	int dy[4] = { 0, 0, -1, 1 };//up, down, left, right

	auto lambdadir = [&dir](int n)
	{
		if (dir == 0 && n == 1)
			return false;
		else if (dir == 1 && n == 0)
			return false;
		else if (dir == 2 && n == 3)
			return false;
		else if (dir == 3 && n == 2)
			return false;
		else
			return true;
	};

	for (i = 0; i < 4; i++)
	{
		int t1 = k + dx[i];
		int t2 = m + dy[i];
		if (x == -1 && y == -1)//mode FRIGHTENED
		{
			if (i == 2 && m == 0)
			{
				dirs.push_back(i);
			}
			else if (i == 3 && m == 27)//m width
			{
				dirs.push_back(i);
			}
			else if (land_[t1][t2] != 0)
			{
				dirs.push_back(i);
			}
		}
		else if (m == 0 && dir == 2)
		{
			m = 27;
			return std::make_pair(k, m);
		}
		else if (m == 27 && dir == 3)
		{
			m = 0;
			return std::make_pair(k, m);
		}
		else if (land_[x][y] == 0 && x == doorCoord_.first && y == doorCoord_.second)//����� �� ���� � �����
		{
			if (k == x + 1 && m == y)
			{
				this->setDirection("UP");
				this->setDoorPassed(true);
				return doorCoord_;
			}
			else if (land_[t1][t2] != 0 && lambdadir(i))
			{
				pow1 = pow(x - t1, 2);
				pow2 = pow(y - t2, 2);
				ch = sqrt(pow1 + pow2);

				if (dist > ch)
				{
					dist = ch;
					x1 = t1;
					y1 = t2;
					new_dir = i;
				}
			}
		}
		else if (lambdadir(i))
		{
			try
			{
				if (land_[t1].at(t2) != 0)
				{
					pow1 = pow(x - t1, 2);
					pow2 = pow(y - t2, 2);
					ch = sqrt(pow1 + pow2);

					if (dist > ch)
					{
						dist = ch;
						x1 = t1;
						y1 = t2;
						new_dir = i;
					}
				}
			}
			catch (std::out_of_range const& ex)
			{
				continue;
			}
		}
	}

	if (x == -1 && y == -1)
	{
		if (dirs.size() > 2)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<>dis(0, static_cast<int>(dirs.size()) - 1);
			int q = dis(gen);
			for (auto a : directions)
			{
				if (a.second == dirs[q])
				{
					this->setDirection(a.first);
					int t1 = k + dx[dirs[q]];
					int t2 = m + dy[dirs[q]];
					return std::make_pair(t1, t2);
				}
			}
		}
		else if (dirs.size() == 2)
		{
			for (int i = 0; i < static_cast<int>(dirs.size()); i++)
			{
				if (lambdadir(dirs[i]))
				{
					for (auto a : directions)
					{
						if (a.second == dirs[i])
						{
							int t1, t2;
							this->setDirection(a.first);
							t1 = k + dx[dirs[i]];

							if (a.second == 2 && m == 0)
							{
								t2 = 27;//width
							}
							else if (a.second == 3 && m == 27)
							{
								t2 = 0;
							}
							else
							{
								t2 = m + dy[dirs[i]];
							}
							return std::make_pair(t1, t2);
						}
					}
				}
			}
			return std::make_pair(INT32_MAX, INT32_MAX);//return error
		}
		else
		{
			int t1 = k + dx[dirs[0]];
			int t2 = m + dy[dirs[0]];
			if (dirs[0] != dir)
			{
				for (auto a : directions)
				{
					if (a.second == dirs[0])
					{
						this->setDirection(a.first);
						break;
					}
				}
			}
			return std::make_pair(t1, t2);
		}
	}
	else
	{
		for (auto a : directions)
		{
			if (a.second == new_dir)
			{
				this->setDirection(a.first);
			}
		}
		return std::make_pair(x1, y1);
	}
}

std::pair<int, int> Ghosts::Go()
{
	this->Character();
	int state = this->getStatus();
	bool door = this->getDoorPassed();

	if (!door)
	{
		return this->FindTargetDirect(doorCoord_);
	}
	else
	{
		switch (state)
		{
		case 0://CHASE
			return this->FindTargetDirect(getTarget());
			break;
		case 1://SCATTER
			return this->FindTargetDirect(this->getTargetField());
			break;
		default://FRIGHTENED
			return this->FindTargetDirect(std::make_pair(-1, -1));
			break;
		}
	}
}

void Ghosts::CheckMode()
{
	int state = this->getStatus();
	std::chrono::time_point<std::chrono::steady_clock> currenttime = this->getPresentTime();
	deltatime_ = static_cast<float>(std::chrono::duration_cast<std::chrono::duration<double>>(currenttime - oldtime_).count());
	
	switch (state)
	{
	case 0://CHASE
		if (chasecount_ > 0)
		{
			if (deltatime_ >= 20)
			{
				this->setStatus("SCATTER");
			}
		}
		break;
	case 1://SCATTER
		if (chasecount_ > 2)
		{
			if (deltatime_ >= 7)
			{
				this->setStatus("CHASE");
				chasecount_--;
			}
			return;
		}
		if (chasecount_ > 0)
		{
			if (deltatime_ >= 5)
			{
				this->setStatus("CHASE");
				chasecount_--;
			}
			return;
		}
		break;
	default://FRIGHTENED
		if (deltatime_ >= 10)
		{
			if (prevstate_ == 0)
			{
				this->setStatus("CHASE");
			}
			else
			{
				this->setStatus("SCATTER");
			}
		}
		else
		{
			if (prevstate_ == 2)
			{
				prevstate_ = 1;
			}
		}
		break;
	}
}

void Ghosts::setChaseCount(int chasecount)
{
	chasecount_ = chasecount;
}

int Ghosts::getPrevState() const
{
	return prevstate_;
}

std::pair<int, int> Ghosts::getDoorPosition() const
{
	return doorCoord_;
}

std::pair<int, int> Ghosts::getDoorFigure() const
{
	return doorFigure_;
}

void Ghosts::HomeMode()
{
	int dir = this->getDirection();
	std::pair<int, int> pos = this->getPosition();

	if (dir == 0)
	{
		if (land_[pos.first - 1][pos.second] != 0)
		{
			this->setPosition(pos.first - 1, pos.second);
		}
		else
		{
			this->setPosition(pos.first + 1, pos.second);
			this->setDirection("DOWN");
		}
	}
	else if (dir == 1)
	{
		if (land_[pos.first + 1][pos.second] != 0)
		{
			this->setPosition(pos.first + 1, pos.second);
		}
		else
		{
			this->setPosition(pos.first - 1, pos.second);
			this->setDirection("UP");
		}
	}
	else
	{
		this->setDirection("UP");

		if (land_[pos.first - 1][pos.second] != 0)
		{
			this->setPosition(pos.first - 1, pos.second);
			this->setDirection("UP");
		}
		else
		{
			this->setPosition(pos.first + 1, pos.second);
			this->setDirection("DOWN");
		}
	}
}

void Ghosts::Character()
{
	return;
}

bool Ghosts::StartCondition(const Info& info)
{
	return true;
}

void Ghosts::setCoins(const Info & info)
{
	return;
}
