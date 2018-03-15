#ifndef COIN_H
#define COIN_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Coin : public Actor
{

	protected:


	public:
	Coin();

	int m_iType;
	int die;
	float aa;



	void Update();
	void Animation();


};


#endif
