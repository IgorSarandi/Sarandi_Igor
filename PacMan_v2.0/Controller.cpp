#include "Controller.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include <thread>
#include <algorithm>
#include <unistd.h>

Controller::Controller(int sizeX, int sizeY) : BaseController(sizeX, sizeY)
{
	CreateMap otherMap(sizeX, sizeY);
	crMap = &otherMap;


	int sqr = X_SIZE * Y_SIZE;
	_borders.reserve(sqr);
	_coins.reserve(sqr);
	_fields.reserve(sqr);
}

Controller::~Controller()
{
}

void Controller::Start()
{
	std::vector<std::vector<int>> map(crMap->globalMap());

	gameObjects_.resize(X_SIZE);
	for (int i = 0; i < static_cast<int>(gameObjects_.size()); i++)
	{
		gameObjects_[i].resize(Y_SIZE);

		for (int j = 0; j < static_cast<int>(gameObjects_[i].size()); j++)
		{
			int tmp = map[i][j];
			if (tmp == 0)
			{
				_borders.push_back(new Border);
				_borders.back()->setPosition(i, j);
				gameObjects_[i][j] = _borders.back()->Figure();
			}
			else if (tmp == 1)
			{
				_fields.push_back(new Field);
				_fields.back()->setPosition(i, j);
				_coins.push_back(new Coins);
				_coins.back()->setPosition(i, j);
				gameObjects_[i][j] = _coins.back()->Figure();
			}
			else if (tmp == 2)
			{
				_fields.push_back(new Field);
				_fields.back()->setPosition(i, j);
				gameObjects_[i][j] = _fields.back()->Figure();
			}
		}
	}

	Pacman* pacman = new Pacman();
	pacman->setStartPosition(23, 14);
	pacman->setPosition(pacman->getStartPosition().first, pacman->getStartPosition().second);
	pacman->setSpeed(10);
	pacman->setDirection("UP");

	if (gameObjects_[pacman->getPosition().first][pacman->getPosition().second] == (*_coins.begin())->Figure())
	{
		for (auto a : _coins)
		{
			if (a->getPosition() == pacman->getPosition())
			{
				auto it = std::find(_coins.begin(), _coins.end(), a);
				(*it)->Destroy();
				_coins.erase(it);
			}
		}
	}
	gameObjects_[pacman->getPosition().first][pacman->getPosition().second] = pacman->Figure();
	_pacman.push_back(pacman);


	Booster* boost = new Booster[4]();
	boost[0].setPosition(3, 1);
	boost[1].setPosition(3, 26);
	boost[2].setPosition(23, 1);
	boost[3].setPosition(23, 26);

	for (int i = 0; i < 4; i++)
	{
		if (gameObjects_[boost[i].getPosition().first][boost[i].getPosition().second] == (*_coins.begin())->Figure())
		{
			for (auto a : _coins)
			{
				if (a->getPosition() == boost[i].getPosition())
				{
					auto it = std::find(_coins.begin(), _coins.end(), a);
					(*it)->Destroy();
					_coins.erase(it);
				}
			}
		}
		gameObjects_[boost[i].getPosition().first][boost[i].getPosition().second] = boost[i].Figure();
		_booster.push_back(&boost[i]);
	}

	Blinky* blinky = new Blinky();
	blinky->setSpeed(6);
	blinky->setDoorPassed(true);
	blinky->setStartPosition(11, 14);
	blinky->setPosition(blinky->getStartPosition().first, blinky->getStartPosition().second);
	blinky->UpdateLand(crMap->globalMap());
	blinky->setStatus("SCATTER");
	blinky->setDirection("LEFT");
	blinky->setLowField((*_fields.begin())->Figure());
	blinky->setTargetField(std::make_pair(0, Y_SIZE - 1));
	gameObjects_[blinky->getPosition().first][blinky->getPosition().second] = blinky->Figure();
	_ghosts.push_back(blinky);

	Pinky* pinky = new Pinky();
	pinky->setSpeed(6);
	pinky->setDoorPassed(false);
	pinky->setStartPosition(14, 14);
	pinky->setPosition(pinky->getStartPosition().first, pinky->getStartPosition().second);
	pinky->UpdateLand(crMap->globalMap());
	pinky->setStatus("SCATTER");
	pinky->setDirection("LEFT");
	pinky->setLowField((*_fields.begin())->Figure());
	pinky->setTargetField(std::make_pair(0, 0));
	gameObjects_[pinky->getPosition().first][pinky->getPosition().second] = pinky->Figure();
	_ghosts.push_back(pinky);

	Inky* inky = new Inky(*blinky);
	inky->setSpeed(6);
	inky->setDoorPassed(false);
	inky->setStartPosition(14, 11);
	inky->setPosition(inky->getStartPosition().first, inky->getStartPosition().second);
	inky->UpdateLand(crMap->globalMap());
	inky->setStatus("SCATTER");
	inky->setDirection("LEFT");
	inky->setLowField((*_fields.begin())->Figure());
	inky->setTargetField(std::make_pair(X_SIZE - 1, Y_SIZE - 1));
	gameObjects_[inky->getPosition().first][inky->getPosition().second] = inky->Figure();
	_ghosts.push_back(inky);

	Clyde* clyde = new Clyde();
	clyde->setSpeed(6);
	clyde->setDoorPassed(false);
	clyde->setStartPosition(14, 16);
	clyde->setPosition(clyde->getStartPosition().first, clyde->getStartPosition().second);
	clyde->UpdateLand(crMap->globalMap());
	clyde->setStatus("SCATTER");
	clyde->setDirection("LEFT");
	clyde->setLowField((*_fields.begin())->Figure());
	clyde->setTargetField(std::make_pair(X_SIZE - 1, 0));
	gameObjects_[clyde->getPosition().first][clyde->getPosition().second] = clyde->Figure();
	_ghosts.push_back(clyde);

	_fruits.resize(2);

	info->remainboosters = static_cast<int>(_booster.size());
	info->remaincoins = static_cast<int>(_coins.size());
	info->totalcoins = 0;

	inky->setCoins(*info);
	clyde->setCoins(*info);
}

