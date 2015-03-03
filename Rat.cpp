#include "Rat.h"
#include "GameApplication.h"
#define _USE_MATH_DEFINES 
#include <math.h>

#define CIRCLE_DIST 5;
#define CIRCLE_RAD 10;
#define ANGLE_CHANGE .1;
#define RAT_SPEED .1;

Rat::Rat(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, NPC::GoodBad t, Rat::RatStates s):
	NPC(SceneManager, name, filename, height, scale, a, l, t)
{
	mBodyNode->setPosition(mBodyNode->getPosition()[0], 0, mBodyNode->getPosition()[2]);
	wanderAngle = 0;
	state = s;
	prevState = NONE;
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
	Player* p = app->getPlayerPointer();
	if (state == GUARD){
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
			mDistance = -6.0;
			prevState = state;
			state = SEARCH;
		}
	}
	else if (state == WANDER){
		wander();
		if (checkInFront()){
			prevState = state;
			state = SEEK;
		}
	}
	else if (state == FLEE){
		flee();
		
	}
	else if (state == SEEK){
		seek();
		lastPlayerPos = p->getPosition();
		if (!checkInFront()){
			prevState = state;
			state = LOST;
		}
		if (p->getPosition().distance(mBodyNode->getPosition()) < 5){
			attackPlayer(p);
		}
		else if (p->getPosition().distance(mBodyNode->getPosition()) > 50){
			prevState = state;
			state = LOST;
		}
	}
	else if(state == LOST){
		if (lastPlayerPos.distance(mBodyNode->getPosition()) < 10){
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
			mDistance = -6.0;
			prevState = state;
			state = SEARCH;
		}
	}
	else if (state == SEARCH){
		if (mWalkList.empty() && mDistance <= -5.0){
			if (prevState == LOST){
				Ogre::Vector3 temp = lastPlayerPos + ((app->getGrid()->getCols() * 5)) - 5;
				temp[1] = 0;
				GridNode* dest = app->getGrid()->getNode((int)temp[2] / 10, (int)temp[0] / 10);
				moveTo(dest);
			}
			else if(prevState == GUARD){
				Ogre::Vector3 temp = startPos + (app->getGrid()->getCols() * 5) - 5;
				temp[1] = 0;
				GridNode* dest = app->getGrid()->getNode((int)temp[2] / 10, (int)temp[0] / 10);
				moveTo(dest);
			}
		}
		if (checkInFront()){
			mWalkList.clear();
		}
		searchMove(deltaTime);
	}
	else if (state == DEAD){
		prevState = state;
		state = NONE;
		mBodyNode->roll(Ogre::Degree(180));
	}
	else if (state == NONE){
		return;
	}
	else{
		lookDir = mDirection;
		mDirection = Ogre::Vector3::ZERO;
	}


	if (health <= 0 && !(state == DEAD || state == NONE)){
		state=DEAD;
	}

	if (state != DEAD && lastHit <= 0){
		canHit = true;
	}
	else if (state != DEAD){
		lastHit -= deltaTime;
	}
	else{//when velocity is zero set idle animations
		mTimer = 0;
	}

	checkHit();

	updateLocomote(deltaTime);
}

