#ifndef __NPC_h_
#define __NPC_h_

#include "Agent.h"
#include "Player.h"

class GameApplication;
class Event;

class NPC : public Agent{

	//search
	//block
	//attack

public:
	enum GoodBad{
		GOOD,
		BAD
	};

	enum States{
		WANDER,
		GUARD,
		FLEE,
		DEAD,
		SEEK,
		LOST,
		SEARCH,
		NONE
	};

	NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, GoodBad t, States s);
	~NPC();

	void update(Ogre::Real deltaTime);
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(char dir, bool on); //set the movemnt
	void setStartPos(){startPos = mBodyNode->getPosition();}

	void getHurt(int d);
	Ogre::Vector3 getPosition(){return mBodyNode->getPosition();}
	Ogre::AxisAlignedBox getBoundingBox() { return mModelEntity->getWorldBoundingBox(); }

	Event* getEvent() { return mEvent; }
	void setEvent(Event* e) { mEvent = e; } 

protected:
	enum AnimId{
		ANIM_NONE
	};

	GoodBad type;

	States state;
	States prevState;
	States startState;
	Ogre::Vector3 startPos;
	double wanderAngle;
	Ogre::Vector3 lastPlayerPos;
	Ogre::Vector3 lookDir;

	Ogre::Real lookAngle;
	int lookRange;

	//stats
	int level;
	int health;
	double crit;
	int damage;
	int defense;

	bool canHit;
	Ogre::Real lastHit;

	Event* mEvent;
		
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

	bool checkInFront();
	void wander();
	void seek();
	void flee();

	void searchMove(Ogre::Real deltaTime);
	void walkToGN(GridNode* n);
	void moveTo(GridNode* n);

	void updateBad(Ogre::Real deltaTime);
	void updateGood(Ogre::Real deltaTime);
	bool nextLocation();

};

#endif