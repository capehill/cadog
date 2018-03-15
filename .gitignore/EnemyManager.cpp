#include "EnemyManager.h"
#include "sdlgl.h"

EnemyManager EnemyManager::I;


int iRealType[] = { 0,0,0,0,0,0,0,55,  58 }; //What enemy emitter [x] really emitts?

void EnemyManager::ReleaseTextures()
{
	for(int t = 0; t < 8; t++)
	{
		if (m_pTexture[t] != 0)
		{
			glDeleteTextures(1,&m_pTexture[t]);
			m_pTexture[t]=0;
		}
	}

	LOG("DEBUG: Enemy Manager has released the Enemies' texture\n");
}

void EnemyManager::CleanUp()
{

	for(ActorList::iterator i = m_actors.begin(); i!=m_actors.end(); ++i){
        delete (*i);
    }

	m_actors.erase( m_actors.begin(), m_actors.end()) ;


	LOG("DEBUG: Cleaning up Enemy Manager\n");
}

void EnemyManager::LoadTextures()
{

	//Load texture
/*	D3DXCreateTextureFromFile(pd3dDevice, "Enemy1.tga", &m_pTexture[0]);
 	D3DXCreateTextureFromFile(pd3dDevice, "Enemy2.tga", &m_pTexture[1]);*/

	Pack->OpenPackFile("Data/Data4.pak");


	if(!bLowRes)
	{
		m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("Enemy1.tga"), Pack->GetSizeByName("Enemy1.tga"), 0, 0);
		m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("Enemy2.tga"), Pack->GetSizeByName("Enemy2.tga"), 0, 0);
		m_pTexture[4] = loadTextureFromFP(Pack->GetFPByName("Enemy3.tga"), Pack->GetSizeByName("Enemy3.tga"), 0, 0);
		m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("Coins.tga"), Pack->GetSizeByName("Coins.tga"), 0, 0);
		m_pTexture[3] = loadTextureFromFP(Pack->GetFPByName("Signs.tga"), Pack->GetSizeByName("Signs.tga"), 0, 0);
	}
	else
	{
		m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("Enemy1.tga"), Pack->GetSizeByName("Enemy1.tga"), 256, 256);
		m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("Enemy2.tga"), Pack->GetSizeByName("Enemy2.tga"), 64, 64);
		m_pTexture[4] = loadTextureFromFP(Pack->GetFPByName("Enemy3.tga"), Pack->GetSizeByName("Enemy3.tga"), 64, 64);
		m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("Coins.tga"), Pack->GetSizeByName("Coins.tga"), 64, 64);
		m_pTexture[3] = loadTextureFromFP(Pack->GetFPByName("Signs.tga"), Pack->GetSizeByName("Signs.tga"), 128, 128);
	}
	Pack->ClosePackFile();

	if(m_pTexture[0] != 0)
		LOG("DEBUG: Enemy Manager has loaded the Enemies' texture\n");
	else
		LOG("DEBUG: ERROR! Enemy Manager has not loaded the Enemies' texture\n");

	for(ActorList::iterator i = m_actors.begin(); i!=m_actors.end(); ++i){
		switch((*i)->iID)
		{
			case ENEMY_BLOB:
			case ENEMY_BLOB_2:
			case ENEMY_BLOB_DUMB:
			case ENEMY_SHEPPY:
			case ENEMY_FIGARO:
			case ENEMY_COCOA:
			(*i)->SetTexture(m_pTexture[0]);
			break;
			
			case ENEMY_BILL:
			case ENEMY_SPIKEY:
			(*i)->SetTexture(m_pTexture[1]);
			break;
		
			case ENEMY_PARROT:
			(*i)->SetTexture(m_pTexture[4]);
			break;

			case COIN_1:
			case COIN_2:
			case COIN_3:
			case COIN_4:
			(*i)->SetTexture(m_pTexture[2]);
			break;
		
		 

			case SIGN_EXIT:
			case SIGN_CHECK:
			(*i)->SetTexture(m_pTexture[3]);
			break;

			default:
			(*i)->SetTexture(0);
		}
    }
}


