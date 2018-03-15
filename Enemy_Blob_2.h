#ifndef ENEMY_BLOB_2_H
#define ENEMY_BLOB_2_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Enemy_Blob_2 : public Actor
{

	protected:
	int move;

	public:
	void Update();
	void Animation();



};


#endif
