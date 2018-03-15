#include "Player.h"
#include "sdlgl.h"


Player Player::I;


Player::Player()
{
	m_bJumpReady = false;
	m_bJumping = false;
	m_iJumpHold = 0;
	m_iBoostJump = 0;
	m_iDuck = 0;
	m_fSpeedUp = false;
	m_iHealth = 100;
	m_iLife = 3;

}

void Player::Die()
{
		m_bFacingRight = true;
}

void Player::ReleaseTextures()
{
	if (m_pTexture)
	{
//		m_pTexture->Release();
		glDeleteTextures(1,&m_pTexture);
		m_pTexture=0;
	}

	if (m_pExtraTexture)
	{
//		m_pExtraTexture->Release();
		glDeleteTextures(1,&m_pExtraTexture);
		m_pExtraTexture=0;
	}

}

void Player::LoadTextures()
{

	//Load texture

	Pack->OpenPackFile("Data/Data6.pak");

 	if(!bLowRes)
	{
		m_pTexture = loadTextureFromFP(Pack->GetFPByName("player.tga"), Pack->GetSizeByName("player.tga"), 0, 0);
		m_pExtraTexture = loadTextureFromFP(Pack->GetFPByName("player2.tga"), Pack->GetSizeByName("player2.tga"), 0, 0);
	}
	else
	{
		
		m_pTexture = loadTextureFromFP(Pack->GetFPByName("player.tga"), Pack->GetSizeByName("player.tga"), 256, 256);
		m_pExtraTexture = loadTextureFromFP(Pack->GetFPByName("player2.tga"), Pack->GetSizeByName("player2.tga"), 256, 256);
	}

	Pack->ClosePackFile(); 

	if(m_pTexture != 0 && m_pExtraTexture != 0)
		LOG("DEBUG: Player's Textures Loaded\n");
	else
		LOG("DEBUG: ERROR! Player's Textures Not Loaded\n");

}



void Player::PlayerAction( int iAction )
{
	m_iAction += iAction;
}



int Player::MoveX( float fDX )
{	
	int i;
	float cx,cy;


	for(i = 0; i < 6; i++)
	{
		if(i < 4)
		{
			cx = (float) m_ptCollision[i].x;
			cy = (float)m_ptCollision[i].y;
		}
		else
		{
			if(i == 4) cx =(float) m_ptCollision[0].x;
			if(i == 5) cx = (float)m_ptCollision[1].x;

			cy = (m_ptCollision[0].y + m_ptCollision[2].y) / 2;
		}

		int iTileID = World->GetTile(int(m_fX + fDX + cx)/32, int(m_fY + cy + 32)/32);
		if( iTileID > 0 && iTileID < SOLID )
		{
		  if( i == 0 || i == 2 || i == 4) m_fX = int(m_fX + fDX) / 32 * 32 + 32 - cx + 1.0f;
		  else m_fX = int(m_fX + fDX)/32*32 + (32 - cx) - 1.0f;
	      return i;
		}
	}

	m_fX += fDX;
	
	return -1;
}

void Player::Flash()
{
	playSound(sndHit);
	

	m_bInvinsible = true;
	m_iFlash = 64;
	m_iFlashCounter = 0;
}

void Player::ForceJump()
{
	m_iBoostJump = 4;
	m_fForce = -9.1f - 4.0f;
	m_fY -= 2.0f;
	m_bJumping = true;
	m_iJumpHold = 12; 

}

