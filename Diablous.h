#ifndef __Diablous_h_
#define __Diablous_h_

#include "NPC.h"
#include "Player.h"

class GameApplication;

class Diablous : public NPC{

public:
	Diablous(Ogre::SceneManager* SceneManager, std::string name, std::string filename, 
		float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s);
	~Diablous();
protected:

	/* Use a map instead, declared in parent.
	enum AnimID
	{
		IDLE,
		HIT,
		ATTACK,  //idle_2
		WALK,
		DIE,
		ANIM_NONE
	};*/

	// use npc's definitions
	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);

private:
	AnimID idOfAnim;
};

#endif