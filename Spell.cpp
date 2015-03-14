#include "Spell.h"
#include "Timer.h"

Spell::Spell(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Agent(SceneManager, name, filename, height, scale, app)
{
	cooldown_timer = NULL;
}

Spell::~Spell(){}

void 
Spell::checkCollision(){}

void 
Spell::shoot(Ogre::Real deltatime){}

void 
Spell::reload(){}

void
Spell::setCoolDown(Ogre::Real time)
{
	cooldown_timer = new Timer(time);
}