void Controller::CheckAlive()
{
	if (info->getHealth().empty())
	{
        reset_();
		clrscr();
		std::cerr << "YOU LOSE" << std::endl;
		std::cerr << "Your SCORE is: " << info->getScore() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));//0.5 seconds
		std::cin.get();
		quick_exit(1);
	}
}

void Controller::Logic()
{
	this->PacmanActions();
	this->UpdateFruits();
	this->GhostsActions();
	this->info->setCheckLevel(_coins.empty(), _booster.empty());
	/* for easy game
	if(_booster.size() < 1)
		this->info->setCheckLevel(true, true);
	 */
}

void Controller::PacmanActions()
{
	if (key == -1)
	{
		quick_exit(0);
	}
	std::pair<char, int> obj;

	auto lambdafruit = [&fruits = _fruits]()
	{
		for (auto a : fruits)
		{
			if (a != nullptr)
				return true;
		}
		return false;
	};
	auto lambdaghost = [&ghosts = _ghosts, &obj]()
	{
		for (auto a : ghosts)
		{
			if (obj == a->Figure())
				return true;
		}
		return false;
	};

	(*_pacman.begin())->UpdateDeltaTime();
	if ((*_pacman.begin())->Update())
	{
		const int dx[4] = { -1, 1, 0, 0 };
		const int dy[4] = { 0, 0, -1, 1 };
		const std::string dirs[4] = {"UP","DOWN","LEFT","RIGHT"};


		const int k = (*_pacman.begin())->getPosition().first;
		const int m = (*_pacman.begin())->getPosition().second;

		const int dir = (*_pacman.begin())->getDirection() + 4;
		const std::map<std::string, int>& all_directions = (*_pacman.begin())->getAllDirections();

		for (int i = 0; i < 8; i++) //4 key + 4 directions
		{
			if (i == key || i == dir)
			{
				if (i == 2 || i == 6) //LEFT
				{
					if (m == 0)
					{
						int new_m = Y_SIZE - 1;
						obj = gameObjects_[k][new_m];
						
						if (obj == (*_fields.begin())->Figure()) //go through the tunnel
						{
							this->PacmanUpdate(k, m, k, new_m, 10);
						}
						return;
					}
				}
				if (i == 3 || i == 7) //RIGHT
				{
					if (m == Y_SIZE - 1)
					{
						int new_m = 0;
						obj = gameObjects_[k][new_m];
						
						if (obj == (*_fields.begin())->Figure()) //go through the tunnel
						{
							this->PacmanUpdate(k, m, k, new_m, 10);
						}
						return;
					}
				}
				
				int new_i = i > 3 ? i - 4 : i;
				const int k1 = k + dx[new_i];
				const int m1 = m + dy[new_i];
				obj = gameObjects_[k1][m1];
				
				auto lambda_new_dir = [&new_i, &all_directions]()
				{
					for (auto a : all_directions)
					{
						if (a.second == new_i)
						{
							return a.first;
						}
					}
					return (*all_directions.begin()).first;
				};

				if (obj == (*_borders.begin())->Figure()) //walls
				{
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					continue;
				}
				else
				{
					(*_pacman.begin())->setDirection(lambda_new_dir());

					if (obj == (*_fields.begin())->Figure()) //fields
					{
						this->PacmanUpdate(k, m, k1, m1, 10);
					}
					else if (obj == (*_coins.begin())->Figure()) //coins
					{
						this->PacmanUpdate(k, m, k1, m1, 10);
						info->Eat(10);
						info->totalcoins++;
						info->remaincoins--;

						for (auto it = _coins.begin(); it != _coins.end(); it++)
						{
							if ((*it)->getPosition() == std::make_pair(k1, m1))
							{
								(*it)->Destroy();
								_coins.erase(it);
								break;
							}
						}
					}
					else if (!_booster.empty() && obj == (*_booster.begin())->Figure()) //energizer
					{
						this->PacmanUpdate(k, m, k1, m1, 10);
						info->Eat(50);
						info->remainboosters--;

						for (auto it = _booster.begin(); it != _booster.end(); it++)
						{
							if ((*it)->getPosition() == std::make_pair(k1, m1))
							{
								(*it)->Destroy();
								_booster.erase(it);
								break;
							}
						}
						for (auto it = _ghosts.begin(); it != _ghosts.end(); it++)
						{
							(*it)->setStatus("FRIGHTENED");
							gameObjects_[(*it)->getPosition().first][(*it)->getPosition().second] = (*it)->Figure();
						}
					}
					else if (lambdaghost()) // ghosts
					{
						for (auto a : _ghosts)
						{
							int gh_x = a->getPosition().first;
							int gh_y = a->getPosition().second;
							a->setSpeed(6);

							if (k1 == gh_x && m1 == gh_y)
							{
								if (a->getStatus() == 2) //FRIGHTENED
								{
									this->PacmanUpdate(k, m, k1, m1, 10);
									info->Eat(200);

									if(a->getLowField() == (*_coins.begin())->Figure())
									{
										try 
										{
											for (auto it = _coins.begin(); it != _coins.end(); it++)
											{
												if ((*it)->getPosition() == std::make_pair(k1, m1))
												{
													(*it)->Destroy();
													_coins.erase(it);
													break;
												}
											}
											info->Eat(10);
											info->totalcoins++;
											info->remaincoins--;
										}
										catch (...)
										{
										}
									}

									gh_x = a->getDoorPosition().first + 3;
									gh_y = a->getDoorPosition().second;
									a->setLowField((*_fields.begin())->Figure());
									a->setPosition(gh_x, gh_y);
									a->setStatus("CHASE");
									a->setDoorPassed(false);
									a->setDirection("UP");
									gameObjects_[gh_x][gh_y] = a->Figure();
								}
								else
								{
									info->DecreaseHP();
									gameObjects_[k1][m1] = (*_fields.begin())->Figure();
									gameObjects_[k][m] = (*_fields.begin())->Figure();
									int pac_x = (*_pacman.begin())->getStartPosition().first;
									int pac_y = (*_pacman.begin())->getStartPosition().second;
									(*_pacman.begin())->setPosition(pac_x, pac_y);
									(*_pacman.begin())->setSpeed(10);
									gameObjects_[pac_x][pac_y] = (*_pacman.begin())->Figure();

									for (auto b : _ghosts)
									{
										gh_x = b->getStartPosition().first;
										gh_y = b->getStartPosition().second;
										int gh_x1 = b->getPosition().first;
										int gh_y1 = b->getPosition().second;
										b->setLowField((*_fields.begin())->Figure());
										gameObjects_[gh_x1][gh_y1] = b->getLowField();
										b->setPosition(gh_x, gh_y);

										if (b->getPrevState() == 0) //CHASE
											b->setStatus("SCATTER");
										else
											b->setStatus("CHASE");

										b->setChaseCount(4);
										b->setDoorPassed(false);
										b->setDirection("LEFT");
										gameObjects_[gh_x][gh_y] = b->Figure();
									}
									break;
								}
							}
						}
					}				
					else if (lambdafruit()) //fruits
					{
						for (auto it = _fruits.begin(); it != _fruits.end(); it++)
						{
							if ((*it) != nullptr && obj == (*it)->Figure())
							{
								if(std::make_pair(k1, m1) != (*it)->getPosition()) continue;

								this->PacmanUpdate(k, m, k1, m1, 10);
								info->Eat((*it)->CalculateScore(info->currentlevel));
								info->addFruit((*it)->Figure());
								(*it)->Destroy();
								*it = nullptr;
								break;
							}
						}
					}
				}
			}
			else
			{
				continue;
			}
			return;
		}
	}
}

