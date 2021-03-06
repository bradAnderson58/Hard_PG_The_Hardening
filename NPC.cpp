#include "NPC.h"
#include "GameApplication.h"
#include "Event.h"
#include "Environment.h"
#define _USE_MATH_DEFINES 
#include <math.h>

#define CIRCLE_DIST 5;
#define CIRCLE_RAD 10;
#define ANGLE_CHANGE .1;
#define MULT_MAG 75.0;

NPC::NPC(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a, int l, GoodBad t, States s):
	Agent(SceneManager, name, filename, height, scale, a)
{
	type = t;
	level = l;
	health = level * 20;
	crit = .01 * level * 5;
	damage = log(level) * level + 5;
	defense = level * 10;  //need to reset based on every enemy
	mWalkSpeed = 0.1;
	wanderAngle = 0;

	canHit = true;
	lastHit = 0;
	state = s;
	startState = s;
	prevState = NONE;

	lookAngle = Ogre::Real(30);
	lookRange = 40;

	mDistance = -5.0;

	mEvent = NULL;

	numAnimations =  mModelEntity->getSkeleton()->getNumAnimations();
}

NPC::~NPC(void)
{
	
}

void NPC::update(Ogre::Real deltaTime){
	if (type == GOOD){
		updateGood(deltaTime);
	}
	else{
		updateBad(deltaTime);
	}
}

void NPC::updateGood(Ogre::Real deltaTime){
	//does nothing yet. NO GOOD GUYS
}

