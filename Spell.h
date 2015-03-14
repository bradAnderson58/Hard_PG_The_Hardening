#ifndef __Spell_h_
#define __Spell_h_

#include "Agent.h"

class GameApplication;
class Timer;
class NPC;

class Spell : public Agent
{
private:
	virtual void shoot(Ogre::Real deltatime);
	virtual void reload();

protected:
	Timer* cooldown_timer;	// time until next cast is available

	double mSpeed;			// speed of the spell
	double mDmg;			// damage to deal

	bool active;

	void checkCollision(NPC* enemy);

public:
	Spell(Ogre::SceneManager* SceneManager, std::string name, 
		std::string filename, float height, float scale, GameApplication* app);
	~Spell();

	bool isActive() { return active; }
	void setCoolDown(Ogre::Real time);
};
#endif