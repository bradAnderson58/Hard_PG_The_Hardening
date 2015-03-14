#include "AoE.h"
#include "Timer.h"

AoE::AoE(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Spell(SceneManager, name, filename, height, scale, app)
{
	active = false;
	mDmg = 1.0;
	mSpeed = 5.0;

	mBodyNode->scale(1.0f,0.02f,1.0f); // make it a flat circle
	mScale = mBodyNode->getScale();
	scaleBy = Ogre::Vector3(1.05, 1.0, 1.05);
	
	mModelEntity->setMaterialName("Examples/icelake");
	mModelEntity->setCastShadows(false);
	mModelEntity->setVisible(false);
}

void
AoE::updateLocomote(Ogre::Real deltaTime)
{
	if(active)
		shoot(deltaTime);
	else if (cooldown_timer != NULL)
		cooldown_timer->update(deltaTime);
}

void 
AoE::checkCollision()
{
	Ogre::Real current_radius = mModelEntity->getBoundingRadius();
	//(x - center_x)^2 + (y - center_y)^2 < radius^2
}

void 
AoE::setupAnimations()
{

}

void 
AoE::fadeAnimations(Ogre::Real deltaTime)
{

}

void 
AoE::updateAnimations(Ogre::Real deltaTime)
{

}

// set up area for expansion
void
AoE::fire(Ogre::Vector3 pos)
{
	//FIX HERE
	if (cooldown_timer->isZero())
	{
		std::cout << "Freeze them all." << std::endl;
		active = true;
		mModelEntity->setVisible(true);
		mBodyNode->setPosition(pos);
	}
}

// scale the area of effect larger until it reaches max radius
void
AoE::shoot(Ogre::Real deltaTime)
{
	double scaleAmt = deltaTime / (deltaTime * 0.98);
	std::cout << "scale by: " << scaleAmt << std::endl;
	mBodyNode->scale(scaleAmt, 1.0, scaleAmt);
	//std::cout << "current scale: " << mBodyNode->getScale() << std::endl;
	if (mBodyNode->getScale()[0] >= 0.1)
		reload();
}

void
AoE::reload()
{
	active = false;
	mModelEntity->setVisible(false);
	mBodyNode->setScale(mScale);
	scaleBy = Ogre::Vector3(1.05, 1.0, 1.05);
}
