#ifndef __NPC_h_
#define __NPC_h_

#include "Agent.h"
#include "Player.h"

class GameApplication;

class NPC : public Agent{

	//search
	//block
	//attack

public:
	enum GoodBad{
		GOOD,
		BAD
	};

	NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, GoodBad t);
	~NPC();

	virtual void update(Ogre::Real deltaTime) = 0;
	virtual void updateLocomote(Ogre::Real deltaTime);
	void setMovement(char dir, bool on); //set the movemnt

	void getHurt(int d);
	Ogre::Vector3 getPosition(){return mBodyNode->getPosition();}
	Ogre::AxisAlignedBox getBoundingBox() { return mModelEntity->getWorldBoundingBox(); }

protected:
	enum AnimId{
		ANIM_NONE
	};

	GoodBad type;
	//stats
	int level;
	int health;
	double crit;
	int damage;
	int defense;
		
	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);

	void attackPlayer(Player* mainPlayer);
	void attack(NPC* otherGuys);
	void interact();
	void dealDamage(NPC* guy);
	void dealDamagePlayer(Player* player);
	void checkHit();

	bool nextLocation();
};

#endif