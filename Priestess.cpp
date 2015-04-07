#include "Priestess.h"
#include "GameApplication.h"
#define _USE_MATH_DEFINES 
#include <math.h>

Priestess::Priestess(Ogre::SceneManager* SceneManager, std::string name, std::string filename, 
	float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s):
	NPC(SceneManager, name, filename, height, scale, a, l, t,s)
{
	mBodyNode->setPosition(mBodyNode->getPosition()[0], 0, mBodyNode->getPosition()[2]);

	defense = level * 1;

	canHit = true;
	lastHit = 0;
	startState = s;
	lookDir = Ogre::Vector3(1,0,0);
	//startPos = mBodyNode->getPosition();

	numAnimations =  5; //mModelEntity->getSkeleton()->getNumAnimations();
	setupAnimations(); // turn this off if you can't find the animations

	mModelEntity->setMaterialName("ToonPriestess");
}

Priestess::~Priestess(void)
{
	
}

void Priestess::updateAnimations(Ogre::Real deltaTime){
	if (idOfAnim != ANIM_NONE){
		mAnims[idOfAnim]->addTime(deltaTime);
	}
	//transitions
	fadeAnimations(deltaTime);
}

void Priestess::fadeAnimations(Ogre::Real deltaTime){
	using namespace Ogre;

	for (int i = 0; i < numAnimations; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Real newWeight = mAnims[i]->getWeight() + deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[i]->getWeight() - deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void Priestess::setupAnimations(){
	this->mTimer = 0;				// Start from the beginning
	this->mVerticalVelocity = 0;	// Not jumping

	// this is very important due to the nature of the exported animations
	mModelEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	// Name of the animations for this character - this will change with new assets
	Ogre::String animNames[] = {"Idle","Hit", "Attack", "Walk", "Die"};

	// populate our animation list
	for (int i = 0; i < numAnimations; i++)
	{
		mAnims[i] = mModelEntity->getAnimationState(animNames[i]);
		
		//Some animations are not looping
		if (animNames[i] == "Idle") mAnims[i]->setLoop(true);
		else if (animNames[i] == "Walk") mAnims[i]->setLoop(true);
		else mAnims[i]->setLoop(false);

		mFadingIn[i] = false;
		mFadingOut[i] = false;
		std::cout << i << std::endl;
	}
	setAnimation(IDLE);
}

void Priestess::setAnimation(AnimID id, bool reset){
	if (id == idOfAnim) return;
	else if (idOfAnim == ATTACK && !mAnims[idOfAnim]->hasEnded()) return;

	if (idOfAnim >= 0 && idOfAnim < numAnimations)
	{
		// if we have an old animation, fade it out
		mFadingIn[idOfAnim] = false;
		mFadingOut[idOfAnim] = true;
	}

	idOfAnim = id; 
	std::cout << "id of Anim: " << idOfAnim << std::endl;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

