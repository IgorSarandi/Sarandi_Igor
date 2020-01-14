#include "Ghosts.h"
#include "Map.h"
#include <math.h>
#include <random>



Ghosts::Ghosts() : land_(CreateMap()), door_(false)
{
	directions = { {"UP", 0}, {"DOWN", 1}, {"LEFT", 2}, {"RIGHT", 3} };
	current_direction_ = "UP";

	status = { {"HOUSE", 0}, {"CHASE", 1}, {"SCATTER", 2}, {"FRIGHTENED", 3} };
	current_status_ = "HOUSE";
}


Ghosts::~Ghosts()
{
}

bool Ghosts::CheckPacman(const Pacman& pac) const
{
	return pac.GetPosition() == this->GetPosition() ? true : false;
}

int Ghosts::GetDirection() const
{
	return directions.at(current_direction_);
}

void Ghosts::SetDirection(std::string current_direction)
{
	current_direction_ = current_direction;
}

int Ghosts::GetStatus() const
{
	return status.at(current_status_);
}

void Ghosts::SetStatus(std::string current_status)
{
	if(this->GetStatus() == 0)
		current_status_ = current_status;
	else
	{
		current_status_ = current_status;
		int x = this->GetPosition().first;
		int y = this->GetPosition().second;
		int dir = this->GetDirection();
		std::vector<int> temp;
		if (land_[x - 1][y] != 0 && dir != 0)
		{
			temp.push_back(0);
		}
		else if (land_[x + 1][y] != 0 && dir != 1)
		{
			temp.push_back(1);
		}
		else if (land_[x][y - 1] != 0 && dir != 2)
		{
			temp.push_back(2);
		}
		else if (land_[x][y + 1] != 0 && dir != 3)
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
				this->SetDirection(a.first);
				return;
			}
		}
	}
}

bool Ghosts::GetDoorStatus() const
{
	return door_;
}

void Ghosts::SetDoorStatus(bool door)
{
	door_ = door;
}

std::pair<int, int> Ghosts::GetTargetField() const
{
	return targetfield_;
}

void Ghosts::SetTargetField(std::pair<int, int> targetfield)
{
	targetfield_ = targetfield;
}

std::pair<int, int> Ghosts::GetStartPos() const
{
	return startpos_;
}

void Ghosts::SetStartPos(int x, int y)
{
	startpos_.first = x;
	startpos_.second = y;
}

std::pair<int, int> Ghosts::FindTargetLee(std::pair<int, int> target)//add restrictions binded with directions
{
	//Lee algorithm(Wave algorithm)

	std::vector<std::vector<int>> map(CreateMap());
	std::map<std::pair<int, int>, int> field;
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map.size(); j++)
		{
			std::pair<int, int> p = std::make_pair(i, j);
			field[p] = INT_MAX;
		}
	}

	int k = this->GetPosition().first;
	int m = this->GetPosition().second;
	int d = 0;

	// распространение волны
	field[std::make_pair(k, m)] = 0;            // стартова€ €чейка помечена 0
	while(field[target] == INT_MAX)
	{
		for (int i = 0; i < map.size(); i++)
		{
			for (int j = 0; j < map[i].size(); j++)
			{
				if (field[std::make_pair(i, j)] == d)                         // €чейка (x, y) помечена числом d
				{
					if (i + 1 < map.size() && map[i + 1][j] != 0 && field[std::make_pair(i + 1, j)] == INT_MAX)
					{
						field[std::make_pair(i + 1, j)] = d + 1;
					}
					else if (i - 1 >= 0 && map[i - 1][j] != 0 && field[std::make_pair(i - 1, j)] == INT_MAX)
					{
						field[std::make_pair(i - 1, j)] = d + 1;
					}
					else if (j + 1 < map[i].size() && map[i][j + 1] != 0 && field[std::make_pair(i, j + 1)] == INT_MAX)
					{
						field[std::make_pair(i, j + 1)] = d + 1;
					}
					else if (j - 1 >= 0 && map[i][j - 1] != 0 && field[std::make_pair(i, j - 1)] == INT_MAX)
					{
						field[std::make_pair(i, j - 1)] = d + 1;
					}
				}
			}
		}
		d++;
	}

	// восстановление пути
	int len = field[target];
	int x = target.first;
	int y = target.second;
	while (len > 1)
	{
		if (x + 1 < map.size() && map[x + 1][y] != 0 && field[std::make_pair(x + 1, y)] == len - 1)
		{
			x++;
			len--;
			continue;
		}
		else if (x - 1 >= 0 && map[x - 1][y] != 0 && field[std::make_pair(x - 1, y)] == len - 1)
		{
			x--;
			len--;
			continue;
		}
		else if (y + 1 < map[x].size() && map[x][y + 1] != 0 && field[std::make_pair(x, y + 1)] == len - 1)
		{
			y++;
			len--;
			continue;
		}
		else if (y - 1 >= 0 && map[x][y - 1] != 0 && field[std::make_pair(x, y - 1)] == len - 1)
		{
			y--;
			len--;
			continue;
		}
	}
	
	return std::make_pair(x, y);
}

