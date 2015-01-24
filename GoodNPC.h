#ifndef __GoodNPC_h_
#define __GoodNPC_h_

#include "NPC.h"

class GameApplication;

class GoodNPC : public NPC{

public:
	GoodNPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a);
	~GoodNPC();

private:

};

#endif