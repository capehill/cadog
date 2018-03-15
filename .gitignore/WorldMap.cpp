#include <SDL/SDL_endian.h>
#include "WorldMap.h"
#include "sdlgl.h"


WorldMap WorldMap::I;

WorldMap::WorldMap()
{
	m_pLayer[0] = NULL;
	m_pLayer[1] = NULL;
	m_pLayer[2] = NULL;

	m_pVB[0] = NULL;
	m_pVB[1] = NULL;
	m_pVB[2] = NULL;

	m_pTexture[0] = 0;
	m_pTexture[1] = 0;
	m_pTexture[2] = 0;

	m_iWorldMapWidth = 1;
	m_iEnemies = 0;

	m_pQuadVerts = NULL;
}

WorldMap::~WorldMap()
{
	CleanUp();
}


void WorldMap::CleanUp()
{
	int i;

	//Cleanup
	for(i = 0; i < 3; i++)
	{
	        if (m_pLayer[i])
		{
			delete [] m_pLayer[i];
			m_pLayer[i] = NULL;
		}

	}


}


int WorldMap::GetTile( int iX, int iY )
{
	if( iY >= 0 && iY < 12 &&
		iX >= 0 && iX < m_iWorldMapWidth)
	{
		return m_pLayer[1][iX*12 + (11-iY)];

	}
	else if( iY > 11 ) return 0;
	else return -1;



}

void WorldMap::PollEnemy(bool bRestart,  MAP_ENEMY &me)
{
	static int oldx = 0;
	static int oldy = 0;

	if(bRestart)
	{
		oldx = 0;
		oldy = 0;

	}

	int x,y;

	me.iType = -1; //check value

	for( x = 0; x < m_iWorldMapWidth; x+=1)
		for( y = 0; y < 12; y+=1)
		{
			if(x + y == 0)
			{
				x = oldx;
				y = oldy;

			}

			if( m_pLayer[2][x * 12 + y] > 53 &&  m_pLayer[2][x * 12 + y] < 63)
			{
				LOG("Enemy Found (%i, %i)! %i\n",x,y,m_pLayer[2][x * 12 + y]);

				me.fX = x * 32;
				me.fY = (11-y) * 32;
				me.iType = m_pLayer[2][x * 12 + y];
				
	
				m_pLayer[2][x*12+y] = 0;

				oldx = x;
				oldy = y+1;

						
				if(oldy == 12) 
				{
					oldy = 0;
					oldx++;
				}

				return;

			}

			//Coins
			if( m_pLayer[2][x * 12 + y] >= 40 && m_pLayer[2][x * 12 + y] < 44)
			{
				LOG("Coin Found (%i, %i)! %i\n",x,y,m_pLayer[2][x * 12 + y]);

				me.fX = x * 32;
				me.fY = (11-y) * 32;
				me.iType = m_pLayer[2][x * 12 + y];
				
	
				m_pLayer[2][x*12+y] = 0;

				oldx = x;
				oldy = y+1;

						
				if(oldy == 12) 
				{
					oldy = 0;
					oldx++;
				}

				return;

			}

			//Signs
			if( m_pLayer[2][x * 12 + y] >= 43 && m_pLayer[2][x * 12 + y] < 46)
			{
				LOG("Sign Found (%i, %i)! %i\n",x,y,m_pLayer[2][x * 12 + y]);

				me.fX = x * 32;
				me.fY = (11-y) * 32;
				me.iType = m_pLayer[2][x * 12 + y];
				
	
				m_pLayer[2][x*12+y] = 0;

				oldx = x;
				oldy = y+1;

						
				if(oldy == 12) 
				{
					oldy = 0;
					oldx++;
				}

				return;

			}
		}


}


