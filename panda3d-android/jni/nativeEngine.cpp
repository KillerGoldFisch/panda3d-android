/*
 * nativeEngine.cpp
 * Created on: 16/01/2013
 * Author: heekinho
 */

#include "nativeEngine.h"


NativeEngine::NativeEngine(struct android_app* app){
	this->app = app;
	this->app->userData = this;
	this->app->onAppCmd = NativeEngine::handle_cmd;
	this->app->onInputEvent = NativeEngine::handle_input;
}

NativeEngine::~NativeEngine(){
	this->app = NULL;
}

void NativeEngine::handle_cmd(struct android_app* app, int32_t cmd){
	((NativeEngine*) app->userData)->handle_cmd(cmd);
}

int32_t NativeEngine::handle_input(struct android_app* app, AInputEvent* input_event){
	return ((NativeEngine*) app->userData)->handle_input(input_event);
}


void NativeEngine::handle_cmd(int32_t cmd){
	switch (cmd) {
		case APP_CMD_INPUT_CHANGED:
			on_input_changed();
			break;
		case APP_CMD_INIT_WINDOW:
			on_init_window();
			break;
		case APP_CMD_TERM_WINDOW:
			on_term_window();
			break;
		case APP_CMD_WINDOW_RESIZED:
			on_window_resized();
			break;
		case APP_CMD_WINDOW_REDRAW_NEEDED:
			on_window_redraw_needed();
			break;
		case APP_CMD_CONTENT_RECT_CHANGED:
			on_content_rect_changed();
			break;
		case APP_CMD_GAINED_FOCUS:
			on_gained_focus();
			break;
		case APP_CMD_LOST_FOCUS:
			on_lost_focus();
			break;
		case APP_CMD_CONFIG_CHANGED:
			on_config_changed();
			break;
		case APP_CMD_LOW_MEMORY:
			on_low_memory();
			break;
		case APP_CMD_START:
			on_start();
			break;
		case APP_CMD_RESUME:
			on_resume();
			break;
		case APP_CMD_SAVE_STATE:
			on_save_state();
			break;
		case APP_CMD_PAUSE:
			on_pause();
			break;
		case APP_CMD_STOP:
			on_stop();
			break;
		case APP_CMD_DESTROY:
			on_destroy();
			break;
	}
}


int32_t NativeEngine::handle_input(AInputEvent* input_event){
	return 0;
}


void NativeEngine::main_loop(){
	while(true){
		int ident, events;
		struct android_poll_source* source;

		// While there's events, process them
		while((ident = ALooper_pollAll(0, NULL, &events, (void**) &source)) > 0){
			// Process the event
			if(source != NULL) source->process(app, source);

			 // If a sensor has data, process it now.
			if(ident == LOOPER_ID_USER) process_sensors();

			// If we are exiting
			if(app->destroyRequested != 0){
				on_term_window();
				return;
			}
		}

		// Drawing is throttled to the screen update rate, so there is no need to do timing here.
		draw_window();
	}
}

