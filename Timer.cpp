#include "Timer.h"

Timer::Timer(Ogre::Real countdown_millsec)
{
	mStartTime = countdown_millsec;
	mCountDown = countdown_millsec;
}

Timer::~Timer(){}

void
Timer::update(Ogre::Real deltaTime)
{
	if (mCountDown < 0)
	{
		mCountDown = 0;
	}
	else if (mCountDown != 0)
		mCountDown -= deltaTime;
}

void
Timer::reset()
{
	mCountDown = mStartTime;
}

bool
Timer::isZero()
{
	return (mCountDown <= 0);
}