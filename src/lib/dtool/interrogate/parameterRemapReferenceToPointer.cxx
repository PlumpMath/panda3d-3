// Filename: parameterRemapReferenceToPointer.cxx
// Created by:  drose (01Aug00)
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

#include "parameterRemapReferenceToPointer.h"
#include "typeManager.h"

#include "cppType.h"
#include "cppDeclaration.h"
#include "cppConstType.h"
#include "cppPointerType.h"
#include "cppReferenceType.h"

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapReferenceToPointer::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ParameterRemapReferenceToPointer::
ParameterRemapReferenceToPointer(CPPType *orig_type) :
  ParameterRemap(orig_type)
{
  _new_type = TypeManager::wrap_pointer(TypeManager::unwrap_reference(orig_type));
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapReferenceToPointer::pass_parameter
//       Access: Public, Virtual
//  Description: Outputs an expression that converts the indicated
//               variable from the new type to the original type, for
//               passing into the actual C++ function.
////////////////////////////////////////////////////////////////////
void ParameterRemapReferenceToPointer::
pass_parameter(ostream &out, const string &variable_name) {
  out << "*" << variable_name;
}

////////////////////////////////////////////////////////////////////
//     Function: ParameterRemapReferenceToPointer::get_return_expr
//       Access: Public, Virtual
//  Description: Returns an expression that evalutes to the
//               appropriate value type for returning from the
//               function, given an expression of the original type.
////////////////////////////////////////////////////////////////////
string ParameterRemapReferenceToPointer::
get_return_expr(const string &expression) {
  return "&(" + expression + ")";
}