void Player::Reset()
{
	m_bInWater = false;
	m_bFacingRight = true;

}
void Player::Update( )
{
	float fMoveX = 0.0f;
	bool bWasInWater = false;
	static bool lastRunRight;


	if(m_iFlash > 0) m_iFlash--;
	if(m_iBoostJump > 0) m_iBoostJump--;

	m_bInvinsible = (m_iFlash > 0);

	if( (m_iAction & DUCK) && m_bOnFloor)
	{
		m_bDuck = true;
		if(m_iDuck == 0) m_iDuck = 8;
		else if(m_iDuck > 1) m_iDuck--;

		if(m_iDuck < 3) SetAABB( 10,30, 22,30, 10,0, 22,0 ); 

	}
	else
	{
		m_bDuck = false;
		m_iDuck = 0;

		SetAABB( 10,45, 22,45, 10,0, 22,0 ); 

		//Speedup for run
		if((m_iAction & RUN) && ((m_iAction & WALK_RIGHT ) || ( m_iAction & WALK_LEFT )) )
		{
			m_fSpeedUp += 0.05;
			if(m_fSpeedUp > 2.0f) m_fSpeedUp = 2.0f;

			lastRunRight = (m_iAction & WALK_RIGHT );
			 
		}
		else
		{
			m_fSpeedUp -= 0.1;
			if(m_fSpeedUp < 0.0f) m_fSpeedUp = 0.0f;

			if(m_fSpeedUp > 0.0f)
				fMoveX += (lastRunRight ? m_fSpeedUp : -m_fSpeedUp);
		}

		if( m_iAction & WALK_RIGHT )
		{
			fMoveX += 4.0f + (((m_iAction & RUN) > 0 ) ? m_fSpeedUp : 0.0f) - (m_bInWater ? 4.0f : 0.0f);
			m_iWalkState = (m_iWalkState + 1 + ((m_iAction & RUN) ? 1 : 0)) % (8*ANIMATION_SPEED);
			
			if(!m_bWalking) m_iWalkState += ANIMATION_SPEED;
			m_bWalking = true;

		}

		if( m_iAction & WALK_LEFT )
		{
			fMoveX -= 4.0f + (((m_iAction & RUN) > 0 ) ? m_fSpeedUp: 0.0f) - (m_bInWater ? 4.0f : 0.0f);
			m_iWalkState =( m_iWalkState + 1 + ((m_iAction & RUN) ? 1 :0 )) % (8*ANIMATION_SPEED);
			if(!m_bWalking) m_iWalkState += ANIMATION_SPEED;
			m_bWalking = true;

		}
		
		if( !(m_iAction & (WALK_RIGHT + WALK_LEFT))) //NO WALK
		{
			if(m_iWalkState / ANIMATION_SPEED == 0 ) m_iWalkState = 1 * ANIMATION_SPEED ;
			if(m_iWalkState / ANIMATION_SPEED == 2 ) m_iWalkState = 1 * ANIMATION_SPEED ;
			if(m_iWalkState / ANIMATION_SPEED == 3 ) m_iWalkState = 5 * ANIMATION_SPEED ;
			if(m_iWalkState / ANIMATION_SPEED == 4 ) m_iWalkState = 5 * ANIMATION_SPEED ;
			if(m_iWalkState / ANIMATION_SPEED == 6 ) m_iWalkState = 5 * ANIMATION_SPEED ;
			if(m_iWalkState / ANIMATION_SPEED == 7 ) m_iWalkState = 1 * ANIMATION_SPEED ;

			m_bWalking = false;
		
		}

		if( (m_iAction & JUMP) &&
			(m_fForce == 0.0f && m_bJumpReady) || 
			(m_bJumping && m_iJumpHold > 0))
		{
			if(!m_bJumping)playSound(sndJump);
			m_fForce = -9.1f - ((m_iBoostJump > 0)? 4.0f : 0.0f); // ALERT! SHOULD NOT END WITH .0f (Will cause player to sndJump in air)
			m_bJumping = true;
			m_bJumpReady = false;

			m_iJumpHold--;
		}
	}

	if(!m_iJumpHold || !(m_iAction & JUMP))
	{
		m_bJumping = false;
		m_iJumpHold = 10; //Amount of frames we allow the player to increase the altitude of the sndJump
	}
 

	m_fForce += (GRAVITY - (m_bInWater ? WATER_PULL_UP : 0.0f));

	if(m_fForce >= 32.0f) m_fForce = 31.0f;
	if(m_bInWater && m_fForce >= 2.0f) m_fForce = 1.9f;

	MoveX(fMoveX);

	bWasInWater = m_bInWater;

	int ir = MoveY(-m_fForce);
	
	if(bWasInWater != m_bInWater) playSound(sndWater);

	if( m_fForce > 0.0f && ir == -1) m_iJumpState = 1; //down
	else if(m_fForce < 0.0f && ir == -1) m_iJumpState = 2;
	else m_iJumpState = 0;
	
	m_bOnFloor = false;

	if( ir != -1)
	if( ir < 2 )
	{
		m_fForce = 0.0f;
		m_bJumping = false;
		m_bJumpReady = false;
		m_iJumpHold = 10;
		m_iJumpState = 1;
	}
	else
	{
		m_fForce = 0.0f;
		m_bJumpReady = !(m_iAction & JUMP);
		m_bOnFloor = true;
		
	}

	//Reset action variable
	m_iAction = IDLE;



	m_iSpecial = m_iJumpState; //Uses the m_iSpecial variable to fill in jumpstate for collision detection
}