void WorldMap::PollEnemyEmitter(bool bRestart, MAP_ENEMY_EMITTER &mee)
{
	static int oldx = 0;
	static int oldy = 0;
	static int iD = 0;

	if(bRestart)
	{
		oldx = 0;
		oldy = 0;
		iD = 0;
	}

	int x,y;
	

	mee.iType = -1; //check value

	for( x = 0; x < m_iWorldMapWidth; x+=1)
		for( y = 0; y < 12; y+=1)
		{
			if(x + y == 0)
				{
					x = oldx;
					y = oldy;
				}

			if( m_pLayer[2][x * 12 + y] == 8 ||  m_pLayer[2][x * 12 + y] == 7)
			{
				 

				LOG("Emitter Found (%i, %i)! \n",x,y);

				mee.fX = x * 32;
				mee.fY = (11-y) * 32;
				mee.iType = m_pLayer[2][x * 12 + y];
			
				//m_pLayer[2][x*12+y] = 0;

				oldx = x;
				oldy = y+1;
			
				if(oldy == 12) 
				{
					oldy = 0;
					oldx++;
				}
				return;

			}
		}


}

void WorldMap::Setup3DStuff( bool bNight )
{

	float texFix = 0.0015f * (bLowRes ? 2 : 1) + (bLowRes ? 0.0008f : 0);
	float fZ = -10.0f;

	int i_nVerticles,x,y,i;

	//Load texture
	Pack->OpenPackFile("Data/Data3.pak");

	if(!bLowRes)
	{
		if(bNight)
			m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("tiles_1_0.tga"), Pack->GetSizeByName("tiles_1_0.tga"), 0, 0);
		else
			m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("tiles_0_0.tga"), Pack->GetSizeByName("tiles_0_0.tga"), 0, 0);
		m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("tiles_0_1.tga"), Pack->GetSizeByName("tiles_0_1.tga"), 0, 0);
		m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("tiles_0_2.tga"), Pack->GetSizeByName("tiles_0_2.tga"), 0, 0);
	}
	else
	{
		if(bNight)
			m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("tiles_1_0.tga"), Pack->GetSizeByName("tiles_1_0.tga"), 256, 256);
		else
			m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("tiles_0_0.tga"), Pack->GetSizeByName("tiles_0_0.tga"), 256, 256);
		m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("tiles_0_1.tga"), Pack->GetSizeByName("tiles_0_1.tga"), 256, 256);
		m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("tiles_0_2.tga"), Pack->GetSizeByName("tiles_0_2.tga"), 256, 256);
	}

	Pack->ClosePackFile();

	if(m_pTexture[0] != 0)	LOG("DEBUG: Loading Texture1 ok\n");
	if(m_pTexture[1] != 0)	LOG("DEBUG: Loading Texture2 ok\n");
	if(m_pTexture[2] != 0)	LOG("DEBUG: Loading Texture3 ok\n");

	LOG("DEBUG: Level Textures loaded!\n");

	//Setup map
	i_nVerticles = m_iWorldMapWidth * 12 * 2 * 3;
	m_pQuadVerts = new QUADVERT[i_nVerticles];
 

	for( i = 0; i < 3; i++)
	{
		if( i == 0 )
		{
			for( x = 0; x < m_iWorldMapWidth; x+=2)
			{

				for( y = 0; y < 12; y+=2)
				{

					if(m_pLayer[i][(x/2)*6+(5-(y/2))] > 0)
					{

						m_pQuadVerts[x*18+(y*3)].x = -sinf((M_PI/80.0f) * -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)].y = -cosf((M_PI/80.0f) * -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)].z = fZ;
						m_pQuadVerts[x*18+(y*3)].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*18+(y*3)].specular = 0x0;
						m_pQuadVerts[x*18+(y*3)].u = (m_pLayer[i][(x/2)*6+(5-(y/2))] % 8) * (0.125f) + texFix;
						m_pQuadVerts[x*18+(y*3)].v = (m_pLayer[i][(x/2)*6+(5-(y/2))] / 8) * (0.125f) + (0.125f) - texFix;

						m_pQuadVerts[x*18+(y*3)+1].x = -sinf((M_PI/80.0f) * -x) * (RADIE + 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+1].y = -cosf((M_PI/80.0f) * -x) * (RADIE + 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+1].z = fZ;
						m_pQuadVerts[x*18+(y*3)+1].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*18+(y*3)+1].specular = 0x0;
						m_pQuadVerts[x*18+(y*3)+1].u = (m_pLayer[i][(x/2)*6+(5-(y/2))] % 8) * (0.125f) + texFix;
						m_pQuadVerts[x*18+(y*3)+1].v = (m_pLayer[i][(x/2)*6+(5-(y/2))] / 8) * (0.125f)+ texFix;

						m_pQuadVerts[x*18+(y*3)+2].x = -sinf((M_PI/80.0f) * -(x+2)) * (RADIE + 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+2].y = -cosf((M_PI/80.0f) * -(x+2)) * (RADIE + 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+2].z = fZ;
						m_pQuadVerts[x*18+(y*3)+2].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*18+(y*3)+2].specular = 0x0;
						m_pQuadVerts[x*18+(y*3)+2].u = (m_pLayer[i][(x/2)*6+(5-(y/2))] % 8) * (0.125f) + (0.125f)- texFix;
						m_pQuadVerts[x*18+(y*3)+2].v = (m_pLayer[i][(x/2)*6+(5-(y/2))] / 8) * (0.125f)+ texFix;


						m_pQuadVerts[x*18+(y*3)+3].x = -sinf((M_PI/80.0f)* -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+3].y = -cosf((M_PI/80.0f) * -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+3].z = fZ;
						m_pQuadVerts[x*18+(y*3)+3].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*18+(y*3)+3].specular = 0x0;
						m_pQuadVerts[x*18+(y*3)+3].u = (m_pLayer[i][(x/2)*6+(5-(y/2))] % 8)* (0.125f)+ texFix;
						m_pQuadVerts[x*18+(y*3)+3].v = (m_pLayer[i][(x/2)*6+(5-(y/2))] / 8)* (0.125f)+ (0.125f)- texFix;

						m_pQuadVerts[x*18+(y*3)+4].x = -sinf((M_PI/80.0f) * -(x+2)) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+4].y = -cosf((M_PI/80.0f) * -(x+2)) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+4].z = fZ;
						m_pQuadVerts[x*18+(y*3)+4].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*18+(y*3)+4].specular = 0x0;
						m_pQuadVerts[x*18+(y*3)+4].u = (m_pLayer[i][(x/2)*6+(5-(y/2))] % 8)* (0.125f)+0.125f- texFix;
						m_pQuadVerts[x*18+(y*3)+4].v = (m_pLayer[i][(x/2)*6+(5-(y/2))] / 8)* (0.125f)+0.125f- texFix;

						m_pQuadVerts[x*18+(y*3)+5].x = -sinf((M_PI/80.0f) * -(x+2)) * (RADIE + 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+5].y = -cosf((M_PI/80.0f)  * -(x+2)) * (RADIE + 32 + (y * 32));
						m_pQuadVerts[x*18+(y*3)+5].z = fZ;
						m_pQuadVerts[x*18+(y*3)+5].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*18+(y*3)+5].specular = 0x0;
						m_pQuadVerts[x*18+(y*3)+5].u = (m_pLayer[i][(x/2)*6+(5-(y/2))] % 8)* (0.125f)+(0.125f)- texFix;
						m_pQuadVerts[x*18+(y*3)+5].v = (m_pLayer[i][(x/2)*6+(5-(y/2))] / 8)* (0.125f)+ texFix;
					}
					else
					{	
						memset(&m_pQuadVerts[x*18+(y*3)],0,sizeof(QUADVERT)*6);

					}
				}
			}

			m_pVB[i] = new GLubyte[sizeof(QUADVERT)*(i_nVerticles/4)];
			if (m_pVB[i] == NULL)
				LOG("DEBUG: Setting up map: ERROR Creating Vertex Buffer nr. %i\n",i);
			memcpy(m_pVB[i],m_pQuadVerts,sizeof(QUADVERT)*(i_nVerticles/4));
