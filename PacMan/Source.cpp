#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <windows.h>
#include <memory>

#include "Border.h"
#include "Coins.h"
#include "Field.h"
#include "Map.h"
#include "Pacman.h"
#include "Booster.h"
#include "Fruits.h"
#include "GameController.h"
#include "Blinky.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::pair;
using std::vector;
using std::random_device;
using std::mt19937;

const int WIDTH = 28;//width of game field
const int HEIGHT = 31;//height of game field
const unsigned char maxlevel = MAXBYTE;//top level
unsigned char currentlevel = 0;
unsigned int totalcoins = 0;

GameController gc;

//Pacman& pacman = Pacman::getSingleton();

pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> Initialization();

void Logic(pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>>& objects, string key);

void DecreaseHP();

void Draw(vector<vector<pair<char, int>>>);

void GlobalCleaning(pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>>& objects);

void MakeLevel();

string Input();

int main()
{
	//Menu();
	for (int i = 0; i < maxlevel; i++)
	{
		system("cls");
		MakeLevel();
		pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> match = Initialization();
		Pacman::SetCheckLevel();

		while (!Pacman::GetCheckLevel())
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(200));
			string key = Input();
			Logic(match, key);
			Draw(match.second);
		}
		GlobalCleaning(match);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));//1 second
	}
	return 0;
}

void MakeLevel()
{
	currentlevel++; 
	totalcoins = 0;

	/*speed = speed - 100;
	if (level == 2)
		limit = 3000;
	else if (level == 3)
		limit = 100000;*/
}

pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> Initialization()
{
	vector<Objects*> _borders;
	vector<Objects*> _coins;
	vector<Objects*> _fields;
	vector<Objects*> _pacman;
	vector<Objects*> _booster;
	vector<Objects*> _fruits;
	vector<Objects*> _ghosts;
	_fruits.resize(2);

	vector<vector<Objects*>> _objects;
	vector<vector<pair<char, int>>> table;
	vector<vector<int>> map(std::move(CreateMap()));

	table.resize(HEIGHT);
	for (int i = 0; i < (int)table.size(); i++)
	{
		table[i].resize(WIDTH);

		for (int j = 0; j < (int)table[i].size(); j++)
		{
			int tmp = map[i][j];
			if (tmp == 0)
			{
				_borders.push_back(new Border);
				_borders.back()->SetPosition(i, j);
				table[i][j] = _borders.back()->Figure();
			}
			else if (tmp == 1)
			{
				_fields.push_back(new Field);
				_fields.back()->SetPosition(i, j);
				_coins.push_back(new Coins);
				_coins.back()->SetPosition(i, j);
				table[i][j] = _coins.back()->Figure();
			}
			else if (tmp == 2)
			{
				_fields.push_back(new Field);
				_fields.back()->SetPosition(i, j);
				table[i][j] = _fields.back()->Figure();
			}
		}
	}

	Pacman& pacman = Pacman::getSingleton();
	pacman.SetSpeed(10);;

	if (table[pacman.GetPosition().first][pacman.GetPosition().second] == (*_coins.begin())->Figure())
	{
		for (auto a : _coins)
		{
			if (a->GetPosition() == pacman.GetPosition())
			{
				auto it = std::find(_coins.begin(), _coins.end(), a);
				(*it)->Destroy();
				_coins.erase(it);
			}
		}
	}
	table[pacman.GetPosition().first][pacman.GetPosition().second] = pacman.Figure();
	_pacman.push_back(&pacman);


	Booster* boost = new Booster[4](); 	
	boost[0].SetPosition(3, 1);
	boost[1].SetPosition(3, 26);
	boost[2].SetPosition(23, 1);
	boost[3].SetPosition(23, 26);

	for (int i = 0; i < 4; i++)
	{
		if (table[boost[i].GetPosition().first][boost[i].GetPosition().second] == (*_coins.begin())->Figure())
		{
			for (auto a : _coins)
			{
				if (a->GetPosition() == boost[i].GetPosition())
				{
					auto it = std::find(_coins.begin(), _coins.end(), a);
					(*it)->Destroy();
					_coins.erase(it);
				}
			}
		}
		table[boost[i].GetPosition().first][boost[i].GetPosition().second] = boost[i].Figure();
		_booster.push_back(&boost[i]);
	}

	Blinky* blinky = new Blinky();
	blinky->SetSpeed(8);
	blinky->SetDoorLock(false);
	blinky->SetStartPos(11, 14);
	blinky->SetPosition(blinky->GetStartPos().first, blinky->GetStartPos().second);
	blinky->SetStatus("SCATTER");
	blinky->SetDirection("LEFT");
	blinky->SetLowField((*_fields.begin())->Figure());
	blinky->SetTargetField(std::make_pair(0, WIDTH - 1));
	table[blinky->GetPosition().first][blinky->GetPosition().second] = blinky->Figure();
	_ghosts.push_back(blinky);

	_objects.push_back(_borders);
	_objects.push_back(_coins);
	_objects.push_back(_fields);
	_objects.push_back(_pacman); 
	_objects.push_back(_booster);
	_objects.push_back(_ghosts);
	_objects.push_back(_fruits);


	pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> match;
	match.first = _objects;
	match.second = table;

	return match;
}

