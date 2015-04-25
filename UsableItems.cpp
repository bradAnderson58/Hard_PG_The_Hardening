#include "UsableItems.h"
#include "Player.h"

std::string UsableItems::prefix[15] =
		{
			"Goring",
			"Shining",
			"Searing",
			"Blinding",
			"Dreadful",
			"Flaming",
			"Freezing",
			"Shocking",
			"Smelly",
			"Delicious",
			"Intense",
			"Brown",
			"Fantastic",
			"Shitty",
			"Haphazard"
		};

std::string UsableItems::suffix[15] =
		{
			"the Bear",
			"the Fox",
			"the Berzerker",
			"the Bemused",
			"the Ugly",
			"the Rock",
			"the Alchemist",
			"the Raven",
			"the Clown",
			"the Radient",
			"the Magister",
			"the Dragon",
			"the Others",
			"the Ta'Veren",
			"the Tal-Vashoth"
		};

UsableItems::UsableItems(itemType mt, int damstat, int defstat, int critstat, int healthstat, int manastat,  std::string n, int resale){
	mType = mt; //my type

	damageStat = damstat;
	defenseStat = defstat;
	critStat = critstat;
	healthStat = healthstat;
	manaStat = manastat;

	resaleVal = resale;

	name = n;
	imgFile = createImg();
}

UsableItems::UsableItems(Player* pl){
	int playerLevel = pl->getLevel();
	damageStat = defenseStat = critStat = healthStat = manaStat = resaleVal = 0;  //make sure all are initialized

	//randomize which type of loot you get
	int random = rand() % 100;
	mType = (random < 14) ? POTION :
				(random < 30) ? WEAPON :
				(random < 44) ? SHIELD :
				(random < 58) ? BOOBPLATE :
				(random < 72) ? PANTS :
				(random < 86) ? HELM :
				NECKLACE;

	imgFile = createImg();

	if (mType != POTION){
		random = rand() % 14;
		std::string pre = getPrefix(random);
		random = rand() % 14;
		std::string suf = getSuffix(random);

		//This will probably need to be play tested for balancing
		if (mType == WEAPON){
			damageStat = rand() % 10 + (playerLevel - 5);
			if (damageStat < 0) damageStat = 1;
		}else{
			defenseStat = rand() % 10 + (playerLevel - 5);
			if (defenseStat < 0) defenseStat = 1;
		}
		name = buildName(pre, suf);
	}else{
		name = "potion";
	}
	resaleVal = 10;  //sell value hardcoded for now (forever?)

}

//Yum, appealing
std::string UsableItems::createImg(){
	return		(mType == POTION) ? "potion.png" :
				(mType == WEAPON) ? "weapon.png" :
				(mType == SHIELD) ? "shield.png" :
				(mType == BOOBPLATE) ? "plate.png" :
				(mType == PANTS) ? "pants.png" :
				(mType == HELM) ? "helm.png" :
				(mType == NECKLACE) ? "necklace.png" :
				"emptySlot.png";
}

std::string UsableItems::buildName(std::string p, std::string s){
	std::string weapon =	(mType == WEAPON) ? " Sword of " :
							(mType == SHIELD) ? " Shield of " :
							(mType == BOOBPLATE) ? " Armor of " :
							(mType == PANTS) ? " Leggings of " :
							(mType == HELM) ? " Helm of " :
							" Amulet of ";

	return p + weapon + s;

}

//return a specific stat from this usable item
int UsableItems::getStat(statType getMe){
	return  (getMe == DAMAGE) ? damageStat :
			(getMe == DEFENSE) ? defenseStat :
			(getMe == CRITICAL) ? critStat :
			(getMe == HEALTH) ? healthStat :
			(getMe == MANA) ? manaStat :
			NULL;
}

