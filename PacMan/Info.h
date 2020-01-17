#pragma once
#include <string>
class Info
{
public:
	Info(unsigned int, unsigned char, unsigned int, unsigned int, unsigned int);
	~Info();

	void addFruit(std::pair<char, int> fruit);

	std::string getLastFruits() const;

	std::string getHealth();

	void setHealth(const unsigned int); 
	
	void Eat(const int mass);

	int getScore();

	void DecreaseHP();

	const unsigned int AMOUNT;

	unsigned char currentlevel;
	unsigned int totalcoins;
	unsigned int remaincoins;
	unsigned int remainboosters;
	std::string fruits;

private:
	unsigned int score_;
	unsigned int health_;
};

