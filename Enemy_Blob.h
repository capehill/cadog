#ifndef ENEMY_BLOB_H
#define ENEMY_BLOB_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Enemy_Blob : public Actor
{

	protected:
	int move;

	public:
	void Update();
	void Animation();



};


#endif
