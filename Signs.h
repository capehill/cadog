#ifndef SIGNS_H
#define SIGNS_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Signs : public Actor
{

	protected:


	public:
	
	Signs();
	int m_iType;
	bool bChecked;



	void Update();
	void Animation();


};


#endif