void EnemyManager::Setup( )
{
	for(int i = 0; i < MAX_EMITTERS; i++)
		m_emitter[i].bInUse = false;

}

void EnemyManager::AddEmitter( float fX, float fY, int iType )
{
	
	int i;

	for(i = 0; i < MAX_EMITTERS; i++)
		if( !m_emitter[i].bInUse )
		{
			LOG("DEBUG: Added enemy emitter (%i, %i) type: %i\n",int(fX),int(fY),iType);
			m_emitter[i].fX = fX;
			m_emitter[i].fY = fY;
			m_emitter[i].iRate = 100;
			m_emitter[i].iCounter = m_emitter[i].iRate;
			m_emitter[i].iType = iRealType[iType];
			m_emitter[i].bInUse = true;
			m_emitter[i].bActive = false;
			break;
		}


                
}

void EnemyManager::Add( float fX, float fY, int iEnemyType )
{
	Actor	*tempEnemy = NULL;
	switch(iEnemyType) //what enemy type
	{
		case 54: //Dumb Blob
		tempEnemy = new Enemy_Parrot;
		tempEnemy->SetTexture(m_pTexture[4]);
		tempEnemy->SetAABB( 4,24,  24,24,  4,12,  28,12 );
		tempEnemy->iID = ENEMY_PARROT;
		LOG("Enemy: Parrot added!\n");
		break;


		case 55: //Dumb Blob
		tempEnemy = new Enemy_Blob_Dumb;
		tempEnemy->SetTexture(m_pTexture[0]);
		tempEnemy->SetAABB( 6,20,  26,20,  6,0,  26,0 );
		tempEnemy->iID = ENEMY_BLOB_DUMB;
		LOG("Enemy: Dumb Blob added!\n");
		break;

		case 56: //Blob
		tempEnemy = new Enemy_Blob;
		tempEnemy->SetTexture(m_pTexture[0]);
		tempEnemy->SetAABB( 6,20,  26,20,  6,0,  26,0 );
		tempEnemy->iID = ENEMY_BLOB;
		LOG("Enemy: Blob added!\n");
		break;
	
		case 57: //Sheppy
		tempEnemy = new Enemy_Sheppy;
		tempEnemy->SetTexture(m_pTexture[0]);
		tempEnemy->SetAABB( 4,28,  34,28,  4,0,  34,0 );
		tempEnemy->iID = ENEMY_SHEPPY;
		LOG("Enemy: Sheppy added!\n");
		break;

		case 58: //Bill
		playSound(sndCannon);
		tempEnemy = new Enemy_Bill;
		tempEnemy->SetTexture(m_pTexture[1]);
		tempEnemy->SetAABB( 4,22,  28,22,  4,10,  28,10 );
		tempEnemy->iID = ENEMY_BILL;
		LOG("Enemy: Bill added!\n");
		break;

		case 59: //Figaro
		tempEnemy = new Enemy_Figaro;
		tempEnemy->SetTexture(m_pTexture[0]);
		tempEnemy->SetAABB( 6,25,  24,25,  6,2,  24,2 );
		tempEnemy->iID = ENEMY_FIGARO;
		LOG("Enemy: Figaro added!\n");
		break;

		case 60: //Blob 2
		tempEnemy = new Enemy_Blob_2;
		tempEnemy->SetTexture(m_pTexture[0]);
		tempEnemy->SetAABB( 6,20,  26,20,  6,0,  26,0 );
		tempEnemy->iID = ENEMY_BLOB_2;
		LOG("Enemy: Blob2 added!\n");
		break;

		case 61: //Cocoa
		tempEnemy = new Enemy_Cocoa;
		tempEnemy->SetTexture(m_pTexture[0]);
		tempEnemy->SetAABB( 13,19,  19,19,  13,13,  19,13 );
		tempEnemy->iID = ENEMY_COCOA;
		LOG("Enemy: Cocoa added!\n");
		break;
	
		case 62: //Spikey
		tempEnemy = new Enemy_Spikey;
		tempEnemy->SetTexture(m_pTexture[1]);
		tempEnemy->SetAABB( 8,32,  16,32,  24,32,  32,32 );
		tempEnemy->iID = ENEMY_SPIKEY;
		LOG("Enemy: Spikey added!\n");
		break;

		case 40: //Coin1
		case 41: //Coin2
		case 42: //Coin3
		case 43: //Coin4
		tempEnemy = new Coin;
		tempEnemy->SetTexture(m_pTexture[2]);
		tempEnemy->SetAABB( 4,28,  28,28,  4,4,  28,4 );
		tempEnemy->iID = COIN_1 + (iEnemyType - 40);
		LOG("Enemy: Coin added(Not hostile)!\n");
		break;

			
		case 44: //Exit Sign
		tempEnemy = new Signs;
		tempEnemy->SetTexture(m_pTexture[3]);
		tempEnemy->SetAABB( 8,48,  56,48,  8,0,  56,0 );
		tempEnemy->iID = SIGN_EXIT;
		LOG("Enemy: Exiit sign added(Not hostile)!\n");
		break;

			
		case 45: //Check Sign
		tempEnemy = new Signs;
		tempEnemy->SetTexture(m_pTexture[3]);
		tempEnemy->SetAABB( 8,48,  56,48,  8,0,  56,0 );
		tempEnemy->iID = SIGN_CHECK;
		LOG("Enemy: Check Point Sign added(Not hostile)!\n");
		break;
	}

	m_iEnemies++;
	tempEnemy->SetXY( fX,fY - 32.0f);

	m_actors.push_back(tempEnemy);

}

