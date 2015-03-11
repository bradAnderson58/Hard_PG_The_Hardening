#include "Spell.h"

Spell::Spell(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Agent(SceneManager, name, filename, height, scale, app)
{
	
}

Spell::~Spell()
{

}

void 
Spell::checkCollision(){}

void 
Spell::shoot(Ogre::Real deltatime){}

void 
Spell::reload(){}