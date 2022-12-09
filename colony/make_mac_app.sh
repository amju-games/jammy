rm -rf Colony.app
mkdir -p Colony.app/Contents/MacOS/
mkdir -p Colony.app/Contents/Resources/assets

# Copy bass dylib
cp source/libbass.dylib Colony.app/Contents/MacOS/

# Copy game exe
cp source/jammy Colony.app/Contents/MacOS/Colony

cp -R assets/ Colony.app/Contents/Resources/assets/

cp Info.plist Colony.app/Contents/


