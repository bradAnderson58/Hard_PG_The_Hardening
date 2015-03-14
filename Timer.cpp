#include "Timer.h"

Timer::Timer(Ogre::Real countdown_millsec)
{
	mCountDown = countdown_millsec;
	//mPrecisionTimer = new Ogre::Timer();
}

Timer::~Timer(){}

void
Timer::update(Ogre::Real deltaTime)
{
	if (mCountDown <= 0)
	{
		std::cout << "end cooldown" << std::endl;
		mCountDown = 0;
	}
	else
		mCountDown -= deltaTime;
}

void
Timer::reset(Ogre::Real countdown_millsec)
{
	mCountDown = countdown_millsec;
}

bool
Timer::isZero()
{
	if (!this || mCountDown <= 0)
		return true;
	else 
		return false;
}