std::pair<int, int> Ghosts::FindTargetDirect(std::pair<int, int> target)
{
	int x = target.first;
	int y = target.second;

	int k = this->GetPosition().first;
	int m = this->GetPosition().second;
	int dir = this->GetDirection();
	int new_dir;
	int x1, y1;
	int i;
	std::vector<int> dirs;

	double pow1, pow2;
	double dist = INT_MAX;
	double ch = 0;

	int dx[4] = {-1, 1, 0, 0 };
	int dy[4] = { 0, 0, -1, 1 };//up, down, left, right

	auto lambdadir = [&dir, &i](int n = 0)
	{
		n = i;
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
			if (land_[t1][t2] != 0)
			{
				dirs.push_back(i);
			}
		}
		else if (land_[t1][t2] != 0 && lambdadir())
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
		else if (land_[x][y] == 0 && x == 12 && y == 14)//выход из дома в дверь
		{
			this->SetDirection("UP");
			return std::make_pair(x, y);
		}
	}
	
	if (x == -1 && y == -1)
	{
		if (dirs.size() > 2)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<>dis(0, (int)dirs.size() - 1);
			int q = dis(gen);
			for (auto a : directions)
			{
				if (a.second == dirs[q])
				{
					this->SetDirection(a.first);
					int t1 = k + dx[q];
					int t2 = m + dy[q];
					return std::make_pair(t1, t2);;
				}
			}
		}
	}
	else
	{
		for (auto a : directions)
		{
			if (a.second == new_dir)
			{
				this->SetDirection(a.first);
			}
		}
		return std::make_pair(x1, y1);
	}
}

void Ghosts::Go(const Pacman& pac)//check pacman//return obj(coin, booster, field,fr)
{
	int st = this->GetStatus();
	int dir = this->GetDirection();
	int x = this->GetPosition().first;
	int y = this->GetPosition().second;

	if (st == 0)
	{
		if (!this->GetDoorStatus())
		{
			if (land_[x - 1][y] == 0)
			{
				this->SetDirection("DOWN");
				this->SetPosition(x + 1, y);
			}
			else
			{
				this->SetDirection("UP");
				this->SetPosition(x - 1, y);
			}
		}
		else
		{
			std::pair<int, int> p = this->FindTargetDirect(std::make_pair(12, 14));
			this->SetPosition(p.first, p.second);
		}
	}
	else if (st == 1)
	{
		std::pair<int, int> p = this->FindTargetDirect(pac.GetPosition());
		this->SetPosition(p.first, p.second);
	}
	else if (st == 2)
	{
		std::pair<int, int> p = this->FindTargetDirect(this->GetTargetField());
		this->SetPosition(p.first, p.second);
	}
	else if (st == 3)
	{
		std::pair<int, int> p = this->FindTargetDirect(std::make_pair(-1, -1));
		this->SetPosition(p.first, p.second);
	}
}