string Input()
{
	if (GetAsyncKeyState(87) //is W
		|| GetAsyncKeyState(119)) // is w
		return "UP";
	else if (GetAsyncKeyState(65) // is A
		|| GetAsyncKeyState(97)) // is a
		return "LEFT";
	else if (GetAsyncKeyState(83) // is S
		|| GetAsyncKeyState(115))//is s
		return "DOWN";
	else if (GetAsyncKeyState(68) // is D
		|| GetAsyncKeyState(100)) // is d
		return "RIGHT";
	else if (GetAsyncKeyState(VK_ESCAPE)) // is Esc
		return "END";
	else
		return "NULL";
}

void Logic(pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>>& objects, string key = "NULL")
{
	vector<Border*> _borders;
	vector<Coins*> _coins;
	vector<Field*> _fields;
	Pacman* _Pacman;
	vector<Booster*> _booster;
	vector<Ghosts*> _ghosts;


	std::transform(objects.first[0].begin(), objects.first[0].end(), std::back_inserter(_borders),
		[](Objects* obj) {return dynamic_cast<Border*>(obj); });
	std::transform(objects.first[1].begin(), objects.first[1].end(), std::back_inserter(_coins),
		[](Objects* obj) {return dynamic_cast<Coins*>(obj); });
	std::transform(objects.first[2].begin(), objects.first[2].end(), std::back_inserter(_fields),
		[](Objects* obj) {return dynamic_cast<Field*>(obj); });

	_Pacman  = dynamic_cast<Pacman*>(objects.first[3][0]);//добавить из Snake.app смерть Pacmana

	std::transform(objects.first[4].begin(), objects.first[4].end(), std::back_inserter(_booster),
		[](Objects* obj) {return dynamic_cast<Booster*>(obj); });
	std::transform(objects.first[5].begin(), objects.first[5].end(), std::back_inserter(_ghosts),
		[](Objects* obj) {return dynamic_cast<Ghosts*>(obj); });

	pair<char, int> obj;
	auto lambdafruit = [objects]()
	{
		for (auto a : objects.first[6])
		{
			if (a != nullptr)
				return true;
		}
		return false;
	};
	auto lambdaghost = [objects, &obj]()
	{
		for (auto a : objects.first[5])
		{
			if (obj == a->Figure())
				return true;
		}
		return false;
	};

	_Pacman->UpdateDeltaTime();
	if (_Pacman->Update())
	{
		if (key == "UP")
		{
			int k = _Pacman->GetPosition().first;
			int m = _Pacman->GetPosition().second;
			obj = objects.second[k - 1][m];

			if (obj == (*_fields.begin())->Figure())
			{
				k = _Pacman->GetPosition().first - 1;
				m = _Pacman->GetPosition().second;
				_Pacman->SetPosition(k, m);
				_Pacman->SetSpeed(10);;
				objects.second[k][m] = _Pacman->Figure();
				objects.second[k + 1][m] = (*_fields.begin())->Figure();
			}
			//чеканные монеты
			else if (obj == (*_coins.begin())->Figure())
			{
				k = _Pacman->GetPosition().first - 1;
				m = _Pacman->GetPosition().second;
				_Pacman->SetPosition(k, m);
				_Pacman->SetSpeed(7);;
				objects.second[k][m] = _Pacman->Figure();
				objects.second[k + 1][m] = (*_fields.begin())->Figure();
				_Pacman->Eat(10);
				totalcoins++;

				for (auto a : _coins)
				{
					if (a->GetPosition() == _Pacman->GetPosition())
					{
						auto it = std::find(objects.first[1].begin(), objects.first[1].end(), a);
						(*it)->Destroy();
						objects.first[1].erase(it);
					}
				}
			}
			//energizer
			else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
			{
				k = _Pacman->GetPosition().first - 1;
				m = _Pacman->GetPosition().second;
				_Pacman->SetPosition(k, m);
				_Pacman->SetSpeed(7);;
				objects.second[k][m] = _Pacman->Figure();
				objects.second[k + 1][m] = (*_fields.begin())->Figure();
				_Pacman->Eat(50);

				for (auto a : _booster)
				{
					if (a->GetPosition() == _Pacman->GetPosition())
					{
						auto it = std::find(objects.first[4].begin(), objects.first[4].end(), a);
						(*it)->Destroy();
						objects.first[4].erase(it);
					}
				}
				for (auto a : _ghosts)
				{
					a->SetStatus("FRIGHTENED");
					objects.second[a->GetPosition().first][a->GetPosition().second] = a->Figure();
				}
			}
			//стены
			else if (obj == (*_borders.begin())->Figure())
			{
				objects.second[k][m] = _Pacman->Figure();
			}
			//враги
			else if (lambdaghost())
			{
				k = _Pacman->GetPosition().first - 1;
				m = _Pacman->GetPosition().second;
				for (auto a : _ghosts)
				{
					int x = a->GetPosition().first;
					int y = a->GetPosition().second;
					a->SetSpeed(8);

					if (k == x && m == y && a->GetStatus() == 3)
					{
						_Pacman->SetPosition(k, m);
						_Pacman->SetSpeed(10);;
						objects.second[k][m] = _Pacman->Figure();
						objects.second[k + 1][m] = (*_fields.begin())->Figure();
						_Pacman->Eat(200);

						x = a->GetStartPos().first + 4;
						y = a->GetStartPos().second;
						a->SetPosition(x, y);
						a->SetStatus("CHASE");
						a->SetDoorLock(true);
						a->SetDirection("UP");
						objects.second[x][y] = a->Figure();
					}
					else if (k == x && m == y && a->GetStatus() != 3)
					{
						DecreaseHP();
						objects.second[k][m] = (*_fields.begin())->Figure();
						objects.second[k + 1][m] = (*_fields.begin())->Figure();
						int q = _Pacman->GetStartPosition().first;
						int w = _Pacman->GetStartPosition().second;
						_Pacman->SetPosition(q, w);
						_Pacman->SetSpeed(7);;
						objects.second[q][w] = _Pacman->Figure();
						for (auto a : _ghosts)
						{
							int x = a->GetStartPos().first;
							int y = a->GetStartPos().second;
							a->SetPosition(x, y);
							a->SetStatus("SCATTER");
							a->SetDoorLock(true);
							a->SetDirection("LEFT");
							objects.second[x][y] = a->Figure();
						}
					}
				}
			}
			//фрукты
			else if (lambdafruit())
			{
				for (auto a : objects.first[6])
				{
					if (a != nullptr && obj == a->Figure())
					{
						Fruits* fr = dynamic_cast<Fruits*>(a);
						k = _Pacman->GetPosition().first - 1;
						m = _Pacman->GetPosition().second;
						if (std::make_pair(k, m) != a->GetPosition()) continue;

						_Pacman->SetPosition(k, m);
						_Pacman->SetSpeed(7);;
						objects.second[k][m] = _Pacman->Figure();
						objects.second[k + 1][m] = (*_fields.begin())->Figure();
						_Pacman->Eat(fr->CalculateScore(currentlevel));
						gc.AddFruit(fr->Figure());
						fr->Destroy();
						auto it = std::find(objects.first[6].begin(), objects.first[6].end(), a);
						*it = nullptr;
						break;
					}
				}
			}
		}
		else if (key == "DOWN")
		{
			int k = _Pacman->GetPosition().first;
			int m = _Pacman->GetPosition().second;
			obj = objects.second[k + 1][m];

			if (obj == (*_fields.begin())->Figure())
			{
				k = _Pacman->GetPosition().first + 1;
				m = _Pacman->GetPosition().second;
				_Pacman->SetPosition(k, m);
				_Pacman->SetSpeed(10);;
				objects.second[k][m] = _Pacman->Figure();
				objects.second[k - 1][m] = (*_fields.begin())->Figure();
			}
			//чеканные монеты
			else if (obj == (*_coins.begin())->Figure())
			{
				k = _Pacman->GetPosition().first + 1;
				m = _Pacman->GetPosition().second;
				_Pacman->SetPosition(k, m);
				_Pacman->SetSpeed(7);;
				objects.second[k][m] = _Pacman->Figure();
				objects.second[k - 1][m] = (*_fields.begin())->Figure();
				_Pacman->Eat(10);
				totalcoins++;

				for (auto a : _coins)
				{
					if (a->GetPosition() == _Pacman->GetPosition())
					{
						auto it = std::find(objects.first[1].begin(), objects.first[1].end(), a);
						(*it)->Destroy();
						objects.first[1].erase(it);
					}
				}
			}
			//energizer
			else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
			{
				k = _Pacman->GetPosition().first + 1;
				m = _Pacman->GetPosition().second;
				_Pacman->SetPosition(k, m);
				_Pacman->SetSpeed(7);;
				objects.second[k][m] = _Pacman->Figure();
				objects.second[k - 1][m] = (*_fields.begin())->Figure();
				_Pacman->Eat(50);

				for (auto a : _booster)
				{
					if (a->GetPosition() == _Pacman->GetPosition())
					{
						auto it = std::find(objects.first[4].begin(), objects.first[4].end(), a);
						(*it)->Destroy();
						objects.first[4].erase(it);
					}
				}
				for (auto a : _ghosts)
				{
					a->SetStatus("FRIGHTENED");
					objects.second[a->GetPosition().first][a->GetPosition().second] = a->Figure();
				}
			}
			//стены
			else if (obj == (*_borders.begin())->Figure())
			{
				objects.second[k][m] = _Pacman->Figure();
			}
			//враги
			else if (lambdaghost())
			{
				k = _Pacman->GetPosition().first + 1;
				m = _Pacman->GetPosition().second;
				for (auto a : _ghosts)
				{
					int x = a->GetPosition().first;
					int y = a->GetPosition().second;
					a->SetSpeed(8);

					if (k == x && m == y && a->GetStatus() == 3)
					{
						_Pacman->SetPosition(k, m);
						_Pacman->SetSpeed(10);;
						objects.second[k][m] = _Pacman->Figure();
						objects.second[k - 1][m] = (*_fields.begin())->Figure();
						_Pacman->Eat(200);

						x = a->GetStartPos().first + 4;
						y = a->GetStartPos().second;
						a->SetPosition(x, y);
						a->SetStatus("CHASE");
						a->SetDoorLock(true);
						a->SetDirection("UP");
						objects.second[x][y] = a->Figure();
					}
					else if (k == x && m == y && a->GetStatus() != 3)
					{
						DecreaseHP();
						objects.second[k][m] = (*_fields.begin())->Figure();
						objects.second[k - 1][m] = (*_fields.begin())->Figure();
						int q = _Pacman->GetStartPosition().first;
						int w = _Pacman->GetStartPosition().second;
						_Pacman->SetPosition(q, w);
						_Pacman->SetSpeed(7);;
						objects.second[q][w] = _Pacman->Figure();
						for (auto a : _ghosts)
						{
							int x = a->GetStartPos().first;
							int y = a->GetStartPos().second;
							a->SetPosition(x, y);
							a->SetStatus("SCATTER");
							a->SetDoorLock(true);
							a->SetDirection("LEFT");
							objects.second[x][y] = a->Figure();
						}
					}
				}
			}
			//фрукты
			else if (lambdafruit())
			{
				for (auto a : objects.first[6])
				{
					if (a != nullptr && obj == a->Figure())
					{
						Fruits* fr = dynamic_cast<Fruits*>(a);
						k = _Pacman->GetPosition().first + 1;
						m = _Pacman->GetPosition().second;
						if (std::make_pair(k, m) != a->GetPosition()) continue;

						_Pacman->SetPosition(k, m);
						_Pacman->SetSpeed(7);;
						objects.second[k][m] = _Pacman->Figure();
						objects.second[k - 1][m] = (*_fields.begin())->Figure();
						_Pacman->Eat(fr->CalculateScore(currentlevel));
						gc.AddFruit(fr->Figure());
						fr->Destroy();
						auto it = std::find(objects.first[6].begin(), objects.first[6].end(), a);
						*it = nullptr;
						break;
					}
				}
			}
		}
		else if (key == "LEFT")
		{
			int k = _Pacman->GetPosition().first;
			int m = _Pacman->GetPosition().second;

			if (m)
			{
				obj = objects.second[k][m - 1];


				//проход по полям
				if (obj == (*_fields.begin())->Figure())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second - 1;
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(10);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][m + 1] = (*_fields.begin())->Figure();
				}
				//чеканные монеты
				else if (obj == (*_coins.begin())->Figure())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second - 1;
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(7);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][m + 1] = (*_fields.begin())->Figure();
					_Pacman->Eat(10);
					totalcoins++;

					for (auto a : _coins)
					{
						if (a->GetPosition() == _Pacman->GetPosition())
						{
							auto it = std::find(objects.first[1].begin(), objects.first[1].end(), a);
							(*it)->Destroy();
							objects.first[1].erase(it);
						}
					}
				}
				//energizer
				else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second - 1;
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(7);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][m + 1] = (*_fields.begin())->Figure();
					_Pacman->Eat(50);

					for (auto a : _booster)
					{
						if (a->GetPosition() == _Pacman->GetPosition())
						{
							auto it = std::find(objects.first[4].begin(), objects.first[4].end(), a);
							(*it)->Destroy();
							objects.first[4].erase(it);
						}
					}
					for (auto a : _ghosts)
					{
						a->SetStatus("FRIGHTENED");
						objects.second[a->GetPosition().first][a->GetPosition().second] = a->Figure();
					}
				}
				//стены
				else if (obj == (*_borders.begin())->Figure())
				{
					objects.second[k][m] = _Pacman->Figure();
				}
				//враги
				else if (lambdaghost())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second - 1;
					for (auto a : _ghosts)
					{
						int x = a->GetPosition().first;
						int y = a->GetPosition().second;
						a->SetSpeed(8);

						if (k == x && m == y && a->GetStatus() == 3)
						{
							_Pacman->SetPosition(k, m);
							_Pacman->SetSpeed(10);;
							objects.second[k][m] = _Pacman->Figure();
							objects.second[k][m + 1] = (*_fields.begin())->Figure();
							_Pacman->Eat(200);

							x = a->GetStartPos().first + 4;
							y = a->GetStartPos().second;
							a->SetPosition(x, y);
							a->SetStatus("CHASE");
							a->SetDoorLock(true);
							a->SetDirection("UP");
							objects.second[x][y] = a->Figure();
						}
						else if (k == x && m == y && a->GetStatus() != 3)
						{
							DecreaseHP();
							objects.second[k][m] = (*_fields.begin())->Figure();
							objects.second[k][m + 1] = (*_fields.begin())->Figure();
							int q = _Pacman->GetStartPosition().first;
							int w = _Pacman->GetStartPosition().second;
							_Pacman->SetPosition(q, w);
							_Pacman->SetSpeed(7);;
							objects.second[q][w] = _Pacman->Figure();
							for (auto a : _ghosts)
							{
								int x = a->GetStartPos().first;
								int y = a->GetStartPos().second;
								a->SetPosition(x, y);
								a->SetStatus("SCATTER");
								a->SetDoorLock(true);
								a->SetDirection("LEFT");
								objects.second[x][y] = a->Figure();
							}
						}
					}
				}
				//фрукты
				else if (lambdafruit())
				{
					for (auto a : objects.first[6])
					{
						if (a != nullptr && obj == a->Figure())
						{
							Fruits* fr = dynamic_cast<Fruits*>(a);
							k = _Pacman->GetPosition().first;
							m = _Pacman->GetPosition().second - 1;
							if (std::make_pair(k, m) != a->GetPosition()) continue;

							_Pacman->SetPosition(k, m);
							_Pacman->SetSpeed(7);;
							objects.second[k][m] = _Pacman->Figure();
							objects.second[k][m + 1] = (*_fields.begin())->Figure();
							_Pacman->Eat(fr->CalculateScore(currentlevel));
							gc.AddFruit(fr->Figure());
							fr->Destroy();
							auto it = std::find(objects.first[6].begin(), objects.first[6].end(), a);
							*it = nullptr;
							break;
						}
					}
				}
			}
			else
			{
				m = WIDTH - 1;
				obj = objects.second[k][m];
				//проход через тоннель
				if (obj == (*_fields.begin())->Figure())
				{
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(10);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][0] = (*_fields.begin())->Figure();
				}
				//add ghosts collision
			}
		}
		else if (key == "RIGHT")
		{
			int k = _Pacman->GetPosition().first;
			int m = _Pacman->GetPosition().second;
			if (m != WIDTH - 1)
			{
				obj = objects.second[k][m + 1];

				if (obj == (*_fields.begin())->Figure())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second + 1;
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(10);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][m - 1] = (*_fields.begin())->Figure();
				}
				//чеканные монеты
				else if (obj == (*_coins.begin())->Figure())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second + 1;
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(7);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][m - 1] = (*_fields.begin())->Figure();
					_Pacman->Eat(10);
					totalcoins++;

					for (auto a : _coins)
					{
						if (a->GetPosition() == _Pacman->GetPosition())
						{
							auto it = std::find(objects.first[1].begin(), objects.first[1].end(), a);
							(*it)->Destroy();
							objects.first[1].erase(it);
						}
					}
				}
				//energizer
				else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second + 1;
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(7);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][m - 1] = (*_fields.begin())->Figure();
					_Pacman->Eat(50);

					for (auto a : _booster)
					{
						if (a->GetPosition() == _Pacman->GetPosition())
						{
							auto it = std::find(objects.first[4].begin(), objects.first[4].end(), a);
							(*it)->Destroy();
							objects.first[4].erase(it);
						}
					}
					for (auto a : _ghosts)
					{
						a->SetStatus("FRIGHTENED");
						objects.second[a->GetPosition().first][a->GetPosition().second] = a->Figure();
					}
				}
				//стены
				else if (obj == (*_borders.begin())->Figure())
				{
					objects.second[k][m] = _Pacman->Figure();
				}
				//враги
				else if (lambdaghost())
				{
					k = _Pacman->GetPosition().first;
					m = _Pacman->GetPosition().second + 1;
					for (auto a : _ghosts)
					{
						int x = a->GetPosition().first;
						int y = a->GetPosition().second;
						a->SetSpeed(8);

						if (k == x && m == y && a->GetStatus() == 3)
						{
							_Pacman->SetPosition(k, m);
							_Pacman->SetSpeed(7);;
							objects.second[k][m] = _Pacman->Figure();
							objects.second[k][m - 1] = (*_fields.begin())->Figure();
							_Pacman->Eat(200);

							x = a->GetStartPos().first + 4;
							y = a->GetStartPos().second;
							a->SetPosition(x, y);
							a->SetStatus("CHASE");
							a->SetDoorLock(true);
							a->SetDirection("UP");
							objects.second[x][y] = a->Figure();
						}
						else if (k == x && m == y && a->GetStatus() != 3)
						{
							DecreaseHP();
							objects.second[k][m] = (*_fields.begin())->Figure();
							objects.second[k][m - 1] = (*_fields.begin())->Figure();
							int q = _Pacman->GetStartPosition().first;
							int w = _Pacman->GetStartPosition().second;
							_Pacman->SetPosition(q, w);
							_Pacman->SetSpeed(7);;
							objects.second[q][w] = _Pacman->Figure();
							for (auto a : _ghosts)
							{
								int x = a->GetStartPos().first;
								int y = a->GetStartPos().second;
								a->SetPosition(x, y);
								a->SetStatus("SCATTER");
								a->SetDoorLock(true);
								a->SetDirection("LEFT");
								objects.second[x][y] = a->Figure();
							}
						}
					}
				}
				//фрукты
				else if (lambdafruit())
				{
					for (auto a : objects.first[6])
					{
						if (a != nullptr && obj == a->Figure())
						{
							Fruits* fr = dynamic_cast<Fruits*>(a);
							k = _Pacman->GetPosition().first;
							m = _Pacman->GetPosition().second + 1;
							if (std::make_pair(k, m) != a->GetPosition()) continue;

							_Pacman->SetPosition(k, m);
							_Pacman->SetSpeed(7);;
							objects.second[k][m] = _Pacman->Figure();
							objects.second[k][m - 1] = (*_fields.begin())->Figure();
							_Pacman->Eat(fr->CalculateScore(currentlevel));
							gc.AddFruit(fr->Figure());
							fr->Destroy();
							auto it = std::find(objects.first[6].begin(), objects.first[6].end(), a);
							*it = nullptr;
							break;
						}
					}
				}
			}
			else
			{
				m = 0;
				obj = objects.second[k][m];
				//проход через тоннель
				if (obj == (*_fields.begin())->Figure())
				{
					_Pacman->SetPosition(k, m);
					_Pacman->SetSpeed(10);;
					objects.second[k][m] = _Pacman->Figure();
					objects.second[k][WIDTH - 1] = (*_fields.begin())->Figure();
				}
				//add ghosts collision
			}
		}
	}

	if (totalcoins == 70 && objects.first[6][0] == nullptr)
	{
		objects.first[6][0] = new Fruits();
		objects.first[6][0]->SetPosition(17, 13);
		objects.second[objects.first[6][0]->GetPosition().first][objects.first[6][0]->GetPosition().second] = objects.first[6][0]->Figure();
	}
	if (totalcoins == 170 && objects.first[6][1] == nullptr)
	{
		objects.first[6][1] = new Fruits();
		objects.first[6][1]->SetPosition(17, 14);
		objects.second[objects.first[6][1]->GetPosition().first][objects.first[6][1]->GetPosition().second] = objects.first[6][1]->Figure();
	}

	for (auto a : _ghosts)
	{
		a->UpdateDeltaTime();
		if (a->Update())
		{
			pair<int, int> p1 = a->GetPosition();//old position		
			//if (a->GetStatus() == 0 && !a->GetDoorLock() && p1.first == 12 && p1.second == 14)
			//{
			//	a->SetStatus("SCATTER");
			//	a->SetDoorLock(true);
			//}
			pair<int, int> p2 = a->Go(*_Pacman);//future position
			objects.second[p1.first][p1.second] = a->GetLowField();
			a->SetPosition(p2.first, p2.second);

			a->GetStatus() == 3 ? a->SetSpeed(5) : a->SetSpeed(8);

			if (a->CheckPacman(*_Pacman))
			{
				if (a->GetStatus() == 3)
				{
					int x = a->GetStartPos().first + 4;
					int y = a->GetStartPos().second;

					objects.second[p2.first][p2.second] = _Pacman->Figure();
					a->SetLowField(objects.second[x][y]);
					_Pacman->Eat(200);

					a->SetPosition(x, y);//fix start position
					a->SetStatus("CHASE");
					a->SetDoorLock(true);
					a->SetDirection("UP");
					objects.second[x][y] = a->Figure();
				}
				else
				{
					DecreaseHP();
					objects.second[p2.first][p2.second] = (*_fields.begin())->Figure();

					int q = _Pacman->GetStartPosition().first;
					int w = _Pacman->GetStartPosition().second;
					_Pacman->SetPosition(q, w);
					objects.second[q][w] = _Pacman->Figure();
					for (auto b : _ghosts)
					{
						int x = b->GetStartPos().first;
						int y = b->GetStartPos().second;
						b->SetLowField(objects.second[x][y]);
						b->SetPosition(x, y);
						b->SetStatus("SCATTER");
						b->SetDoorLock(true);
						b->SetDirection("LEFT");
						objects.second[x][y] = a->Figure();
					}
					break;
				}
			}
			else
			{
				a->SetLowField(objects.second[p2.first][p2.second]);
				objects.second[p2.first][p2.second] = a->Figure();
			}
		}
	}


	Pacman::SetCheckLevel(_coins.empty(), _booster.empty());
	//if(_booster.size() < 3)
	//Pacman::SetCheckLevel(true, true);
}

