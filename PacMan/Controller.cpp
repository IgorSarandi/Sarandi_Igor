#include "Controller.h"
#include "Blinky.h"
#include "Pinky.h"
#include <thread>


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
	blinky->setSpeed(8);
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
	pinky->setSpeed(8);
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


	_fruits.resize(2);

	info_->remainboosters = static_cast<int>(_booster.size());
	info_->remaincoins = static_cast<int>(_coins.size());
	info_->totalcoins = 0;
}

void Controller::CheckAlive()
{
	if (info_->getHealth().empty())
	{
		system("cls");
		std::cerr << "YOU LOSE" << std::endl;
		std::cerr << "Your SCORE is: " << info_->getScore() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));//0.5 seconds
		system("pause");
		quick_exit(1);
	}
}

void Controller::Logic()
{
	this->PacmanActions();
	this->UpdateFruits();
	this->GhostsActions();
	this->info_->setCheckLevel(_coins.empty(), _booster.empty());
	/*if(_booster.size() < 1)
		this->info_->setCheckLevel(true, true);*/
}

void Controller::PacmanActions()
{
	std::pair<char, int> obj;

	std::vector<Border*> borders(_borders);//0
	std::vector<Coins*> coins(_coins);//1
	std::vector<Field*> fields(_fields);//2
	std::vector<Pacman*> pacman(_pacman);//3
	std::vector<Booster*> booster(_booster);//4
	std::vector<Fruits*> fruits(_fruits);//5
	std::vector<Ghosts*> ghosts(_ghosts);//6

	auto lambdafruit = [fruits]()
	{
		for (auto a : fruits)
		{
			if (a != nullptr)
				return true;
		}
		return false;
	};
	auto lambdaghost = [ghosts, &obj]()
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
		if (key == "UP")
		{
			int k = (*_pacman.begin())->getPosition().first;
			int m = (*_pacman.begin())->getPosition().second;
			obj = gameObjects_[k - 1][m];

			//стены
			if (obj == (*_borders.begin())->Figure())
			{
				gameObjects_[k][m] = (*_pacman.begin())->Figure();
			}
			else
			{
				(*_pacman.begin())->setDirection(key);

				//поле
				if (obj == (*_fields.begin())->Figure())
				{
					k = (*_pacman.begin())->getPosition().first - 1;
					m = (*_pacman.begin())->getPosition().second;
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(10);
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
				}
				//чеканные монеты
				else if (obj == (*_coins.begin())->Figure())
				{
					k = (*_pacman.begin())->getPosition().first - 1;
					m = (*_pacman.begin())->getPosition().second;
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(7);
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
					info_->Eat(10);
					info_->totalcoins++;
					info_->remaincoins--;

					for (auto a : _coins)
					{
						if (a->getPosition() == (*_pacman.begin())->getPosition())
						{
							auto it = std::find(_coins.begin(), _coins.end(), a);
							(*it)->Destroy();
							_coins.erase(it);
						}
					}
				}
				//energizer
				else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
				{
					k = (*_pacman.begin())->getPosition().first - 1;
					m = (*_pacman.begin())->getPosition().second;
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(7);;
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
					info_->Eat(50);
					info_->remainboosters--;

					for (auto a : _booster)
					{
						if (a->getPosition() == (*_pacman.begin())->getPosition())
						{
							auto it = std::find(_booster.begin(), _booster.end(), a);
							(*it)->Destroy();
							_booster.erase(it);
						}
					}
					for (auto a : _ghosts)
					{
						a->setStatus("FRIGHTENED");
						gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
					}
				}
				//враги
				else if (lambdaghost())
				{
					k = (*_pacman.begin())->getPosition().first - 1;
					m = (*_pacman.begin())->getPosition().second;
					for (auto a : _ghosts)
					{
						int x = a->getPosition().first;
						int y = a->getPosition().second;
						a->setSpeed(8);

						if (k == x && m == y && a->getStatus() == 2)
						{
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(10);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
							info_->Eat(200);

							x = a->getStartPosition().first + 4;
							y = a->getStartPosition().second;
							a->setLowField((*_fields.begin())->Figure());
							a->setPosition(x, y);
							a->setStatus("CHASE");
							a->setDoorPassed(false);
							a->setDirection("UP");
							gameObjects_[x][y] = a->Figure();
						}
						else if (k == x && m == y && a->getStatus() != 2)
						{
							info_->DecreaseHP();
							gameObjects_[k][m] = (*_fields.begin())->Figure();
							gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
							int q = (*_pacman.begin())->getStartPosition().first;
							int w = (*_pacman.begin())->getStartPosition().second;
							(*_pacman.begin())->setPosition(q, w);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[q][w] = (*_pacman.begin())->Figure();
							for (auto a : _ghosts)
							{
								int x = a->getStartPosition().first;
								int y = a->getStartPosition().second;
								int x1 = a->getPosition().first;
								int y1 = a->getPosition().second;
								gameObjects_[x1][y1] = a->getLowField();
								a->setPosition(x, y);

								if (a->getPrevState() == 0)
									a->setStatus("SCATTER");
								else
									a->setStatus("CHASE");

								a->setChaseCount(4);
								a->setDoorPassed(false);
								a->setDirection("LEFT");
								gameObjects_[x][y] = a->Figure();
							}
						}
					}
				}
				//фрукты
				else if (lambdafruit())
				{
					for (auto a : _fruits)
					{
						if (a != nullptr && obj == a->Figure())
						{
							Fruits* fr = dynamic_cast<Fruits*>(a);
							k = (*_pacman.begin())->getPosition().first - 1;
							m = (*_pacman.begin())->getPosition().second;
							if (std::make_pair(k, m) != a->getPosition()) continue;

							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
							info_->Eat(fr->CalculateScore(info_->currentlevel));
							info_->addFruit(fr->Figure());
							fr->Destroy();
							auto it = std::find(_fruits.begin(), _fruits.end(), a);
							*it = nullptr;
							break;
						}
					}
				}
				return;
			}
		}
		if (key == "DOWN")
		{
			int k = (*_pacman.begin())->getPosition().first;
			int m = (*_pacman.begin())->getPosition().second;
			obj = gameObjects_[k + 1][m];

			//стены
			if (obj == (*_borders.begin())->Figure())
			{
				gameObjects_[k][m] = (*_pacman.begin())->Figure();
			}
			else
			{
				(*_pacman.begin())->setDirection(key);
				if (obj == (*_fields.begin())->Figure())
				{
					k = (*_pacman.begin())->getPosition().first + 1;
					m = (*_pacman.begin())->getPosition().second;
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(10);;
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
				}
				//чеканные монеты
				else if (obj == (*_coins.begin())->Figure())
				{
					k = (*_pacman.begin())->getPosition().first + 1;
					m = (*_pacman.begin())->getPosition().second;
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(7);;
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
					info_->Eat(10);
					info_->totalcoins++;
					info_->remaincoins--;

					for (auto a : _coins)
					{
						if (a->getPosition() == (*_pacman.begin())->getPosition())
						{
							auto it = std::find(_coins.begin(), _coins.end(), a);
							(*it)->Destroy();
							_coins.erase(it);
						}
					}
				}
				//energizer
				else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
				{
					k = (*_pacman.begin())->getPosition().first + 1;
					m = (*_pacman.begin())->getPosition().second;
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(7);;
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
					info_->Eat(50);
					info_->remainboosters--;

					for (auto a : _booster)
					{
						if (a->getPosition() == (*_pacman.begin())->getPosition())
						{
							auto it = std::find(_booster.begin(), _booster.end(), a);
							(*it)->Destroy();
							_booster.erase(it);
						}
					}
					for (auto a : _ghosts)
					{
						a->setStatus("FRIGHTENED");
						gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
					}
				}
				//враги
				else if (lambdaghost())
				{
					k = (*_pacman.begin())->getPosition().first + 1;
					m = (*_pacman.begin())->getPosition().second;
					for (auto a : _ghosts)
					{
						int x = a->getPosition().first;
						int y = a->getPosition().second;
						a->setSpeed(8);

						if (k == x && m == y && a->getStatus() == 2)
						{
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(10);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
							info_->Eat(200);

							x = a->getStartPosition().first + 4;
							y = a->getStartPosition().second;
							a->setLowField((*_fields.begin())->Figure());
							a->setPosition(x, y);
							a->setStatus("CHASE");
							a->setDoorPassed(false);
							a->setDirection("UP");
							gameObjects_[x][y] = a->Figure();
						}
						else if (k == x && m == y && a->getStatus() != 2)
						{
							info_->DecreaseHP();
							gameObjects_[k][m] = (*_fields.begin())->Figure();
							gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
							int q = (*_pacman.begin())->getStartPosition().first;
							int w = (*_pacman.begin())->getStartPosition().second;
							(*_pacman.begin())->setPosition(q, w);
							(*_pacman.begin())->setSpeed(7);
							gameObjects_[q][w] = (*_pacman.begin())->Figure();
							for (auto a : _ghosts)
							{
								int x = a->getStartPosition().first;
								int y = a->getStartPosition().second;
								int x1 = a->getPosition().first;
								int y1 = a->getPosition().second;
								gameObjects_[x1][y1] = a->getLowField();
								a->setPosition(x, y);

								if (a->getPrevState() == 0)
									a->setStatus("SCATTER");
								else
									a->setStatus("CHASE");

								a->setChaseCount(4);
								a->setDoorPassed(false);
								a->setDirection("LEFT");
								gameObjects_[x][y] = a->Figure();
							}
						}
					}
				}
				//фрукты
				else if (lambdafruit())
				{
					for (auto a : _fruits)
					{
						if (a != nullptr && obj == a->Figure())
						{
							Fruits* fr = dynamic_cast<Fruits*>(a);
							k = (*_pacman.begin())->getPosition().first + 1;
							m = (*_pacman.begin())->getPosition().second;
							if (std::make_pair(k, m) != a->getPosition()) continue;

							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
							info_->Eat(fr->CalculateScore(info_->currentlevel));
							info_->addFruit(fr->Figure());
							fr->Destroy();
							auto it = std::find(_fruits.begin(), _fruits.end(), a);
							*it = nullptr;
							break;
						}
					}
				}
				return;
			}
		}
		if (key == "LEFT")
		{
			int k = (*_pacman.begin())->getPosition().first;
			int m = (*_pacman.begin())->getPosition().second;

			if (m)
			{
				obj = gameObjects_[k][m - 1];

				//стены
				if (obj == (*_borders.begin())->Figure())
				{
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
				}
				else
				{
					(*_pacman.begin())->setDirection(key);
					//проход по полям
					if (obj == (*_fields.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second - 1;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(10);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
					}
					//чеканные монеты
					else if (obj == (*_coins.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second - 1;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
						info_->Eat(10);
						info_->totalcoins++;
						info_->remaincoins--;

						for (auto a : _coins)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_coins.begin(), _coins.end(), a);
								(*it)->Destroy();
								_coins.erase(it);
							}
						}
					}
					//energizer
					else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second - 1;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
						info_->Eat(50);
						info_->remainboosters--;

						for (auto a : _booster)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_booster.begin(), _booster.end(), a);
								(*it)->Destroy();
								_booster.erase(it);
							}
						}
						for (auto a : _ghosts)
						{
							a->setStatus("FRIGHTENED");
							gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
						}
					}
					//враги
					else if (lambdaghost())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second - 1;
						for (auto a : _ghosts)
						{
							int x = a->getPosition().first;
							int y = a->getPosition().second;
							a->setSpeed(8);

							if (k == x && m == y && a->getStatus() == 2)
							{
								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(10);
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
								info_->Eat(200);

								x = a->getStartPosition().first + 4;
								y = a->getStartPosition().second;
								a->setLowField((*_fields.begin())->Figure());
								a->setPosition(x, y);
								a->setStatus("CHASE");
								a->setDoorPassed(false);
								a->setDirection("UP");
								gameObjects_[x][y] = a->Figure();
							}
							else if (k == x && m == y && a->getStatus() != 2)
							{
								info_->DecreaseHP();
								gameObjects_[k][m] = (*_fields.begin())->Figure();
								gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
								int q = (*_pacman.begin())->getStartPosition().first;
								int w = (*_pacman.begin())->getStartPosition().second;
								(*_pacman.begin())->setPosition(q, w);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[q][w] = (*_pacman.begin())->Figure();
								for (auto a : _ghosts)
								{
									int x = a->getStartPosition().first;
									int y = a->getStartPosition().second;
									int x1 = a->getPosition().first;
									int y1 = a->getPosition().second;
									gameObjects_[x1][y1] = a->getLowField();
									a->setPosition(x, y);

									if (a->getPrevState() == 0)
										a->setStatus("SCATTER");
									else
										a->setStatus("CHASE");

									a->setChaseCount(4);
									a->setDoorPassed(false);
									a->setDirection("LEFT");
									gameObjects_[x][y] = a->Figure();
								}
							}
						}
					}
					//фрукты
					else if (lambdafruit())
					{
						for (auto a : _fruits)
						{
							if (a != nullptr && obj == a->Figure())
							{
								Fruits* fr = dynamic_cast<Fruits*>(a);
								k = (*_pacman.begin())->getPosition().first;
								m = (*_pacman.begin())->getPosition().second - 1;
								if (std::make_pair(k, m) != a->getPosition()) continue;

								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
								info_->Eat(fr->CalculateScore(info_->currentlevel));
								info_->addFruit(fr->Figure());
								fr->Destroy();
								auto it = std::find(_fruits.begin(), _fruits.end(), a);
								*it = nullptr;
								break;
							}
						}
					}
					return;
				}
			}
			else
			{
				m = Y_SIZE - 1;
				obj = gameObjects_[k][m];
				//проход через тоннель
				if (obj == (*_fields.begin())->Figure())
				{
					(*_pacman.begin())->setDirection(key);
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(10);;
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k][0] = (*_fields.begin())->Figure();
				}
				return;
				//add ghosts collision
			}
		}
		if (key == "RIGHT")
		{
			int k = (*_pacman.begin())->getPosition().first;
			int m = (*_pacman.begin())->getPosition().second;
			if (m != Y_SIZE - 1)
			{
				obj = gameObjects_[k][m + 1];

				//стены
				if (obj == (*_borders.begin())->Figure())
				{
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
				}
				else
				{

					(*_pacman.begin())->setDirection(key);
					if (obj == (*_fields.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second + 1;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(10);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
					}
					//чеканные монеты
					else if (obj == (*_coins.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second + 1;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
						info_->Eat(10);
						info_->totalcoins++;
						info_->remaincoins--;

						for (auto a : _coins)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_coins.begin(), _coins.end(), a);
								(*it)->Destroy();
								_coins.erase(it);
							}
						}
					}
					//energizer
					else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second + 1;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
						info_->Eat(50);
						info_->remainboosters--;

						for (auto a : _booster)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_booster.begin(), _booster.end(), a);
								(*it)->Destroy();
								_booster.erase(it);
							}
						}
						for (auto a : _ghosts)
						{
							a->setStatus("FRIGHTENED");
							gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
						}
					}
					//враги
					else if (lambdaghost())
					{
						k = (*_pacman.begin())->getPosition().first;
						m = (*_pacman.begin())->getPosition().second + 1;
						for (auto a : _ghosts)
						{
							int x = a->getPosition().first;
							int y = a->getPosition().second;
							a->setSpeed(8);

							if (k == x && m == y && a->getStatus() == 2)
							{
								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(7);
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
								info_->Eat(200);

								x = a->getStartPosition().first + 4;
								y = a->getStartPosition().second;
								a->setLowField((*_fields.begin())->Figure());
								a->setPosition(x, y);
								a->setStatus("CHASE");
								a->setDoorPassed(false);
								a->setDirection("UP");
								gameObjects_[x][y] = a->Figure();
							}
							else if (k == x && m == y && a->getStatus() != 2)
							{
								info_->DecreaseHP();
								gameObjects_[k][m] = (*_fields.begin())->Figure();
								gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
								int q = (*_pacman.begin())->getStartPosition().first;
								int w = (*_pacman.begin())->getStartPosition().second;
								(*_pacman.begin())->setPosition(q, w);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[q][w] = (*_pacman.begin())->Figure();
								for (auto a : _ghosts)
								{
									int x = a->getStartPosition().first;
									int y = a->getStartPosition().second;
									int x1 = a->getPosition().first;
									int y1 = a->getPosition().second;
									gameObjects_[x1][y1] = a->getLowField();
									a->setPosition(x, y);

									if (a->getPrevState() == 0)
										a->setStatus("SCATTER");
									else
										a->setStatus("CHASE");

									a->setChaseCount(4);
									a->setDoorPassed(false);
									a->setDirection("LEFT");
									gameObjects_[x][y] = a->Figure();
								}
							}
						}
					}
					//фрукты
					else if (lambdafruit())
					{
						for (auto a : _fruits)
						{
							if (a != nullptr && obj == a->Figure())
							{
								Fruits* fr = dynamic_cast<Fruits*>(a);
								k = (*_pacman.begin())->getPosition().first;
								m = (*_pacman.begin())->getPosition().second + 1;
								if (std::make_pair(k, m) != a->getPosition()) continue;

								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
								info_->Eat(fr->CalculateScore(info_->currentlevel));
								info_->addFruit(fr->Figure());
								fr->Destroy();
								auto it = std::find(_fruits.begin(), _fruits.end(), a);
								*it = nullptr;
								break;
							}
						}
					}
					return;
				}
			}
			else
			{
				m = 0;
				obj = gameObjects_[k][m];
				//проход через тоннель
				if (obj == (*_fields.begin())->Figure())
				{
					(*_pacman.begin())->setDirection(key);
					(*_pacman.begin())->setPosition(k, m);
					(*_pacman.begin())->setSpeed(10);;
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
					gameObjects_[k][Y_SIZE - 1] = (*_fields.begin())->Figure();
				}
				return;
				//add ghosts collision
			}
		}
		if (key == "END")
		{
			quick_exit(0);
		}

		{
			int dir = (*_pacman.begin())->getDirection();

			if (dir == 0)//up
			{
				int k = (*_pacman.begin())->getPosition().first;
				int m = (*_pacman.begin())->getPosition().second;
				obj = gameObjects_[k - 1][m];

				//стены
				if (obj == (*_borders.begin())->Figure())
				{
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
				}
				else
				{
					(*_pacman.begin())->setDirection("UP");
					if (obj == (*_fields.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first - 1;
						m = (*_pacman.begin())->getPosition().second;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(10);
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
					}
					//чеканные монеты
					else if (obj == (*_coins.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first - 1;
						m = (*_pacman.begin())->getPosition().second;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
						info_->Eat(10);
						info_->totalcoins++;
						info_->remaincoins--;

						for (auto a : _coins)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_coins.begin(), _coins.end(), a);
								(*it)->Destroy();
								_coins.erase(it);
							}
						}
					}
					//energizer
					else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first - 1;
						m = (*_pacman.begin())->getPosition().second;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
						info_->Eat(50);
						info_->remainboosters--;

						for (auto a : _booster)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_booster.begin(), _booster.end(), a);
								(*it)->Destroy();
								_booster.erase(it);
							}
						}
						for (auto a : _ghosts)
						{
							a->setStatus("FRIGHTENED");
							gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
						}
					}
					//враги
					else if (lambdaghost())
					{
						k = (*_pacman.begin())->getPosition().first - 1;
						m = (*_pacman.begin())->getPosition().second;
						for (auto a : _ghosts)
						{
							int x = a->getPosition().first;
							int y = a->getPosition().second;
							a->setSpeed(8);

							if (k == x && m == y && a->getStatus() == 2)
							{
								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(10);;
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
								info_->Eat(200);

								x = a->getStartPosition().first + 4;
								y = a->getStartPosition().second;
								a->setPosition(x, y);
								a->setStatus("CHASE");
								a->setDoorPassed(false);
								a->setDirection("UP");
								gameObjects_[x][y] = a->Figure();
							}
							else if (k == x && m == y && a->getStatus() != 2)
							{
								info_->DecreaseHP();
								gameObjects_[k][m] = (*_fields.begin())->Figure();
								gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
								int q = (*_pacman.begin())->getStartPosition().first;
								int w = (*_pacman.begin())->getStartPosition().second;
								(*_pacman.begin())->setPosition(q, w);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[q][w] = (*_pacman.begin())->Figure();
								for (auto a : _ghosts)
								{
									int x = a->getStartPosition().first;
									int y = a->getStartPosition().second;
									int x1 = a->getPosition().first;
									int y1 = a->getPosition().second;
									gameObjects_[x1][y1] = a->getLowField();
									a->setPosition(x, y);

									if (a->getPrevState() == 0)
										a->setStatus("SCATTER");
									else
										a->setStatus("CHASE");

									a->setChaseCount(4);
									a->setDoorPassed(false);
									a->setDirection("LEFT");
									gameObjects_[x][y] = a->Figure();
								}
							}
						}
					}
					//фрукты
					else if (lambdafruit())
					{
						for (auto a : _fruits)
						{
							if (a != nullptr && obj == a->Figure())
							{
								Fruits* fr = dynamic_cast<Fruits*>(a);
								k = (*_pacman.begin())->getPosition().first - 1;
								m = (*_pacman.begin())->getPosition().second;
								if (std::make_pair(k, m) != a->getPosition()) continue;

								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k + 1][m] = (*_fields.begin())->Figure();
								info_->Eat(fr->CalculateScore(info_->currentlevel));
								info_->addFruit(fr->Figure());
								fr->Destroy();
								auto it = std::find(_fruits.begin(), _fruits.end(), a);
								*it = nullptr;
								break;
							}
						}
					}
				}
			}
			else if (dir == 1)//down
			{
				int k = (*_pacman.begin())->getPosition().first;
				int m = (*_pacman.begin())->getPosition().second;
				obj = gameObjects_[k + 1][m];

				//стены
				if (obj == (*_borders.begin())->Figure())
				{
					gameObjects_[k][m] = (*_pacman.begin())->Figure();
				}
				else
				{
					(*_pacman.begin())->setDirection("DOWN");
					if (obj == (*_fields.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first + 1;
						m = (*_pacman.begin())->getPosition().second;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(10);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
					}
					//чеканные монеты
					else if (obj == (*_coins.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first + 1;
						m = (*_pacman.begin())->getPosition().second;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
						info_->Eat(10);
						info_->totalcoins++;
						info_->remaincoins--;

						for (auto a : _coins)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_coins.begin(), _coins.end(), a);
								(*it)->Destroy();
								_coins.erase(it);
							}
						}
					}
					//energizer
					else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
					{
						k = (*_pacman.begin())->getPosition().first + 1;
						m = (*_pacman.begin())->getPosition().second;
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(7);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
						info_->Eat(50);
						info_->remainboosters--;

						for (auto a : _booster)
						{
							if (a->getPosition() == (*_pacman.begin())->getPosition())
							{
								auto it = std::find(_booster.begin(), _booster.end(), a);
								(*it)->Destroy();
								_booster.erase(it);
							}
						}
						for (auto a : _ghosts)
						{
							a->setStatus("FRIGHTENED");
							gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
						}
					}
					//враги
					else if (lambdaghost())
					{
						k = (*_pacman.begin())->getPosition().first + 1;
						m = (*_pacman.begin())->getPosition().second;
						for (auto a : _ghosts)
						{
							int x = a->getPosition().first;
							int y = a->getPosition().second;
							a->setSpeed(8);

							if (k == x && m == y && a->getStatus() == 2)
							{
								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(10);;
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
								info_->Eat(200);

								x = a->getStartPosition().first + 4;
								y = a->getStartPosition().second;
								a->setPosition(x, y);
								a->setStatus("CHASE");
								a->setDoorPassed(false);
								a->setDirection("UP");
								gameObjects_[x][y] = a->Figure();
							}
							else if (k == x && m == y && a->getStatus() != 2)
							{
								info_->DecreaseHP();
								gameObjects_[k][m] = (*_fields.begin())->Figure();
								gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
								int q = (*_pacman.begin())->getStartPosition().first;
								int w = (*_pacman.begin())->getStartPosition().second;
								(*_pacman.begin())->setPosition(q, w);
								(*_pacman.begin())->setSpeed(7);
								gameObjects_[q][w] = (*_pacman.begin())->Figure();
								for (auto a : _ghosts)
								{
									int x = a->getStartPosition().first;
									int y = a->getStartPosition().second;
									int x1 = a->getPosition().first;
									int y1 = a->getPosition().second;
									gameObjects_[x1][y1] = a->getLowField();
									a->setPosition(x, y);

									if (a->getPrevState() == 0)
										a->setStatus("SCATTER");
									else
										a->setStatus("CHASE");

									a->setChaseCount(4);
									a->setDoorPassed(false);
									a->setDirection("LEFT");
									gameObjects_[x][y] = a->Figure();
								}
							}
						}
					}
					//фрукты
					else if (lambdafruit())
					{
						for (auto a : _fruits)
						{
							if (a != nullptr && obj == a->Figure())
							{
								Fruits* fr = dynamic_cast<Fruits*>(a);
								k = (*_pacman.begin())->getPosition().first + 1;
								m = (*_pacman.begin())->getPosition().second;
								if (std::make_pair(k, m) != a->getPosition()) continue;

								(*_pacman.begin())->setPosition(k, m);
								(*_pacman.begin())->setSpeed(7);;
								gameObjects_[k][m] = (*_pacman.begin())->Figure();
								gameObjects_[k - 1][m] = (*_fields.begin())->Figure();
								info_->Eat(fr->CalculateScore(info_->currentlevel));
								info_->addFruit(fr->Figure());
								fr->Destroy();
								auto it = std::find(_fruits.begin(), _fruits.end(), a);
								*it = nullptr;
								break;
							}
						}
					}
				}
			}
			else if (dir == 2)//left
			{
				int k = (*_pacman.begin())->getPosition().first;
				int m = (*_pacman.begin())->getPosition().second;

				if (m)
				{
					obj = gameObjects_[k][m - 1];

					//стены
					if (obj == (*_borders.begin())->Figure())
					{
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
					}
					else
					{
						(*_pacman.begin())->setDirection("LEFT");
						//проход по полям
						if (obj == (*_fields.begin())->Figure())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second - 1;
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(10);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
						}
						//чеканные монеты
						else if (obj == (*_coins.begin())->Figure())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second - 1;
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
							info_->Eat(10);
							info_->totalcoins++;
							info_->remaincoins--;

							for (auto a : _coins)
							{
								if (a->getPosition() == (*_pacman.begin())->getPosition())
								{
									auto it = std::find(_coins.begin(), _coins.end(), a);
									(*it)->Destroy();
									_coins.erase(it);
								}
							}
						}
						//energizer
						else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second - 1;
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
							info_->Eat(50);
							info_->remainboosters--;

							for (auto a : _booster)
							{
								if (a->getPosition() == (*_pacman.begin())->getPosition())
								{
									auto it = std::find(_booster.begin(), _booster.end(), a);
									(*it)->Destroy();
									_booster.erase(it);
								}
							}
							for (auto a : _ghosts)
							{
								a->setStatus("FRIGHTENED");
								gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
							}
						}
						//враги
						else if (lambdaghost())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second - 1;
							for (auto a : _ghosts)
							{
								int x = a->getPosition().first;
								int y = a->getPosition().second;
								a->setSpeed(8);

								if (k == x && m == y && a->getStatus() == 2)
								{
									(*_pacman.begin())->setPosition(k, m);
									(*_pacman.begin())->setSpeed(10);;
									gameObjects_[k][m] = (*_pacman.begin())->Figure();
									gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
									info_->Eat(200);

									x = a->getStartPosition().first + 4;
									y = a->getStartPosition().second;
									a->setPosition(x, y);
									a->setStatus("CHASE");
									a->setDoorPassed(false);
									a->setDirection("UP");
									gameObjects_[x][y] = a->Figure();
								}
								else if (k == x && m == y && a->getStatus() != 2)
								{
									info_->DecreaseHP();
									gameObjects_[k][m] = (*_fields.begin())->Figure();
									gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
									int q = (*_pacman.begin())->getStartPosition().first;
									int w = (*_pacman.begin())->getStartPosition().second;
									(*_pacman.begin())->setPosition(q, w);
									(*_pacman.begin())->setSpeed(7);;
									gameObjects_[q][w] = (*_pacman.begin())->Figure();
									for (auto a : _ghosts)
									{
										int x = a->getStartPosition().first;
										int y = a->getStartPosition().second;
										int x1 = a->getPosition().first;
										int y1 = a->getPosition().second;
										gameObjects_[x1][y1] = a->getLowField();
										a->setPosition(x, y);

										if (a->getPrevState() == 0)
											a->setStatus("SCATTER");
										else
											a->setStatus("CHASE");

										a->setChaseCount(4);
										a->setDoorPassed(false);
										a->setDirection("LEFT");
										gameObjects_[x][y] = a->Figure();
									}
								}
							}
						}
						//фрукты
						else if (lambdafruit())
						{
							for (auto a : _fruits)
							{
								if (a != nullptr && obj == a->Figure())
								{
									Fruits* fr = dynamic_cast<Fruits*>(a);
									k = (*_pacman.begin())->getPosition().first;
									m = (*_pacman.begin())->getPosition().second - 1;
									if (std::make_pair(k, m) != a->getPosition()) continue;

									(*_pacman.begin())->setPosition(k, m);
									(*_pacman.begin())->setSpeed(7);;
									gameObjects_[k][m] = (*_pacman.begin())->Figure();
									gameObjects_[k][m + 1] = (*_fields.begin())->Figure();
									info_->Eat(fr->CalculateScore(info_->currentlevel));
									info_->addFruit(fr->Figure());
									fr->Destroy();
									auto it = std::find(_fruits.begin(), _fruits.end(), a);
									*it = nullptr;
									break;
								}
							}
						}
					}
				}
				else
				{
					m = Y_SIZE - 1;
					obj = gameObjects_[k][m];
					//проход через тоннель
					if (obj == (*_fields.begin())->Figure())
					{
						(*_pacman.begin())->setDirection("LEFT");
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(10);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][0] = (*_fields.begin())->Figure();
					}
					//add ghosts collision
				}
			}
			else if (dir == 3)//right
			{
				int k = (*_pacman.begin())->getPosition().first;
				int m = (*_pacman.begin())->getPosition().second;
				if (m != Y_SIZE - 1)
				{
					obj = gameObjects_[k][m + 1];
					//стены
					if (obj == (*_borders.begin())->Figure())
					{
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
					}
					else
					{
						(*_pacman.begin())->setDirection("RIGHT");
						if (obj == (*_fields.begin())->Figure())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second + 1;
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(10);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
						}
						//чеканные монеты
						else if (obj == (*_coins.begin())->Figure())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second + 1;
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
							info_->Eat(10);
							info_->totalcoins++;
							info_->remaincoins--;

							for (auto a : _coins)
							{
								if (a->getPosition() == (*_pacman.begin())->getPosition())
								{
									auto it = std::find(_coins.begin(), _coins.end(), a);
									(*it)->Destroy();
									_coins.erase(it);
								}
							}
						}
						//energizer
						else if (!_booster.empty() && obj == (*_booster.begin())->Figure())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second + 1;
							(*_pacman.begin())->setPosition(k, m);
							(*_pacman.begin())->setSpeed(7);;
							gameObjects_[k][m] = (*_pacman.begin())->Figure();
							gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
							info_->Eat(50);
							info_->remainboosters--;

							for (auto a : _booster)
							{
								if (a->getPosition() == (*_pacman.begin())->getPosition())
								{
									auto it = std::find(_booster.begin(), _booster.end(), a);
									(*it)->Destroy();
									_booster.erase(it);
								}
							}
							for (auto a : _ghosts)
							{
								a->setStatus("FRIGHTENED");
								gameObjects_[a->getPosition().first][a->getPosition().second] = a->Figure();
							}
						}
						//враги
						else if (lambdaghost())
						{
							k = (*_pacman.begin())->getPosition().first;
							m = (*_pacman.begin())->getPosition().second + 1;
							for (auto a : _ghosts)
							{
								int x = a->getPosition().first;
								int y = a->getPosition().second;
								a->setSpeed(8);

								if (k == x && m == y && a->getStatus() == 2)
								{
									(*_pacman.begin())->setPosition(k, m);
									(*_pacman.begin())->setSpeed(7);;
									gameObjects_[k][m] = (*_pacman.begin())->Figure();
									gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
									info_->Eat(200);

									x = a->getStartPosition().first + 4;
									y = a->getStartPosition().second;
									a->setPosition(x, y);
									a->setStatus("CHASE");
									a->setDoorPassed(false);
									a->setDirection("UP");
									gameObjects_[x][y] = a->Figure();
								}
								else if (k == x && m == y && a->getStatus() != 2)
								{
									info_->DecreaseHP();
									gameObjects_[k][m] = (*_fields.begin())->Figure();
									gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
									int q = (*_pacman.begin())->getStartPosition().first;
									int w = (*_pacman.begin())->getStartPosition().second;
									(*_pacman.begin())->setPosition(q, w);
									(*_pacman.begin())->setSpeed(7);;
									gameObjects_[q][w] = (*_pacman.begin())->Figure();
									for (auto a : _ghosts)
									{
										int x = a->getStartPosition().first;
										int y = a->getStartPosition().second;
										int x1 = a->getPosition().first;
										int y1 = a->getPosition().second;
										gameObjects_[x1][y1] = a->getLowField();
										a->setPosition(x, y);

										if (a->getPrevState() == 0)
											a->setStatus("SCATTER");
										else
											a->setStatus("CHASE");

										a->setChaseCount(4);
										a->setDoorPassed(false);
										a->setDirection("LEFT");
										gameObjects_[x][y] = a->Figure();
									}
								}
							}
						}
						//фрукты
						else if (lambdafruit())
						{
							for (auto a : _fruits)
							{
								if (a != nullptr && obj == a->Figure())
								{
									Fruits* fr = dynamic_cast<Fruits*>(a);
									k = (*_pacman.begin())->getPosition().first;
									m = (*_pacman.begin())->getPosition().second + 1;
									if (std::make_pair(k, m) != a->getPosition()) continue;

									(*_pacman.begin())->setPosition(k, m);
									(*_pacman.begin())->setSpeed(7);;
									gameObjects_[k][m] = (*_pacman.begin())->Figure();
									gameObjects_[k][m - 1] = (*_fields.begin())->Figure();
									info_->Eat(fr->CalculateScore(info_->currentlevel));
									info_->addFruit(fr->Figure());
									fr->Destroy();
									auto it = std::find(_fruits.begin(), _fruits.end(), a);
									*it = nullptr;
									break;
								}
							}
						}
					}
				}
				else
				{
					m = 0;
					obj = gameObjects_[k][m];
					//проход через тоннель
					if (obj == (*_fields.begin())->Figure())
					{
						(*_pacman.begin())->setDirection("RIGHT");
						(*_pacman.begin())->setPosition(k, m);
						(*_pacman.begin())->setSpeed(10);;
						gameObjects_[k][m] = (*_pacman.begin())->Figure();
						gameObjects_[k][Y_SIZE - 1] = (*_fields.begin())->Figure();
					}
					//add ghosts collision
				}
			}
			else
			{
				return;
			}
		}
	}
}

