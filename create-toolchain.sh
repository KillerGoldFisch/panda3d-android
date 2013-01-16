#!/bin/sh
# Creates a toolchain based on a user.config file

# Save the current directory
CURRENT_PATH=$PWD

# Source the config file
. "$CURRENT_PATH"/user.config

# We need to be at the ndk root
cd $ANDROID_NDK_PATH

# Removes old toolchain
rm -R -f $CURRENT_PATH/$ANDROID_TOOLCHAIN_NAME

# Create new toolchain
sh "$ANDROID_NDK_PATH"/build/tools/make-standalone-toolchain.sh \
--platform=android-"$ANDROID_LEVEL" --install-dir="$CURRENT_PATH"/$ANDROID_TOOLCHAIN_NAME --toolchain="$ANDROID_ARCH_COMPLETE"-"$ANDROID_COMPILER"

# Restore the path we started in
cd $CURRENT_PATH
