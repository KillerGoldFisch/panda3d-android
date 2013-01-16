// Filename: androidGraphicsWindow.h
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

#ifndef ANDROIDGRAPHICSWINDOW_H
#define ANDROIDGRAPHICSWINDOW_H

#include "pandabase.h"

#include "androidGraphicsPipe.h"
#include "graphicsWindow.h"
#include "buttonHandle.h"

////////////////////////////////////////////////////////////////////
//       Class : AndroidGraphicsWindow
// Description : An interface to the egl system for managing GLES
//               windows under Android.
////////////////////////////////////////////////////////////////////
class AndroidGraphicsWindow : public GraphicsWindow {
public:
  AndroidGraphicsWindow(GraphicsEngine *engine, GraphicsPipe *pipe,
                    const string &name,
                    const FrameBufferProperties &fb_prop,
                    const WindowProperties &win_prop,
                    int flags,
                    GraphicsStateGuardian *gsg,
                    GraphicsOutput *host);
  virtual ~AndroidGraphicsWindow();

  virtual bool begin_frame(FrameMode mode, Thread *current_thread);
  virtual void end_frame(FrameMode mode, Thread *current_thread);
  virtual void end_flip();

  virtual void set_properties_now(WindowProperties &properties);

  virtual void process_events();
protected:
  virtual void close_window();
  virtual bool open_window();

private:
  EGLDisplay _egl_display;
  EGLSurface _egl_surface;
  //PT(AndroidGraphicsPipe) _android_pipe;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    GraphicsWindow::init_type();
    register_type(_type_handle, "androidGraphicsWindow",
                  GraphicsWindow::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif
