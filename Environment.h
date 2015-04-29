#ifndef __Environment_h_
#define __Environment_h_

#include "Objects.h"
#include <string.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

class GameApplication;
class UsableItems;
class Player;
class Event;

class Environment : public Objects{
private:

	UsableItems* inner;  //This what you get when you pick me up, if u pick me up?
	Ogre::SceneManager* mSceneMgr;  //need this
	float height;
	float scale;

	Event* popup;
	bool correct;

	GameApplication* app;
	Ogre::SceneNode* mBodyNode;  //this is the object
	Ogre::Entity* mBodyEntity;

	bool animate;	//doors need to do an animation
	float amountDown;	//check how far the door has gone down
	bool passable;	//open doors can be walked through

public:
	//what type of environment object is this?
	enum
	EnvType
	{
		LOOT,
		DOOR,
		LOCKED_DOOR,
		MOVEABLE,
		PLACEMENT,
		CHEST,
		TRANSPORT,
		KEY
	};

	Environment(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale,
		GameApplication* a, EnvType tp);


	~Environment();

	//Get set position
	void setPosition(float x, float y, float z);
	Ogre::Vector3 getPosition(){ return mBodyNode->getPosition(); };

	Ogre::SceneNode* getNode(){ return mBodyNode; }	//for picking up objects

	UsableItems* getItem(){ return inner; }  //give this to player for picked up items

	bool checkWithPlayer();  //checking to see if player is nearbyto interact

	EnvType mType;

	EnvType handleInteraction(Player* pl);
	
	bool isAnimating(){ return animate; }
	bool isPassable(){ return passable; }
	void update(Ogre::Real dt);				//update door animation

	void checkPenguins();  //check if we have a penguin on us
	bool isCorrect(){ return correct; }
	void setType(EnvType et) { mType = et; }

};

#endif