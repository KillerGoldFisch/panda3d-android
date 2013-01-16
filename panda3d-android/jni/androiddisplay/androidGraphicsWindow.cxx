// Filename: AndroidGraphicsWindow.cxx
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

#include "androidGraphicsWindow.h"
#include "androidGraphicsStateGuardian.h"
#include "config_androiddisplay.h"
#include "androidGraphicsPipe.h"

#include "graphicsPipe.h"
#include "clockObject.h"
#include "pStatTimer.h"
#include "textEncoder.h"
#include "throw_event.h"
#include "lightReMutexHolder.h"
#include "nativeWindowHandle.h"

#include <errno.h>
#include <sys/time.h>

#ifdef HAVE_LINUX_INPUT_H
#include <linux/input.h>
#endif

TypeHandle AndroidGraphicsWindow::_type_handle;

#define test_bit(bit, array) ((array)[(bit)/8] & (1<<((bit)&7)))

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
AndroidGraphicsWindow::
AndroidGraphicsWindow(GraphicsEngine *engine, GraphicsPipe *pipe,
                  const string &name,
                  const FrameBufferProperties &fb_prop,
                  const WindowProperties &win_prop,
                  int flags,
                  GraphicsStateGuardian *gsg,
                  GraphicsOutput *host) :
  GraphicsWindow(engine, pipe, name, fb_prop, win_prop, flags, gsg, host)
{
  AndroidGraphicsPipe *android_pipe;
  DCAST_INTO_V(android_pipe, _pipe);
  _egl_display = android_pipe->_egl_display;
  _egl_surface = 0;
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
AndroidGraphicsWindow::
~AndroidGraphicsWindow() {
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::begin_frame
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               before beginning rendering for a given frame.  It
//               should do whatever setup is required, and return true
//               if the frame should be rendered, or false if it
//               should be skipped.
////////////////////////////////////////////////////////////////////
bool AndroidGraphicsWindow::
begin_frame(FrameMode mode, Thread *current_thread) {
  //cout << "begin_frame" << endl;

  PStatTimer timer(_make_current_pcollector, current_thread);

  begin_frame_spam(mode);
  if (_gsg == (GraphicsStateGuardian *)NULL) {
    return false;
  }

  AndroidGraphicsStateGuardian *androidgsg;
  DCAST_INTO_R(androidgsg, _gsg, false);
  {
    if (eglGetCurrentDisplay() == _egl_display &&
        eglGetCurrentSurface(EGL_READ) == _egl_surface &&
        eglGetCurrentSurface(EGL_DRAW) == _egl_surface &&
        eglGetCurrentContext() == androidgsg->_context) {
      // No need to make the context current again.  Short-circuit
      // this possibly-expensive call.
    } else {
      // Need to set the context.
      if (!eglMakeCurrent(_egl_display, _egl_surface, _egl_surface, androidgsg->_context)) {
        androidegldisplay_cat.error() << "Failed to call eglMakeCurrent: "
          << get_egl_error_string(eglGetError()) << "\n";
      }
    }
  }

  // Now that we have made the context current to a window, we can
  // reset the GSG state if this is the first time it has been used.
  // (We can't just call reset() when we construct the GSG, because
  // reset() requires having a current context.)
  androidgsg->reset_if_new();

  if (mode == FM_render) {
    // begin_render_texture();
    clear_cube_map_selection();
  }

  _gsg->set_current_properties(&get_fb_properties());
  return _gsg->begin_frame(current_thread);
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::end_frame
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               after rendering is completed for a given frame.  It
//               should do whatever finalization is required.
////////////////////////////////////////////////////////////////////
void AndroidGraphicsWindow::
end_frame(FrameMode mode, Thread *current_thread) {
  //cout << "end_frame" << endl;
  end_frame_spam(mode);
  nassertv(_gsg != (GraphicsStateGuardian *)NULL);

  if (mode == FM_render) {
    // end_render_texture();
    copy_to_textures();
  }

  _gsg->end_frame(current_thread);

  if (mode == FM_render) {
    trigger_flip();
    clear_cube_map_selection();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::end_flip
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               after begin_flip() has been called on all windows, to
//               finish the exchange of the front and back buffers.
//
//               This should cause the window to wait for the flip, if
//               necessary.
////////////////////////////////////////////////////////////////////
void AndroidGraphicsWindow::
end_flip() {
  //cout << "end_flip" << endl;
  if (_gsg != (GraphicsStateGuardian *)NULL && _flip_ready) {

    // It doesn't appear to be necessary to ensure the graphics
    // context is current before flipping the windows, and insisting
    // on doing so can be a significant performance hit.

    //make_current();

    eglSwapBuffers(_egl_display, _egl_surface);
  }
  GraphicsWindow::end_flip();
}


////////////////////////////////////////////////////////////////////
//     Function: eglGraphicsWindow::process_events
//       Access: Public, Virtual
//  Description: Do whatever processing is necessary to ensure that
//               the window responds to user events.  Also, honor any
//               requests recently made via request_properties()
//
//               This function is called only within the window
//               thread.
////////////////////////////////////////////////////////////////////
void AndroidGraphicsWindow::
process_events() {
	GraphicsWindow::process_events();
//
//	nout << "PROPERTIES: " << endl;
//	nout << _properties.get_x_size() << endl;
//	nout << _properties.get_y_size() << endl;
//
//	AndroidGraphicsPipe *android_pipe;
//	DCAST_INTO_V(android_pipe, _pipe);
////
//	int win_width = ANativeWindow_getWidth(android_pipe->get_android_app()->window);
//	int win_height = ANativeWindow_getHeight(android_pipe->get_android_app()->window);
//	_properties.set_size(win_width, win_height);
//	system_changed_size(win_width, win_height);
//	system_changed_properties(_properties);
//
//	int w, h;
//    eglQuerySurface(_egl_display, _egl_surface, EGL_WIDTH, &w);
//    eglQuerySurface(_egl_display, _egl_surface, EGL_HEIGHT, &h);
//
//    EGLint format;
//    eglGetConfigAttrib(_egl_display, DCAST(AndroidGraphicsStateGuardian, _gsg)->_fbconfig, EGL_NATIVE_VISUAL_ID, &format);
//
//    ANativeWindow_setBuffersGeometry(android_pipe->get_android_app()->window, w, h, format);
//    nout << "EGL QUERY: " << w << " " << h << endl;
}
////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::set_properties_now
//       Access: Public, Virtual
//  Description: Applies the requested set of properties to the
//               window, if possible, for instance to request a change
//               in size or minimization status.
//
//               The window properties are applied immediately, rather
//               than waiting until the next frame.  This implies that
//               this method may *only* be called from within the
//               window thread.
//
//               The return value is true if the properties are set,
//               false if they are ignored.  This is mainly useful for
//               derived classes to implement extensions to this
//               function.
////////////////////////////////////////////////////////////////////
void AndroidGraphicsWindow::
set_properties_now(WindowProperties &properties) {
  if (_pipe == (GraphicsPipe *)NULL) {
    // If the pipe is null, we're probably closing down.
    GraphicsWindow::set_properties_now(properties);
    return;
  }

  AndroidGraphicsPipe *android_pipe;
  DCAST_INTO_V(android_pipe, _pipe);

  // Fullscreen mode is implemented with a hint to the window manager.
  // However, we also implicitly set the origin to (0, 0) and the size
  // to the desktop size, and request undecorated mode, in case the
  // user has a less-capable window manager (or no window manager at
  // all).
  if (properties.get_fullscreen()) {
    properties.set_undecorated(true);
    properties.set_origin(0, 0);

    int win_width = ANativeWindow_getWidth(android_pipe->get_android_app()->window);
    int win_height = ANativeWindow_getHeight(android_pipe->get_android_app()->window);

    //    properties.set_size(android_pipe->get_display_width(),
    //                        android_pipe->get_display_height());
    properties.set_size(win_width, win_height);
  }

  GraphicsWindow::set_properties_now(properties);
  if (!properties.is_any_specified()) {
    // The base class has already handled this case.
    return;
  }

  // The window is already open; we are limited to what we can change
  // on the fly.

  // We'll pass some property requests on as a window manager hint.
  WindowProperties wm_properties = _properties;
  wm_properties.add_properties(properties);

  // The window title may be changed by issuing another hint request.
  // Assume this will be honored.
  if (properties.has_title()) {
    _properties.set_title(properties.get_title());
    properties.clear_title();
  }

  // Ditto for fullscreen mode.
  if (properties.has_fullscreen()) {
    _properties.set_fullscreen(properties.get_fullscreen());
    properties.clear_fullscreen();
  }

}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::close_window
//       Access: Protected, Virtual
//  Description: Closes the window right now.  Called from the window
//               thread.
////////////////////////////////////////////////////////////////////
void AndroidGraphicsWindow::
close_window() {
  if (_gsg != (GraphicsStateGuardian *)NULL) {
    if (!eglMakeCurrent(_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
      androidegldisplay_cat.error() << "Failed to call eglMakeCurrent: "
        << get_egl_error_string(eglGetError()) << "\n";
    }
    _gsg.clear();
  }

  if (_egl_surface != 0) {
    if (!eglDestroySurface(_egl_display, _egl_surface)) {
      androidegldisplay_cat.error() << "Failed to destroy surface: "
        << get_egl_error_string(eglGetError()) << "\n";
    }
  }

  GraphicsWindow::close_window();
}

////////////////////////////////////////////////////////////////////
//     Function: AndroidGraphicsWindow::open_window
//       Access: Protected, Virtual
//  Description: Opens the window right now.  Called from the window
//               thread.  Returns true if the window is successfully
//               opened, or false if there was a problem.
////////////////////////////////////////////////////////////////////
bool AndroidGraphicsWindow::
open_window() {
  cout << "Opening Window" << endl;

  AndroidGraphicsPipe *android_pipe;
  DCAST_INTO_R(android_pipe, _pipe, false);

  // GSG Creation/Initialization
  AndroidGraphicsStateGuardian *androidgsg;
  if (_gsg == 0) {
    // There is no old gsg.  Create a new one.
	nout << "There is no old gsg.  Create a new one." << endl;
    androidgsg = new AndroidGraphicsStateGuardian(_engine, _pipe, NULL);
    androidgsg->choose_pixel_format(_fb_properties, false, false);
    _gsg = androidgsg;
  } else {
    // If the old gsg has the wrong pixel format, create a
    // new one that shares with the old gsg.
    DCAST_INTO_R(androidgsg, _gsg, false);
    if (!androidgsg->get_fb_properties().subsumes(_fb_properties)) {
      androidgsg = new AndroidGraphicsStateGuardian(_engine, _pipe, androidgsg);
      androidgsg->choose_pixel_format(_fb_properties, false, false);
      _gsg = androidgsg;
    }
  }

  /*
   * Here specify the attributes of the desired configuration.
   * Below, we select an EGLConfig with at least 8 bits per color
   * component compatible with on-screen windows
   */
  const EGLint attribs[] = {
	      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // <--- OpenGL ES 2.0
          EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
          EGL_BLUE_SIZE, 8,
          EGL_GREEN_SIZE, 8,
          EGL_RED_SIZE, 8,
          EGL_NONE
  };
  EGLint dummy, format;
  EGLint numConfigs;


  //_egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  //eglInitialize(android_pipe->_egl_display, 0, 0);
  eglChooseConfig(_egl_display, attribs, &androidgsg->_fbconfig, 1, &numConfigs);
  eglGetConfigAttrib(_egl_display, androidgsg->_fbconfig, EGL_NATIVE_VISUAL_ID, &format);

  androidgsg->_egl_display = _egl_display;

#ifdef OPENGLES_2
  EGLint contextAttrs[] = {
       EGL_CONTEXT_CLIENT_VERSION, 2,
       EGL_NONE
  };
  androidgsg->_context = eglCreateContext(_egl_display, androidgsg->_fbconfig, NULL, contextAttrs);
#else
  androidgsg->_context = eglCreateContext(_egl_display, androidgsg->_fbconfig, NULL, NULL);
#endif


  ANativeWindow_setBuffersGeometry(android_pipe->get_android_app()->window, 0, 0, format);
  _egl_surface = eglCreateWindowSurface(_egl_display, androidgsg->_fbconfig, android_pipe->get_android_app()->window, NULL);

  if (eglMakeCurrent(_egl_display, _egl_surface, _egl_surface, androidgsg->_context) == EGL_FALSE) {
      nout << "Unable to eglMakeCurrent" << endl;
      return false;
  }

  nout << "Perguntando EGL_WIDTH e EGL_HEIGHT: " << endl;
  EGLint w, h;
  eglQuerySurface(_egl_display, _egl_surface, EGL_WIDTH, &w);
  eglQuerySurface(_egl_display, _egl_surface, EGL_HEIGHT, &h);
  nout << "Width: " << w << endl;
  nout << "Height: " << h << endl;

//  if (!_properties.has_origin()) {
//    _properties.set_origin(0, 0);
//  }
//  if (!_properties.has_size()) {
//    _properties.set_size(100, 100);
//  }

//  /* Here, the application chooses the configuration it desires. In this
//   * sample, we have a very simplified selection process, where we pick
//   * the first EGLConfig that matches our criteria */
//  eglChooseConfig(display, attribs, &config, 1, &numConfigs);
//
//  /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
//   * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
//   * As soon as we picked a EGLConfig, we can safely reconfigure the
//   * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
//  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
//  ANativeWindow_setBuffersGeometry(android_pipe->_app->window, 0, 0, format);

  // initialize OpenGL ES and EGL

//  AndroidGraphicsPipe *android_pipe = (AndroidGraphicsPipe *) get_pipe();
  //DCAST_INTO_R(android_pipe, android_pipe, NULL);



















//  /*
//   * Here specify the attributes of the desired configuration.
//   * Below, we select an EGLConfig with at least 8 bits per color
//   * component compatible with on-screen windows
//   */
//  const EGLint attribs[] = {
//          EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
//          EGL_BLUE_SIZE, 8,
//          EGL_GREEN_SIZE, 8,
//          EGL_RED_SIZE, 8,
//          EGL_NONE
//  };
//  EGLint w, h, dummy, format;
//  EGLint numConfigs;
//  EGLConfig _fbconfig;
//  EGLSurface surface;
//  EGLContext context;
//
//  //  /* Here, the application chooses the configuration it desires. In this
//  //   * sample, we have a very simplified selection process, where we pick
//  //   * the first EGLConfig that matches our criteria */
//  eglChooseConfig(_egl_display, attribs, &_fbconfig, 1, &numConfigs);
//  //
//  //  /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
//  //   * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
//  //   * As soon as we picked a EGLConfig, we can safely reconfigure the
//  //   * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
//
//  eglGetConfigAttrib(_egl_display, _fbconfig, EGL_NATIVE_VISUAL_ID, &format);
//  ANativeWindow_setBuffersGeometry(android_pipe->get_android_app()->window, 0, 0, format);
//
//  _egl_surface = eglCreateWindowSurface(_egl_display, androidgsg->_fbconfig, android_pipe->_app->window, NULL);
//  if (eglGetError() != EGL_SUCCESS) {
//    androidegldisplay_cat.error()
//      << "Failed to create window surface.\n";
//    return false;
//  }
//
//  androidgsg->_context = eglCreateContext(_egl_display, androidgsg->_fbconfig, NULL, NULL);
//  if (!androidgsg->_context) {
//    androidegldisplay_cat.error() << "Failed to call eglMakeCurrent: "
//      << get_egl_error_string(eglGetError()) << "\n";
//  }
//  else {
//    cout << "Context created successfully!" << endl;
//  }














//  androidgsg->reset_if_new();
//  if (!androidgsg->is_valid()) {
//    close_window();
//    return false;
//  }
//  if (!androidgsg->get_fb_properties().verify_hardware_software
//      (_fb_properties, androidgsg->get_gl_renderer())) {
//    close_window();
//    return false;
//  }
//  _fb_properties = androidgsg->get_fb_properties();


  return true;
}