/*			for (int j = 0; j < (i_nVerticles/4); j++) {
				QUADVERT *ptr = &((QUADVERT*)m_pVB[i])[j];
				GLubyte r = (ptr->diffuse>>16)&0xFF;
				GLubyte g = (ptr->diffuse>>8)&0xFF;
				GLubyte b = ptr->diffuse&0xFF;
				GLubyte a = (ptr->diffuse>>24)&0xFF;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				ptr->diffuse = r | (g<<8) | (b<<16) | (a<<24);
#else
				ptr->diffuse = (r<<24) | (g<<16) | (b<<8) | a;
#endif
			}
*/
		}
		else
		{
			for( x = 0; x < m_iWorldMapWidth; x++)
			{

				for( y = 0; y < 12; y++)
				{

					if(m_pLayer[i][x*12+(11-y)] > 0)
					{

						m_pQuadVerts[x*72+(y*6)].x = -sinf((M_PI/80.0f) * -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*72+(y*6)].y = -cosf((M_PI/80.0f) * -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*72+(y*6)].z = fZ;
						m_pQuadVerts[x*72+(y*6)].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*72+(y*6)].specular = 0x0;
						m_pQuadVerts[x*72+(y*6)].u = (m_pLayer[i][x*12+(11-y)] % 8) * (0.125f) + texFix;
						m_pQuadVerts[x*72+(y*6)].v = (m_pLayer[i][x*12+(11-y)] / 8) * (0.125f) + (0.125f) - texFix;

						m_pQuadVerts[x*72+(y*6)+1].x = -sinf((M_PI/80.0f) * -x) * (RADIE + (y * 32));
						m_pQuadVerts[x*72+(y*6)+1].y = -cosf((M_PI/80.0f) * -x) * (RADIE + (y * 32));
						m_pQuadVerts[x*72+(y*6)+1].z = fZ;
						m_pQuadVerts[x*72+(y*6)+1].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*72+(y*6)+1].specular = 0x0;
						m_pQuadVerts[x*72+(y*6)+1].u = (m_pLayer[i][x*12+(11-y)] % 8) * (0.125f) + texFix;
						m_pQuadVerts[x*72+(y*6)+1].v = (m_pLayer[i][x*12+(11-y)] / 8) * (0.125f)+ texFix;

						m_pQuadVerts[x*72+(y*6)+2].x = -sinf((M_PI/80.0f) * -(x+1)) * (RADIE + (y * 32));
						m_pQuadVerts[x*72+(y*6)+2].y = -cosf((M_PI/80.0f) * -(x+1)) * (RADIE + (y * 32));
						m_pQuadVerts[x*72+(y*6)+2].z = fZ;
						m_pQuadVerts[x*72+(y*6)+2].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*72+(y*6)+2].specular = 0x0;
						m_pQuadVerts[x*72+(y*6)+2].u = (m_pLayer[i][x*12+(11-y)] % 8) * (0.125f) + (0.125f)- texFix;
						m_pQuadVerts[x*72+(y*6)+2].v = (m_pLayer[i][x*12+(11-y)] / 8) * (0.125f)+ texFix;


						m_pQuadVerts[x*72+(y*6)+3].x = -sinf((M_PI/80.0f)* -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*72+(y*6)+3].y = -cosf((M_PI/80.0f) * -x) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*72+(y*6)+3].z = fZ;
						m_pQuadVerts[x*72+(y*6)+3].diffuse = 0xFFFFFFFF;
				//		m_pQuadVerts[x*72+(y*6)+3].specular = 0x0;
						m_pQuadVerts[x*72+(y*6)+3].u = (m_pLayer[i][x*12+(11-y)] % 8)* (0.125f)+ texFix;
						m_pQuadVerts[x*72+(y*6)+3].v = (m_pLayer[i][x*12+(11-y)] / 8)* (0.125f)+ (0.125f)- texFix;

						m_pQuadVerts[x*72+(y*6)+4].x = -sinf((M_PI/80.0f) * -(x+1)) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*72+(y*6)+4].y = -cosf((M_PI/80.0f) * -(x+1)) * (RADIE - 32 + (y * 32));
						m_pQuadVerts[x*72+(y*6)+4].z = fZ;
						m_pQuadVerts[x*72+(y*6)+4].diffuse = 0xFFFFFFFF;
