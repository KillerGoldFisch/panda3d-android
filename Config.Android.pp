//
// Config.Linux.pp
//
// This file defines some custom config variables for the Android
// platform.  It makes some initial guesses about compiler features,
// etc.
//

// *******************************************************************
// NOTE: you should not attempt to copy this file verbatim as your own
// personal Config.pp file.  Instead, you should start with an empty
// Config.pp file, and add lines to it when you wish to override
// settings given in here.  In the normal ppremake system, this file
// will always be read first, and then your personal Config.pp file
// will be read later, which gives you a chance to override the
// default settings found in this file.  However, if you start by
// copying the entire file, it will be difficult to tell which
// settings you have customized, and it will be difficult to upgrade
// to a subsequent version of Panda.
// *******************************************************************

// The initial configuration is similar of the Linux one
#include $[THISDIRPREFIX]Config.Linux.pp

#define LINK_ALL_STATIC yes


//#define INSTALL_DIR /usr/local/panda3d/$[IOS_PLATFORM]
#define INSTALL_DIR $[PANDA_ANDROID_BUILD_PATH]
//#define UNIVERSAL_BINARIES 1 

#define IS_ANDROID 1
#define HAVE_ANDROID 1
//#define ANDROID_TOOLCHAIN
//#defer ARCH_FLAGS arm
//#define ARCH_FLAGS -I$[ANDROID_PLATFORM_PATH]/usr/include

// Original
//#defer COMPILE_C $[CC] $[CFLAGS_GEN] $[ARCH_FLAGS] -c -o $[target] $[ipath:%=-I%] $[flags] $[source]
//#defer COMPILE_C++ $[CXX] $[C++FLAGS_GEN] $[ARCH_FLAGS] -c -o $[target] $[ipath:%=-I%] $[flags] $[source]

#define ANDROID_LIBS_  -L../$[ANDROID_ARCH_TOOLS_PREFIX]/lib -lm -lgnustl_shared -lsupc++ -lstdc++
#define ARCH_FLAGS -ffunction-sections -funwind-tables -fomit-frame-pointer -fstrict-aliasing -funswitch-loops


//#define LD_FLAGS -L../$[ANDROID_ARCH_TOOLS_PREFIX]/lib -lm -lgnustl_shared -lsupc++ -lstdc++
//#defer CFLAGS_SHARED -fPIC $[LD_FLAGS]

#defer COMPILE_C $[CC] --sysroot=$[ANDROID_SYSROOT] -c $[CFLAGS_GEN] $[ARCH_FLAGS] -o $[target] -I$[ANDROID_PLATFORM_PATH]/usr/include $[ipath:%=-I%] $[flags] $[source]
#defer COMPILE_C++ $[CXX] --sysroot=$[ANDROID_SYSROOT] -c $[C++FLAGS_GEN] $[ARCH_FLAGS] -o $[target] -I$[ANDROID_PLATFORM_PATH]/usr/include $[ipath:%=-I%] $[flags] $[source]

// Android Toolchain. The bin directory should be in the path
//#define ANDROID_PLATFORM_PATH /home/heekinho/Downloads/android-ndk-r7c/platforms/android-9/arch-arm


// How to generate a shared C or C++ library.  $[source] and $[target]
// as above, and $[libs] is a space-separated list of dependent
// libraries, and $[lpath] is a space-separated list of directories in
// which those libraries can be found.
#defer SHARED_LIB_C $[cc_ld] -shared $[LFLAGS] -o $[target] $[sources] $[lpath:%=-L%] $[libs:%=-l%] $[ANDROID_LIBS_]
#defer SHARED_LIB_C++ $[cxx_ld] -shared $[LFLAGS] -o $[target] $[sources] $[lpath:%=-L%] $[libs:%=-l%] $[ANDROID_LIBS_]


#defer LINK_BIN_C $[cc_ld] $[ARCH_FLAGS] -o $[target] $[sources] $[flags] $[lpath:%=-L%] $[libs:%=-l%]\
 $[fpath:%=-Wl,-F%] $[patsubst %,-framework %, $[bin_frameworks]]  $[ANDROID_LIBS_]
