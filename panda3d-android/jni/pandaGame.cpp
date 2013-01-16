/*
 * pandaGame.cpp
 * Created on: 16/01/2013
 * Author: heekinho
 */

#include "pandaGame.h"

/* Panda Includes */
#include "pandaFramework.h"
#include "windowFramework.h"
#include "androidGraphicsPipe.h"
#include "graphicsPipe.h"
#include "clockObject.h"
#include "load_prc_file.h"
#include "cardMaker.h"
#include "config_androiddisplay.h"

PandaGame::PandaGame(struct android_app* app) : NativeEngine(app) {

}

PandaGame::~PandaGame(){

}

void PandaGame::on_init_window(){
	if (app->window != NULL) {
		load_prc_file("/sdcard/ConfigAndroid.prc");

		char** argv = NULL;
		int argc = '0';
		framework.open_framework(argc, argv);

		// We need to explicitly create a Android Pipe to pass the state (android_app)
		aPipe = new AndroidGraphicsPipe("my-android-pipe", app);

		WindowProperties winprops = WindowProperties::get_config_properties();
		winprops.set_size(480, 728);
		winprops.set_fullscreen(true);

		window = framework.open_window(winprops, GraphicsPipe::BF_require_window, aPipe, 0);
		AndroidGraphicsWindow* aWindows = DCAST(AndroidGraphicsWindow, window->get_graphics_window());

//		nout << "+===================================+" << endl;
//		nout << " Info about GraphicsWindows" << endl;
//		nout << "Active? " << (aWindows->is_active() ? "Yes" : "No") << endl;
//		nout << "Fullscreen? " << (aWindows->is_fullscreen() ? "Yes" : "No") << endl;

		if(window == NULL) {
			cout << "Mensagem: Não abriu a janela" << endl;
		}

		fps_text = new TextNode("FPS Label");
		fps_text->set_text("Panda3D");
		fps_text_np = window->get_render_2d().attach_new_node(fps_text);
		fps_text_np.set_scale(0.1f);
		//fps_text_np.set_z(0.8f);


//		//panda = window->load_model(window->get_render(), "/data/data/com.heekinho.android.panda3d/panda.egg");
//		panda = window->load_model(window->get_render(), "/sdcard/panda.egg");
//		panda.set_y(50);
	}

	draw_window();
}

void PandaGame::draw_window(){
	if(window){
		float fps = ClockObject::get_global_clock()->get_average_frame_rate();
		cout << fps << endl;
		stringstream ss;
		ss << fps;
		//fps_text->set_text(ss.str());

//		//panda.set_h(ClockObject::get_global_clock()->get_frame_count());
		fps_text_np.set_r(ClockObject::get_global_clock()->get_frame_count());
		framework.do_frame(Thread::get_current_thread());
	}
}

void PandaGame::on_term_window(){
	cout << "Fechando janela..." << endl;
	framework.close_framework();
}
