// Filename: plane_src.cxx
// Created by:  drose (03Apr01)
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
//     Function: Plane::get_reflection_mat
//       Access: Published
//  Description: This computes a transform matrix that reflects the
//               universe to the other side of the plane, as in a
//               mirror.
////////////////////////////////////////////////////////////////////
FLOATNAME(LMatrix4) FLOATNAME(Plane)::
get_reflection_mat() const {
  FLOATTYPE aa = _v.v._0 * _v.v._0; 
  FLOATTYPE ab = _v.v._0 * _v.v._1;
  FLOATTYPE ac = _v.v._0 * _v.v._2;
  FLOATTYPE ad = _v.v._0 * _v.v._3;
  FLOATTYPE bb = _v.v._1 * _v.v._1;
  FLOATTYPE bc = _v.v._1 * _v.v._2;
  FLOATTYPE bd = _v.v._1 * _v.v._3;
  FLOATTYPE cc = _v.v._2 * _v.v._2;
  FLOATTYPE cd = _v.v._2 * _v.v._3;

  return FLOATNAME(LMatrix4)(  1-2*aa,  -2*ab,  -2*ac,     0,
                              -2*ab, 1-2*bb,  -2*bc,     0,
                              -2*ac,  -2*bc, 1-2*cc,     0,
                              -2*ad,  -2*bd,  -2*cd,     1  );
}

////////////////////////////////////////////////////////////////////
//     Function: Plane::get_point
//       Access: Published
//  Description: Returns an arbitrary point in the plane.  This can be
//               used along with the normal returned by get_normal()
//               to reconstruct the plane.
////////////////////////////////////////////////////////////////////
FLOATNAME(LPoint3) FLOATNAME(Plane)::
get_point() const {
  // Choose the denominator based on the largest axis in the normal.
  if (cabs(_v.v._0) >= cabs(_v.v._1) && cabs(_v.v._0) >= cabs(_v.v._2)) {
    nassertr(_v.v._0 != 0.0f, FLOATNAME(LPoint3)(0.0f, 0.0f, 0.0f));
    return FLOATNAME(LPoint3)(-_v.v._3 / _v.v._0, 0.0f, 0.0f);
  } else if (cabs(_v.v._1) >= cabs(_v.v._2)) {
    nassertr(_v.v._1 != 0.0f, FLOATNAME(LPoint3)(0.0f, 0.0f, 0.0f));
    return FLOATNAME(LPoint3)(0.0f, -_v.v._3 / _v.v._1, 0.0f);
  } else {
    nassertr(_v.v._2 != 0.0f, FLOATNAME(LPoint3)(0.0f, 0.0f, 0.0f));
    return FLOATNAME(LPoint3)(0.0f, 0.0f, -_v.v._3 / _v.v._2);
  }
}


////////////////////////////////////////////////////////////////////
//     Function: Plane::intersects_plane
//       Access: Published
//  Description: Returns true if the two planes intersect, false if
//               they do not.  If they do intersect, then from and
//               delta are filled in with the parametric
//               representation of the line of intersection: that is,
//               from is a point on that line, and delta is a vector
//               showing the direction of the line.
////////////////////////////////////////////////////////////////////
bool FLOATNAME(Plane)::
intersects_plane(FLOATNAME(LPoint3) &from,
                 FLOATNAME(LVector3) &delta,
                 const FLOATNAME(Plane) &other) const {
  FLOATNAME(LVector3) n1 = get_normal();
  FLOATNAME(LVector3) n2 = other.get_normal();

  // The delta will be the cross product of the planes' normals.
  delta = cross(n1, n2);

  // If the delta came out to zero, the planes were parallel and do
  // not intersect.
  if (delta.almost_equal(FLOATNAME(LVector3)::zero())) {
    return false;
  }

  FLOATTYPE n1n1 = ::dot(n1, n1);
  FLOATTYPE n2n2 = ::dot(n2, n2);
  FLOATTYPE n1n2 = ::dot(n1, n2);
 
  FLOATTYPE determinant_inv = 1.0f / (n1n1 * n2n2 - n1n2 * n1n2);
  FLOATTYPE c1 = (other._v.v._3 * n1n2 - _v.v._3 * n2n2) * determinant_inv;
  FLOATTYPE c2 = (_v.v._3 * n1n2 - other._v.v._3 * n1n1) * determinant_inv;
  from = n1 * c1 + n2 * c2;

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Plane::intersects_parabola
//       Access: Published
//  Description: Determines whether and where the indicated parabola
//               intersects with the plane.
//
//               If there is no intersection with the plane, the
//               function returns false and leaves t1 and t2
//               undefined.  If there is an intersection with the
//               plane, the function returns true and sets t1 and t2
//               to the parametric value that defines the two points
//               of intersection.  If the parabola is exactly tangent
//               to the plane, then t1 == t2.
////////////////////////////////////////////////////////////////////
bool FLOATNAME(Plane)::
intersects_parabola(FLOATTYPE &t1, FLOATTYPE &t2,
                    const FLOATNAME(Parabola) &parabola) const {
  //
  // The parabola intersects the plane wherever:
  //
  // a * t^2 + b * t + c == 0
  //
  // where a = normal dot parabola.get_a(),
  //       b = normal dot parabola.get_b(),
  //       c = normal dot parabola.get_c() + d.
  //

  FLOATNAME(LVector3) normal = get_normal();
  FLOATTYPE a = normal.dot(parabola.get_a());
  FLOATTYPE b = normal.dot(parabola.get_b());
  FLOATTYPE c = normal.dot(parabola.get_c()) + _v.v._3;

  if (IS_NEARLY_ZERO(a)) {
    // It's not quadratic.  The equation is actually:
    //    b * t + c == 0.
    // Which means:
    //    t = -c / b.

    if (IS_NEARLY_ZERO(b)) {
      // It's not even linear.  The parabola must be completely
      // parallel to the plane, or if c == 0, it's completely within
      // the plane.  In both cases, we'll call it no intersection.
      return false;
    }

    t1 = -c / b;
    t2 = t1;
    return true;
  }

  // Now use the quadratic equation to solve for t.
  FLOATTYPE discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0f) {
    // No intersection.
    return false;
  }

  FLOATTYPE sqrd = csqrt(discriminant);

  t1 = (-b - sqrd) / (2.0 * a);
  t2 = (-b + sqrd) / (2.0 * a);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Plane::output
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void FLOATNAME(Plane)::
output(ostream &out) const {
  out << "Plane(";
  FLOATNAME(LVecBase4)::output(out);
  out << ")";
}

////////////////////////////////////////////////////////////////////
//     Function: Plane::write
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void FLOATNAME(Plane)::
write(ostream &out, int indent_level) const {
  indent(out, indent_level) << *this << "\n";
}
