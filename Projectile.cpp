#include "Projectile.h"
#include "Timer.h"

#define FORWARDVEL 10.0

Projectile::Projectile(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Spell(SceneManager, name, filename, height, scale, app)
{
	// nodes, entities, and other fields handled through inheritance

	active = false;
	
	mTarget = NULL;
	mDmg = 10.0;
	mMass = 2.5;
	mHeight = height;	// pass shooters mBodyNode position
	mDirection = Ogre::Vector3(0,0,0);
	mSpeed = 4.0;

	mModelEntity->setMaterialName( "ToonFireBall" );
	mModelEntity->setCastShadows(false);
	mModelEntity->setVisible(false);

}

Projectile::~Projectile(){

}

// inherit update from agent.cpp

// override with projectile motion
void
Projectile::updateLocomote(Ogre::Real deltaTime)
{
	if(active)
		shoot(deltaTime);
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
//also pass in shooters's current position. Fire from that position
void
Projectile::fire(Ogre::Real vx, Ogre::Real vy, Ogre::Real vz,
				 Ogre::Vector3 pos)
{
	active = true; // turns on the movement, which will call shoot
	mModelEntity->setVisible(true);
	// set up the initial state
	mBodyNode->setPosition(pos);
	this->vel.x = vx * FORWARDVEL;
	this->vel.y = vy;
	this->vel.z = vz * FORWARDVEL;
	gravity.x = 0;
	gravity.y = Ogre::Real(-9.81);
	gravity.z = 0;
}

//updates the current velocity and position of the agent
//sets a new position according to math
//agent stops when he touches the ground
//need to adjust for collision
void
Projectile::shoot(Ogre::Real deltaTime) 
{
	using namespace Ogre;
	Vector3 pos = this->mBodyNode->getPosition();
	vel = vel + (mMass*gravity * deltaTime);
	pos = pos + (vel * mSpeed * deltaTime);	
	pos = pos + 0.5 * mMass*gravity * deltaTime * deltaTime;

	// move and animate the fireball
	this->mBodyNode->setPosition(pos);
	this->mBodyNode->roll(Ogre::Degree(2));
	this->mBodyNode->yaw(Ogre::Degree(2));

	// if a fireball hits something, explode it and reload
	for (NPC *enemy : app->getNPCs())
	{
		if (checkCollision(enemy))
			reload();
	}
	//TODO: needs a particle effect on hits
	if (this->mBodyNode->getPosition().y <= -0.5) // if it get close to the ground, stop
		reload();	// finished reset
	else if(hitsWall())
		reload();
}

// ready to fire again!
// disable projectile
void
Projectile::reload()
{
	active = false;
	mModelEntity->setVisible(false);
}

bool
Projectile::hitsWall()
{
	Ogre::Vector3 myPos = mBodyNode->getPosition();
	for(Ogre::Vector3 wPos : app->getWallList())
	{
		//Ogre::Vector3 wPos = wall;
		if ((myPos[0] >= (wPos[0] - 7) && myPos[0] <= (wPos[0] + 7)) 
			&& (myPos[2] >= (wPos[2] - 7) && myPos[2] <= (wPos[2] + 7)))
		{
			return true;
		}
	}
	return false;
}