// Filename: pandaFileStream.I
// Created by:  drose (08Sep08)
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


////////////////////////////////////////////////////////////////////
//     Function: IFileStream::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE IFileStream::
IFileStream() : istream(&_buf) {
}

////////////////////////////////////////////////////////////////////
//     Function: IFileStream::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE IFileStream::
IFileStream(const char *filename, ios::openmode mode) : istream(&_buf) {
  open(filename, mode);
}

////////////////////////////////////////////////////////////////////
//     Function: IFileStream::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE IFileStream::
~IFileStream() {
  close();
}

////////////////////////////////////////////////////////////////////
//     Function: IFileStream::open
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void IFileStream::
open(const char *filename, ios::openmode mode) {
  clear((ios_iostate)0);
  _buf.open(filename, mode);
  if (!_buf.is_open()) {
    clear(ios::failbit);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: IFileStream::close
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void IFileStream::
close() {
  _buf.close();
}

////////////////////////////////////////////////////////////////////
//     Function: OFileStream::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE OFileStream::
OFileStream() : ostream(&_buf) {
}

////////////////////////////////////////////////////////////////////
//     Function: OFileStream::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE OFileStream::
OFileStream(const char *filename, ios::openmode mode) : ostream(&_buf) {
  open(filename, mode);
}

////////////////////////////////////////////////////////////////////
//     Function: OFileStream::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE OFileStream::
~OFileStream() {
  close();
}

////////////////////////////////////////////////////////////////////
//     Function: OFileStream::open
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void OFileStream::
open(const char *filename, ios::openmode mode) {
  clear((ios_iostate)0);
  _buf.open(filename, mode);
  if (!_buf.is_open()) {
    clear(ios::failbit);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: OFileStream::close
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void OFileStream::
close() {
  _buf.close();
}

////////////////////////////////////////////////////////////////////
//     Function: FileStream::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE FileStream::
FileStream() : iostream(&_buf) {
}

////////////////////////////////////////////////////////////////////
//     Function: FileStream::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE FileStream::
FileStream(const char *filename, ios::openmode mode) : iostream(&_buf) {
  open(filename, mode);
}

////////////////////////////////////////////////////////////////////
//     Function: FileStream::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE FileStream::
~FileStream() {
  close();
}

////////////////////////////////////////////////////////////////////
//     Function: FileStream::open
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void FileStream::
open(const char *filename, ios::openmode mode) {
  clear((ios_iostate)0);
  _buf.open(filename, mode);
  if (!_buf.is_open()) {
    clear(ios::failbit);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: FileStream::close
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void FileStream::
close() {
  _buf.close();
}
