
#ifndef PACKMANAGER_H
#define PACKMANAGER_H

#include <stdio.h>
#include <iostream>
#include "Log.h"

#define MAX_FILES_PER_PACK	64

struct FILE_INDEX
{
	char szFileName[32];
	unsigned int iFileSize;
	unsigned int iFileLocation;
	unsigned int iReserved;
};

class PackManager
{

	protected:
		
	FILE *filePackFile;
	FILE_INDEX	fiMainIndex[MAX_FILES_PER_PACK]; 
	char m_szFileName[128];
	
	unsigned char *tmpBuffer; //for a file

	int	 m_iAmountOfFiles;
	bool m_bFileOpen;

	public:
	PackManager();
	~PackManager();

	//For creating packfiles
	void CreateNewPackFile();
	bool AddFile(char *szFileName);
	bool ProcessPackFile( char *szFileName );
	void CleanUp();

	//For reading packfiles
	bool OpenPackFile(char *szFileName);

	int  GetFileAmount();
	const char *GetNameByIndex(int iIndex);
	int  GetIndexByName(char *szFileName);

	unsigned char *GetDataByIndex( int iIndex );
	unsigned char *GetDataByName( char *szFileName );
	FILE *GetFPByIndex(int iIndex, bool shared = true);
	FILE *GetFPByName(char *szFileName, bool shared = true);

	unsigned int GetSizeByIndex( int iIndex );
	unsigned int GetSizeByName( char *szFileName );

	void ClosePackFile();





	static PackManager* Instance() {return &I;}



	private:
	static PackManager I;
};

#define Pack PackManager::Instance()
 
#endif