void Controller::GhostsActions()
{
	for (auto a : _ghosts)
	{
		a->UpdateDeltaTime();
		if (a->StartCondition(info_))
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

				status == 2 ? a->setSpeed(5) : a->setSpeed(8);

				if (a->CheckPacman((*_pacman.begin())->getPosition()))
				{
					if (a->getStatus() == 2)
					{
						int x = a->getStartPosition().first + 4;
						int y = a->getStartPosition().second;

						gameObjects_[p2.first][p2.second] = (*_pacman.begin())->Figure();
						a->setLowField(gameObjects_[x][y]);
						info_->Eat(200);//fix

						a->setLowField((*_fields.begin())->Figure());
						a->setPosition(x, y);//fix start position
						a->setStatus("CHASE");
						a->setDoorPassed(false);
						a->setDirection("UP");
						gameObjects_[x][y] = a->Figure();
					}
					else
					{
						info_->DecreaseHP();
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
							gameObjects_[x1][y1] = b->getLowField();

							b->setLowField(gameObjects_[x][y]);
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
					a->setLowField(gameObjects_[p2.first][p2.second]);
					gameObjects_[p2.first][p2.second] = a->Figure();
				}
			}
		}
	}
}

void Controller::UpdateFruits()
{
	if (info_->totalcoins == 70 && _fruits[0] == nullptr)
	{
		_fruits[0] = new Fruits();
		_fruits[0]->setPosition(17, 13);
		gameObjects_[_fruits[0]->getPosition().first][_fruits[0]->getPosition().second] = _fruits[0]->Figure();
	}
	if (info_->totalcoins == 170 && _fruits[1] == nullptr)
	{
		_fruits[1] = new Fruits();
		_fruits[1]->setPosition(17, 14);
		gameObjects_[_fruits[1]->getPosition().first][_fruits[1]->getPosition().second] = _fruits[1]->Figure();
	}
}

void Controller::Run()
{
	//Menu();
	for (; info_->currentlevel < info_->MAX_LEVEL; info_->currentlevel++)
	{
		system("cls");
		this->Start();
		while (!info_->getCheckLevel())
		{
			CheckAlive();
			key = this->Input();
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
	info_->setCheckLevel(false, false);
}
