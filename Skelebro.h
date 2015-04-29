#ifndef __Skelebro_h_
#define __Skelebro_h_

#include "NPC.h"
#include "Player.h"

class GameApplication;

class Skelebro : public NPC{

public:
	Skelebro(Ogre::SceneManager* SceneManager, std::string name, std::string filename, 
		float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s);
	~Skelebro();
protected:

	enum SkelAnimID
	{
		IDLE,
		ATTACK,
		WALK,
		ANIM_NONE
	};

	// use npc's definitions
	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(SkelAnimID id, bool reset = false);
	void update(Ogre::Real deltaTime);
	void attackPlayer(Player* mainPlayer);

private:
	SkelAnimID idOfAnim;
	Ogre::Real deadTimer;
};

#endif