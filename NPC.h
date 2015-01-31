#ifndef __NPC_h_
#define __NPC_h_

#include "Agent.h"

class GameApplication;

class NPC : public Agent{

	//search
	//block
	//attack

public:
	static enum goodBad{
		GOOD,
		BAD
	};

	NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, goodBad t);
	~NPC();

	void update(Ogre::Real deltaTime);		// update hero!
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(char dir, bool on); //set the movemnt

	void getHurt(int d);
	

protected:

	goodBad type;
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
};

#endif