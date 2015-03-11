#ifndef __AoE_h_
#define __AoE_h_

#include "Spell.h"

class GameApplication;

class AoE : public Spell
{
private:

	enum AnimID;

	Ogre::Vector3 mScale;	// starting scale
	Ogre::Vector3 scaleBy;	// scale up by this much

	double mRadius;			// radius of the effect
	double cRadius;			// current radius

	// maybe just cycle through textures for AoE animations
	void setupAnimations();							// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);		// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);	// update the animation frame

	void checkCollision();
	void shoot(Ogre::Real deltatime);
	void reload();

	void updateLocomote(Ogre::Real deltaTime);

public:

	AoE(Ogre::SceneManager* SceneManager, std::string name, 
		std::string filename, float height, float scale, 
		GameApplication* app);
	~AoE();

	void fire(Ogre::Vector3 pos);

};

#endif