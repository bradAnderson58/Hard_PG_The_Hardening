#include "BadNPC.h"
#include "GameApplication.h"

BadNPC::BadNPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s):
	NPC(SceneManager, name, filename, height, scale, a, l, t, s)
{
	
}

BadNPC::~BadNPC(void)
{
	
}