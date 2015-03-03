#ifndef __Rat_h_
#define __Rat_h_

#include "NPC.h"
#include "Player.h"

class GameApplication;

class Rat : public NPC{

public:
	enum RatStates{
		WANDER,
		GUARD,
		FLEE,
		DEAD,
		SEEK,
		LOST,
		SEARCH,
		NONE
	};
	Rat(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, Rat::RatStates s);
	~Rat();

	void update(Ogre::Real deltaTime);
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(char dir, bool on); //set the movemnt
	void setStartPos(){startPos = mBodyNode->getPosition();}

	bool canHit;
	Ogre::Real lastHit;

private:

	enum AnimID
	{
		SWIM,
		ANIM_NONE
	};
	
	AnimID ratAnim;
	RatStates state;
	RatStates prevState;

	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);

	RatStates startState;
	Ogre::Vector3 startPos;
	double wanderAngle;
	Ogre::Vector3 lastPlayerPos;
	Ogre::Vector3 lookDir;

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

};

#endif