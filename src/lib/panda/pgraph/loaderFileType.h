// Filename: loaderFileType.h
// Created by:  drose (20Jun00)
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

#ifndef LOADERFILETYPE_H
#define LOADERFILETYPE_H

#include "pandabase.h"

#include "typedObject.h"
#include "filename.h"
#include "pandaNode.h"
#include "pointerTo.h"
#include "dSearchPath.h"

class LoaderOptions;
class BamCacheRecord;

////////////////////////////////////////////////////////////////////
//       Class : LoaderFileType
// Description : This is the base class for a family of scene-graph
//               file types that the Loader supports.  Each kind of
//               loader that's available should define a corresponding
//               LoaderFileType object and register itself.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH LoaderFileType : public TypedObject {
protected:
  LoaderFileType();

public:
  virtual ~LoaderFileType();

PUBLISHED:
  virtual string get_name() const=0;
  virtual string get_extension() const=0;
  virtual string get_additional_extensions() const;
  virtual bool supports_compressed() const;

  virtual bool get_allow_disk_cache(const LoaderOptions &options) const;
  virtual bool get_allow_ram_cache(const LoaderOptions &options) const;

public:
  virtual PT(PandaNode) load_file(const Filename &path, const LoaderOptions &options,
                                  BamCacheRecord *record) const;

protected:
  int _no_cache_flags;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedObject::init_type();
    register_type(_type_handle, "LoaderFileType",
                  TypedObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

