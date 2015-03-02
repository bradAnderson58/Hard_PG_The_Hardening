#include "Player.h"
#include "GameApplication.h"
#include "GUIController.h"
#include "Environment.h"
#include "NPC.h"
#define _USE_MATH_DEFINES   
#include <math.h>


Player::Player(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a):
	Agent(SceneManager, name, filename, height, scale, a)
{
	//set player to origin to attach the camera
	mBodyNode->setPosition(0.0,0.0,0.0);
	mBodyNode->yaw(Ogre::Radian(M_PI));

	//Camera set up here
	Ogre::Vector3 temp(mBodyNode->getPosition());
	Ogre::Vector3 temp1(temp[0], temp[1]+40, temp[2]+40);//move camera to location. temp[1] is height relative to player temp[2] is z distance
	Ogre::Camera* cam = app->getCamera();
	cam->pitch(Ogre::Radian(-10 * M_PI /180));
	cam->setPosition(temp1);

	//attaching camera code here
	mNullCamera = mBodyNode->createChildSceneNode();
	Ogre::SceneNode* camNode = mNullCamera->createChildSceneNode();
	camNode->attachObject(cam);

	fForward = false;  //starts by not moving
	doingStuff = false;  //starts not doing anything
	speed = 2;	//You're a quick one - DELETE?

	//mBodyNode->showBoundingBox(true);				//for testing purposes Player box

	//attack space
	mAttackNode = mModelNode->createChildSceneNode();
	mAttackEntity = mSceneMgr->createEntity("attackCube", Ogre::SceneManager::PT_CUBE);
	mAttackEntity->setMaterialName("Examples/RustySteel");
	mAttackNode->attachObject(mAttackEntity);
	
	//set up the position of the attackNode
	mAttackNode->setPosition(0.0f ,100.0f, -100.0f);
	mAttackNode->setVisible(false);
	//mAttackNode->showBoundingBox(true);		//for testing  Attack box

	//for constant rotation
	mRotator = 0;
	fRot = false;
	isBlocking = false;

	level = 1;				//your level

	//attributes for player
	evilAtt = 1;			// affects crit, mana
	strengthAtt = 1;		// affects dam, def
	dexterityAtt = 1;		// affects heal, crit
	constitutionAtt = 1;	// affects def, heal
	intelligenceAtt = 1;	// affects mana, dam

	equippedWpn = new UsableItems(UsableItems::WEAPON, 1, 0, 0, 0, 0, "Bandito Shiv", 1);	//The weapon you are using
	equippedShield = NULL;
	equippedHelm = new UsableItems(UsableItems::HELM, 0, 0, 0, 0, 0, "Skull Cap", 1);
	equippedBoobs = new UsableItems(UsableItems::BOOBPLATE, 0, 0, 0, 0, 0, "Dark Hoodie", 2);
	equippedPants = new UsableItems(UsableItems::PANTS, 0, 0, 0, 0, 0, "Pantaloons", 2);
	equippedNeck = new UsableItems(UsableItems::NECKLACE, 0, 0, 0, 0, 0, "Dangling Pointer", 15);

	inventory.push_back(new UsableItems(UsableItems::WEAPON, 20, 0, 0, 0, 0, "The Allocator!", 5000));  //Super Awesome weapon for testing

	// some projectile to throw
	mFireball = new Projectile(SceneManager, "fireball", "geosphere4500.mesh",
		height, scale/4, a); 


	srand(time(NULL));  //seed for random number generation

	//intialize stats with update stats function
	updateStats();
	updateGUI = a->getGUICont();
	//updateGUI->recordUpdator();
	
	setupAnimations();
}

Player::~Player(void)
{
	
}

