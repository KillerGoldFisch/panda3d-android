/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>

#ifdef OPENGLES_1
#include <GLES/gl.h>
#endif

#ifdef OPENGLES_2
#include <GLES2/gl2.h>
#endif

#include <dlfcn.h>

#include <iostream>
using namespace std;


#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#include <unistd.h>


#include "pandaGame.h"

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* app) {
	// Wait the debug attach
	//sleep(3);

	LOGI("Android Main! Rolando o baba! =D");
	cout << "Testando cout" << endl;

    // Make sure glue isn't stripped.
    app_dummy();


//    NativeEngine* engine = new NativeEngine(app);
//    engine->main_loop();

    PandaGame* engine = new PandaGame(app);
    engine->main_loop();
}
//END_INCLUDE(all)
