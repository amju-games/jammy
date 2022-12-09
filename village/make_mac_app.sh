rm -rf Village.app
mkdir -p Village.app/Contents/MacOS/
mkdir -p Village.app/Contents/Resources/assets

# Copy bass dylib
cp source/libbass.dylib Village.app/Contents/MacOS/

# Copy game exe
cp source/jammy Village.app/Contents/MacOS/Village

cp -R assets/ Village.app/Contents/Resources/assets/

cp Info.plist Village.app/Contents/


