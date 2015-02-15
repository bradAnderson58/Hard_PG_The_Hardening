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
		NONE
	};
	Rat(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, Rat::RatStates s);
	~Rat();

	void update(Ogre::Real deltaTime);
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(char dir, bool on); //set the movemnt
	Ogre::SceneNode *rayNode;
	Ogre::Entity *rayEntity;

	Ogre::ManualObject* myManualObject; 
	Ogre::SceneNode* myManualObjectNode; 
 
	// NOTE: The second parameter to the create method is the resource group the material will be added to.
	// If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
	Ogre::MaterialPtr myManualObjectMaterial; 

private:

	enum AnimID
	{
		SWIM,
		ANIM_NONE
	};
	
	AnimID ratAnim;
	RatStates state;

	Ogre::SceneNode *mVisionNode;
	Ogre::Entity *mVisionEntity;

	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);


	double wanderAngle;
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

};

#endif