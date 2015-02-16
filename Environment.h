#ifndef __Environment_h_
#define __Environment_h_

#include "Objects.h"
#include <string.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

class GameApplication;
class UsableItems;

class Environment : public Objects{
private:

	UsableItems* inner;  //This what you get when you pick me up
	Ogre::SceneManager* mSceneMgr;  //need this
	float height;
	float scale;

	GameApplication* app;
	Ogre::SceneNode* mBodyNode;  //this is the object
	Ogre::Entity* mBodyEntity;


public:
	
	Environment(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a);
	~Environment();

	void setPosition(float x, float y, float z);

	UsableItems* getItem(){ return inner; }  //give this to player

	void checkWithPlayer();
};

#endif