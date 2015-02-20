#ifndef __Projectile_h_
#define __Projectile_h_

#include "GameApplication.h"
#include "Agent.h"

class GameApplication;
class Agent;

class Projectile : public Agent
{
private:

	double mSpeed;		// speed of the orb
	double mDmg;		// damage to deal

	Ogre::SceneNode *blastNode;		// use for explosion area

	void setupAnimations();							// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);		// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);	// update the animation frame

	void checkCollision();

	void updateLocomote(Ogre::Real deltaTime);

public:
	
	Projectile(Ogre::SceneManager* SceneManager, std::string name, 
		std::string filename, float height, float scale, GameApplication* a);
	~Projectile();


};
#endif