void Player::updateStats(){
	//call to quickly update them stats when needed
	// Brandon NOTE: called when changing gear?
	// maybe call when leveling too, just need to include that in math
	criticalStat = ((1.5 * (double) evilAtt) + (.5 * (double) dexterityAtt) ) / 200;	//initially 1% critical strike chance
	damageStat = (1.5 * (double) strengthAtt) + (.5 * (double) intelligenceAtt);		//base damage initially 2
	defenseStat = (1.5 * (double) constitutionAtt) + (.5 * (double) strengthAtt);		//base defense initially 2
	healthStat = (15 * (double) dexterityAtt) + (10 * (double) constitutionAtt);		//base health intially 25
	manaStat = (10 * (double) intelligenceAtt) + (5 * (double) evilAtt);				//base mana initially 15

	healthNow = healthStat;		// updates health and mana as well, would do this
	manaNow = manaNow;			// if lvling up calls updateStats
								// if changing gear calls this, than adjust this.

	//This is what my damage or my defense is
	updateDamDef();
}
//This is what my damage or my defense is
//Player will probably equip more frequently than level up
void Player::updateDamDef(){
	mDamage = (equippedWpn != NULL) ? damageStat + equippedWpn->getStat(UsableItems::DAMAGE) : damageStat;
	mDefense =	defenseStat;
	if (equippedHelm != NULL) mDefense += equippedHelm->getStat(UsableItems::DEFENSE);
	if (equippedBoobs != NULL) mDefense += equippedBoobs->getStat(UsableItems::DEFENSE);
	if (equippedPants != NULL) mDefense += equippedPants->getStat(UsableItems::DEFENSE);
	if (equippedNeck != NULL) mDefense += equippedNeck->getStat(UsableItems::DEFENSE);

}

void Player::update(Ogre::Real deltaTime){

	this->updateAnimations(deltaTime);		// Update animation playback
	this->updateLocomote(deltaTime);		// Update Locomotion

	this->mFireball->update(deltaTime);		// update projectile 

}

// change face img file based on current status
// call in getHit()?
void Player::updateFace()
{
	if (healthNow <= 0)
		mFace = "thumb_cel.png";
	else
		mFace = "thumb_cel.png";
}

void Player::updateLocomote(Ogre::Real deltaTime){
	//rotate the camera using joystick
	if (fForward) rotationCode(mPlayerRot);

	Ogre::Quaternion q;
	Ogre::Vector3 translator = Ogre::Vector3::ZERO;

	//rotate from current direction
	q.FromAngleAxis(Ogre::Radian(M_PI) / 2, Ogre::Vector3(0,1,0));
	Ogre::Vector3 side = q*mDirection;

	//set translation based on controller input
	if (fForward) translator += (side * -mPlayerVel);
	if (fRot) mNullCamera->yaw(Ogre::Degree(mRotator));

	//Set Yoshimi Animation based on movement (if not already doing stuff)
	if (!doingStuff){
		if (!fForward){
			if (playerAnim != IDLE_THREE) setAnimation(IDLE_THREE);
		}else{
			if (playerAnim != STEALTH) setAnimation(STEALTH);
		}
	}
	//border wall coordinates
	float maxX = app->getXmax() * 10 -5;
	float maxZ = app->getZmax() * 10 -5;

	Ogre::Vector3 newPos = translator + getPosition();
	//doing collisions with border walls - this will need to be expanded for all walls and objects
	if (newPos[0] > maxX -.5){
		newPos[0] = maxX -.5;
	}
	else if (newPos[0] < -maxX + .5){
		newPos[0] = -maxX + .5;
	}
	if (newPos[2] > maxZ - .5){
		newPos[2] = maxZ - .5;
	}
	else if(newPos[2] < -maxZ + .5){
		newPos[2] = -maxZ + .5;
	}
	newPos = collisionWalls(newPos);
	newPos = collisionRobots(newPos);
	newPos = collisionObjects(newPos);
	mBodyNode->setPosition(newPos);
}

//Set movement flag
void Player::setMovement(bool on){
	//Set the correct flag to the new boolean value
	fForward = on;
}

void Player::rotationCode(OIS::MouseEvent arg){
	mNullCamera->yaw(Ogre::Degree(arg.state.X.rel * -0.5f));
}

void Player::rotationCode(double rad){
	//update mDirection
	mDirection = mNullCamera->getOrientation() * Ogre::Vector3(std::sin(rad), 0, std::cos(rad));

	//always rotating
	Ogre::Vector3 src = mModelNode->getOrientation() * Ogre::Vector3::UNIT_X;//rotate for first location
	if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
	{
		mModelNode->yaw(Ogre::Degree(180));
	}
	else
	{
		Ogre::Quaternion quat = src.getRotationTo(mDirection.normalisedCopy());
		mModelNode->rotate(quat);
	}
	
}

