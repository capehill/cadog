#include <stdio.h>
#include "SDL/SDL.h"
#include <SDL/SDL_endian.h>
#include "PackManager.h"
#if defined(__APPLE__) && defined(__MACH__)
#include <CoreFoundation/CoreFoundation.h>
#endif
 
PackManager PackManager::I;

PackManager::PackManager()
{
	filePackFile = NULL;
	m_iAmountOfFiles = 0;
    m_bFileOpen = false;
	tmpBuffer = NULL;

	for(int i = 0; i < MAX_FILES_PER_PACK; i++)
	{
		fiMainIndex[i].iFileSize = 0;
		fiMainIndex[i].iReserved = 0;
		memset(fiMainIndex[i].szFileName,0,sizeof(fiMainIndex[i].szFileName));
	}

}
	
PackManager::~PackManager()
{
	CleanUp();
}

void PackManager::CreateNewPackFile()
{
	m_iAmountOfFiles = 0;
}


bool PackManager::AddFile(char *szFileName)
{
	FILE *f = NULL;


	f = fopen(szFileName,"rb");

	if(f == NULL) return false;

	fseek(f, 0,SEEK_END);

	unsigned int size = ftell(f);
	
	fclose(f);

	fiMainIndex[m_iAmountOfFiles].iFileSize = size;
	fiMainIndex[m_iAmountOfFiles].iReserved = 0;

	unsigned int location = 0;

	for(int i = 0; i < m_iAmountOfFiles; i++)
	{
		location += fiMainIndex[i].iFileSize;
	}

	fiMainIndex[m_iAmountOfFiles].iFileLocation = location;


	char tmpFileName[32];

	memset(tmpFileName,'\0',sizeof(tmpFileName));
#ifdef WIN32
	_snprintf(tmpFileName,32,"%s",szFileName);
#else
	snprintf(tmpFileName,32,"%s",szFileName);
#endif

	memcpy(fiMainIndex[m_iAmountOfFiles].szFileName,tmpFileName,32);

	printf("\n----- Add File Info -----\n");
	printf("Index: %i\n",m_iAmountOfFiles);
	printf("File Name: %s\n",fiMainIndex[m_iAmountOfFiles].szFileName);
	printf("File Size: %u\n",fiMainIndex[m_iAmountOfFiles].iFileSize);
	printf("File Location: %u\n",fiMainIndex[m_iAmountOfFiles].iFileLocation);


	m_iAmountOfFiles++;

	return true;
}
 
bool PackManager::ProcessPackFile( char *szFileName )
{
	int i;
	FILE *f = NULL;

	filePackFile = fopen(szFileName,"wb");
	
	if(filePackFile == NULL) return false;
	SDL_RWops *rw = SDL_RWFromFP(filePackFile,1);

//	fwrite(&m_iAmountOfFiles,4,1,filePackFile); //Write amount of files
	SDL_WriteLE32(rw,m_iAmountOfFiles);


	//Write index
//	fwrite(&fiMainIndex,sizeof(FILE_INDEX),m_iAmountOfFiles,filePackFile);
	for (i = 0; i < m_iAmountOfFiles; i++) {
		SDL_RWwrite(rw,&fiMainIndex[i].szFileName,sizeof(fiMainIndex[i].szFileName),1);
		SDL_WriteLE32(rw,fiMainIndex[i].iFileSize);
		SDL_WriteLE32(rw,fiMainIndex[i].iFileLocation);
		SDL_WriteLE32(rw,fiMainIndex[i].iReserved);
	}


	unsigned char *tmpBuff = NULL;

	//Write data
	for(i = 0; i < m_iAmountOfFiles; i++)
	{
		f = fopen(fiMainIndex[i].szFileName,"rb");
		tmpBuff = new unsigned char[fiMainIndex[i].iFileSize];
		fread(tmpBuff,1,fiMainIndex[i].iFileSize,f);
		fclose(f);

//		fwrite(tmpBuff,1,fiMainIndex[i].iFileSize,filePackFile);
		SDL_RWwrite(rw,tmpBuff,fiMainIndex[i].iFileSize,1);
		
		delete [] tmpBuff;
		tmpBuff = NULL;

	}

//	fclose(filePackFile);
	SDL_RWclose(rw);
	filePackFile = NULL;
	

	return true;
}

void PackManager::CleanUp()
{
	if(filePackFile != NULL) 
	{
		fclose(filePackFile);
		filePackFile = NULL;
	}

	m_iAmountOfFiles = 0;
}

/*
void getResourcePath(char *buffer) {
	NSBundle *bundle = [NSBundle mainBundle];
	NSString *dir = [bundle resourcePath];
	const char *str = [dir UTF8String];
	strcpy(buffer, str);
	strcat(buffer, "/");
	[dir autorelease];
	[bundle autorelease];
}
*/

