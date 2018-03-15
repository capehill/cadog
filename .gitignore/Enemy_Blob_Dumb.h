#ifndef ENEMY_BLOB_DUMB_H
#define ENEMY_BLOB_DUMB_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Enemy_Blob_Dumb : public Actor
{

	protected:
	int move;

	public:
	void Update();
	void Animation();



};


#endif
