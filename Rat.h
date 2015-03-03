#ifndef __Rat_h_
#define __Rat_h_

#include "NPC.h"
#include "Player.h"

class GameApplication;

class Rat : public NPC{

public:
	Rat(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s);
	~Rat();
private:

	enum AnimID
	{
		SWIM,
		ANIM_NONE
	};
	
	AnimID ratAnim;

	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);

};

#endif