//Own Update as the player is 64x32, not 32x32
void Player::Animation()
{
	
	float anim = 0.0f; //temp
	float anim2 = 0.0f;
	float tempY = m_fY;
	float z = -10;

	GLuint color;

	if(m_iFlash == 0) color = 0xFFFFFFFF;
	else
	{
		if(( m_iFlash % 8) >= 4 ) m_iFlashCounter+=2;
		else m_iFlashCounter-=2;

		color = 0xFFFFFFFF - ((m_iFlashCounter*32)<<24); //- ((m_iFlashCounter*32)<<8)- ((m_iFlashCounter*32));

	}

	m_fY = m_fY -3.0f; //feet arn't on Y = 0

	anim =  (m_iWalkState / ANIMATION_SPEED ) % 4;
	anim2 =  ((m_iWalkState / ANIMATION_SPEED) < 4) ? 0.0f : 0.5f;
	
	if( m_iJumpState )
	{
		if(m_iJumpState == 1)
		{
			anim = 3.0f;
			anim2 = 0.5f;
		}
		else if(m_iJumpState == 2)
		{
			anim = 0.0f;
			anim2 = 0.0f;
		}
	}
	
	if( m_bDuck ) 
	{
		anim = 2 - (m_iDuck / 3);
		anim2 = 0;
	}

	if( m_bFacingRight )
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = z;
		m_QuadVerts[0].diffuse = color;
	//	m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.25f * anim);
		m_QuadVerts[0].v = 0.50f + anim2;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = z;
		m_QuadVerts[1].diffuse = color;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.25f + (0.25f * anim);
		m_QuadVerts[1].v = 0.50f + anim2;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].z = z;
		m_QuadVerts[2].diffuse = color;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.25f * anim);
		m_QuadVerts[2].v = 0.0f + anim2;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = z;
		m_QuadVerts[3].diffuse = color;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.25f + (0.25f * anim);
		m_QuadVerts[3].v = 0.50f + anim2;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 64 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 64 + m_fY);
		m_QuadVerts[4].z = z;
		m_QuadVerts[4].diffuse = color;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.25f + (0.25f * anim);
		m_QuadVerts[4].v = 0.0f + anim2;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].z = z;
		m_QuadVerts[5].diffuse = color;
	//	m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.25f * anim);
		m_QuadVerts[5].v = 0.0f + anim2;
	}
	else
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = z;
		m_QuadVerts[0].diffuse = color;
	//	m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.25f * anim);
		m_QuadVerts[0].v = 0.50f + anim2;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = z;
		m_QuadVerts[1].diffuse = color;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.25f + (0.25f * anim);
		m_QuadVerts[1].v = 0.50f + anim2;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].z = z;
		m_QuadVerts[2].diffuse = color;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.25f * anim);
		m_QuadVerts[2].v = 0.0f + anim2;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = z;
		m_QuadVerts[3].diffuse = color;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.25f + (0.25f * anim);
		m_QuadVerts[3].v = 0.50f + anim2;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 64 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 64 + m_fY);
		m_QuadVerts[4].z = z;
		m_QuadVerts[4].diffuse = color;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.25f + (0.25f * anim);
		m_QuadVerts[4].v = 0.0f + anim2;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].z = z;
		m_QuadVerts[5].diffuse = color;
	//	m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.25f * anim);
		m_QuadVerts[5].v = 0.0f + anim2;
	}

	m_fY = tempY;
}

bool Player::Render()
{ 
	
	glEnable(GL_TEXTURE_2D);
	 if(!m_bDuck) {
//		pd3dDevice->SetTexture( 0, m_pTexture );
		glBindTexture(GL_TEXTURE_2D, m_pTexture);
	 } else {
//		pd3dDevice->SetTexture( 0, m_pExtraTexture );
		glBindTexture(GL_TEXTURE_2D, m_pExtraTexture);
	}

//	 pd3dDevice->SetVertexShader(D3DFVF_QUADVERT);
//	 pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_QuadVerts, sizeof(QUADVERT));
	drawQuadVerts(m_QuadVerts,0,1,4,2);

	 return true;
}