void Rat::updateLocomote(Ogre::Real deltaTime){
	
	//always rotating
	Ogre::Vector3 src = mBodyNode->getOrientation() * Ogre::Vector3::UNIT_X;//rotate for first location
	if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
	{
		mBodyNode->yaw(Ogre::Degree(180));
	}
	else
	{
		Ogre::Quaternion quat = src.getRotationTo(mDirection.normalisedCopy());
		mBodyNode->rotate(quat);
	}
	//mDirection = mDirection * 100;
	mBodyNode->translate(mDirection);
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

void Rat::attackPlayer(Player* mainPlayer){
	if (canHit){
		dealDamagePlayer(mainPlayer);
		canHit = false;
		lastHit = 2;
	}
}

void Rat::attack(NPC* otherGuys){
	if (canHit){
		dealDamage(otherGuys);
		canHit = false;
		lastHit = 2;
	}
}

void Rat::interact(){
	//possible interactions good or bad//// intro to boss sections
}

void Rat::dealDamage(NPC* guy){
	double randVal = (double)(rand()%100)/100;
	if (randVal <= crit){
		guy->getHurt(damage*1.5);
	}
	else{
		guy->getHurt(damage);
	}
}

void Rat::dealDamagePlayer(Player* player){
	double randVal = (double)(rand()%100)/100;
	if (randVal <= crit){
		player->getHurt(damage*1.5);
	}
	else{
		player->getHurt(damage);
	}
}

void Rat::checkHit(){
	Player* p = app->getPlayerPointer();
	std::list<Ogre::SceneNode*> walls = app->getWallList();
	std::list<NPC*> rats = app->getNPCs();
	if (mDirection != Ogre::Vector3::ZERO){//if the velocity isnt zero set up animations
		mTimer = 0;

		float xBound = (app->getXmax() * 10) - 5;
		float zBound = (app->getZmax() * 10) - 5;
		Ogre::Vector3 myPos = mBodyNode->getPosition();

		if (myPos.x <= -xBound){
			//hit bounds in x direction
			mBodyNode->setPosition(-xBound + 2, myPos.y, myPos.z);
			mDirection.x = -0;
			wanderAngle -= M_PI;
		}else if ( myPos.x >= xBound){
	
			mBodyNode->setPosition(xBound - 2, myPos.y, myPos.z);
			mDirection.x = 0;
			wanderAngle -= M_PI;
		}
		if (myPos.z <= -zBound){
			//hit bounds in z direction
			mBodyNode->setPosition( myPos.x, myPos.y, -zBound + 2);
			mDirection.z = 0;
			wanderAngle -= M_PI;
			
		}else if (myPos.z >= zBound){
			
			mBodyNode->setPosition( myPos.x, myPos.y, zBound - 2);
			mDirection.z = 0;
			wanderAngle -= M_PI;
		}
	}
	
	if (p->getPosition().distance(mBodyNode->getPosition()) < 2.5){
		Ogre::Vector3 playerpos = p->getPosition();

		if (playerpos[0] >= mBodyNode->getPosition()[0]){
			if (mDirection[0] > 0){
				mDirection[0] = 0;
			}
		}
		else{
			if (mDirection[0] < 0){
				mDirection[0] = 0;
			}
		}
		if (playerpos[2] >= mBodyNode->getPosition()[2]){
			if (mDirection[2] > 0){
				mDirection[2] = 0;
			}
		}
		else{
			if (mDirection[2] < 0){
				mDirection[2] = 0;
			}
		}
	}

	Ogre::Vector3 myPos = mBodyNode->getPosition();

	for (Ogre::SceneNode* w : walls){
		Ogre::Vector3 wPos = w->getPosition();
		if ((myPos[0] >= (wPos[0] - 7) && myPos[0] <= (wPos[0] + 7)) && (myPos[2] >= (wPos[2] - 7) && myPos[2] <= (wPos[2] + 7))){
			if(abs(myPos[0] - wPos[0]) < abs(myPos[2] - wPos[2])){
				if (abs(myPos[2] - (wPos[2] +7 )) < abs(myPos[2]-(wPos[2] - 7))){
					myPos[2] = wPos[2] + 7;
				}
				else{
					myPos[2] = wPos[2] - 7;
				}
			}
			else{
				if (abs(myPos[0] - (wPos[0] + 7)) < abs(myPos[0] - (wPos[0] - 7))){
					myPos[0] = wPos[0] + 7;
				}
				else{
					myPos[0] = wPos[0] - 7;
				}
			}
			mBodyNode->setPosition(myPos);
		}
	}
	double vX;
	double vY;
	double magV;
	double aX;
	double aY;
	for (NPC* r : rats){
		if (r != this){
			if (r->getPosition().distance(getPosition()) < 8){
				vX = getPosition()[0] - r->getPosition()[0];
				vY = getPosition()[2] - r->getPosition()[2];
				magV = sqrt(vX * vX + vY * vY);
				aX = r->getPosition()[0] + vX / magV * 8;
				aY = r->getPosition()[2] + vY / magV * 8;
				mBodyNode->setPosition(aX, getPosition()[1], aY);
			}
		}
	}
}

bool Rat::checkInFront(){
	//get line between fish direction and player. check angle between 2 vectors
	//vision calculated by doing the distance between 2 vectors and getting the angle between.
	Ogre::Vector3 inbetween = app->getPlayerPointer()->getPosition() - getPosition();
	inbetween[1] = 0;

	Ogre::Radian angleBetween;
	std::list<Ogre::SceneNode*> walls = app->getWallList();
	Ogre::Vector3 distFromWall;

	if (mBodyNode->getPosition().distance(app->getPlayerPointer()->getPosition()) <= 40){
		if (mDirection != Ogre::Vector3::ZERO){
			angleBetween = mDirection.angleBetween(inbetween);
		}
		else{
			angleBetween = lookDir.angleBetween(inbetween);
		}
		if (angleBetween.valueDegrees() >= 0 && angleBetween.valueDegrees() <= 30){//will have to change when real model gets in
			for (Ogre::SceneNode* w : walls){
				distFromWall = w->getPosition();
				distFromWall[1] = 0;
				distFromWall = distFromWall - getPosition();
				if (distFromWall.angleBetween(inbetween).valueDegrees() <= 5){
					if (distFromWall.length() < inbetween.length()){
						return false;
					}
				}
			}
			return true;
		}
	}
	return false;
}

void Rat::wander(){
	Ogre::Vector3 circleCenter = Ogre::Vector3::ZERO;
	circleCenter = mDirection;
	circleCenter.normalise();
	circleCenter *= CIRCLE_DIST;				//.scaleBy(Circle_Distance);
	Ogre::Vector3 displacement(0,0,-1);			//displacement = vector(0,-1);
	displacement *= CIRCLE_RAD;					//displacement.scaleBy(Circle_radius);
	double len = displacement.length();
	displacement[0] = cos(wanderAngle) * len;
	displacement[2] = sin(wanderAngle) * len;
	wanderAngle += ((double)rand() / RAND_MAX) * ANGLE_CHANGE;
	wanderAngle -= .5 * ANGLE_CHANGE;
	
	mDirection = circleCenter + displacement;
	mDirection.normalise();
	mDirection *= RAT_SPEED;
	mDirection[1] = 0;
}

void Rat::seek(){
	flee();
	mDirection *= -1;
}

void Rat::flee(){
	//run in the opposite direction of Yoshimi
	Ogre::Vector3 playerPos = app->getPlayerPointer()->getPosition();
	Ogre::Vector3 mPosition = mBodyNode->getPosition();
	Ogre::Vector3 desired = (mPosition - playerPos);
	desired.normalise();
	desired *= .05;
	desired[1] = 0;
	mDirection = desired;
	mDirection.normalise();
	mDirection *= RAT_SPEED;
	mDirection[1] = 0;
	/*Ogre::Vector3 steer = desired - mDirection;
	steer[1] = 0;
	steer += mDirection*/							//if you wanna make it impossible to catch the guy;
}




void Rat::searchMove(Ogre::Real deltaTime){
	if (mDistance <= 0.0f){
		mBodyNode->setPosition(mDestination);
		if (!nextLocation()){
			state = prevState;
			prevState = SEARCH;
		}
		else{
			mDestination = mWalkList.front();  // this gets the front of the deque
			mWalkList.pop_front();             // this removes the front of the deque
			
			
			Ogre::Vector3 mPos = getPosition();
			mDirection = mDestination - mPos;// getPosition();
			mDirection[1] = 0;
			mDistance = mDirection.normalise();
			mDirection *= RAT_SPEED;

		}
	}
	mDistance -= mDirection.length();
}


void Rat::walkToGN(GridNode* n){
	//puts a new destination on walklist
		Ogre::Vector3 temp = n->getPosition(app->getGrid()->getRows(), app->getGrid()->getCols());
		temp[1] = height;
		if (!(mDestination.positionEquals(temp, 0.001f))){
			mWalkList.push_back(temp);
		}
}

void Rat::moveTo(GridNode* n){
	std::list<GridNode*> path;
	std::list<GridNode*>::iterator pathIter;
	Grid* grid = app->getGrid();
	Ogre::Vector3 myPos = mBodyNode->getPosition();
	myPos[0] += ((grid->getRows()) * 5) + 5;
	myPos[2] += ((grid->getCols()) * 5) + 5;
	int zVal = (int)(Ogre::Math::Floor(myPos[0])) / 10;
	int xVal = (int)(Ogre::Math::Floor(myPos[2])) / 10;

	GridNode* pos = grid->getNode(xVal, zVal);

	mDestination = pos->getPosition(grid->getRows(), grid->getCols());
	mDistance = 0.0;
	Ogre::Vector3 temp;
	path = app->getGrid()->aStar(pos,n);//get the optimal path
	if (!path.empty()){
		for(pathIter = path.begin(); pathIter != path.end(); pathIter++){//put the path at the end of the current walkList to prevent jumping in space
			temp = (*pathIter)->getPosition(app->getGrid()->getRows(), app->getGrid()->getCols());
			temp[1] = height;//assign the height of the destination so OGRE doesn't rotate
			mWalkList.push_back(temp);
		}
		pos = n; //set position of the agent to goal for when its done moving
	}


}