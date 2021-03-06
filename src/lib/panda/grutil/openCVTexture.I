// Filename: openCVTexture.I
// Created by:  zacpavlov (19Aug05)
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
//     Function: OpenCVTexture::VideoStream::is_valid
//       Access: Public
//  Description: Returns true if this stream is open and ready, false
//               otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool OpenCVTexture::VideoStream::
is_valid() const {
  return (_capture != NULL);
}

////////////////////////////////////////////////////////////////////
//     Function: OpenCVTexture::VideoStream::is_from_file
//       Access: Public
//  Description: Returns true if this stream takes its input from a
//               video file, false otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool OpenCVTexture::VideoStream::
is_from_file() const {
  return !_filename.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: OpenCVTexture::VideoPage::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE OpenCVTexture::VideoPage::
VideoPage() {
}

////////////////////////////////////////////////////////////////////
//     Function: OpenCVTexture::VideoPage::Copy Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE OpenCVTexture::VideoPage::
VideoPage(const OpenCVTexture::VideoPage &copy) :
  _color(copy._color),
  _alpha(copy._alpha)
{
}

////////////////////////////////////////////////////////////////////
//     Function: OpenCVTexture::VideoPage::Destructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE OpenCVTexture::VideoPage::
~VideoPage() {
}

////////////////////////////////////////////////////////////////////
//     Function: OpenCVTexture::consider_update
//       Access: Protected
//  Description: Calls update_frame() if the current frame has
//               changed.
////////////////////////////////////////////////////////////////////
INLINE void OpenCVTexture::
consider_update() {
  int this_frame = ClockObject::get_global_clock()->get_frame_count();
  if (this_frame != _last_frame_update) {
    int frame = get_frame();
    if (_current_frame != frame) {
      update_frame(frame);
      _current_frame = frame;
    } else {
      // Loop through the pages to see if there's any camera stream to update.
      int max_z = max(_z_size, (int)_pages.size());
      for (int z = 0; z < max_z; ++z) {
        VideoPage &page = _pages[z];
        if (!page._color.is_from_file() || !page._alpha.is_from_file()) {
          update_frame(frame, z);
        }
      }
    }
    _last_frame_update = this_frame;
  }
}