#defer LINK_BIN_C++ $[cxx_ld] $[ARCH_FLAGS] \
 -o $[target] $[sources]\
 $[flags]\
 $[lpath:%=-L%] $[libs:%=-l%]\
 $[fpath:%=-Wl,-F%] $[patsubst %,-framework %, $[bin_frameworks]]  $[ANDROID_LIBS_]


#define CC $[ANDROID_ARCH_TOOLS_PREFIX]-gcc
#define CXX $[ANDROID_ARCH_TOOLS_PREFIX]-g++
#define AR $[ANDROID_ARCH_TOOLS_PREFIX]-ar
#defer RANLIB $[ANDROID_ARCH_TOOLS_PREFIX]-ranlib $[target]
#define get_ld $[ANDROID_ARCH_TOOLS_PREFIX]-ld
//#define LD $[ANDROID_ARCH_TOOLS_PREFIX]-ld



// Is a third-party STL library installed, and where?  This is only
// necessary if the default include and link lines that come with the
// compiler don't provide adequate STL support.  At least some form of
// STL is absolutely required in order to build Panda.
//#define STL_IPATH 
//#define STL_LPATH
//#define STL_CFLAGS
//#define STL_LIBS


// What level of compiler optimization/debug symbols should we build?
// The various optimize levels are defined as follows:
//
//   1 - No compiler optimizations, debug symbols, debug heap, lots of checks
//   2 - Full compiler optimizations, debug symbols, debug heap, lots of checks
//   3 - Full compiler optimizations, full debug symbols, fewer checks
//   4 - Full optimizations, no debug symbols, and asserts removed
//
#define OPTIMIZE $[ANDROID_OPTIMIZE]


// Do we have RTTI (and <typeinfo>)?
//#define HAVE_RTTI 1

// Do we have <unistd.h>?
#define PHAVE_UNISTD_H

// Do we have SSL?
#define HAVE_OPENSSL

// Do we have <glob.h> (and do we want to use it instead of dirent.h)?
#define PHAVE_GLOB_H

// Do not use Python yet.
#define HAVE_PYTHON
#define INTERROGATE_PYTHON_INTERFACE
#define HAVE_INTERROGATE

// Do not enable Threads yet.
#define HAVE_THREADS

// Does <string> define the typedef wstring?  Most do, but for some
// reason, versions of gcc before 3.0 didn't do this.
#define HAVE_WSTRING

// Do not use Native Net yet.
//#define WANT_NATIVE_NET
//#define HAVE_NET

// Do not use PStats
#define DO_PSTATS

// Do not use CG yet.
#define CG_FRAMEWORK
#define HAVE_CG

// Do not use OpenAl yet.
#define HAVE_OPENAL

// Do not use Freetype yet.
#define HAVE_FREETYPE 

#define HAVE_VIDEO4LINUX

// Do not use Tinydisplay yet.
#define HAVE_TINYDISPLAY

// Do not use X11.
#define HAVE_X11

#define HAVE_GL
#define HAVE_JPEG
#define HAVE_PNG
#define HAVE_TIFF
#define HAVE_ZLIB
#define HAVE_PHYSX
#define HAVE_FFMPEG
#define HAVE_ODE


// Is OpenGL ES 1.x installed, and where? This is a minimal subset of
// OpenGL for mobile devices.
#define GLES_IPATH $[ANDROID_SYSROOT]/usr/include
#define GLES_LPATH $[ANDROID_SYSROOT]/usr/lib
#define GLES_LIBS GLESv1_CM
#defer HAVE_GLES $[libtest $[GLES_LPATH],$[GLES_LIBS]]

// OpenGL ES 2.x is a version of OpenGL ES but without fixed-function
// pipeline - everything is programmable there.
#define GLES2_IPATH $[ANDROID_SYSROOT]/usr/include
#define GLES2_LPATH $[ANDROID_SYSROOT]/usr/lib
#define GLES2_LIBS GLESv2
#defer HAVE_GLES2 $[libtest $[GLES2_LPATH],$[GLES2_LIBS]]

// EGL is like GLX, but for OpenGL ES.
#defer EGL_IPATH $[ANDROID_PLATFORM_PATH]/usr/include
#defer EGL_LPATH $[ANDROID_PLATFORM_PATH]/usr/lib
#defer EGL_LIBS EGL
#defer HAVE_EGL $[libtest $[EGL_LPATH],$[EGL_LIBS]]
//#define HAVE_EGL
