#ifndef __Spell_h_
#define __Spell_h_

#include "Agent.h"

class GameApplication;

class Spell : public Agent
{
private:

	virtual void checkCollision();
	virtual void shoot(Ogre::Real deltatime);
	virtual void reload();

protected:

	enum AnimID;		// depends on Spell?

	double mSpeed;		// speed of the spell
	double mDmg;		// damage to deal

	bool active;

public:
	
	Spell(Ogre::SceneManager* SceneManager, std::string name, 
		std::string filename, float height, float scale, 
		GameApplication* app);
	~Spell();

	bool isActive() { return active; }

};
#endif