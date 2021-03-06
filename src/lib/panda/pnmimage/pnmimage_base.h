// Filename: pnmimage_base.h
// Created by:  drose (14Jun00)
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

#ifndef PNMIMAGE_BASE_H
#define PNMIMAGE_BASE_H

// This header file make a few typedefs and other definitions
// essential to everything in the PNMImage package.

#include "pandabase.h"
#include "pnotify.h"

// Since we no longer include pnm.h directly, we have to provide our
// own definitions for xel and xelval.

// For now, we have PGM_BIGGRAYS defined, which gives us 16-bit
// channels.  Undefine this if you have memory problems and need to
// use 8-bit channels instead.
#define PGM_BIGGRAYS

#ifdef PGM_BIGGRAYS
typedef unsigned short gray;
#define PGM_MAXMAXVAL 65535
#else  // PGM_BIGGRAYS
typedef unsigned char gray;
#define PGM_MAXMAXVAL 255
#endif  // PGM_BIGGRAYS

#define PNM_MAXMAXVAL PGM_MAXMAXVAL

struct pixel {
PUBLISHED:
  pixel() { }
  pixel(gray r, gray g, gray b) : r(r), g(g), b(b) { }
  static int size() { return 3; }
  gray operator [](int i) const { nassertr(i >= 0 && i < 3, 0); return *(&r + i); }
  gray &operator [](int i) { nassertr(i >= 0 && i < 3, r); return *(&r + i); }
#ifdef HAVE_PYTHON
  void __setitem__(int i, gray v) { operator[](i) = v; }
#endif

  gray r, g, b;
};

typedef gray pixval;
typedef pixel xel;
typedef gray xelval;

// These macros are borrowed from ppm.h.

#define PPM_GETR(p) ((p).r)
#define PPM_GETG(p) ((p).g)
#define PPM_GETB(p) ((p).b)

#define PPM_PUTR(p,red) ((p).r = (red))
#define PPM_PUTG(p,grn) ((p).g = (grn))
#define PPM_PUTB(p,blu) ((p).b = (blu))

#define PPM_ASSIGN(p,red,grn,blu) { (p).r = (red); (p).g = (grn); (p).b = (blu); }
#define PPM_EQUAL(p,q) ( (p).r == (q).r && (p).g == (q).g && (p).b == (q).b )
#define PNM_ASSIGN1(x,v) PPM_ASSIGN(x,0,0,v)

#define PPM_DEPTH(newp,p,oldmaxval,newmaxval) \
    PPM_ASSIGN( (newp), \
        ( (int) PPM_GETR(p) * (newmaxval) + (oldmaxval) / 2 ) / (oldmaxval), \
        ( (int) PPM_GETG(p) * (newmaxval) + (oldmaxval) / 2 ) / (oldmaxval), \
        ( (int) PPM_GETB(p) * (newmaxval) + (oldmaxval) / 2 ) / (oldmaxval) )


// pnm defines these functions, and it's easier to emulate them than
// to rewrite the code that calls them.
EXPCL_PANDA_PNMIMAGE void pm_message(const char *format, ...);
EXPCL_PANDA_PNMIMAGE void pm_error(const char *format, ...);  // doesn't return.

EXPCL_PANDA_PNMIMAGE int pm_maxvaltobits(int maxval);
EXPCL_PANDA_PNMIMAGE int pm_bitstomaxval(int bits);

EXPCL_PANDA_PNMIMAGE char *pm_allocrow(int cols, int size);
EXPCL_PANDA_PNMIMAGE void pm_freerow(char *itrow);

EXPCL_PANDA_PNMIMAGE int pm_readbigshort(istream *in, short *sP);
EXPCL_PANDA_PNMIMAGE int pm_writebigshort(ostream *out, short s);
EXPCL_PANDA_PNMIMAGE int pm_readbiglong(istream *in, long *lP);
EXPCL_PANDA_PNMIMAGE int pm_writebiglong(ostream *out, long l);
EXPCL_PANDA_PNMIMAGE int pm_readlittleshort(istream *in, short *sP);
EXPCL_PANDA_PNMIMAGE int pm_writelittleshort(ostream *out, short s);
EXPCL_PANDA_PNMIMAGE int pm_readlittlelong(istream *in, long *lP);
EXPCL_PANDA_PNMIMAGE int pm_writelittlelong(ostream *out, long l);


// These ratios are used to compute the brightness of a colored pixel; they
// define the relative contributions of each of the components.
static const double lumin_red = 0.299;
static const double lumin_grn = 0.587;
static const double lumin_blu = 0.114;


#endif
