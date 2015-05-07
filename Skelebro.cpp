#include "Skelebro.h"
#include "GameApplication.h"
#define _USE_MATH_DEFINES 
#include <math.h>

#define MULT_MAG 90.0;

Skelebro::Skelebro(Ogre::SceneManager* SceneManager, std::string name, std::string filename, 
	float height, float scale, GameApplication* a, int l, NPC::GoodBad t, NPC::States s):
	NPC(SceneManager, name, filename, height, scale, a, l, t,s)
{
	mBodyNode->setPosition(mBodyNode->getPosition()[0], 0, mBodyNode->getPosition()[2]);

	defense = level * 1;
	damage *= 1.5;
	health *= 1.5;
	mWalkSpeed *= 1.5;
	canHit = true;
	lastHit = 0;
	startState = s;
	lookDir = Ogre::Vector3(1,0,0);
	//startPos = mBodyNode->getPosition();
	mModelNode->yaw(Ogre::Radian(Ogre::Degree(90)));

	numAnimations =  3; //mModelEntity->getSkeleton()->getNumAnimations();
	setupAnimations(); // turn this off if you can't find the animations
	deadTimer = 0;

	lookRange = 30;
	lookAngle = Ogre::Real(180);

}

Skelebro::~Skelebro(void)
{
	
}

void Skelebro::update(Ogre::Real deltaTime){
	// each state can be used as input to get the next animation - B note
	Player* p = app->getPlayerPointer();
	if (state == GUARD){
		setAnimation(IDLE);
		if (checkInFront()){
			prevState = state;
			state = SEEK;
		}

		if (startPos.distance(getPosition()) < 2){
			if (mDirection != Ogre::Vector3::ZERO){
				lookDir = mDirection;
			}
			mDirection = Ogre::Vector3::ZERO;
		}
		else if (prevState == SEARCH){
			lookDir = mDirection;
			mDirection = Ogre::Vector3::ZERO;
		}
		else{
			prevState = state;
			state = SEARCH;
		}
	}
	else if (state == WANDER){
		setAnimation(WALK);
		wander();
		mDirection *= deltaTime * MULT_MAG;
		if (checkInFront()){
			prevState = state;
			state = SEEK;
		}
	}
	else if (state == FLEE){
		setAnimation(WALK);
		flee();
		mDirection *= deltaTime * MULT_MAG;
	}
	else if (state == SEEK){
		setAnimation(WALK);
		seek();
		mDirection *= deltaTime * MULT_MAG;
		lastPlayerPos = p->getPosition();
		if (!checkInFront()){
			prevState = state;
			state = LOST;
		}
		if (p->getPosition().distance(mBodyNode->getPosition()) < 7){
			prevState = state;
			state = ATT;
		}
		else if (p->getPosition().distance(mBodyNode->getPosition()) > (lookRange + 20)){
			prevState = state;
			state = LOST;
		}
	}
	else if(state == LOST){
		setAnimation(WALK);
		if (lastPlayerPos.distance(mBodyNode->getPosition()) < 5){
			prevState = state;
			state = startState;
		}
		else if (prevState == SEARCH){
			prevState = state;
			state = startState;
		}
		else{
			if (checkInFront()){
				prevState = state;
				state = SEEK;
			}
			else{
				prevState = state;
				state = SEARCH;
			}
		}
	}
	else if (state == SEARCH){
		setAnimation(WALK);
		if (mWalkList.empty() && mDistance <= -5.0){
			if (prevState == LOST){
				GridNode* dest = app->getGrid()->getContainedNode(lastPlayerPos);
				moveTo(dest);
			}
			else if(prevState == GUARD){
				GridNode* dest = app->getGrid()->getContainedNode(startPos);
				moveTo(dest);
			}
			else{
				state = startState;
				prevState = SEARCH;
			}
		}
		if (checkInFront()){
			mWalkList.clear();
			state = SEEK;
			prevState = SEARCH;
		}
		else{
			if (mWalkList.empty() && startPos.distance(getPosition()) < .01 && mDestination.positionEquals(startPos, .001)){
				state = startState;
				prevState = SEARCH;
				mDistance = -6.0f;
			}
			else{
				searchMove(deltaTime);
			}
		}
	}
	else if (state == DEAD){
		prevState = state;
		state = NONE;
		mDirection = Ogre::Vector3::ZERO;
	}
	else if (state == NONE){
		updateAnimations(deltaTime);
		updateLocomote(deltaTime);
		deadTimer += deltaTime;
		if (deadTimer > 15){
			state = SEEK;
			health = level * 20;
			deadTimer = 0;
		}
		return;
	}
	else{
		lookDir = mDirection;
		mDirection = Ogre::Vector3::ZERO;
	}

	if (health <= 0 && (state != DEAD && state != NONE)){
		state=DEAD;
		setAnimation(IDLE);
	}

	if (state != DEAD && lastHit <= 0){
		canHit = true;
	}
	else if (state != DEAD && state != NONE){
		lastHit -= deltaTime;
	}
	else{//when velocity is zero set idle animations
		mTimer = 0;
	}

	if (state == ATT){
		mDirection = Ogre::Vector3::ZERO;
		attackPlayer(app->getPlayerPointer());
		prevState = state;
		state = SEEK;
	}
	else{
		updateLocomote(deltaTime);
	}
	checkHit();
	updateAnimations(deltaTime);
}

void Skelebro::attackPlayer(Player* mainPlayer){


	if (canHit){
		setAnimation(ATTACK);
		dealDamagePlayer(mainPlayer);
		canHit = false;
		lastHit = 2;
	}
}

void Skelebro::updateAnimations(Ogre::Real deltaTime){
	if (idOfAnim != ANIM_NONE){
		mAnims[idOfAnim]->addTime(deltaTime);
	}
	//transitions
	fadeAnimations(deltaTime);
}

void Skelebro::fadeAnimations(Ogre::Real deltaTime){
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

void Skelebro::setupAnimations(){
	this->mTimer = 0;				// Start from the beginning
	this->mVerticalVelocity = 0;	// Not jumping

	// this is very important due to the nature of the exported animations
	mModelEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	// Name of the animations for this character - this will change with new assets
	Ogre::String animNames[] = {"Idle", "Attack", "Walk"};

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
	}
	setAnimation(IDLE);
}

void Skelebro::setAnimation(SkelAnimID id, bool reset){
	if (id == idOfAnim) return;
	if ((idOfAnim == SkelAnimID::ATTACK) && !mAnims[idOfAnim]->hasEnded()) return;

	if (idOfAnim >= 0 && idOfAnim < numAnimations)
	{
		// if we have an old animation, fade it out
		mFadingIn[idOfAnim] = false;
		mFadingOut[idOfAnim] = true;
	}

	idOfAnim = id; 
	if (idOfAnim == ATTACK) mAnims[idOfAnim]->setTimePosition(0);
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

