#!/bin/sh

set -e

# Save the current directory
CURRENT_PATH=$PWD

# Source the config file
. "$CURRENT_PATH"/user.config

# Source the functions files
. "$CURRENT_PATH"/build-functions.sh

# The script =)
# Install Ppremake
cd "$PANDA_PATH/ppremake"
#install_ppremake

cd $CURRENT_PATH
#cp -i "Config.Android.pp" "$PANDA_PATH/dtool"

#set_environment_variables

# Go to dtool and build it
cd  "$PANDA_PATH/dtool"
#rm -r -f src/*/Opt*-Android
#build_dtool

# Go to panda and build it
cd "$PANDA_PATH/panda"
#rm -r -f src/*/Opt*-Android
#build_panda

# Finally get the library! =D
echo "Build shared library"
cd $CURRENT_PATH
build_shared_library Opt"$ANDROID_OPTIMIZE"-Android

echo "Installing shared libraries"
cd $CURRENT_PATH
cp -u libp3android.so $INSTALL_LIB_PATH/$ANDROID_ARCH_2/libp3android.so

# Restore the path we started in
cd $CURRENT_PATH