void Player::cameraRot(double rad){
	//set rotation variables for update function
	fRot = true;
	mRotator = rad;
}

void Player::playerRot(double rad){
	//update player rotation
	mPlayerRot = rad;
}

void Player::setVelocity(double vel){
	//set velocity
	mPlayerVel = vel;
}

void Player::updateAnimations(Ogre::Real deltaTime){

	//If Yoshimi has an active animation, call the update method
	if (playerAnim != ANIM_NONE){
		mAnims[playerAnim]->addTime(deltaTime * speed);
		if (mAnims[playerAnim]->hasEnded() && playerAnim != BLOCK && playerAnim != DEATH_ONE){
			doingStuff = false;   //no longer doing stuff
			speed = 2;
		}
	}
	//transitions
	fadeAnimations(deltaTime);
	
}

void Player::fadeAnimations(Ogre::Real deltaTime){
	using namespace Ogre;

	for (int i = 0; i < 20; i++)
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

void Player::setupAnimations(){

	
	this->mTimer = 0;	// Start from the beginning
	this->mVerticalVelocity = 0;	// Not jumping

	// this is very important due to the nature of the exported animations
	mModelEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	// Name of the animations for this character - this will change with new assets
	Ogre::String animNames[] =
		{  "Attack1", "Attack2", "Attack3", "Backflip", "Block", "Climb", "Crouch", "Death1", "Death2", "HighJump", "Idle1", "Idle2",
		"Idle3", "Jump", "JumpNoHeight", "Kick", "SideKick", "Spin", "Stealth", "Walk"};

	// populate our animation list
	for (int i = 0; i < 20; i++)
	{
		mAnims[i] = mModelEntity->getAnimationState(animNames[i]);
		
		//Some animations are not looping
		if (animNames[i] == "Idle3" || animNames[i] == "Stealth") mAnims[i]->setLoop(true);
		else mAnims[i]->setLoop(false);

		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setAnimation(IDLE_TWO);

}

void Player::setAnimation(AnimID id, bool reset){
	if (playerAnim >= 0 && playerAnim < 20)
	{
		// if we have an old animation, fade it out
		mFadingIn[playerAnim] = false;
		mFadingOut[playerAnim] = true;
	}

	playerAnim = id; 

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

void Player::buttonAnimation(int en, bool start){
	if (en == OIS::KC_SPACE){
		setAnimation(JUMP, true);
	}
	else if (en == OIS::KC_Q) setAnimation(ATTACK_ONE, true);  //throw fishbomb?
	else if (en == OIS::MB_Left ) setAnimation(ATTACK_THREE, true);	//use sword
	else if (en == OIS::MB_Right && start) setAnimation(BLOCK, true);		  //Blocking
	else if (en == OIS::MB_Right) setAnimation(IDLE_THREE, true);
	else if (en == OIS::KC_F) setAnimation(ATTACK_TWO, true);
}

//Player checks the robot list to see if any robots are close enough to hit
//If so, they get hurted and such
//TODO: This will be updated as enemies are introduced
void Player::checkHits(){
	
	//get bounding boxes for Player and her adversaries
	Ogre::AxisAlignedBox aRange = mAttackEntity->getWorldBoundingBox();
	Ogre::AxisAlignedBox rRange;

	for (NPC *enemy : app->getNPCs()){
		rRange = enemy->getBoundingBox();

		if (aRange.intersects(rRange)){
			dealDamage(enemy);	//hit em
		}
	}
	//ADD MORE
	//app->
}

void Player::dealDamage(NPC *enemy){

	//base damage plus weapon damage
	int damage = mDamage;
	float critPerc = (float)(rand() % 100) / 100.0;
	if (critPerc <= criticalStat){ //critical strike - extra damages!
		damage = 1.5 * damage;
	}

	enemy->getHurt(damage);
}

//damage done to the player
void Player::getHurt(int damage){
	int temp = mDefense;
	//blocking adds 1 defense plus whatever defense the shield provides if equipped
	if (isBlocking){
		temp += (equippedShield != NULL) ? equippedShield->getStat(UsableItems::DEFENSE) + 1 : 1;
	}

	//you loose health equal to the attackers damage minus your defense
	//if your defense is higher than the enemies damage, you take no damage?
	healthNow -= ((damage - temp) >= 0) ? damage - temp : 0;
	if (healthNow <= 0){
		healthNow = 0;
		app->toggleState(GameApplication::DEAD_STATE);
	}

}

// show and fire a projectile
void
Player::shoot()
{
	if (!mFireball->isActive() && !doingStuff)
	{
		std::cout << "Fireball!" << std::endl;
		mFireball->fire(mDirection[0], mDirection[1]+10.0, mDirection[2],
						getPosition());
	}
}

//Expand to cover collisions with all NPCs
//robots will push yoshimi out of the way
Ogre::Vector3 Player::collisionRobots(Ogre::Vector3 myPos){
	std::list<NPC*> npcs = app->getNPCs();
	double vX;
	double vY;
	double magV;
	double aX;
	double aY;
	for (NPC* n : npcs){
		if (myPos.distance(n->getPosition()) < 2){
			vX = getPosition()[0] - n->getPosition()[0];
			vY = getPosition()[2] - n->getPosition()[2];
			magV = sqrt(vX * vX + vY * vY);
			aX = n->getPosition()[0] + vX / magV * 2.5;
			aY = n->getPosition()[2] + vY / magV * 2.5;
			return Ogre::Vector3(aX, getPosition()[1], aY);
		}
	}
	return myPos;
}

//moved into updatelocomote - will probably be added back in
Ogre::Vector3 Player::collisionWalls(Ogre::Vector3 myPos){
	std::list<Ogre::SceneNode*> walls = app->getWallList();
	//Ogre::Vector3 myPos = mBodyNode->getPosition();

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
			return myPos;
		}
	}
	return myPos;
	//Repurpose
}

//hacky copy-paste.  Can probably refactor.
//This checks to see if we are colliding with an Environment objects
Ogre::Vector3 Player::collisionObjects(Ogre::Vector3 myPos){
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
				return myPos;
			}
		}
	}
	return myPos;
}

