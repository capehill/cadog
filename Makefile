CXXFLAGS = -Wall `sdl-config --cflags` -Wno-non-virtual-dtor -Wno-conversion
#CXXFLAGS += -DHAVE_MUS_RW
LDFLAGS = `sdl-config --libs` -lSDL_image -lpng -lTIFF -ljpeg -lwebp -lz -lSDL_mixer -lSMPEG -lmikmod -lmodplug -lFLAC -lvorbisfile -lvorbis -logg -lSDL_orig -lGL -lGLU -lpthread


cadog: Actor.o ArmyGaem.o Coin.o EnemyManager.o Enemy_Bill.o Enemy_Blob.o Enemy_Blob_2.o Enemy_Blob_Dumb.o Enemy_Cocoa.o Enemy_Figaro.o Enemy_Parrot.o Enemy_Sheppy.o Enemy_Spikey.o Menu.o NumberDraw.o PackManager.o Player.o Signs.o WorldMap.o sdlapp.o sdlgl.o rwops.o Log.o
	$(CXX) -o $@ $+ $(LDFLAGS)

makepak: makepak.o PackManager.o
	$(CXX) -o $@ $+ $(LDFLAGS)

clean:
	rm -f cadog *.o core core.*

