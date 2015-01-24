#include "GoodNPC.h"
#include "GameApplication.h"


GoodNPC::GoodNPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a):
	NPC(SceneManager, name, filename, height, scale, a)
{
	
}

GoodNPC::~GoodNPC(void)
{
	
}