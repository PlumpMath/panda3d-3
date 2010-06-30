// Filename: glxGraphicsPipe.h
// Created by:  mike (09Jan97)
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

#ifndef GLXGRAPHICSPIPE_H
#define GLXGRAPHICSPIPE_H

#include "pandabase.h"
#include "graphicsWindow.h"
#include "graphicsPipe.h"
#include "glgsg.h"
#include "lightMutex.h"
#include "lightReMutex.h"

class FrameBufferProperties;

#ifdef CPPPARSER
// A simple hack so interrogate can parse this file.
typedef int Display;
typedef int Window;
typedef int XErrorEvent;
typedef int XVisualInfo;
typedef int Atom;
typedef int Cursor;
typedef int XIM;
typedef int XIC;
#else
#include <X11/Xlib.h>
#include <GL/glx.h>

#if defined(GLX_VERSION_1_3)
  // If the system glx version is at least 1.3, then we know we have
  // GLXFBConfig and GLXPbuffer.
  #define HAVE_GLXFBCONFIG
  #define HAVE_OFFICIAL_GLXFBCONFIG
#endif

// This must be included after we have included glgsg.h (which
// includes gl.h), and after we have checked GLX_VERSION_1_3.  But we
// must also include it before we redefine the GLXFBConfig types,
// below.
#include <GL/glext.h>

// drose: the version of GL/glx.h that ships with Fedora Core 2 seems
// to define GLX_VERSION_1_4, but for some reason does not define
// GLX_SAMPLE_BUFFERS or GLX_SAMPLES.  We work around that here.

#ifndef GLX_SAMPLE_BUFFERS
#define GLX_SAMPLE_BUFFERS                 100000
#endif
#ifndef GLX_SAMPLES
#define GLX_SAMPLES                        100001
#endif


#if !defined(HAVE_GLXFBCONFIG) && defined(GLX_SGIX_fbconfig) && defined(GLX_SGIX_pbuffer)
  // If the system glx version isn't 1.3, but these were defined as
  // extensions, we can work with that.
  #define GLXFBConfig GLXFBConfigSGIX
  #define GLXPbuffer GLXPbufferSGIX
  #define glXChooseFBConfig glXChooseFBConfigSGIX
  #define glXCreateNewContext glXCreateContextWithConfigSGIX
  #define glXGetVisualFromFBConfig glXGetVisualFromFBConfigSGIX
  #define glXGetFBConfigAttrib glXGetFBConfigAttribSGIX
  #define glXDestroyPbuffer glXDestroyGLXPbufferSGIX

  #define HAVE_GLXFBCONFIG
  #define HAVE_SGI_GLXFBCONFIG
#endif

#endif  // CPPPARSER

////////////////////////////////////////////////////////////////////
//       Class : glxGraphicsPipe
// Description : This graphics pipe represents the interface for
//               creating OpenGL graphics windows on an X-based
//               (e.g. Unix) client.
////////////////////////////////////////////////////////////////////
class glxGraphicsPipe : public GraphicsPipe {
public:
  glxGraphicsPipe(const string &display = string());
  virtual ~glxGraphicsPipe();

  virtual string get_interface_name() const;
  static PT(GraphicsPipe) pipe_constructor();

  INLINE Display *get_display() const;
  INLINE int get_screen() const;
  INLINE Window get_root() const;
  INLINE XIM get_im() const;

  INLINE Cursor get_hidden_cursor();

public:
  virtual PreferredWindowThread get_preferred_window_thread() const;

public:
  // Atom specifications.
  Atom _wm_delete_window;
  Atom _net_wm_window_type;
  Atom _net_wm_window_type_splash;
  Atom _net_wm_window_type_fullscreen;
  Atom _net_wm_state;
  Atom _net_wm_state_fullscreen;
  Atom _net_wm_state_above;
  Atom _net_wm_state_below;
  Atom _net_wm_state_add;
  Atom _net_wm_state_remove;

protected:
  virtual PT(GraphicsOutput) make_output(const string &name,
                                         const FrameBufferProperties &fb_prop,
                                         const WindowProperties &win_prop,
                                         int flags,
                                         GraphicsEngine *engine,
                                         GraphicsStateGuardian *gsg,
                                         GraphicsOutput *host,
                                         int retry,
                                         bool &precertify);

private:
  void make_hidden_cursor();
  void release_hidden_cursor();

  static void install_error_handlers();
  static int error_handler(Display *display, XErrorEvent *error);
  static int io_error_handler(Display *display);

  Display *_display;
  int _screen;
  Window _root;
  XIM _im;

  Cursor _hidden_cursor;

  typedef int ErrorHandlerFunc(Display *, XErrorEvent *);
  typedef int IOErrorHandlerFunc(Display *);
  static bool _error_handlers_installed;
  static ErrorHandlerFunc *_prev_error_handler;
  static IOErrorHandlerFunc *_prev_io_error_handler;

public:
  // This Mutex protects any X library calls, which all have to be
  // single-threaded.  In particular, it protects glXMakeCurrent().
  static LightReMutex _x_mutex;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    GraphicsPipe::init_type();
    register_type(_type_handle, "glxGraphicsPipe",
                  GraphicsPipe::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "glxGraphicsPipe.I"

#endif