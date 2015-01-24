#include "BadNPC.h"
#include "GameApplication.h"

BadNPC::BadNPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a):
	NPC(SceneManager, name, filename, height, scale, a)
{
	
}

BadNPC::~BadNPC(void)
{
	
}