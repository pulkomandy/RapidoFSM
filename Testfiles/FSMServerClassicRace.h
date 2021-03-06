//////////////////////////////////////////////////////////////////////////////////////////////
/*
	FSMServerClassicRace State machine
	
	File generated 17/05/2010 at 20:50:18
	
	Do not edit this file directly.
	Use Rapido! by Cedric Guillemet.
	
	Rapido! is licensed under GPLV2. Here is a copy of Rapido! License :
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
*/
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FSMServerClassicRace_H__
#define FSMServerClassicRace_H__

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "ZFSM.h"



///////////////////////////////////////////////////////////////////////////////////////////////////

enum {

};

///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////

class FSMServerClassicRace : public ZFSM
{
public:
	FSMServerClassicRace()
	{
		
		mMinimalPlayersNumber = 1;
		mState = Undefined;
		EnterUndefined();
		
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	void Tick(float timeEllapsed);

	//////////////////////////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////////////////////////
protected:

	enum STATE
	{
	WaitingEnoughPlayer,
Undefined,
WaitingCountDown,
CountDownAndRacing,
NotEnoughPlayerToContinue

	};

	//////////////////////////////////////////////////////////////////////////////////////////////
	
	STATE mState;
	ZRushGame *mGame;
int mMinimalPlayersNumber;
int mWaitingTime;

	//////////////////////////////////////////////////////////////////////////////////////////////
	
	void SetState(STATE newState)
	{
			switch(mState)
	{
		case WaitingEnoughPlayer:
			LeaveWaitingEnoughPlayer();
			break;
		case Undefined:
			LeaveUndefined();
			break;
		case WaitingCountDown:
			LeaveWaitingCountDown();
			break;
		case CountDownAndRacing:
			LeaveCountDownAndRacing();
			break;
		case NotEnoughPlayerToContinue:
			LeaveNotEnoughPlayerToContinue();
			break;
	}
mState = newState;	switch(newState)
	{
		case WaitingEnoughPlayer:
			EnterWaitingEnoughPlayer();
			break;
		case Undefined:
			EnterUndefined();
			break;
		case WaitingCountDown:
			EnterWaitingCountDown();
			break;
		case CountDownAndRacing:
			EnterCountDownAndRacing();
			break;
		case NotEnoughPlayerToContinue:
			EnterNotEnoughPlayerToContinue();
			break;
	}
		
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	inline void TickWaitingEnoughPlayer(float timeEllapsed)
{

if (mGame->GetClientCount() >= mMinimalPlayersNumber)
{
    SetState(WaitingCountDown);
}
}
inline void TickUndefined(float timeEllapsed)
{


}
inline void TickWaitingCountDown(float timeEllapsed)
{

if (OnTimer(mWaitingTime))
{
    SetState(CountDownAndRacing);
}

}
inline void TickCountDownAndRacing(float timeEllapsed)
{

if (mGame->GetClientCount() < mMinimalPlayersNumber)
{
    SetState(NotEnoughPlayerToContinue);
}
}
inline void TickNotEnoughPlayerToContinue(float timeEllapsed)
{

if ((mGame->GetClientCount() == 0)||(OnTimer(mWaitingTime)))
{
    mGame->SetGameSet(GDedicatedGames->GetNextGame());
}
}

	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	inline void EnterWaitingEnoughPlayer()
{


}
inline void EnterUndefined()
{


}
inline void EnterWaitingCountDown()
{

float timeToWait = 10.f;
mWaitingTime = NewTimer(timeToWait);
mGame->SendNetMessage(ZNMSG_RaceStartsInNSeconds(timeToWait));



}
inline void EnterCountDownAndRacing()
{

mGame->SendNetMessage(ZNMSG_CountDownStarts());
}
inline void EnterNotEnoughPlayerToContinue()
{

float mSecondsBeforeRaceEnds = 5.f;
mGame->SendNetMessage(ZNMSG_RaceEndsInNSeconds(mSecondsBeforeRaceEnds));
mWaitingTime = NewTimer(mSecondsBeforeRaceEnds);

}

	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	inline void LeaveWaitingEnoughPlayer()
{


}
inline void LeaveUndefined()
{


}
inline void LeaveWaitingCountDown()
{

RemoveTimer(mWaitingTime);
}
inline void LeaveCountDownAndRacing()
{


}
inline void LeaveNotEnoughPlayerToContinue()
{

RemoveTimer(mWaitingTime);
}

	
	//////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////////////////////////////////////

inline void FSMServerClassicRace::Tick(float timeEllapsed)
{
	ZFSM::Tick(timeEllapsed);
	
		switch(mState)
	{
		case WaitingEnoughPlayer:
			TickWaitingEnoughPlayer(timeEllapsed);
			break;
		case Undefined:
			TickUndefined(timeEllapsed);
			break;
		case WaitingCountDown:
			TickWaitingCountDown(timeEllapsed);
			break;
		case CountDownAndRacing:
			TickCountDownAndRacing(timeEllapsed);
			break;
		case NotEnoughPlayerToContinue:
			TickNotEnoughPlayerToContinue(timeEllapsed);
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif

