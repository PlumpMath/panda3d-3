// Filename: nurbsCurveInterface.cxx
// Created by:  drose (02Mar01)
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

#include "nurbsCurveInterface.h"
#include "parametricCurve.h"
#include "config_parametrics.h"

TypeHandle NurbsCurveInterface::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: NurbsCurveInterface::Destructor
//       Access: Published, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
NurbsCurveInterface::
~NurbsCurveInterface() {
}

////////////////////////////////////////////////////////////////////
//     Function: NurbsCurveInterface::set_cv_weight
//       Access: Published
//  Description: Sets the weight of the indicated CV without affecting
//               its position in 3-d space.
////////////////////////////////////////////////////////////////////
bool NurbsCurveInterface::
set_cv_weight(int n, float w) {
  nassertr(n >= 0 && n < get_num_cvs(), false);
  LVecBase4f cv = get_cv(n);
  if (cv[3] == 0.0f) {
    cv.set(0.0f, 0.0f, 0.0f, w);
  } else {
    cv *= w / cv[3];
  }
  return set_cv(n, cv);
}

////////////////////////////////////////////////////////////////////
//     Function: NurbsCurveInterface::write_cv
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void NurbsCurveInterface::
write_cv(ostream &out, int n) const {
  nassertv(n >= 0 && n < get_num_cvs());

  out << "CV " << n << ": " << get_cv_point(n) << ", weight "
      << get_cv_weight(n) << "\n";
}

////////////////////////////////////////////////////////////////////
//     Function: NurbsCurveInterface::write
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
void NurbsCurveInterface::
write(ostream &out, int indent_level) const {
  indent(out, indent_level);

  float min_t = 0.0f;
  float max_t = 0.0f;

  if (get_num_knots() > 0) {
    min_t = get_knot(0);
    max_t = get_knot(get_num_knots() - 1);
  }

  out << "NurbsCurve, order " << get_order() << ", " << get_num_cvs()
      << " CV's.  t ranges from " << min_t << " to " << max_t << ".\n";

  indent(out, indent_level)
    << "CV's:\n";
  int i;
  int num_cvs = get_num_cvs();
  for (i = 0; i < num_cvs; i++) {
    indent(out, indent_level)
      << i << ") " << get_cv_point(i) << ", weight "
      << get_cv_weight(i) << "\n";
  }

  indent(out, indent_level)
    << "Knots: ";
  int num_knots = get_num_knots();
  for (i = 0; i < num_knots; i++) {
    out << " " << get_knot(i);
  }
  out << "\n";
}


////////////////////////////////////////////////////////////////////
//     Function: NurbsCurveInterface::format_egg
//       Access: Protected
//  Description: Formats the Nurbs curve for output to an Egg file.
////////////////////////////////////////////////////////////////////
bool NurbsCurveInterface::
format_egg(ostream &out, const string &name, const string &curve_type,
           int indent_level) const {
  indent(out, indent_level)
    << "<VertexPool> " << name << ".pool {\n";

  int num_cvs = get_num_cvs();
  int cv;
  for (cv = 0; cv < get_num_cvs(); cv++) {
    indent(out, indent_level+2)
      << "<Vertex> " << cv << " { " << get_cv(cv) << " }\n";
  }
  indent(out, indent_level)
    << "}\n";

  indent(out, indent_level)
    << "<NurbsCurve> " << name << " {\n";

  if (!curve_type.empty()) {
    indent(out, indent_level+2)
      << "<Scalar> type { " << curve_type << " }\n";
  }

  indent(out, indent_level+2) << "<Order> { " << get_order() << " }\n";

  indent(out, indent_level+2) << "<Knots> {";
  int k;
  int num_knots = get_num_knots();

  for (k = 0; k < num_knots; k++) {
    if (k%6 == 0) {
      out << "\n";
      indent(out, indent_level+4);
    }
    out << get_knot(k) << " ";
  }
  out << "\n";
  indent(out, indent_level+2) << "}\n";

  indent(out, indent_level+2) << "<VertexRef> {";
  for (cv = 0; cv < num_cvs; cv++) {
    if (cv%10 == 0) {
      out << "\n";
      indent(out, indent_level+3);
    }
    out << " " << cv;
  }
  out << "\n";
  indent(out, indent_level+4)
    << "<Ref> { " << name << ".pool }\n";
  indent(out, indent_level+2) << "}\n";

  indent(out, indent_level) << "}\n";

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: NurbsCurveInterface::convert_to_nurbs
//       Access: Protected
//  Description: Stores in the indicated NurbsCurve a NURBS
//               representation of an equivalent curve.  Returns true
//               if successful, false otherwise.
////////////////////////////////////////////////////////////////////
bool NurbsCurveInterface::
convert_to_nurbs(ParametricCurve *nc) const {
  NurbsCurveInterface *nurbs = nc->get_nurbs_interface();
  nassertr(nurbs != (NurbsCurveInterface *)NULL, false);

  nurbs->remove_all_cvs();
  nurbs->set_order(get_order());

  int num_cvs = get_num_cvs();
  int i;
  for (i = 0; i < num_cvs; i++) {
    nurbs->append_cv(get_cv(i));
  }

  int num_knots = get_num_knots();
  for (i = 0; i < num_knots; i++) {
    nurbs->set_knot(i, get_knot(i));
  }

  return nc->recompute();
}