void Controller::PacmanUpdate(int old_x, int old_y, int new_x, int new_y, int speed)
{
	(*_pacman.begin())->setPosition(new_x, new_y);
	(*_pacman.begin())->setSpeed(speed);
	gameObjects_[new_x][new_y] = (*_pacman.begin())->Figure();
	gameObjects_[old_x][old_y] = (*_fields.begin())->Figure();
}

void Controller::GhostsActions()
{
	for (auto a : _ghosts)
	{
		a->UpdateDeltaTime();
		if (a->StartCondition(*info))
		{
			if (a->Update())
			{
				a->CheckMode();
				std::pair<int, int> p1 = a->getPosition();//old position
				a->setTarget((*_pacman.begin()));
				std::pair<int, int> p2 = a->Go();//future position

				gameObjects_[p1.first][p1.second] = a->getLowField();
				int status = a->getStatus();
				a->setPosition(p2.first, p2.second);

				status == 2 ? a->setSpeed(3) : a->setSpeed(6);

				if (a->CheckPacman((*_pacman.begin())->getPosition()))
				{
					if (a->getStatus() == 2)
					{
						int gh_x = a->getDoorPosition().first + 3;
						int gh_y = a->getDoorPosition().second;

						gameObjects_[p2.first][p2.second] = (*_pacman.begin())->Figure();
						info->Eat(200);//fix

						if (a->getLowField() == (*_coins.begin())->Figure())
						{
							try
							{
								for (auto b : _coins)
								{
									if (b->getPosition() == std::make_pair(p2.first, p2.second))
									{
										auto it = std::find(_coins.begin(), _coins.end(), b);
										(*it)->Destroy();
										_coins.erase(it);
									}
								}
								info->Eat(10);
								info->totalcoins++;
								info->remaincoins--;
							}
							catch (...)
							{
							}
						}

						a->setLowField((*_fields.begin())->Figure());
						a->setPosition(gh_x, gh_y);
						a->setStatus("CHASE");
						a->setDoorPassed(false);
						a->setDirection("UP");
						gameObjects_[gh_x][gh_y] = a->Figure();
					}
					else
					{
						info->DecreaseHP();
						gameObjects_[p2.first][p2.second] = (*_fields.begin())->Figure();

						int q = (*_pacman.begin())->getStartPosition().first;
						int w = (*_pacman.begin())->getStartPosition().second;
						(*_pacman.begin())->setPosition(q, w);
						gameObjects_[q][w] = (*_pacman.begin())->Figure();
						for (auto b : _ghosts)
						{
							int x = b->getStartPosition().first;
							int y = b->getStartPosition().second;
							int x1 = b->getPosition().first;
							int y1 = b->getPosition().second;
							b->setLowField((*_fields.begin())->Figure());
							gameObjects_[x1][y1] = b->getLowField();

							b->setPosition(x, y);

							if(b->getPrevState() == 0)
								b->setStatus("SCATTER");
							else
								b->setStatus("CHASE");

							b->setChaseCount(4);
							b->setDoorPassed(false);
							b->setDirection("LEFT");
							gameObjects_[x][y] = a->Figure();
						}
						break;
					}
				}
				else
				{
					bool flag = false;
					for (auto it = _coins.begin(); it != _coins.end(); it++)
					{
						if ((*it)->getPosition() == std::make_pair(p2.first, p2.second))
						{
							a->setLowField((*it)->Figure());
							flag = true;
							break;
						}
					}
					if (!flag)
					{
						for (auto it = _fruits.begin(); it != _fruits.end(); it++)
						{
							if ((*it) != nullptr)
							{
								if ((*it)->getPosition() == std::make_pair(p2.first, p2.second))
								{
									a->setLowField((*it)->Figure());
									flag = true;
									break;
								}
							}
						}
					}
					if (!flag)
					{
						if (a->getDoorPosition() == std::make_pair(p2.first, p2.second))
						{
							a->setLowField(a->getDoorFigure());
							flag = true;
						}
					}
					if (!flag)
					{
						for (auto it = _booster.begin(); it != _booster.end(); it++)
						{
							if ((*it)->getPosition() == std::make_pair(p2.first, p2.second))
							{
								a->setLowField((*it)->Figure());
								flag = true;
								break;
							}
						}
					}
					if (!flag)
					{
						for (auto it = _fields.begin(); it != _fields.end(); it++)
						{
							if ((*it)->getPosition() == std::make_pair(p2.first, p2.second))
							{
								a->setLowField((*it)->Figure());
								flag = true;
								break;
							}
						}
					}

					gameObjects_[p2.first][p2.second] = a->Figure();
				}
			}
		}
		else
		{
			if (a->Update())
			{
				std::pair<int, int> p1 = a->getPosition();//old position
				a->HomeMode();
				std::pair<int, int> p2 = a->getPosition();//future position
				gameObjects_[p1.first][p1.second] = (*_fields.begin())->Figure();
				gameObjects_[p2.first][p2.second] = a->Figure();
			}
		}
	}
}

