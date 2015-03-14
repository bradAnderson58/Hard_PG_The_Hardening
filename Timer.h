// Countdown timer for skills and other such things
// using Ogre's built in timer class
// create one of these for each skill?

#ifndef __Timer_h_
#define __Timer_h_

#include "GameApplication.h"

class GameApplication; //whyyyyyyyyy

class Timer
{
private:
	Ogre::Real mStartTime;
	Ogre::Real mCountDown;
	//Ogre::Timer* mPrecisionTimer;

public:
	Timer(Ogre::Real countdown_millsec);
	~Timer();

	void update(Ogre::Real deltaTime);
	void reset();
	bool isZero();
};
#endif