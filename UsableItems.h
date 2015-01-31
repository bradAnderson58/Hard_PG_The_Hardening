#ifndef __UsableItems_h_
#define __UsableItems_h_

#include "Objects.h"
#include <string>
#include <iostream>

class UsableItems : public Objects{

public:
	static enum statType
	{
		DAMAGE,
		DEFENSE,
		CRITICAL,
		HEALTH,
		MANA
	};

	static enum itemType
	{
		POTION,
		WEAPON,
		SHIELD,
		BOOBPLATE,
		PANTS,
		HELM,
		NECKLACE
	};

	UsableItems(itemType mt, int damstat, int defstat, int critstat, int healthstat, int manastat, std::string n, int resale);
	~UsableItems();

	int getStat(statType getMe);
	std::string getName(){ return name; };

private:
	itemType mType;  //enum

	int damageStat;
	int defenseStat;
	int critStat;
	int healthStat;
	int manaStat;

	//display stuff
	std::string name;

	int resaleVal;  //how much its worth
	
};

#endif