//m_pQuadVerts[x*72+(y*6)+4].specular = 0x0;
						m_pQuadVerts[x*72+(y*6)+4].u = (m_pLayer[i][x*12+(11-y)] % 8)* (0.125f)+0.125f- texFix;
						m_pQuadVerts[x*72+(y*6)+4].v = (m_pLayer[i][x*12+(11-y)] / 8)* (0.125f)+0.125f- texFix;

						m_pQuadVerts[x*72+(y*6)+5].x = -sinf((M_PI/80.0f) * -(x+1)) * (RADIE + (y * 32));
						m_pQuadVerts[x*72+(y*6)+5].y = -cosf((M_PI/80.0f)  * -(x+1)) * (RADIE + (y * 32));
						m_pQuadVerts[x*72+(y*6)+5].z = fZ;
						m_pQuadVerts[x*72+(y*6)+5].diffuse = 0xFFFFFFFF;
					//	m_pQuadVerts[x*72+(y*6)+5].specular = 0x0;
						m_pQuadVerts[x*72+(y*6)+5].u = (m_pLayer[i][x*12+(11-y)] % 8)* (0.125f)+(0.125f)- texFix;
						m_pQuadVerts[x*72+(y*6)+5].v = (m_pLayer[i][x*12+(11-y)] / 8)* (0.125f)+ texFix;
					}
					else
					{	
						memset(&m_pQuadVerts[x*72+(y*6)],0,sizeof(QUADVERT)*6);

					}
				}
			}

			m_pVB[i] = new GLubyte[sizeof(QUADVERT)*(i_nVerticles)];
			if (m_pVB[i] == NULL)
				LOG("DEBUG: Setting up map: ERROR Creating Vertex Buffer nr. %i\n",i);
			memcpy(m_pVB[i],m_pQuadVerts,sizeof(QUADVERT)*(i_nVerticles));
