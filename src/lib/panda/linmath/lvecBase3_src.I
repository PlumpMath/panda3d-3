// Filename: lvecBase3_src.I
// Created by:  drose (08Mar00)
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
//     Function: LVecBase3::Default Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::
FLOATNAME(LVecBase3)() {
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Copy Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::
FLOATNAME(LVecBase3)(const FLOATNAME(LVecBase3) &copy) {
  TAU_PROFILE("LVecBase3::LVecBase3(LVecBase3 &)", " ", TAU_USER);
  _v.v._0 = copy._v.v._0;
  _v.v._1 = copy._v.v._1;
  _v.v._2 = copy._v.v._2;
//  (*this) = copy;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Copy Assignment Operator
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) &FLOATNAME(LVecBase3)::
operator = (const FLOATNAME(LVecBase3) &copy) {
  TAU_PROFILE("void LVecBase3::operator =(LVecBase3 &)", " ", TAU_USER);
  _v.v._0 = copy._v.v._0;
  _v.v._1 = copy._v.v._1;
  _v.v._2 = copy._v.v._2;
//  set(copy[0], copy[1], copy[2]);
  return *this;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Fill Assignment Operator
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) &FLOATNAME(LVecBase3)::
operator = (FLOATTYPE fill_value) {
  fill(fill_value);
  return *this;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::
FLOATNAME(LVecBase3)(FLOATTYPE fill_value) {
  fill(fill_value);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::
FLOATNAME(LVecBase3)(FLOATTYPE x, FLOATTYPE y, FLOATTYPE z) {
  TAU_PROFILE("void LVecBase3::operator = (FLOATTYPE, ...)", " ", TAU_USER);
  _v.v._0 = x;
  _v.v._1 = y;
  _v.v._2 = z;
//  set(x, y, z);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::zero Named Constructor
//       Access: Public
//  Description: Returns a zero-length vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH const FLOATNAME(LVecBase3) &FLOATNAME(LVecBase3)::
zero() {
  return _zero;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::unit_x Named Constructor
//       Access: Public
//  Description: Returns a unit X vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH const FLOATNAME(LVecBase3) &FLOATNAME(LVecBase3)::
unit_x() {
  return _unit_x;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::unit_y Named Constructor
//       Access: Public
//  Description: Returns a unit Y vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH const FLOATNAME(LVecBase3) &FLOATNAME(LVecBase3)::
unit_y() {
  return _unit_y;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::unit_z Named Constructor
//       Access: Public
//  Description: Returns a unit Z vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH const FLOATNAME(LVecBase3) &FLOATNAME(LVecBase3)::
unit_z() {
  return _unit_z;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::
~FLOATNAME(LVecBase3)() {
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Indexing Operator
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
operator [](int i) const {
  nassertr(i >= 0 && i < 3, 0.0);
  return _v.data[i];
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::Indexing Operator
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE &FLOATNAME(LVecBase3)::
operator [](int i) {
  nassertr(i >= 0 && i < 3, _v.data[0]);
  return _v.data[i];
}

#ifdef HAVE_PYTHON
////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::__setitem__
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
__setitem__(int i, FLOATTYPE v) {
  nassertv(i >= 0 && i < 3);
  _v.data[i] = v;
}
#endif  // HAVE_PYTHON

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::size
//       Access: Public, Static
//  Description: Returns 3: the number of components of a LVecBase3.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH int FLOATNAME(LVecBase3)::
size() {
  return 3;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::is_nan
//       Access: Public
//  Description: Returns true if any component of the vector is
//               not-a-number, false otherwise.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
is_nan() const {
  TAU_PROFILE("bool LVecBase3::is_nan()", " ", TAU_USER);
  return cnan(_v.v._0) || cnan(_v.v._1) || cnan(_v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_cell
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
get_cell(int i) const {
  nassertr(i >= 0 && i < 3, 0);
  return _v.data[i];
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_x
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
get_x() const {
  return _v.v._0;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_y
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
get_y() const {
  return _v.v._1;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_z
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
get_z() const {
  return _v.v._2;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::set_cell
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
set_cell(int i, FLOATTYPE value) {
  nassertv(i >= 0 && i < 3);
  _v.data[i] = value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::set_x
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
set_x(FLOATTYPE value) {
  _v.v._0 = value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::set_y
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
set_y(FLOATTYPE value) {
  _v.v._1 = value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::set_z
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
set_z(FLOATTYPE value) {
  _v.v._2 = value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_xy
//       Access: Public
//  Description: Returns a 2-component vector that shares just the
//               first two components of this vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase2) FLOATNAME(LVecBase3)::
get_xy() const {
  return FLOATNAME(LVecBase2)(_v.v._0, _v.v._1);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_xz
//       Access: Public
//  Description: Returns a 2-component vector that shares just the
//               first and last components of this vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase2) FLOATNAME(LVecBase3)::
get_xz() const {
  return FLOATNAME(LVecBase2)(_v.v._0, _v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_yz
//       Access: Public
//  Description: Returns a 2-component vector that shares just the
//               last two components of this vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase2) FLOATNAME(LVecBase3)::
get_yz() const {
  return FLOATNAME(LVecBase2)(_v.v._1, _v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::add_to_cell
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
add_to_cell(int i, FLOATTYPE value) {
  nassertv(i >= 0 && i < 3);
  _v.data[i] += value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::add_x
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
add_x(FLOATTYPE value) {
  _v.v._0 += value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::add_y
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
add_y(FLOATTYPE value) {
  _v.v._1 += value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::add_z
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
add_z(FLOATTYPE value) {
  _v.v._2 += value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_data
//       Access: Public
//  Description: Returns the address of the first of the three data
//               elements in the vector.  The remaining elements
//               occupy the next positions consecutively in memory.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH const FLOATTYPE *FLOATNAME(LVecBase3)::
get_data() const {
  return _v.data;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_num_components
//       Access: Public
//  Description: Returns the number of elements in the vector, three.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH int FLOATNAME(LVecBase3)::
get_num_components() const {
  return 3;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::begin
//       Access: Public
//  Description: Returns an iterator that may be used to traverse the
//               elements of the matrix, STL-style.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::iterator FLOATNAME(LVecBase3)::
begin() {
  return _v.data;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::end
//       Access: Public
//  Description: Returns an iterator that may be used to traverse the
//               elements of the matrix, STL-style.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::iterator FLOATNAME(LVecBase3)::
end() {
  return begin() + get_num_components();
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::begin
//       Access: Public
//  Description: Returns an iterator that may be used to traverse the
//               elements of the matrix, STL-style.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::const_iterator FLOATNAME(LVecBase3)::
begin() const {
  return _v.data;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::end
//       Access: Public
//  Description: Returns an iterator that may be used to traverse the
//               elements of the matrix, STL-style.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3)::const_iterator FLOATNAME(LVecBase3)::
end() const {
  return begin() + get_num_components();
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::fill
//       Access: Published
//  Description: Sets each element of the vector to the indicated
//               fill_value.  This is particularly useful for
//               initializing to zero.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
fill(FLOATTYPE fill_value) {
  TAU_PROFILE("void LVecBase3::fill()", " ", TAU_USER);
  _v.v._0 = fill_value;
  _v.v._1 = fill_value;
  _v.v._2 = fill_value;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::set
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
set(FLOATTYPE x, FLOATTYPE y, FLOATTYPE z) {
  TAU_PROFILE("void LVecBase3::set()", " ", TAU_USER);
  _v.v._0 = x;
  _v.v._1 = y;
  _v.v._2 = z;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::length
//       Access: Published
//  Description: Returns the length of the vector, by the Pythagorean
//               theorem.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
length() const {
  TAU_PROFILE("FLOATTYPE LVecBase3::length()", " ", TAU_USER);
  return csqrt((*this).dot(*this));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::length_squared
//       Access: Published
//  Description: Returns the square of the vector's length, cheap and
//               easy.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
length_squared() const {
  TAU_PROFILE("FLOATTYPE LVecBase3::length_squared()", " ", TAU_USER);
  return (*this).dot(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::normalize
//       Access: Published
//  Description: Normalizes the vector in place.  Returns true if the
//               vector was normalized, false if it was a zero-length
//               vector.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
normalize() {
  TAU_PROFILE("bool LVecBase3::normalize()", " ", TAU_USER);
  FLOATTYPE l2 = length_squared();
  if (l2 == (FLOATTYPE)0.0f) {
    set(0.0f, 0.0f, 0.0f);
    return false;

  } else if (!IS_THRESHOLD_EQUAL(l2, 1.0f, (NEARLY_ZERO(FLOATTYPE) * NEARLY_ZERO(FLOATTYPE)))) {
    (*this) /= csqrt(l2);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::dot
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATTYPE FLOATNAME(LVecBase3)::
dot(const FLOATNAME(LVecBase3) &other) const {
  TAU_PROFILE("FLOATTYPE LVecBase3::dot()", " ", TAU_USER);
  return _v.v._0 * other._v.v._0 + _v.v._1 * other._v.v._1 + _v.v._2 * other._v.v._2;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::cross
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
cross(const FLOATNAME(LVecBase3) &other) const {
  TAU_PROFILE("LVecBase3 LVecBase3::cross()", " ", TAU_USER);
  return FLOATNAME(LVecBase3)(_v.v._1 * other._v.v._2 - other._v.v._1 * _v.v._2,
                              other._v.v._0 * _v.v._2 - _v.v._0 * other._v.v._2,
                              _v.v._0 * other._v.v._1 - other._v.v._0 * _v.v._1);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::project
//       Access: Published
//  Description: Returns a new vector representing the projection of
//               this vector onto another one.  The resulting vector
//               will be a scalar multiple of onto.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
project(const FLOATNAME(LVecBase3) &onto) const {
  return onto * (dot(onto) / onto.length_squared());
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator <
//       Access: Published
//  Description: This performs a lexicographical comparison.  It's of
//               questionable mathematical meaning, but sometimes has
//               a practical purpose for sorting unique vectors,
//               especially in an STL container.  Also see
//               compare_to().
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
operator < (const FLOATNAME(LVecBase3) &other) const {
  TAU_PROFILE("bool LVecBase3::operator <(const LVecBase3 &)", " ", TAU_USER);
  return (compare_to(other) < 0);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator ==
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
operator == (const FLOATNAME(LVecBase3) &other) const {
  TAU_PROFILE("bool LVecBase3::operator ==(const LVecBase3 &)", " ", TAU_USER);
  return (_v.v._0 == other._v.v._0 &&
          _v.v._1 == other._v.v._1 &&
          _v.v._2 == other._v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator !=
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
operator != (const FLOATNAME(LVecBase3) &other) const {
  return !operator == (other);
}

////////////////////////////////////////////////////////////////////
//     Function: get_standardized_rotation
//       Access: file
//  Description: return value in the range -180.0 to 179.99999.
//     See Also: get_standardized_hpr
////////////////////////////////////////////////////////////////////
static INLINE_LINMATH FLOATTYPE
get_standardized_rotation(FLOATTYPE angle_in_degrees) {
  if (angle_in_degrees<0.0) {
    angle_in_degrees = FLOATCONST(360.0) - fmod(angle_in_degrees * FLOATCONST(-1.0), FLOATCONST(360.0));
  } else {
    angle_in_degrees = fmod(angle_in_degrees, FLOATCONST(360.0));
  }
  // This can be changed to return values in the range 0.0 to 359.99999
  // by skipping this next part and returning now.
  
  return (angle_in_degrees<FLOATCONST(180.0))?
      angle_in_degrees:
      angle_in_degrees - FLOATCONST(360.0);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_standardized_hpr
//       Access: Published
//  Description: Try to un-spin the hpr to a standard form.  Like
//               all standards, someone decides between many
//               arbitrary posible standards.  This function assumes
//               that 0 and 360 are the same, as is 720 and
//               -360.  Also 180 and -180 are the same.  Another
//               example is -90 and 270.
//               Each element will be in the range -180.0 to 179.99999.
//               The original usage of this function is for human
//               readable output.
//
//               It doesn't work so well for asserting that foo_hpr 
//               is roughly equal to bar_hpr.  Try using
//               LQuaternionf::is_same_direction() for that.
//     See Also: get_standardized_rotation,
//               LQuaternion::is_same_direction
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
get_standardized_hpr() const {
  return FLOATNAME(LVecBase3)(
      get_standardized_rotation(_v.v._0),
      get_standardized_rotation(_v.v._1),
      get_standardized_rotation(_v.v._2));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::compare_to
//       Access: Published
//  Description: This flavor of compare_to uses a default threshold
//               value based on the numeric type.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH int FLOATNAME(LVecBase3)::
compare_to(const FLOATNAME(LVecBase3) &other) const {
  TAU_PROFILE("int LVecBase3::compare_to(const LVecBase3 &)", " ", TAU_USER);
  return compare_to(other, NEARLY_ZERO(FLOATTYPE));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::compare_to
//       Access: Published
//  Description: Sorts vectors lexicographically, componentwise.
//               Returns a number less than 0 if this vector sorts
//               before the other one, greater than zero if it sorts
//               after, 0 if they are equivalent (within the indicated
//               tolerance).
////////////////////////////////////////////////////////////////////
INLINE_LINMATH int FLOATNAME(LVecBase3)::
compare_to(const FLOATNAME(LVecBase3) &other, FLOATTYPE threshold) const {
  TAU_PROFILE("int LVecBase3::compare_to(const LVecBase3 &, FLOATTYPE)", " ", TAU_USER);
  if (!IS_THRESHOLD_COMPEQ(_v.v._0, other._v.v._0, threshold)) {
    return (_v.v._0 < other._v.v._0) ? -1 : 1;
  }
  if (!IS_THRESHOLD_COMPEQ(_v.v._1, other._v.v._1, threshold)) {
    return (_v.v._1 < other._v.v._1) ? -1 : 1;
  }
  if (!IS_THRESHOLD_COMPEQ(_v.v._2, other._v.v._2, threshold)) {
    return (_v.v._2 < other._v.v._2) ? -1 : 1;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_hash
//       Access: Published
//  Description: Returns a suitable hash for phash_map.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH size_t FLOATNAME(LVecBase3)::
get_hash() const {
  TAU_PROFILE("size_t LVecBase3::get_hash()", " ", TAU_USER);
  return add_hash(0);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::get_hash
//       Access: Published
//  Description: Returns a suitable hash for phash_map.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH size_t FLOATNAME(LVecBase3)::
get_hash(FLOATTYPE threshold) const {
  TAU_PROFILE("size_t LVecBase3::get_hash(FLOATTYPE)", " ", TAU_USER);
  return add_hash(0, threshold);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::add_hash
//       Access: Published
//  Description: Adds the vector into the running hash.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH size_t FLOATNAME(LVecBase3)::
add_hash(size_t hash) const {
  TAU_PROFILE("size_t LVecBase3::add_hash(size_t)", " ", TAU_USER);
  return add_hash(hash, NEARLY_ZERO(FLOATTYPE));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::add_hash
//       Access: Published
//  Description: Adds the vector into the running hash.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH size_t FLOATNAME(LVecBase3)::
add_hash(size_t hash, FLOATTYPE threshold) const {
  TAU_PROFILE("LVecBase3::add_hash(size_t, FLOATTYPE)", " ", TAU_USER);
  float_hash fhasher(threshold);
  hash = fhasher.add_hash(hash, _v.v._0);
  hash = fhasher.add_hash(hash, _v.v._1);
  hash = fhasher.add_hash(hash, _v.v._2);
  return hash;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::unary -
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
operator - () const {
  return FLOATNAME(LVecBase3)(-_v.v._0, -_v.v._1, -_v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::vector + vector
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
operator + (const FLOATNAME(LVecBase3) &other) const {
  return FLOATNAME(LVecBase3)(_v.v._0 + other._v.v._0,
                              _v.v._1 + other._v.v._1,
                              _v.v._2 + other._v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::vector - vector
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
operator - (const FLOATNAME(LVecBase3) &other) const {
  return FLOATNAME(LVecBase3)(_v.v._0 - other._v.v._0,
                              _v.v._1 - other._v.v._1,
                              _v.v._2 - other._v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::vector * scalar
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
operator * (FLOATTYPE scalar) const {
  return FLOATNAME(LVecBase3)(_v.v._0 * scalar,
                              _v.v._1 * scalar,
                              _v.v._2 * scalar);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::vector / scalar
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
operator / (FLOATTYPE scalar) const {
  FLOATTYPE recip_scalar = 1.0f/scalar;
  return FLOATNAME(LVecBase3)(_v.v._0 * recip_scalar,
                              _v.v._1 * recip_scalar,
                              _v.v._2 * recip_scalar);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator +=
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
operator += (const FLOATNAME(LVecBase3) &other) {
  _v.v._0 += other._v.v._0;
  _v.v._1 += other._v.v._1;
  _v.v._2 += other._v.v._2;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator -=
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
operator -= (const FLOATNAME(LVecBase3) &other) {
  _v.v._0 -= other._v.v._0;
  _v.v._1 -= other._v.v._1;
  _v.v._2 -= other._v.v._2;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator *=
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
operator *= (FLOATTYPE scalar) {
  _v.v._0 *= scalar;
  _v.v._1 *= scalar;
  _v.v._2 *= scalar;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::operator /=
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
operator /= (FLOATTYPE scalar) {
  FLOATTYPE recip_scalar = 1.0f/scalar;
  _v.v._0 *= recip_scalar;
  _v.v._1 *= recip_scalar;
  _v.v._2 *= recip_scalar;
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::fmax
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
fmax(const FLOATNAME(LVecBase3) &other) {
  TAU_PROFILE("LVecBase3::fmax()", " ", TAU_USER);
  return FLOATNAME(LVecBase3)(_v.v._0 > other._v.v._0 ? _v.v._0 : other._v.v._0,
                              _v.v._1 > other._v.v._1 ? _v.v._1 : other._v.v._1,
                              _v.v._2 > other._v.v._2 ? _v.v._2 : other._v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::fmin
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH FLOATNAME(LVecBase3) FLOATNAME(LVecBase3)::
fmin(const FLOATNAME(LVecBase3) &other) {
  TAU_PROFILE("LVecBase3::fmin()", " ", TAU_USER);
  return FLOATNAME(LVecBase3)(_v.v._0 < other._v.v._0 ? _v.v._0 : other._v.v._0,
                              _v.v._1 < other._v.v._1 ? _v.v._1 : other._v.v._1,
                              _v.v._2 < other._v.v._2 ? _v.v._2 : other._v.v._2);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::cross product (with assigment)
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
cross_into(const FLOATNAME(LVecBase3) &other) {
  (*this) = cross(other);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::almost_equal
//       Access: Published
//  Description: Returns true if two vectors are memberwise equal
//               within a specified tolerance.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
almost_equal(const FLOATNAME(LVecBase3) &other, FLOATTYPE threshold) const {
  TAU_PROFILE("bool LVecBase3::almost_equal(LVecBase3 &, FLOATTYPE)", " ", TAU_USER);
  return (IS_THRESHOLD_EQUAL(_v.v._0, other._v.v._0, threshold) &&
          IS_THRESHOLD_EQUAL(_v.v._1, other._v.v._1, threshold) &&
          IS_THRESHOLD_EQUAL(_v.v._2, other._v.v._2, threshold));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::almost_equal
//       Access: Published
//  Description: Returns true if two vectors are memberwise equal
//               within a default tolerance based on the numeric type.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH bool FLOATNAME(LVecBase3)::
almost_equal(const FLOATNAME(LVecBase3) &other) const {
  TAU_PROFILE("bool LVecBase3::almost_equal(LVecBase3 &)", " ", TAU_USER);
  return almost_equal(other, NEARLY_ZERO(FLOATTYPE));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::output
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
output(ostream &out) const {
  out << MAYBE_ZERO(_v.v._0) << " "
      << MAYBE_ZERO(_v.v._1) << " "
      << MAYBE_ZERO(_v.v._2);
}

#ifdef HAVE_PYTHON
////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::python_repr
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
python_repr(ostream &out, const string &class_name) const {
  out << class_name << "(" 
      << MAYBE_ZERO(_v.v._0) << ", "
      << MAYBE_ZERO(_v.v._1) << ", "
      << MAYBE_ZERO(_v.v._2) << ")";
}
#endif  // HAVE_PYTHON


////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::generate_hash
//       Access: Public
//  Description: Adds the vector to the indicated hash generator.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
generate_hash(ChecksumHashGenerator &hashgen) const {
  TAU_PROFILE("LVecBase3::generate_hash(ChecksumHashGenerator &)", " ", TAU_USER);
  generate_hash(hashgen, NEARLY_ZERO(FLOATTYPE));
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::generate_hash
//       Access: Public
//  Description: Adds the vector to the indicated hash generator.
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
generate_hash(ChecksumHashGenerator &hashgen, FLOATTYPE threshold) const {
  TAU_PROFILE("LVecBase3::generate_hash(ChecksumHashGenerator &, FLOATTYPE)", " ", TAU_USER);
  hashgen.add_fp(_v.v._0, threshold);
  hashgen.add_fp(_v.v._1, threshold);
  hashgen.add_fp(_v.v._2, threshold);
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::write_datagram
//       Access: Public
//  Description: Function to write itself into a datagram
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
write_datagram(Datagram &destination) const {
#if FLOATTOKEN == 'f'
  destination.add_float32(_v.v._0);
  destination.add_float32(_v.v._1);
  destination.add_float32(_v.v._2);
#else
  destination.add_float64(_v.v._0);
  destination.add_float64(_v.v._1);
  destination.add_float64(_v.v._2);
#endif
}

////////////////////////////////////////////////////////////////////
//     Function: LVecBase3::read_datagram
//       Access: Public
//  Description: Function to read itself from a datagramIterator
////////////////////////////////////////////////////////////////////
INLINE_LINMATH void FLOATNAME(LVecBase3)::
read_datagram(DatagramIterator &source) {
#if FLOATTOKEN == 'f'
  _v.v._0 = source.get_float32();
  _v.v._1 = source.get_float32();
  _v.v._2 = source.get_float32();
#else
  _v.v._0 = source.get_float64();
  _v.v._1 = source.get_float64();
  _v.v._2 = source.get_float64();
#endif
}