void DecreaseHP()
{
	string s = Pacman::GetHealth();
	int hp = 0;
	for (int i = 0; i < static_cast<int>(s.size()); i++)
	{
		if (s[i] == char(3))
		{
			hp++;
		}
	}
	Pacman::SetHealth(hp - 1);
}

void Draw(vector<vector<pair<char, int>>> objects)
{

	/*HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int k = 1; k < 255; k++)
	{
		SetConsoleTextAttribute(hc, k);
		std::cout << k << " Hello world!" << std::endl;
	}*/
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = { WIDTH + 1, HEIGHT + 4 };
	SMALL_RECT src = { 0,0,bufferSize.X - 1,bufferSize.Y - 1 };
	SetConsoleWindowInfo(hConsole, true, &src);
	SetConsoleScreenBufferSize(hConsole, bufferSize);
	SetConsoleTitle("Pac-man");
	bufferSize.X = 0;
	bufferSize.Y = 0;
	SetConsoleCursorPosition(hConsole, bufferSize);

	for (int i = 0; i < (int)objects.size(); i++)
	{
		for (int j = 0; j < (int)objects[i].size(); j++)
		{
			SetConsoleTextAttribute(hConsole, objects[i][j].second);
			cout << objects[i][j].first;
		}
		cout << endl;
	}

	cout << endl;
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Level: ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << (int)currentlevel;
	SetConsoleTextAttribute(hConsole, 10);
	cout << "\t" << gc.GetLastFruits();

	cout << endl;
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Lives: ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << Pacman::GetHealth();

	cout << endl;
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Score: ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << Pacman::GetScore();
}

void GlobalCleaning(pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>>& objects)
{
	for (int i = 0; i < static_cast<int>(objects.first.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(objects.first[i].size()); j++)
		{
			if (i != 3 && i != 6)
			{
				objects.first[i][j]->Destroy();
			}
		}
	}
	objects.first.clear();
	objects.second.clear();
}
