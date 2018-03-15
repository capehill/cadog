#ifndef ENEMY_BILL_H
#define ENEMY_BILL_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Enemy_Bill : public Actor
{

	protected:


	public:
	void Update();
	void Animation();
	void CheckIfActive(float fX);


};


#endif
