#ifndef __BadNPC_h_
#define __BadNPC_h_

#include "NPC.h"

class GameApplication;

class BadNPC : public NPC{

public:
	BadNPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s);
	~BadNPC();

private:

};
#endif