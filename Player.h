#ifndef __Player_h_
#define __Player_h_

#include "Agent.h"
#include "Projectile.h"
#include "AoE.h"

class GameApplication;
class GUIController;
class NPC;
class UsableItems;
class Environment;
class Spell;
class Projectile;
class AoE;

class Player : public Agent{

public:
	enum skillID
	{
		ABSORB,
		BASH,
		HAMSTRING,
		MASSIVEBLOW,
		FIREBALL,
		FREEZE,
		SHOCK
	};

	Player(Ogre::SceneManager* SceneManager, std::string name, std::string filename, float height, float scale, GameApplication* a);
	~Player();

	void update(Ogre::Real deltaTime);			// update hero!
	void updateLocomote(Ogre::Real deltaTime);
	void setMovement(bool on);					//set the movemnt
	void rotationCode(OIS::MouseEvent arg);		//Rotate the hero when we move the mouse

	void doUpdateGUI();

	void rotationCode(double arg);				//Rotate the hero
	void cameraRot(double arg);					//for rotating camera with RS joystick
	void playerRot(double arg);					//for rotating player with LS joystick
	void setVelocity(double arg);				//Set how fast to walk
	void buttonAnimation(int en, bool start);	//tell me what animation to do
	
	double mPlayerRot;
	double mPlayerVel;
	bool doingStuff;		//if Yoshimi is currently doing a non-looping animation
	void changeSpeed(double s) { speed = s; }	//change the speed for different animations

	void updateStats();
	void updateDamDef();
	void die();

	double getHealthStat()	{ return healthStat; }
	double getHealthNow()	{ return healthNow; }
	double getManaStat()	{ return manaStat; }
	double getManaNow()		{ return manaNow; }

	void checkBump();
	Ogre::Vector3 getPosition(){ return mBodyNode->getPosition(); }
	void restart();
	void setInitPos(Ogre::Vector3 p){initPos = p;}

	void checkHits();			// for attacks - check if an enemy gets hit
	void getHurt(int dam);		// code for when your getting hurt
	void setBlocking(bool y) { isBlocking = y; }

	void shoot(skillID skill);	//shoot a projectile

	UsableItems* getWpn(){ return equippedWpn; }		// The weapon you are using
	UsableItems* getShield(){ return equippedShield; }
	UsableItems* getHelm(){	return equippedHelm; }
	UsableItems* getBoobs(){ return equippedBoobs; }	// really?
	UsableItems* getPants(){ return equippedPants; }
	UsableItems* getNeck(){ return equippedNeck; }

	void setWpn(UsableItems* wpn);
	void setShield(UsableItems* shld);
	void setHelm(UsableItems* helm);
	void setBoobs(UsableItems* boobs);
	void setPants(UsableItems* pants);
	void setNeck(UsableItems* neck);

	//get Attributes for player
	int getStrength(){ return strengthAtt; }
	int getDex(){ return dexterityAtt; }
	int getConstitution(){ return constitutionAtt; }
	int getIntel(){ return intelligenceAtt; }
	int getEvil(){ return evilAtt; }

	GUIController* updateGUI;		//need a pointer to the GUIController for updating

	void updateFace();			// change face img file based on status
	std::string getFace() { return mFace; }

	std::vector<UsableItems*> getInventory(){ return inventory; }  //return inventory for GUI purposes
	void pushInventory(UsableItems* i);   //push
	void switchEquipment(int ind);		//switch some inventory item with a current equipped item

	Ogre::AxisAlignedBox getBoundingBox() { return mModelEntity->getWorldBoundingBox(); }
	Ogre::SceneNode* getMBodyNode(){return mModelNode;}

	void carryMe(Environment* obj);  //pick up an object
	void dropMe();
	bool isCarrying();

	NPC* findConversant(std::list<NPC*> npcs);

private:
	bool fForward;		//how am I moving? Each flag indicates a direction

	bool fRot;
	double mRotator;
	double speed;		//how fast to move

	Ogre::SceneNode *mAttackNode;	//use this for bounding area for Yoshimi attacks
	Ogre::Entity *mAttackEntity;
	Ogre::SceneNode* mNullCamera;	//For rotating just the camera

	enum AnimID
	{
		ATTACK_ONE,
		ATTACK_TWO,
		ATTACK_THREE,
		BACKFLIP,
		BLOCK,
		CLIMB,
		CROUCH,
		DEATH_ONE,
		DEATH_TWO,
		HIGH_JUMP,
		IDLE_ONE,
		IDLE_TWO,
		IDLE_THREE,
		JUMP,
		JUMP_NO_HEIGHT,
		KICK,
		SIDE_KICK,
		SPIN,
		STEALTH,
		WALK,
		ANIM_NONE
	};

	AnimID playerAnim;
	void setupAnimations();									// load this character's animations
	void fadeAnimations(Ogre::Real deltaTime);				// blend from one animation to another
	void updateAnimations(Ogre::Real deltaTime);			// update the animation frame
	void setAnimation(AnimID id, bool reset = false);
	Ogre::Vector3 collisionRobots(Ogre::Vector3 myPos);
	Ogre::Vector3 collisionWalls(Ogre::Vector3 myPos);
	Ogre::Vector3 collisionObjects(Ogre::Vector3 myPos);

	//Stats for player
	double criticalStat;	// chance to cause critical hit
	double damageStat;		// base damage
	double defenseStat;		// base defense
	double healthStat;		// base health
	double manaStat;		// base mana
	double healthNow;		// current health
	double manaNow;			// current mana

	int level;				// your level

	//attributes for player
	int evilAtt;			// affects crit, mana
	int strengthAtt;		// affects dam, def
	int dexterityAtt;		// affects heal, crit
	int constitutionAtt;	// affects def, heal
	int intelligenceAtt;	// affects mana, dam
	
	//equipment for player
	UsableItems* equippedWpn;		//The weapon you are using
	UsableItems* equippedShield;
	UsableItems* equippedHelm;
	UsableItems* equippedBoobs;
	UsableItems* equippedPants;
	UsableItems* equippedNeck;

	Projectile* mFireball;
	AoE* mFreeze;

	Ogre::Light* mLight;

	double mDamage;
	double mDefense;

	std::string mFace;			// filename for img to represent character status
	// will either hard code additional faces or have a list of them

	void dealDamage(NPC *enemy);	// deal damage to an enemy
	bool isBlocking;				// whether the player is currently blocking

	std::vector<UsableItems*> inventory;

	Environment* carrying;		//This is a pointer to a moveable object that we may carry

};

#endif