int EnemyManager::HitEnemy(int iId, Actor *actor)
{

	if( actor->m_bInvinsible && (iId != ENEMY_SPIKEY)) return 0;

	//Remove health
	switch(iId)
	{
		case ENEMY_BLOB:
		case ENEMY_BLOB_DUMB:
		Number->AddFadeNumber(-25,true);
		actor->m_iHealth -= 25;
		break;
	
		case ENEMY_BLOB_2:
		Number->AddFadeNumber(-35,true);
		actor->m_iHealth -= 35;
		break;

		case ENEMY_BILL:
		Number->AddFadeNumber(-30,true);
		actor->m_iHealth -= 30;
		break;

		case ENEMY_SHEPPY:
		Number->AddFadeNumber(-40,true);
		actor->m_iHealth -= 40;
		break;


		case ENEMY_FIGARO:
		Number->AddFadeNumber(-20,true);
		actor->m_iHealth -= 20;
		break;

		case ENEMY_COCOA:
		Number->AddFadeNumber(-30,true);
		actor->m_iHealth -= 30;
		break;

		case ENEMY_SPIKEY:
		Number->AddFadeNumber(-100,true);
		actor->m_iHealth -=100;
		break;

		case ENEMY_PARROT:
		Number->AddFadeNumber(-50,true);
		actor->m_iHealth -=50;
		break;
	}

	return 10;

}

