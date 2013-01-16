/*
 * pandaGame.h
 * Created on: 16/01/2013
 * Author: heekinho
 */

#ifndef PANDAGAME_H
#define PANDAGAME_H

#include "nativeEngine.h"
#include "androidGraphicsWindow.h"

#include "pandaFramework.h"

class TextNode;

class PandaGame : public NativeEngine {
public:
	PandaGame(struct android_app* app);
	virtual ~PandaGame();

	virtual void draw_window();

	virtual void on_init_window();
	virtual void on_term_window();


	TextNode* fps_text;
	NodePath fps_text_np;

	PandaFramework framework;
	WindowFramework* window;

	AndroidGraphicsPipe* aPipe;
	AndroidGraphicsWindow* custom_window;

};


#endif /* PANDAGAME_H */
