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
	//mVisionNode = mModelNode->createChildSceneNode();
	//mVisionEntity = mSceneMgr->createEntity("visionCube" + mBodyNode->getName(), Ogre::SceneManager::PT_CUBE);
	//mVisionEntity->setMaterialName("Examples/testing");
	//mVisionNode->setScale(.4,.02,.1);
	//mVisionNode->attachObject(mVisionEntity);
	//mVisionNode->showBoundingBox(true);
	//mVisionNode->setPosition(mBodyNode->getPosition()[0]-20, mBodyNode->getPosition()[1], mBodyNode->getPosition()[2]);
	//mVisionNode->setVisible(false);

	//rayNode = SceneManager->getRootSceneNode()->createChildSceneNode();
	/*rayEntity = mSceneMgr->createEntity("rayCube" + mBodyNode->getName(), Ogre::SceneManager::PT_CUBE);
	rayEntity->setMaterialName("Examples/testing");
	rayNode->setScale(.05,.05,.05);
	rayNode->attachObject(rayEntity);*/
	//rayNode->setPosition(mBodyNode->getPosition());


	//mBodyNode->yaw(Ogre::Degree(-180.0f));	//rotate so fish faces the right direction	mVisionNode = mModelNode->createChildSceneNode();
	mBodyNode->setPosition(mBodyNode->getPosition()[0], 0, mBodyNode->getPosition()[2]);
	wanderAngle = 0;
	state = s;
	defense = level * 1;
	canHit = true;
	lastHit = 0;
	setupAnimations();

	//Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject("manual1"); 
	//Ogre::SceneNode* myManualObjectNode = mBodyNode->createChildSceneNode("manual1_node"); 
 //
	//// NOTE: The second parameter to the create method is the resource group the material will be added to.
	//// If the group you name does not exist (in your resources.cfg file) the library will assert() and your program will crash
	//Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create("manual1Material","General"); 
	//myManualObjectMaterial->setReceiveShadows(false); 
	//myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
	//myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(1,0,0,0); 
	//myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
	//myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 

	////myManualObjectMaterial->dispose();  // dispose pointer, not the material
 //
	//myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST); 
	//myManualObject->position(mModelNode->getPosition()[0], 2, mModelNode->getPosition()[2]);
	//myManualObject->position(40, 2, 0); 
	//// etc 
	//myManualObject->end(); 
 //
	//myManualObjectNode->attachObject(myManualObject);
}

Rat::~Rat(void)
{
	
}

void Rat::update(Ogre::Real deltaTime){
	Player* p = app->getPlayerPointer();
	//std::cout << "DELTATIME: " << deltaTime << std::endl;
	if (state == GUARD){
		mDirection = Ogre::Vector3::ZERO;
		if (checkInFront()){
			state = SEEK;
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
		if (p->getPosition().distance(mBodyNode->getPosition()) < 5){
			if (canHit){
				p->getHurt(damage);
				canHit = false;
				lastHit = 2;
			}
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
	
	if (mDirection != Ogre::Vector3::ZERO){//if the velocity isnt zero set up animations
		//if (ratAnim != WALK){
		//	setAnimation(WALK);
		//}
		mTimer = 0;

		//check wall issues
		//checkBoundaryCollision();  
		float xBound = (app->getXmax() * 10) - 5;
		float zBound = (app->getZmax() * 10) - 5;
		Ogre::Vector3 myPos = mBodyNode->getPosition();
		//Ogre::Vector3 house = app->getHousePointer()->getPosition();
		//house.y = 0;  //to prevent crazies

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
	else{//when velocity is zero set idle animations
		//if(rarAnim != IDLE ){
			//setAnimation(IDLE);
		//}
		mTimer = 0;
	}

}

void Rat::updateLocomote(Ogre::Real deltaTime){
	//will overwrite
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
	//possibly change to something else
}

void Rat::attack(NPC* otherGuys){
	//possibly change to something else
}

void Rat::interact(){
	//possible interactions good or bad//// intro to boss sections
}

void Rat::dealDamage(NPC* guy){
	//hitting any Rat
}

void Rat::dealDamagePlayer(Player* player){
	//hitting the player
}

void Rat::checkHit(){
	//look through gameapp to see if something was hit
}

bool Rat::checkInFront(){
	//Ogre::AxisAlignedBox aRange = mVisionEntity->getWorldBoundingBox();			//get ratVisionBox to see if player is visible
	//Ogre::AxisAlignedBox rRange = app->getPlayerPointer()->getBoundingBox();	//get the players bounding box
	//return aRange.intersects(rRange);												//return true if the bounding boxes intersect

	double rad = 0.2618;
	Ogre::Vector3 startPos = Ogre::Vector3(mBodyNode->getPosition());
	startPos[1] = 5;
	Ogre::Vector3 ray1Dir ;
	Ogre::Vector3 ray2Dir ;
	Ogre::Vector3 ray3Dir ;
	Ogre::Vector3 ray4Dir ;
	Ogre::Vector3 ray5Dir ;
	if (mDirection != Ogre::Vector3::ZERO){
		ray1Dir = Ogre::Vector3(mDirection * Ogre::Vector3(sin(rad), 0, cos(rad)));
		ray2Dir = Ogre::Vector3(mDirection * Ogre::Vector3(sin(2*rad), 0, cos(2*rad)));
		ray3Dir = Ogre::Vector3(mDirection * Ogre::Vector3(sin(-rad), 0, cos(-rad)));
		ray4Dir = Ogre::Vector3(mDirection * Ogre::Vector3(sin(-2*rad), 0, cos(-2*rad)));
		ray5Dir = Ogre::Vector3(mDirection);
	}
	else{
		ray1Dir = Ogre::Vector3(Ogre::Vector3(1,0,0) * Ogre::Vector3(sin(rad), 0, cos(rad)));
		ray2Dir = Ogre::Vector3(Ogre::Vector3(1,0,0) * Ogre::Vector3(sin(2*rad), 0, cos(2*rad)));
		ray3Dir = Ogre::Vector3(Ogre::Vector3(1,0,0) * Ogre::Vector3(sin(-rad), 0, cos(-rad)));
		ray4Dir = Ogre::Vector3(Ogre::Vector3(1,0,0) * Ogre::Vector3(sin(-2*rad), 0, cos(-2*rad)));
		ray5Dir = Ogre::Vector3(1, 0, 0);
	}
	Ogre::Ray ray1 = Ogre::Ray(startPos, ray1Dir);
	Ogre::Ray ray2 = Ogre::Ray(startPos, ray2Dir);
	Ogre::Ray ray3 = Ogre::Ray(startPos, ray3Dir);
	Ogre::Ray ray4 = Ogre::Ray(startPos, ray4Dir);
	Ogre::Ray ray5 = Ogre::Ray(startPos, ray5Dir);
	std::list<Ogre::Ray> rayList;
	rayList.push_back(ray1);
	rayList.push_back(ray2);
	rayList.push_back(ray3);
	rayList.push_back(ray4);
	rayList.push_back(ray5);
	//rayNode->setPosition(ray5.getPoint(36));
	Player *p = app->getPlayerPointer();
	if (p->getPosition().distance(mBodyNode->getPosition()) <= 40){
		for (Ogre::Ray ray : rayList){
			if(ray.intersects(p->getBoundingBox()).first){
				//std::cout << ray.intersects(p->getBoundingBox()).second << std::endl;
				std::cout << "I see you!!" << std::endl;
				return true;
			}
		}
	}
	else{ 
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
