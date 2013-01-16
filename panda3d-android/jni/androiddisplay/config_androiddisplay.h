// Filename: config_androiddisplay.h
// Created by:  cary (29Apr12)
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

#ifndef CONFIG_ANDROIDDISPLAY_H
#define CONFIG_ANDROIDDISPLAY_H

#include "pandabase.h"
#include "notifyCategoryProxy.h"
#include "configVariableString.h"
#include "configVariableBool.h"
#include "configVariableInt.h"

// No windows yet. Fix. Name the module accordingly... ANDROIDES2DISPLAY
#define EXPCL_ANDROIDDISPLAY
#define EXPTP_ANDROIDDISPLAY

#if defined(OPENGLES_1) && defined(OPENGLES_2)
  #error OPENGLES_1 and OPENGLES_2 cannot be defined at the same time!
#endif
#if !defined(OPENGLES_1) && !defined(OPENGLES_2)
  #error Either OPENGLES_1 or OPENGLES_2 must be defined when compiling androiddisplay!
#endif

//#ifdef OPENGLES_2
//  NotifyCategoryDecl(androiddisplay, EXPCL_ANDROIDDISPLAY, EXPTP_ANDROIDDISPLAY);
//
//  extern EXPCL_ANDROIDDISPLAY void init_libandroiddisplay();
//  extern EXPCL_ANDROIDDISPLAY const string get_egl_error_string(int error);
//#else
  NotifyCategoryDecl(androidegldisplay, EXPCL_ANDROIDDISPLAY, EXPCL_ANDROIDDISPLAY);

  extern EXPCL_ANDROIDDISPLAY void init_libandroiddisplay();
  extern EXPCL_ANDROIDDISPLAY const string get_egl_error_string(int error);
//#endif


#endif /* CONFIG_ANDROIDDISPLAY_H */
