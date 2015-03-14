#include "AoE.h"
#include "Timer.h"

AoE::AoE(Ogre::SceneManager* SceneManager, std::string name, 
	std::string filename, float height, float scale, GameApplication* app):
	Spell(SceneManager, name, filename, height, scale, app)
{
	active = false;
	mDmg = 2.0;
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
	else if (cooldown_timer)
		cooldown_timer->update(deltaTime);
}

// check if an agent give position is in range of my ice
//void 
//AoE::checkCollision(NPC* enemy)
//{
//	// just use a distance check to see if agent is within radius
//	Ogre::AxisAlignedBox aoeBox = mModelEntity->getWorldBoundingBox();
//	Ogre::AxisAlignedBox enemyBox = enemy->getBoundingBox();
//	if (aoeBox.intersects(enemyBox))
//		enemy->getHurt(mDmg);
//}

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
	if (!cooldown_timer || cooldown_timer->isZero())
	{
		std::cout << "Freeze them all." << std::endl;
		active = true;
		mModelEntity->setVisible(true);
		mBodyNode->setPosition(pos);
	}
	else std::cout << "Not off cooldown" << std::endl;
}

// scale the area of effect larger until it reaches max radius
void
AoE::shoot(Ogre::Real deltaTime)
{
	double scaleAmt = deltaTime / (deltaTime * 0.98);
	//std::cout << "scale by: " << scaleAmt << std::endl;
	mBodyNode->scale(scaleAmt, 1.0, scaleAmt);

	// check to see if aoe is hitting anything
	for (NPC *enemy : app->getNPCs())
	{
		checkCollision(enemy);
	}

	// grown to max size, reset it
	if (mBodyNode->getScale()[0] >= 0.1)
		reload();
}

// reset the scale and the cooldown timer
void
AoE::reload()
{
	active = false;
	mModelEntity->setVisible(false);
	mBodyNode->setScale(mScale);
	scaleBy = Ogre::Vector3(1.05, 1.0, 1.05);
	if (cooldown_timer) 
		cooldown_timer->reset();
}
