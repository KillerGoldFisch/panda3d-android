#!/bin/sh

#set -e

# Save the current directory
CURRENT_PATH=$PWD

# Source the config file
. "$CURRENT_PATH"/user.config

print_header() {
	echo "\n------------------------------------------------------"
	echo $1
	echo "------------------------------------------------------\n"
}

install_ppremake () {
	echo "\nInstalling ppremake"
	aclocal
	autoheader
	automake --foreign -a
	autoconf
	./configure --prefix="$PANDA_BUILD_PATH"
	make install
}

set_environment_variables() {
	echo "\nSetting environment variables"
	PATH="$PANDA_BUILD_PATH/bin":$PATH
	PATH="$ANDROID_TOOLCHAIN_PATH/bin":$PATH
	LD_LIBRARY_PATH="$PANDA_BUILD_PATH/lib":$LD_LIBRARY_PATH

	export PATH
	export LD_LIBRARY_PATH
	#echo $PATH
	#echo $LD_LIBRARY_PATH

	# Build system may use these
	export PANDA_PATH
	#export ANDROID_GLUE_INCLUDE_PATH
	#export ANDROID_GLUE_LIB_PATH
	#export ANDROID_GLUE_LIB

	export ANDROID_ARCH
	export ANDROID_ARCH_2
	export ANDROID_ARCH_TOOLS_PREFIX
	export ANDROID_PLATFORM_PATH
	export ANDROID_TOOLCHAIN_PATH
	export ANDROID_SYSROOT
	export ANDROID_OPTIMIZE

	export PANDA_ANDROID_BUILD_PATH
}


build_dtool() {
	print_header "Building dtool..."

	print_header "dtool:Ppremake"
	ppremake -p Android

	print_header "dtool:Make Clean"
	make clean

	print_header "dtool:Make"
	make

	print_header "dtool:Make Install"
	make install
}


build_panda() {
	print_header "Building panda..."

	print_header "panda:Ppremake"
	ppremake -p Android

	print_header "panda:Make clean"
	make clean

	print_header "panda:Make"
	make

	print_header "panda:Make Install"
	make install
}

build_shared_library(){
	mkdir -p tmp
	cd tmp

	cp -u $PANDA_PATH/dtool/src/*/$1/*.o .
	cp -u $PANDA_PATH/dtool/metalibs/*/$1/*.o .

	cp -u $PANDA_PATH/panda/src/*/$1/*.o .
	cp -u $PANDA_PATH/panda/metalibs/*/$1/*.o .

	_ARCH_FLAGS=" -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now"
	_FLAGS_ANDROID=" -lm -llog -landroid -lEGL -lGLESv1_CM -lGLESv2 -L../$ANDROID_ARCH_TOOLS_PREFIX/lib -lgnustl_shared -lsupc++ -lstdc++"

	echo "Building p3dandroid library"
	"$ANDROID_TOOLCHAIN_PATH"/bin/"$ANDROID_ARCH_TOOLS_PREFIX"-g++ -Xlinker -zmuldefs -shared *.o $_ARCH_FLAGS -o ../libp3android.so -L. -lEGL -lGLESv1_CM -lGLESv2 $_FLAGS_ANDROID
}