bool PackManager::OpenPackFile(char *szFileName)
{
#if defined(__APPLE__) && defined(__MACH__)
	char buffer[1000];
//	getResourcePath(buffer);
	CFURLRef url = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	CFURLGetFileSystemRepresentation(url, true, (UInt8*) buffer, sizeof(buffer));
	CFRelease(url);
	strcat(buffer, "/");
	strcat(buffer, szFileName);
	szFileName = buffer;
#endif
	strcpy(m_szFileName,szFileName);
	filePackFile = fopen(szFileName,"rb");
	
	if(filePackFile == NULL) return false;
	SDL_RWops *rw = SDL_RWFromFP(filePackFile,0); // no autoclose, so the FP is valid after SDL_RWclose

//	fread(&m_iAmountOfFiles,4,1,filePackFile);
	m_iAmountOfFiles = SDL_ReadLE32(rw);

	
//	fread(&fiMainIndex, sizeof(FILE_INDEX), m_iAmountOfFiles, filePackFile);
	for (int i = 0; i < m_iAmountOfFiles; i++) {
		SDL_RWread(rw,&fiMainIndex[i].szFileName,sizeof(fiMainIndex[i].szFileName),1);
		fiMainIndex[i].iFileSize = SDL_ReadLE32(rw);
		fiMainIndex[i].iFileLocation = SDL_ReadLE32(rw);
		fiMainIndex[i].iReserved = SDL_ReadLE32(rw);
	}
	SDL_RWclose(rw);

	m_bFileOpen = true;

//	printf("Opened %s and found: %i file(s)\n",szFileName,m_iAmountOfFiles);

	return true;

}

int  PackManager::GetFileAmount()
{
	return m_iAmountOfFiles;
}

int  PackManager::GetIndexByName(char *szFileName)
{
	for(int i = 0; i < m_iAmountOfFiles; i++)
	{
		if(strcmp(fiMainIndex[i].szFileName,szFileName) == 0) return i;
	}

	printf("GetIndexByName could not find file\n"); 
	return -1;
}

const char *PackManager::GetNameByIndex(int iIndex) {
	return fiMainIndex[iIndex].szFileName;
}

unsigned char *PackManager::GetDataByIndex( int iIndex )
{

	if( !m_bFileOpen ) return NULL;
	if(iIndex >= m_iAmountOfFiles) return NULL;

	if(tmpBuffer != NULL) //Clear old one
	{
		delete [] tmpBuffer;
		tmpBuffer = NULL;
	}

	tmpBuffer = new unsigned char[fiMainIndex[iIndex].iFileSize];

	fseek(filePackFile,4 + fiMainIndex[iIndex].iFileLocation + sizeof(FILE_INDEX) * m_iAmountOfFiles,SEEK_SET);

	fread(tmpBuffer,1,fiMainIndex[iIndex].iFileSize,filePackFile);

	if(tmpBuffer != NULL) return tmpBuffer;

	return NULL;

}
FILE *PackManager::GetFPByIndex( int iIndex, bool shared )
{

	if( !m_bFileOpen ) return NULL;
	if(iIndex >= m_iAmountOfFiles) return NULL;

	FILE *fp = filePackFile;
	if (!shared) {
		fp = fopen(m_szFileName,"rb");
		if (fp==NULL) return NULL;
	}
	fseek(fp,4 + fiMainIndex[iIndex].iFileLocation + sizeof(FILE_INDEX) * m_iAmountOfFiles,SEEK_SET);
	return fp;

}
unsigned char *PackManager::GetDataByName( char *szFileName )
{
	if(!m_bFileOpen) return NULL;

	for(int i = 0; i < m_iAmountOfFiles; i++)
	{
		if(strcmp(fiMainIndex[i].szFileName,szFileName) == 0)
		{
			return GetDataByIndex(i);
			break;
		}
	}

	return NULL;

}

FILE *PackManager::GetFPByName( char *szFileName, bool shared )
{
	if(!m_bFileOpen) return NULL;

	for(int i = 0; i < m_iAmountOfFiles; i++)
	{
		if(strcmp(fiMainIndex[i].szFileName,szFileName) == 0)
		{
			return GetFPByIndex(i, shared);
			break;
		}
	}

	return NULL;

}

unsigned int PackManager::GetSizeByIndex( int iIndex )
{
	if(!m_bFileOpen) return (unsigned int)-1;

	return fiMainIndex[iIndex].iFileSize;

}

unsigned int PackManager::GetSizeByName( char *szFileName )
{
	if(!m_bFileOpen) return (unsigned int)-1;

	for(int i = 0; i < m_iAmountOfFiles; i++)
	{
		if(strcmp(fiMainIndex[i].szFileName,szFileName) == 0)
		{
			return fiMainIndex[i].iFileSize;
			break;
		}
	}

	return (unsigned int)-1;

}

void PackManager::ClosePackFile()
{
	if(tmpBuffer != NULL) //Clear tmp buffer
	{
		delete [] tmpBuffer;
		tmpBuffer = NULL;
	}

	fclose(filePackFile);
	filePackFile = NULL;
	m_bFileOpen = false;
}
