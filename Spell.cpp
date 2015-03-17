#include "Spell.h"
#include "Timer.h"

Spell::Spell(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Agent(SceneManager, name, filename, height, scale, app)
{
	cooldown_timer = NULL;
}

Spell::~Spell(){}

bool 
Spell::checkCollision(NPC* enemy)
{
	// just use a distance check to see if agent is within radius
	Ogre::AxisAlignedBox spellBox = mModelEntity->getWorldBoundingBox();
	Ogre::AxisAlignedBox enemyBox = enemy->getBoundingBox();
	if (spellBox.intersects(enemyBox))
	{
		enemy->getHurt(mDmg);
		return true;
	}
	return false;
}

void 
Spell::shoot(Ogre::Real deltatime){}

void 
Spell::reload(){}

void
Spell::setCoolDown(Ogre::Real time)
{
	this->cooldown_timer = new Timer(time);
}