void NPC::updateBad(Ogre::Real deltaTime){
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
		mDirection *= deltaTime * MULT_MAG;
		//mBodyNode->roll(Ogre::Degree(180));
	}
	else if (state == NONE){
		updateAnimations(deltaTime);
		updateLocomote(deltaTime);
		return;
	}
	else{
		lookDir = mDirection;
		mDirection = Ogre::Vector3::ZERO;
		mDirection *= deltaTime * MULT_MAG;
	}

	if (health <= 0 && (state != DEAD && state != NONE)){
		state=DEAD;
		setAnimation(DIE);
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

void NPC::updateLocomote(Ogre::Real deltaTime){
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

void NPC::attackPlayer(Player* mainPlayer){


	if (canHit){
		setAnimation(ATTACK);
		dealDamagePlayer(mainPlayer);
		canHit = false;
		lastHit = 2;
	}
}

void NPC::attack(NPC* otherGuys){
	if (canHit){
		dealDamage(otherGuys);
		canHit = false;
		lastHit = 2;
	}
}

void NPC::interact(){
	//possible interactions good or bad//// intro to boss sections
}

void NPC::dealDamage(NPC* guy){
	double randVal = (double)(rand()%100)/100;
	if (randVal <= crit){
		guy->getHurt(damage*1.5);
	}
	else{
		guy->getHurt(damage);
	}
}

void NPC::dealDamagePlayer(Player* player){
	double randVal = (double)(rand()%100)/100;
	if (randVal <= crit){
		player->getHurt(damage*1.5);
	}
	else{
		player->getHurt(damage);
	}
}

void NPC::checkHit(){
	Player* p = app->getPlayerPointer();
	std::list<Ogre::Vector3> walls = app->getWallList();
	std::list<NPC*> rats = app->getNPCs();
	if (mDirection != Ogre::Vector3::ZERO){//if the velocity isnt zero set up animations
		mTimer = 0;

		float xBound = (app->getZmax() * 10) - 5;
		float zBound = (app->getXmax() * 10) - 5;
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
	
	if (p->getPosition().distance(mBodyNode->getPosition()) <= 5){
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

	for (Ogre::Vector3 w : walls){
		//Ogre::Vector3 wPos = w->getPosition();
		if ((myPos[0] >= (w[0] - 7) && myPos[0] <= (w[0] + 7)) && (myPos[2] >= (w[2] - 7) && myPos[2] <= (w[2] + 7))){
			if(abs(myPos[0] - w[0]) < abs(myPos[2] - w[2])){
				if (abs(myPos[2] - (w[2] +7 )) < abs(myPos[2]-(w[2] - 7))){
					myPos[2] = w[2] + 7;
				}
				else{
					myPos[2] = w[2] - 7;
				}
			}
			else{
				if (abs(myPos[0] - (w[0] + 7)) < abs(myPos[0] - (w[0] - 7))){
					myPos[0] = w[0] + 7;
				}
				else{
					myPos[0] = w[0] - 7;
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

	std::vector<Environment*> objs = app->getEnvObj();

	for (Environment* ob : objs){
		if (!ob->isPassable()){
			Ogre::Vector3 wPos = ob->getPosition();

			//some magic numbers in hur son!
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
	}
}

bool NPC::checkInFront(){
	//get line between fish direction and player. check angle between 2 vectors
	//vision calculated by doing the distance between 2 vectors and getting the angle between.
	Ogre::Vector3 inbetween = app->getPlayerPointer()->getPosition() - getPosition();
	inbetween[1] = 0;

	Ogre::Radian angleBetween;
	std::list<Ogre::Vector3> walls = app->getWallList();
	Ogre::Vector3 distFromWall;

	if (mBodyNode->getPosition().distance(app->getPlayerPointer()->getPosition()) <= lookRange){
		if (mDirection != Ogre::Vector3::ZERO){
			angleBetween = mDirection.angleBetween(inbetween);
		}
		else{
			angleBetween = lookDir.angleBetween(inbetween);
		}
		if (angleBetween.valueDegrees() >= 0 && angleBetween.valueDegrees() <= lookAngle){//will have to change when real model gets in
			for (Ogre::Vector3 distFromWall : walls){
				//distFromWall = w->getPosition();
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

void NPC::wander(){
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
	mDirection *= mWalkSpeed;
	mDirection[1] = 0;
}

void NPC::seek(){
	flee();
	mDirection *= -1;
}

void NPC::flee(){
	//run in the opposite direction of Yoshimi
	Ogre::Vector3 playerPos = app->getPlayerPointer()->getPosition();
	Ogre::Vector3 mPosition = mBodyNode->getPosition();
	Ogre::Vector3 desired = (mPosition - playerPos);
	desired.normalise();
	desired *= .05;
	desired[1] = 0;
	mDirection = desired;
	mDirection.normalise();
	mDirection *= mWalkSpeed;
	mDirection[1] = 0;
	/*Ogre::Vector3 steer = desired - mDirection;
	steer[1] = 0;
	steer += mDirection*/							//if you wanna make it impossible to catch the guy;
}

// recoil effect
void
NPC::getPushed(Ogre::Vector3 direction)
{
	Ogre::Vector3 push_force = Ogre::Vector3(2.0, 1.0, 2.0);
	mBodyNode->translate(direction * push_force);
}

void NPC::getHurt(int d){
	if (state != DEAD && state != NONE){
		state = SEEK;
		if (d > defense){
			health -= (d - defense);
		}
	}
}

bool 
NPC::nextLocation()
{
	if (mWalkList.empty()){
		return false;
	}
	return true;
}

void NPC::searchMove(Ogre::Real deltaTime){
	if (mDistance <= 0){
		mBodyNode->setPosition(mDestination);
		if (!nextLocation()){
			mDistance = -5.0;
			mWalkList.clear();
			state = SEARCH;
			prevState = startState;
			mDirection = Ogre::Vector3::ZERO;
		}
		else{
			mDestination = mWalkList.front();  // this gets the front of the deque
			mWalkList.pop_front();             // this removes the front of the deque
			
			Ogre::Vector3 mPos = getPosition();
			mDirection = mDestination - mPos; // getPosition();
			mDirection[1] = 0;
			mDistance = mDirection.normalise();
			mDirection *= mWalkSpeed * deltaTime * MULT_MAG;
		}
	}
	mDirection[1] = 0;
	mDistance -= mDirection.length();
}


void NPC::walkToGN(GridNode* n){
	//puts a new destination on walklist
		Ogre::Vector3 temp = n->getPosition(app->getGrid()->getRows(), app->getGrid()->getCols());
		temp[1] = height;
		if (!(mDestination.positionEquals(temp, 0.001f))){
			mWalkList.push_back(temp);
		}
}

void NPC::moveTo(GridNode* n){
	std::list<GridNode*> path;
	std::list<GridNode*>::iterator pathIter;
	Grid* grid = app->getGrid();
	Ogre::Vector3 myPos = mBodyNode->getPosition();
	Ogre::Vector3 temp; 

	GridNode* pos = grid->getContainedNode(myPos);
	if (pos != NULL){
		int rows = grid->getRows();
		int cols = grid->getCols();

		mDestination = pos->getPosition(rows, cols);
		mDestination[1] = height;
		mDistance = 0.0;

		path = app->getGrid()->aStar(pos,n);//get the optimal path

		mWalkList.push_back(mDestination);//push first node onto the list
		mDirection = mDestination - myPos;//set first direction and speed
		mDirection[1] = 0;
		mDistance = mDirection.normalise();
		mDirection *= mWalkSpeed;

		if (!path.empty()){
			for(pathIter = path.begin(); pathIter != path.end(); pathIter++){//put the path at the end of the current walkList to prevent jumping in space
				temp = (*pathIter)->getPosition(rows, cols);
				temp[1] = height;//assign the height of the destination so OGRE doesn't rotate
				mWalkList.push_back(temp);
			}
			pos = n; //set position of the agent to goal for when its done moving
			temp = n->getPosition(rows,cols);
			temp[1] = height;
			mWalkList.push_back(temp);
		}
	}

}

//virtuals ========================================
void NPC::updateAnimations(Ogre::Real deltaTime){

}

void NPC::fadeAnimations(Ogre::Real deltaTime){

}

void NPC::setupAnimations(){
	// do this one in NPC's children
}

void NPC::setAnimation(AnimID id, bool reset){

}