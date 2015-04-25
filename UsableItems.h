#ifndef __UsableItems_h_
#define __UsableItems_h_

#include "Objects.h"
#include <string>
#include <iostream>

class Player;

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


	UsableItems(Player* pl);
	~UsableItems();

	//getters for stuffs
	int getStat(statType getMe);
	std::string getName(){ return name; }
	std::string getImgFile(){ return imgFile; }
	std::string createImg();  //based on type
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

	static std::string prefix[15];
	static std::string suffix[15];

	std::string getPrefix(int r) { return prefix[r]; }
	std::string getSuffix(int r) { return suffix[r]; }

	std::string buildName(std::string p, std::string s);  //build my epic name!
	
};

#endif