void Controller::UpdateFruits()
{
	if (info->totalcoins == 70 && _fruits[0] == nullptr)
	{
		_fruits[0] = new Fruits();
		_fruits[0]->setPosition(17, 13);
		gameObjects_[_fruits[0]->getPosition().first][_fruits[0]->getPosition().second] = _fruits[0]->Figure();
	}
	if (info->totalcoins == 170 && _fruits[1] == nullptr)
	{
		_fruits[1] = new Fruits();
		_fruits[1]->setPosition(17, 14);
		gameObjects_[_fruits[1]->getPosition().first][_fruits[1]->getPosition().second] = _fruits[1]->Figure();
	}
}

void Controller::Run()
{
	//Menu();
	for (; info->currentlevel < info->MAX_LEVEL; info->currentlevel++)
	{
        clrscr();//clear screen
		this->Start();
		while (!info->getCheckLevel())
		{
			CheckAlive();
            Input();
			key = old_key;
			this->Logic();
			this->Draw(gameObjects_);
		}
		GlobalCleaning();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));//1 second
	}
}

void Controller::GlobalCleaning()
{
	gameObjects_.clear();
	_borders.clear();
	_coins.clear();
	_fields.clear();
	_pacman.clear();
	_booster.clear();
	_fruits.clear();
	_ghosts.clear();
	info->setCheckLevel(false, false);
}