int EnemyManager::CheckCollision(Actor *actor)
{
	int i;
	int cy,cx;

	for(ActorList::iterator test = m_actors.begin(); test!=m_actors.end(); ++test){
	
		if((*test)->m_bActive)
		if(((*test)->iID > (ENEMY_PARROT - 1) && (*test)->iID < (ENEMY_FIGARO + 1)) && !(*test)->m_bDead)
		{
		
			for(i = 0; i < 4; i++) //bad hits?
			{
			
				if(i == 0) cy = actor->m_fY + actor->m_ptCollision[0].y;
				else if(i == 1) cy = actor->m_fY + actor->m_ptCollision[0].y;
				else if(i == 2) cy = actor->m_fY + (actor->m_ptCollision[2].y + actor->m_ptCollision[0].y) / 2 - 4.0f;
				else if(i == 3) cy = actor->m_fY + (actor->m_ptCollision[2].y + actor->m_ptCollision[0].y) / 2 - 4.0f;
				
				if(  ((*test)->m_fX + (*test)->m_ptCollision[0].x < actor->m_fX + actor->m_ptCollision[i].x) &&
					 ((*test)->m_fX + (*test)->m_ptCollision[1].x > actor->m_fX + actor->m_ptCollision[i].x) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[0].y > cy) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[2].y < cy) )
				{
					
					return HitEnemy( (*test)->iID, actor ); //hit! Oh nooo!

				}
			}

		 	for(i = 4; i < 6; i++) // good hits (or bad)?
			{
				if(i == 4) cy = actor->m_fY + actor->m_ptCollision[2].y;
				if(i == 5) cy = actor->m_fY + actor->m_ptCollision[3].y;
					
				if(  ((*test)->m_fX + (*test)->m_ptCollision[0].x < actor->m_fX + actor->m_ptCollision[i-2].x) &&
					 ((*test)->m_fX + (*test)->m_ptCollision[1].x > actor->m_fX + actor->m_ptCollision[i-2].x) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[0].y > cy) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[2].y < cy) )
				{
					if(actor->m_iSpecial == 1)
					{
						
						playSound(sndEnemy);
						(*test)->m_bDead = true; //Sorry, you're |)00^^3|)!
						(*test)->m_fY -= 24;
						
						int iAddScore = 0;

						switch( (*test)->iID )
						{
							case ENEMY_BLOB:
							case ENEMY_BLOB_DUMB:
							iAddScore = 10;
							break;
	
							case ENEMY_BLOB_2:
							iAddScore = 20;
							break;

							case ENEMY_BILL:
							iAddScore = 15;
							break;

							case ENEMY_SHEPPY:
							iAddScore = 30;
							break;


							case ENEMY_FIGARO:
							iAddScore = 40;
							break;
							
							case ENEMY_PARROT:
							iAddScore = 50;
							break;

							default:
							iAddScore = 0;
							
						}
						actor->MoveY(8);
						actor->m_iScore += iAddScore;
						Number->AddFadeNumber(iAddScore,false);
						return 5; //Good one!
					}

					return HitEnemy( (*test)->iID, actor );

				}
			}
		}
		else if(((*test)->iID > (COIN_1 - 1) && (*test)->iID < SIGN_CHECK + 1) && !(*test)->m_bDead) //coins!
		{
			for(i = 0; i < 6; i++) 
			{
			
				if(i < 5 )
				{
					cy = actor->m_fY + actor->m_ptCollision[i].y;
					cx = actor->m_fX + actor->m_ptCollision[i].x;
				}
				else if(i == 4)
				{
					cy = actor->m_fY + (actor->m_ptCollision[2].y + actor->m_ptCollision[0].y) / 2 - 4.0f;
					cx = actor->m_fX + actor->m_ptCollision[0].x;
				}
				else if(i == 5)
				{
					cy = actor->m_fY + (actor->m_ptCollision[2].y + actor->m_ptCollision[0].y) / 2 - 4.0f;
					cx = actor->m_fX + actor->m_ptCollision[1].x;
				}
		
				if(  ((*test)->m_fX + (*test)->m_ptCollision[0].x < cx) &&
					 ((*test)->m_fX + (*test)->m_ptCollision[1].x > cx) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[0].y > cy) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[2].y < cy) )
				{
					//You got the coin!
					

					if((*test)->iID == COIN_1)
					{
						actor->m_iHealth += 5;
						Number->AddFadeNumber(5,true);
						playSound(sndCoin);

						(*test)->m_bDead = true;
					}
					else if((*test)->iID == COIN_2)
					{
						actor->m_iHealth += 10;
						Number->AddFadeNumber(10,true);
						playSound(sndCoin);

						(*test)->m_bDead = true;
					}
					else if((*test)->iID == COIN_3)
					{
						actor->m_iHealth += 50;
						Number->AddFadeNumber(50,true);
						playSound(sndCoin);

						(*test)->m_bDead = true;
					}
					else if((*test)->iID == COIN_4)
					{
						actor->m_iHealth += 100;
						Number->AddFadeNumber(100,true);
						playSound(sndCoin);

						(*test)->m_bDead = true;
					}

					else if((*test)->iID == SIGN_EXIT)
					{
						playSound(sndLC);
						Number->AddFadeNumber(200,false);
						(*test)->m_iSpecial = 10;
						bChangeLevel = true;
					}

 					else if((*test)->iID == SIGN_CHECK)
					{
						if((*test)->m_iSpecial != 2)
						{
							Number->AddFadeNumber(100,false);
							actor->m_fCX = actor->m_fX;
							playSound(sndCheck);
							(*test)->m_iSpecial = 2;
						}
					}

					if(actor->m_iHealth > 100)
					{
						actor->m_iScore += (actor->m_iHealth - 100);
						actor->m_iHealth = 100;
					}

			 
					return 0;
						
				}
			}
		}
		else if((*test)->iID == ENEMY_COCOA || (*test)->iID == ENEMY_SPIKEY ) 
		{
	
			for(i = 0; i < 4; i++) 
			{
			
				if(  ((*test)->m_fX + (*test)->m_ptCollision[i].x < actor->m_fX + actor->m_ptCollision[1].x ) &&
					 ((*test)->m_fX + (*test)->m_ptCollision[i].x > actor->m_fX + actor->m_ptCollision[0].x ) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[i].y > actor->m_fY + actor->m_ptCollision[2].y) &&
					 ((*test)->m_fY + (*test)->m_ptCollision[i].y < actor->m_fY + actor->m_ptCollision[0].y) )
				{
			
					if((*test)->iID == ENEMY_SPIKEY) return HitEnemy( (*test)->iID, actor );

					if((*test)->m_iSpecial)		
						return HitEnemy( (*test)->iID, actor );
					else
						return 0;
						
				}
			}
		}


    }

	return 0; //no hits
}
void EnemyManager::UpdateEnemies( float fX )
{

	//Update enemies
	for(ActorList::iterator i = m_actors.begin(); i!=m_actors.end(); ++i){
		/*if((*i)->m_bActive)*/
		(*i)->CheckIfActive(fX);
		(*i)->Update();
    }

	//Update emitters
	for(int e = 0; e < MAX_EMITTERS; e++)
	{
		if(m_emitter[e].bInUse)
		{
			if(  fabs(m_emitter[e].fX - fX ) < 1000.0f)
			{
				m_emitter[e].iCounter--;

				if(m_emitter[e].iCounter <= 0)
				{
					m_emitter[e].iCounter = m_emitter[e].iRate;
					Add(m_emitter[e].fX, m_emitter[e].fY, m_emitter[e].iType);
					LOG("DEBUG: Fire away! %f, %f type: %i\n",m_emitter[e].fX, m_emitter[e].fY, m_emitter[e].iType);
				}
			}
		}
	}
	//I see dead people!?

again:
	for(ActorList::iterator i = m_actors.begin(); i!=m_actors.end(); ++i){
  		if((*i)->m_fY < -256)
		{
			delete (*i);
			m_actors.erase(i);
			m_iEnemies--;
			LOG("DEBUG: Killing off an enemy\n");
			goto again;
		}
    }


}
void EnemyManager::AnimateEnemies()
{
	for(ActorList::iterator i = m_actors.begin(); i!=m_actors.end(); ++i){
        if((*i)->m_bActive)(*i)->Animation();
    }
}
void EnemyManager::RenderEnemies()
{
    for(ActorList::iterator i = m_actors.begin(); i!=m_actors.end(); ++i){
        if((*i)->m_bActive)(*i)->Render();
    }
}