//reset player to the initial position and reset initialize variables - will need again eventually
void Player::restart(){
	mBodyNode->setPosition(initPos);
	mBodyNode->yaw(Ogre::Radian(M_PI));
	fForward = false;  //starts by not moving
	doingStuff = false;  //starts not doing anything
}

void Player::die(){
	setAnimation(DEATH_ONE);
	doingStuff = true;
	fForward = false;
}

void Player::switchEquipment(int ind){
	//determine the type of equipment, and swtich it out
	UsableItems::itemType typ = inventory[ind]->getType();
	UsableItems* temp = inventory[ind];
	inventory.erase(inventory.begin()+ind);

	//Replace the correct equipment
	if (typ == UsableItems::WEAPON){
		std::cout << "Switching Weapons" << std::endl;
		if (equippedWpn != NULL) inventory.push_back(equippedWpn);
		equippedWpn = temp;
	}
	else if (typ == UsableItems::BOOBPLATE){
		if (equippedBoobs != NULL) inventory.push_back(equippedBoobs);
		equippedBoobs = temp;
	}
	else if (typ == UsableItems::HELM){
		if (equippedHelm != NULL) inventory.push_back(equippedHelm);
		equippedHelm = temp;
	}
	else if (typ == UsableItems::NECKLACE){
		if (equippedNeck != NULL) inventory.push_back(equippedNeck);
		equippedNeck = temp;
	}
	else if (typ == UsableItems::PANTS){
		if (equippedPants != NULL) inventory.push_back(equippedPants);
		equippedPants= temp;
	}
	else if (typ == UsableItems::SHIELD){
		if (equippedShield != NULL) inventory.push_back(equippedShield);
		equippedShield = temp;
	}
	else {std::cout << "ERROR INVALID ITEM TYPE" << std::endl;}

	//apply changes for new weapons
	updateDamDef();
}