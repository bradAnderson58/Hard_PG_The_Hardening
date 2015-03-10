#include "UsableItems.h"

UsableItems::UsableItems(itemType mt, int damstat, int defstat, int critstat, int healthstat, int manastat,  std::string n, int resale){
	mType = mt; //my type

	damageStat = damstat;
	defenseStat = defstat;
	critStat = critstat;
	healthStat = healthstat;
	manaStat = manastat;

	resaleVal = resale;

	name = n;
	imgFile = (mType == POTION) ? "potion.png" :
				(mType == WEAPON) ? "weapon.png" :
				(mType == SHIELD) ? "shield.png" :
				(mType == BOOBPLATE) ? "plate.png" :
				(mType == PANTS) ? "pants.png" :
				(mType == HELM) ? "helm.png" :
				(mType == NECKLACE) ? "necklace.png" :
				"emptySlot.png";
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

