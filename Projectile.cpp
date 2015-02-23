#include "Projectile.h"
#include "GameApplication.h"

Projectile::Projectile(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Agent(SceneManager, name, filename, height, scale, app)
{
	// nodes, entities, and other fields handled through inheritance

	projectile = false;
	
	mTarget = NULL;
	mDmg = 1.0;
	mMass = 1.0;
	mHeight = height;	// pass shooters mBodyNode position
	mDirection = Ogre::Vector3(0,0,0);
	mSpeed = 5.0;


	mModelEntity->setCastShadows(false);
	mModelEntity->setVisible(false);
}

// inherit update from agent.cpp

// override with projectile motion
void
Projectile::updateLocomote(Ogre::Real deltaTime)
{
	if(projectile)
		shoot(deltaTime);
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

//intializes the initial velocities and toggles the agent to shoot
//also give shooters's current position.
void
Projectile::fire(Ogre::Real vx, Ogre::Real vy, Ogre::Real vz,
				 Ogre::Vector3 pos)
{
	std::cout << "I Said FIRE!!" << std::endl;
	projectile = true; // turns on the movement, which will call shoot
	mModelEntity->setVisible(true);
	// set up the initial state
	mBodyNode->setPosition(pos);
	this->vel.x = vx;
	this->vel.y = vy;
	this->vel.z = vz;
	gravity.x = 0;
	gravity.y = Ogre::Real(-9.81);
	gravity.z = 0;
	//this->mBodyNode->yaw(Ogre::Degree(180));
	//this->mBodyNode->pitch(Ogre::Degree(45));
	this->mBodyNode->showBoundingBox(true); 
}

//updates the current velocity and position of the agent
//sets a new position according to math
//agent stops when he touches the ground
//need to adjust for barrel collision
void
Projectile::shoot(Ogre::Real deltaTime) // lecture 12 call for every frame of the animation
{
	using namespace Ogre;
	std::cout << "Firing Captain!" << std::endl;
	Vector3 pos = this->mBodyNode->getPosition();
	vel = vel + (mMass*gravity * deltaTime);
	pos = pos + (vel * mSpeed * deltaTime); // velocity
	pos = pos + 0.5 * mMass*gravity * deltaTime * deltaTime; // acceleration

	this->mBodyNode->setPosition(pos);
	//this->mBodyNode->pitch(Ogre::Degree(20));

	if (this->mBodyNode->getPosition().y <= -0.5) // if it get close to the ground, stop
	{
		reload();	// finished reset
	}
}

// reset the agent to starting position
// ready to fire again!
void
Projectile::reload()
{
	projectile = false;
	mModelEntity->setVisible(false);
}