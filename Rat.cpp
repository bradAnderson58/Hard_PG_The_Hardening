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
	defense = level * 1;
	canHit = true;
	lastHit = 0;
	startState = s;
	lookDir = Ogre::Vector3(1,0,0);
	//startPos = mBodyNode->getPosition();
	setupAnimations();

	Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject("manual1"); 
	Ogre::SceneNode* myManualObjectNode = mBodyNode->createChildSceneNode("manual1_node"); 
 
	// NOTE: The second parameter to the create method is the resource group the material will be added to.
	// If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
	Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material","General"); 
	myManualObjectMaterial->setReceiveShadows(false); 
	myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
	myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(1,0,0,0); 
	myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
	myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 

	//myManualObjectMaterial->dispose();  // dispose pointer, not the material
 
	myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	myManualObject->position(mBodyNode->getPosition()[0], 2, mBodyNode->getPosition()[2]);
	myManualObject->position(Ogre::Vector3(40,2,0)); 
	// etc 
	myManualObject->end(); 
 
	myManualObjectNode->attachObject(myManualObject);

	
}

Rat::~Rat(void)
{
	
}

void Rat::update(Ogre::Real deltaTime){
	Player* p = app->getPlayerPointer();
	if (state == GUARD){
		if (checkInFront()){
				state = SEEK;
		}

		if (startPos.distance(getPosition()) < 2){
			if (mDirection != Ogre::Vector3::ZERO){
				lookDir = mDirection;
			}
			mDirection = Ogre::Vector3::ZERO;
		}
		else{
			mDirection = startPos - mBodyNode->getPosition();
			mDirection.normalise();
			mDirection *= RAT_SPEED;
		}
	}
	else if (state == WANDER){
		wander();
		if (checkInFront()){
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
			state = LOST;
		}
		if (p->getPosition().distance(mBodyNode->getPosition()) < 5){
			attackPlayer(p);
		}
		else if (p->getPosition().distance(mBodyNode->getPosition()) > 50){
			state = LOST;
		}
	}
	else if(state == LOST){
		if (lastPlayerPos.distance(mBodyNode->getPosition()) < 2){
			state = startState;
		}
		else{
			if (checkInFront()){
				state = SEEK;
			}
			mDirection = lastPlayerPos - mBodyNode->getPosition();
			mDirection.normalise();
			mDirection *= RAT_SPEED;
		}
	}
	else if (state == DEAD){
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
	Ogre::Vector3 inbetween = mBodyNode->getPosition() - app->getPlayerPointer()->getPosition();

	Ogre::Radian angleBetween;
	std::list<Ogre::Entity*> walls = app->getWallEntities();
	std::pair<bool, Ogre::Real> temp;
	Ogre::AxisAlignedBox box;

	int inCount = 0;
	int outCount = 0;

	Ogre::Ray test = Ogre::Ray(mBodyNode->getPosition(), inbetween * -1);
	//temp = test.intersects(app->getPlayerPointer()->getBoundingBox());

	if (mBodyNode->getPosition().distance(app->getPlayerPointer()->getPosition()) <= 40){
		if (mDirection != Ogre::Vector3::ZERO){
			angleBetween = mDirection.angleBetween(inbetween);
		}
		else{
			angleBetween = lookDir.angleBetween(inbetween);
		}
		//std::cout << "pos:: " << mBodyNode->getPosition() << std::endl;
		if (angleBetween.valueDegrees() >= 150 && angleBetween.valueDegrees() <= 180){//will have to change when real model gets in
			Ogre::Ray ratRay = Ogre::Ray(mBodyNode->getPosition(), inbetween * -1);
			//std::cout << "ORIGIN: " << mBodyNode->getPosition() << " DIR: " << inbetween << std::endl;
			for (Ogre::Entity* w : walls){
				box = w->getWorldBoundingBox(true);
				temp = ratRay.intersects(box);
				if (temp.first = true){
					std::cout << "intersects" << std::endl;
					if (ratRay.getPoint(temp.second).length() < inbetween.length()){
						std::cout << "blocked by a wall===========================================================" << std::endl;
						return false;
					}
				}
			}
			std::cout << "I see you!!!" << std::endl;
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
