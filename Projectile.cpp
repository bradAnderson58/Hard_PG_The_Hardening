#include "Projectile.h"

Projectile::Projectile(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* a):
	Agent(SceneManager, name, filename, height, scale, a)
{
	projectile = true;
	mSpeed = 2.5;
	gravity = Ogre::Vector3(0.0, 9.5, 0.0);
}

// inherit update from agent.cpp

// override with projectile motion
void
Projectile::updateLocomote(Ogre::Real deltaTime)
{
	Ogre::Real move;

	move = mSpeed * deltaTime;
	mBodyNode->translate(mDirection * move);
}

void 
Projectile::checkCollision()
{

}

void Projectile::setupAnimations()
{

}

void Projectile::fadeAnimations(Ogre::Real deltaTime)
{

}

void Projectile::updateAnimations(Ogre::Real deltaTime)
{

}