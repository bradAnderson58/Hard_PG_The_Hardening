#include "Rat.h"
#include "GameApplication.h"
#define _USE_MATH_DEFINES 
#include <math.h>

Rat::Rat(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s):
	NPC(SceneManager, name, filename, height, scale, a, l, t,s)
{
	mBodyNode->setPosition(mBodyNode->getPosition()[0], 0, mBodyNode->getPosition()[2]);

	defense = level * 1;

	canHit = true;
	lastHit = 0;
	startState = s;
	lookDir = Ogre::Vector3(1,0,0);
	//startPos = mBodyNode->getPosition();

	setupAnimations();

}

Rat::~Rat(void)
{
	
}

void Rat::update(Ogre::Real deltaTime){
	if (state == DEAD){
		app->engine->play2D("../../media/ratDie.ogg");
	}
	NPC::update(deltaTime);
}

void Rat::attackPlayer(Player* mainPlayer){
	if (canHit){
		app->engine->play2D("../../media/ratAttack.ogg");
		NPC::attackPlayer(mainPlayer);
	}
}

void Rat::getHurt(int d){
	app->engine->play2D("../../media/ratHurt.ogg");
	NPC::getHurt(d);
}

void Rat::updateAnimations(Ogre::Real deltaTime){
	if (ratAnim != ANIM_NONE){
		mAnims[ratAnim]->addTime(deltaTime);
	}
	//transitions
	fadeAnimations(deltaTime);
}

void Rat::fadeAnimations(Ogre::Real deltaTime){
	using namespace Ogre;

	for (int i = 0; i < 1; i++)
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

void Rat::setupAnimations(){
	this->mTimer = 0;	// Start from the beginning
	this->mVerticalVelocity = 0;	// Not jumping

	// this is very important due to the nature of the exported animations
	mModelEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	// Name of the animations for this character - this will change with new assets
	Ogre::String animNames[] =
		{"swim"};

	// populate our animation list
	for (int i = 0; i < 1; i++)
	{
		mAnims[i] = mModelEntity->getAnimationState(animNames[i]);
		
		//Some animations are not looping
		if (animNames[i] == "swim") mAnims[i]->setLoop(true);
		else mAnims[i]->setLoop(false);

		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setAnimation(SWIM);
}

void Rat::setAnimation(AnimID id, bool reset){
	if (ratAnim >= 0 && ratAnim < 1)
	{
		// if we have an old animation, fade it out
		mFadingIn[ratAnim] = false;
		mFadingOut[ratAnim] = true;
	}

	ratAnim = id; 

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

