///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : ZFSM.h
// Creation : 02/11/2009
// Author : Cedric Guillemet
// Description :
// Reliability : 
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZFSM_H__
#define ZFSM_H__


class ZFSM
{
public:
	ZFSM()
	{
	}
	virtual ~ZFSM()
	{
	}
	unsigned int NewTimer( float aDelay )
	{
		mTimers.push_back(timer_t(0.f, aDelay));
		return (mTimers.size()-1);
	}

	void Tick(float aTimeEllapsed)
	{
		std::vector<timer_t>::iterator iter = mTimers.begin();
		for (; iter != mTimers.end(); ++iter)
		{
			(*iter).mTime += aTimeEllapsed;
		}
	}

	bool OnTimer(unsigned int aTimer)
	{
		if (mTimers[aTimer].mTime > mTimers[aTimer].mDelay)
		{
			mTimers[aTimer].mTime -= mTimers[aTimer].mDelay;
			return true;
		}
		return false;
	}

	void RemoveTimer(unsigned int aTimer)
	{
		mTimers.erase(mTimers.begin() + aTimer);
	}
protected:
	typedef struct timer_t
	{
		timer_t(float aTime, float aDelay)
		{
			mTime = aTime;
			mDelay = aDelay;
		}
		float mTime;
		float mDelay;
	} timer_t;
	std::vector<timer_t> mTimers;




};


#endif