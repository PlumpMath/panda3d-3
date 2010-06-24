// Filename: shaderInput.I
// Created by: jyelon (01Sep05)
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
//     Function: ShaderInput::Destructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE ShaderInput::
~ShaderInput()
{
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE ShaderInput::
ShaderInput(InternalName *name, int priority) :
  _name(name),
  _type(M_invalid),
  _priority(priority),
  _stored_texture(NULL),
  _stored_nodepath(NodePath()),
  _stored_vector(LVector4f(0,0,0,1))
{
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE ShaderInput::
ShaderInput(InternalName *name, Texture *tex, int priority) :
  _name(name),
  _type(M_texture),
  _priority(priority),
  _stored_texture(tex),
  _stored_nodepath(NodePath()),
  _stored_vector(LVector4f(0,0,0,1))
{
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE ShaderInput::
ShaderInput(InternalName *name, const NodePath &np, int priority) :
  _name(name),
  _type(M_nodepath),
  _priority(priority),
  _stored_texture(NULL),
  _stored_nodepath(np),
  _stored_vector(LVector4f(0,0,0,1))
{
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::Constructor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE ShaderInput::
ShaderInput(InternalName *name, const LVector4f &vec, int priority) :
  _name(name),
  _type(M_vector),
  _priority(priority),
  _stored_texture(NULL),
  _stored_nodepath(NodePath()),
  _stored_vector(vec)
{
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::get_name
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE InternalName *ShaderInput::
get_name() const {
  return _name;
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::get_value_type
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE int ShaderInput::
get_value_type() const {
  return _type;
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::get_priority
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE int ShaderInput::
get_priority() const {
  return _priority;
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::get_texture
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE Texture *ShaderInput::
get_texture() const {
  return _stored_texture;
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::get_nodepath
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE const NodePath &ShaderInput::
get_nodepath() const {
  return _stored_nodepath;
}

////////////////////////////////////////////////////////////////////
//     Function: ShaderInput::get_vector
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE const LVector4f &ShaderInput::
get_vector() const {
  return _stored_vector;
}
