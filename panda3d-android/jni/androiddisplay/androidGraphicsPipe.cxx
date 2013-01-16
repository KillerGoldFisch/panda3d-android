// Filename: AndroidGraphicsPipe.cxx
// Created by:  pro-rsoft (21May09)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

//#include "eglGraphicsBuffer.h"
#include "androidGraphicsPipe.h"
//#include "eglGraphicsPixmap.h"
#include "androidGraphicsWindow.h"
#include "androidGraphicsStateGuardian.h"
#include "config_androiddisplay.h"
#include "frameBufferProperties.h"

TypeHandle AndroidGraphicsPipe::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsPipe::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
AndroidGraphicsPipe::
AndroidGraphicsPipe(const string &display, struct android_app* app) :
  _app (app)
{
  cout << "Iniciando AndroidGraphicsPipe..." << endl;
  
  // Make sure glue isn't stripped.
  app_dummy();

  if(_app == NULL){
    androidegldisplay_cat.warning() << "Please provide a non null android_app "
        << "for AndroidGraphicsPipe constructor";
  }

  if(app->window){
	  _display_width = ANativeWindow_getWidth(app->window);
	  _display_height = ANativeWindow_getHeight(app->window);
  }

  _egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  _is_valid = true;
  _supported_types = OT_window | OT_buffer | OT_texture_buffer;


  if (!eglInitialize(_egl_display, NULL, NULL)) {
    androidegldisplay_cat.error() << "Couldn't initialize the EGL display: "
        << get_egl_error_string(eglGetError()) << "\n";
  }

  if (!eglBindAPI(EGL_OPENGL_ES_API)) {
    androidegldisplay_cat.error() << "Couldn't bind EGL to the OpenGL ES API: "
        << get_egl_error_string(eglGetError()) << "\n";
  }

  nout << "Informations about GraphicsPipe: " << endl;
  nout << "display_width: " << GraphicsPipe::get_display_width() << endl;
  nout << "display_height: " << GraphicsPipe::get_display_height() << endl;
  nout << "display_num_cpu_cores: " << GraphicsPipe::get_display_information()->_num_cpu_cores << endl;

}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsPipe::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
AndroidGraphicsPipe::
~AndroidGraphicsPipe() {
  if (_egl_display) {
    if (!eglTerminate(_egl_display)) {
      androidegldisplay_cat.error() << "Failed to terminate EGL display: "
        << get_egl_error_string(eglGetError()) << "\n";
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsPipe::get_interface_name
//       Access: Published, Virtual
//  Description: Returns the name of the rendering interface
//               associated with this GraphicsPipe.  This is used to
//               present to the user to allow him/her to choose
//               between several possible GraphicsPipes available on a
//               particular platform, so the name should be meaningful
//               and unique for a given platform.
////////////////////////////////////////////////////////////////////
string AndroidGraphicsPipe::
get_interface_name() const {
  return "OpenGL ES";
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsPipe::pipe_constructor
//       Access: Public, Static
//  Description: This function is passed to the GraphicsPipeSelection
//               object to allow the user to make a default
//               AndroidGraphicsPipe.
////////////////////////////////////////////////////////////////////
PT(GraphicsPipe) AndroidGraphicsPipe::
pipe_constructor() {
  return new AndroidGraphicsPipe;
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsPipe::get_preferred_window_thread
//       Access: Public, Virtual
//  Description: Returns an indication of the thread in which this
//               GraphicsPipe requires its window processing to be
//               performed: typically either the app thread (e.g. X)
//               or the draw thread (Windows).
////////////////////////////////////////////////////////////////////
GraphicsPipe::PreferredWindowThread
AndroidGraphicsPipe::get_preferred_window_thread() const {
  // Actually, since we're creating the graphics context in
  // open_window() now, it appears we need to ensure the open_window()
  // call is performed in the draw thread for now, even though X wants
  // all of its calls to be single-threaded.

  // This means that all X windows may have to be handled by the same
  // draw thread, which we didn't intend (though the global _x_mutex
  // may allow them to be technically served by different threads,
  // even though the actual X calls will be serialized).  There might
  // be a better way.

  return PWT_draw;
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsPipe::make_output
//       Access: Protected, Virtual
//  Description: Creates a new window on the pipe, if possible.
////////////////////////////////////////////////////////////////////
PT(GraphicsOutput) AndroidGraphicsPipe::
make_output(const string &name,
            const FrameBufferProperties &fb_prop,
            const WindowProperties &win_prop,
            int flags,
            GraphicsEngine *engine,
            GraphicsStateGuardian *gsg,
            GraphicsOutput *host,
            int retry,
            bool &precertify) {

  if (!_is_valid) {
    return NULL;
  }

//  AndroidGraphicsStateGuardian *androidgsg = 0;
//  if (gsg != 0) {
//    DCAST_INTO_R(androidgsg, gsg, NULL);
//  }


  // First thing to try: an AndroidGraphicsWindow
  if (retry == 0) {
    if (((flags&BF_require_parasite)!=0)||
        ((flags&BF_refuse_window)!=0)||
        ((flags&BF_resizeable)!=0)||
        ((flags&BF_size_track_host)!=0)||
        ((flags&BF_can_bind_color)!=0)||
        ((flags&BF_can_bind_every)!=0)) {
      return NULL;
    }

    cout << "Criando Janela" << endl;
    return new AndroidGraphicsWindow(engine, this, name, fb_prop, win_prop, flags, gsg, host);
  }

  // Nothing else left to try.
  return NULL;
}
