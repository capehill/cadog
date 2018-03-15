#include <math.h>
#include <stdio.h>
#include "PackManager.h"


int main(int argc, char *argv[]) {

/*	Pack->OpenPackFile("Data/Sound.pak");
	for (int i = 0; i < Pack->GetFileAmount(); i++) {
		char buffer[512];
		sprintf(buffer,"Data/unpacked/%s",Pack->GetNameByIndex(i));
		FILE *out = fopen(buffer,"wb");
		fwrite(Pack->GetDataByIndex(i),Pack->GetSizeByIndex(i),1,out);
		fclose(out);
	}
	Pack->ClosePackFile();
	Pack->OpenPackFile("Data/Music.pak");
	for (int i = 0; i < Pack->GetFileAmount(); i++) {
		char buffer[512];
		sprintf(buffer,"Data/unpacked/%s",Pack->GetNameByIndex(i));
		FILE *out = fopen(buffer,"wb");
		fwrite(Pack->GetDataByIndex(i),Pack->GetSizeByIndex(i),1,out);
		fclose(out);
	}
	Pack->ClosePackFile();
*/

	Pack->CleanUp();
	Pack->AddFile("sndBack.wav");
	Pack->AddFile("sndCannon.wav");
	Pack->AddFile("sndCheck.wav");
	Pack->AddFile("sndCoin.wav");
	Pack->AddFile("sndEnemy.wav");
	Pack->AddFile("sndGameOver.wav");
	Pack->AddFile("sndGameWon.wav");
	Pack->AddFile("sndHit.wav");
	Pack->AddFile("sndJump.wav");
	Pack->AddFile("sndLC.wav");
	Pack->AddFile("sndLevelCompleted.wav");
	Pack->AddFile("sndSelect.wav");
	Pack->AddFile("sndWater.wav");
	Pack->ProcessPackFile("newsound.pak");

	return 0;

}

