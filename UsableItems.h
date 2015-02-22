#ifndef __UsableItems_h_
#define __UsableItems_h_

#include "Objects.h"
#include <string>
#include <iostream>

class UsableItems : public Objects{

public:
	enum statType
	{
		DAMAGE,
		DEFENSE,
		CRITICAL,
		HEALTH,
		MANA
	};

	enum itemType
	{
		POTION,
		WEAPON,
		SHIELD,
		BOOBPLATE,
		PANTS,
		HELM,
		NECKLACE
	};

	UsableItems(itemType mt, int damstat, int defstat, int critstat, 
		int healthstat, int manastat, std::string n, int resale);
	~UsableItems();

	//getters for stuffs
	int getStat(statType getMe);
	std::string getName(){ return name; }
	std::string getImgFile(){ return imgFile; }
	itemType getType(){ return mType; }

	void setImgFile(std::string filename) { imgFile = filename; }

private:
	itemType mType;  //enum

	int damageStat;
	int defenseStat;
	int critStat;
	int healthStat;
	int manaStat;

	//display stuff
	std::string name;
	// need an img/icon to use in inventory
	std::string imgFile;
	// need a model to display in game

	int resaleVal;  //how much its worth
	
};

#endif