/*			for (int j = 0; j < (i_nVerticles/4); j++) {
				QUADVERT *ptr = &((QUADVERT*)m_pVB[i])[j];
				GLubyte r = (ptr->diffuse>>16)&0xFF;
				GLubyte g = (ptr->diffuse>>8)&0xFF;
				GLubyte b = ptr->diffuse&0xFF;
				GLubyte a = (ptr->diffuse>>24)&0xFF;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				ptr->diffuse = r | (g<<8) | (b<<16) | (a<<24);
#else
				ptr->diffuse = (r<<24) | (g<<16) | (b<<8) | a;
#endif
			}
*/

		}
	}

	if(m_pQuadVerts)
	{
		delete [] m_pQuadVerts;
		m_pQuadVerts = NULL;
	}

}

void WorldMap::Release3DStuff()
{
	int i;

	for(i = 0; i < 3; i++)
	{
		if (m_pTexture[i])
		{
			glDeleteTextures(1,&m_pTexture[i]);
			m_pTexture[i]=0;
		}

		if (m_pVB[i])
		{
			delete [] m_pVB[i];
			m_pVB[i]=NULL;
		}
	}

}

bool WorldMap::LoadMap(char *szFileName)
{
 


	LOG("DEBUG: Loading map %s\n",szFileName);

	Pack->OpenPackFile("Data/Levels.pak");

	Uint8 *data = (Uint8*) Pack->GetDataByName(szFileName);
//	memcpy(&m_iWorldMapWidth,Pack->GetDataByName(szFileName),4);
	memcpy(&m_iWorldMapWidth,data,4);
	m_iWorldMapWidth = SDL_SwapLE32(m_iWorldMapWidth);


	m_pLayer[0] = new int[(m_iWorldMapWidth*12)/4];
	m_pLayer[1] = new int[m_iWorldMapWidth*12];
	m_pLayer[2] = new int[m_iWorldMapWidth*12];

//	memcpy(m_pLayer[0],Pack->GetDataByName(szFileName) + 4,4 * (m_iWorldMapWidth*12)/4);
//	memcpy(m_pLayer[1],Pack->GetDataByName(szFileName) + 4 + 4 * (m_iWorldMapWidth*12)/4,4 * m_iWorldMapWidth*12);
//	memcpy(m_pLayer[2],Pack->GetDataByName(szFileName) + 4 + 4 * (m_iWorldMapWidth*12)/4 + 4 * m_iWorldMapWidth*12,4 * m_iWorldMapWidth*12);
	memcpy(m_pLayer[0],data + 4,4 * (m_iWorldMapWidth*12)/4);
	memcpy(m_pLayer[1],data + 4 + 4 * (m_iWorldMapWidth*12)/4,4 * m_iWorldMapWidth*12);
	memcpy(m_pLayer[2],data + 4 + 4 * (m_iWorldMapWidth*12)/4 + 4 * m_iWorldMapWidth*12,4 * m_iWorldMapWidth*12);

	Pack->ClosePackFile();

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	for (int i = 0; i < (m_iWorldMapWidth*12)/4; i++)
		m_pLayer[0][i] = SDL_SwapLE32(m_pLayer[0][i]);
	for (int i = 0; i < m_iWorldMapWidth*12; i++) {
		m_pLayer[1][i] = SDL_SwapLE32(m_pLayer[1][i]);
		m_pLayer[2][i] = SDL_SwapLE32(m_pLayer[2][i]);
	}
#endif


	LOG("DEBUG: Setting up map: Map Width: %i\n", m_iWorldMapWidth);



	

	LOG("DEBUG: Exiting Map Loading...\n");
	return true;
}



