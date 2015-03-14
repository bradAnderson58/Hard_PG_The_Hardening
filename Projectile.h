#ifndef __Projectile_h_
#define __Projectile_h_

#include "Spell.h"

class GameApplication;

class Projectile : public Spell
{
private:

	double mHeight;
	double mMass;
	Agent* mTarget;		// use for targeted attackes, else null

	Ogre::SceneNode *blastNode;		// use for explosion area

	void setupAnimations();							// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);		// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);	// update the animation frame

	void shoot(Ogre::Real deltatime);
	void reload();

	void updateLocomote(Ogre::Real deltaTime);

public:
	
	Projectile(Ogre::SceneManager* SceneManager, std::string name, 
		std::string filename, float height, float scale, 
		GameApplication* app);
	~Projectile();

	void fire(Ogre::Real vx, Ogre::Real vy, Ogre::Real vz,
			  Ogre::Vector3 pos);

};
#endif