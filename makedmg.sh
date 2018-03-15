#!/bin/sh
mkdir -p "Cadog Adventures"
cd "Cadog Adventures"
rm -rf "Cadog Adventures.app"
cp -R ../Cadog.app .
strip Cadog.app/Contents/MacOS/cadog
mkdir -p Cadog.app/Contents/Frameworks
cp -R ~/sdlmain/SDL.framework Cadog.app/Contents/Frameworks
cp -R ~/sdlmain/SDL_image.framework Cadog.app/Contents/Frameworks
cp -R ~/sdlmain/SDL_mixer.framework Cadog.app/Contents/Frameworks
mv Cadog.app "Cadog Adventures.app"
iconv -f latin1 -t macroman ../Readme.txt > Readme.txt
cd ..
rm -f cadog-osx.dmg
hdiutil create -srcfolder "Cadog Adventures" tmp.dmg
hdiutil convert -format UDZO -imagekey zlib-level=9 tmp.dmg -o cadog-osx.dmg
rm -f tmp.dmg
rm -rf "Cadog Adventures"


