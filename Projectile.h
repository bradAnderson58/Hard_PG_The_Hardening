#pragma once
#include "GameApplication.h"
#include "Player.h"
#include "Agent.h"

class Agent;
class Player;
class GameApplication;

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