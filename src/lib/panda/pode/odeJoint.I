// Filename: odeJoint.I
// Created by:  joswilso (27Dec06)
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
//     Function: OdeJoint::is_empty
//       Access: Published
//  Description: Returns true if the ID is 0, meaning the OdeJoint
//               does not point to a valid joint. It is an error to
//               call a method on an empty joint.
//               Note that an empty OdeJoint also evaluates to False.
////////////////////////////////////////////////////////////////////
INLINE bool OdeJoint::
is_empty() const {
  return (_id == 0);
}

////////////////////////////////////////////////////////////////////
//     Function: OdeJoint::get_id
//       Access: Published
//  Description: Returns the underlying dJointID.
////////////////////////////////////////////////////////////////////
INLINE dJointID OdeJoint::
get_id() const {
  return _id;
}

/*
INLINE void OdeJoint::
set_data(void *data) {
  dJointSetData(_id, data);
}

INLINE void *OdeJoint::
get_data() {
  return dJointGetData(_id);
}
*/

INLINE int OdeJoint::
get_joint_type() const {
  return dJointGetType(_id);
}

INLINE void OdeJoint::
set_feedback(OdeJointFeedback *feedback) {
  if (_feedback != NULL) {
    delete _feedback;
  }
  
  _feedback = feedback;
  
  if (_feedback != NULL) {
    dJointSetFeedback(_id, _feedback->get_feedback_ptr());
  }
  else {
    dJointSetFeedback(_id, NULL);
  }
}

INLINE void OdeJoint::
set_feedback(bool flag) {
  if (flag) {
    if (_feedback != NULL) {
      return;
    }
    _feedback = new OdeJointFeedback;
    dJointSetFeedback(_id, _feedback->get_feedback_ptr());
  } else if (_feedback != NULL) {
    dJointSetFeedback(_id, NULL);
    delete _feedback;
    _feedback = NULL;
  }
}

INLINE OdeJointFeedback *OdeJoint::
get_feedback() const {
  return _feedback;
}

INLINE int OdeJoint::
compare_to(const OdeJoint &other) const {
  if (_id != other._id) {
    return _id < other._id ? -1 : 1;
  }
  return 0;
}

INLINE bool OdeJoint::
operator == (const OdeJoint &other) const {
  return _id == other._id;
}
