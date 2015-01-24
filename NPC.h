#ifndef __NPC_h_
#define __NPC_h_

#include "Agent.h"

class GameApplication;

class NPC : public Agent{

public:
	NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a);
	~NPC();

	void update(Ogre::Real deltaTime);		// update hero!
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(char dir, bool on); //set the movemnt
	

private:
	Ogre::Vector3 mDirection;
	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);
};

#endif