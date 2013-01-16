/*
 * nativeEngine.cpp
 * Created on: 16/01/2013
 * Author: heekinho
 */

#ifndef NATIVEENGINE_H
#define NATIVEENGINE_H

#include "android_native_app_glue.h"

class NativeEngine {
public:
	NativeEngine(struct android_app* app);
	virtual ~NativeEngine();

	static void handle_cmd(struct android_app* app, int32_t cmd);
	static int32_t handle_input(struct android_app* app, AInputEvent* input_event);

	virtual void handle_cmd(int32_t cmd);

	virtual int32_t handle_input(AInputEvent* input_event);

	// Events
	virtual void on_input_changed(){}
	virtual void on_init_window(){}
	virtual void on_window_resized(){}
	virtual void on_term_window(){}
	virtual void on_window_redraw_needed(){}
	virtual void on_content_rect_changed(){}
	virtual void on_gained_focus(){}
	virtual void on_lost_focus(){}
	virtual void on_config_changed(){}
	virtual void on_low_memory(){}
	virtual void on_start(){}
	virtual void on_resume(){}
	virtual void on_save_state(){}
	virtual void on_pause(){}
	virtual void on_stop(){}
	virtual void on_destroy(){}


	virtual void draw_window(){}
	virtual void process_sensors(){}
	virtual void main_loop();


	struct android_app* app;
};



#endif /* NATIVEENGINE_H */