bool WorldMap::RenderWorld(float fX, int iLayer)
{ 

	glColor4ub(0xFF,0xFF,0xFF,0xFF);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glInterleavedArrays(GL_T2F_C4UB_V3F, sizeof(QUADVERT), m_pVB[iLayer]);

	glVertexPointer(3,GL_FLOAT,sizeof(QUADVERT), m_pVB[iLayer]+sizeof(GLfloat)*2+sizeof(GLuint));
	glTexCoordPointer(2,GL_FLOAT,sizeof(QUADVERT), m_pVB[iLayer]+0);
//	glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(QUADVERT), m_pVB[iLayer]+sizeof(GLfloat)*2);

	glBindTexture(GL_TEXTURE_2D, m_pTexture[iLayer]);
	glEnable(GL_TEXTURE_2D);

	int i_nVerticles = m_iWorldMapWidth * 12 * 2 * 3;
	if(iLayer == 0)
	{
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(320,1220,0);
	
		glRotatef(180*(-0.7 + fX*(M_PI/(640*4)))/M_PI,0,0,1);

	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-130,760,580,-300,1,100);

//		int start = (int(-fX)/64)*36;
//		int n = 480*3;
		int start = (int(100-fX)/64)*36;
		int n = 220*3;
		if (start+n>i_nVerticles/4) n = i_nVerticles/4-start;
		glDrawArrays(GL_TRIANGLES, start, n);
	}
	else if(iLayer == 1)
	{


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,640,480,0,1,100);

//		int start = (int(-fX)/32)*72;
//		int n = 720*3;
		int start = (int(200-fX)/32)*72;
		int n = 600*3;
		if (start+n>i_nVerticles) n = i_nVerticles-start;
		glDrawArrays(GL_TRIANGLES, start, n);


	}
	else if( iLayer == 2)
	{
	
//		int start = (int(-fX)/32)*72;
//		int n = 720*3;
		int start = (int(200-fX)/32)*72;
		int n = 600*3;
		if (start+n>i_nVerticles) n = i_nVerticles-start;
		glDrawArrays(GL_TRIANGLES, start, n);
	}


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	return true;
}
