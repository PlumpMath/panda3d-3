// Filename: hermiteCurve.cxx
// Created by:  drose (27Feb98)
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

#include "pandabase.h"
#include "hermiteCurve.h"
#include "config_parametrics.h"
#include "luse.h"

#include "indent.h"
#include "datagram.h"
#include "datagramIterator.h"
#include "bamWriter.h"
#include "bamReader.h"

#include <math.h>

TypeHandle HermiteCurve::_type_handle;

static const LVecBase3f zerovec_3f = LVecBase3f(0.0f, 0.0f, 0.0f);
// This is returned occasionally from some of the functions, and is
// used from time to time as an initializer.


////////////////////////////////////////////////////////////////////
//     Function: show_vec3
//  Description: This function writes a LVecBase3f, with a specified
//               number of significant dimensions.
////////////////////////////////////////////////////////////////////
static ostream &
show_vec3(ostream &out, int indent_level, const LVecBase3f &v,
          int num_dimensions) {
  indent(out, indent_level) << v[0];
  for (int i = 1; i<num_dimensions; i++) {
    out << " " << v[i];
  }
  return out;
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
HermiteCurveCV::
HermiteCurveCV() {
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::copy_constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
HermiteCurveCV::
HermiteCurveCV(const HermiteCurveCV &c) :
  _p(c._p), _in(c._in), _out(c._out),
  _type(c._type), _name(c._name)
{
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
HermiteCurveCV::
~HermiteCurveCV() {
}



////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::set_in
//       Access: Public
//  Description: Sets the CV's in tangent.
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
set_in(const LVecBase3f &in) {
  _in = in;
  /*
  float l;
  switch (_type) {
  case HC_G1:
    l = _in.length();
    if (l!=0.0f) {
      _out = _in * _out.length() / l;
    }
    break;

  case HC_SMOOTH:
    _out = _in;
    break;
  }
  */
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::set_out
//       Access: Public
//  Description: Sets the CV's out tangent.
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
set_out(const LVecBase3f &out) {
  _out = out;
  /*
  float l;
  switch (_type) {
  case HC_G1:
    l = _out.length();
    if (l!=0.0f) {
      _in = _out * _in.length() / l;
    }
    break;

  case HC_SMOOTH:
    _in = _out;
    break;
  }
  */
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::set_type
//       Access: Public
//  Description: Sets the continuity type of the CV.  Values may be
//               HC_CUT, indicating a discontinous break in the curve,
//               HC_FREE, for unconstrained in and out tangents,
//               HC_G1, for in and out tangents constrained to be
//               collinear, and HC_SMOOTH, for in and out tangents
//               constrained to be equal.  Other than HC_CUT, these
//               are for documentation only; setting this has no
//               direct effect on the tangents.
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
set_type(int type) {
  _type = type;
  /*
  switch (_type) {
  case HC_G1:
    _out = _out.length() * _in;
    break;

  case HC_SMOOTH:
    _out = _in;
    break;
  }
  */
}



////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::set_name
//       Access: Public
//  Description: Sets the name associated with the CV.
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
set_name(const string &name) {
  _name = name;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::format_egg
//       Access: Public
//  Description: Formats the CV for output to an egg file.
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
format_egg(ostream &out, int indent_level, int num_dimensions,
       bool show_in, bool show_out,
       float scale_in, float scale_out) const {
  if (show_in) {
    indent(out, indent_level) << "<Vertex> {\n";
    show_vec3(out, indent_level + 2, _p - scale_in * _in / 3.0,
              num_dimensions) << "\n";
    indent(out, indent_level) << "}\n";
  }

  indent(out, indent_level) << "<Vertex> {\n";
  show_vec3(out, indent_level + 2, _p, num_dimensions) << "\n";

  indent(out, indent_level+2) << "<Scalar> continuity-type { ";
  switch (_type) {
  case HC_CUT:
    out << "Cut";
    break;

  case HC_FREE:
    out << "Free";
    break;

  case HC_G1:
    out << "G1";
    break;

  case HC_SMOOTH:
    out << "Smooth";
    break;
  };
  out << " }\n";

  indent(out, indent_level) << "}\n";

  if (show_out) {
    indent(out, indent_level) << "<Vertex> {\n";
    show_vec3(out, indent_level + 2, _p + scale_out * _out / 3.0,
              num_dimensions) << "\n";
    indent(out, indent_level) << "}\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::write_datagram
//       Access: Public
//  Description: Function to write the important information in
//               the particular object to a Datagram
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
write_datagram(BamWriter *, Datagram &me) const {
  _p.write_datagram(me);
  _in.write_datagram(me);
  _out.write_datagram(me);
  me.add_int8(_type);
  me.add_string(_name);
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveCV::fillin
//       Access: Public
//  Description: Function that reads out of the datagram (or asks
//               manager to read) all of the data that is needed to
//               re-create this object and stores it in the appropiate
//               place
////////////////////////////////////////////////////////////////////
void HermiteCurveCV::
fillin(DatagramIterator &scan, BamReader *) {
  _p.read_datagram(scan);
  _in.read_datagram(scan);
  _out.read_datagram(scan);

  _type = scan.get_int8();
  _name = scan.get_string();
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
HermiteCurve::
HermiteCurve() {
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::Copy Constructor
//       Access: Published
//  Description: Constructs a Hermite from the indicated (possibly
//               non-hermite) curve.
////////////////////////////////////////////////////////////////////
HermiteCurve::
HermiteCurve(const ParametricCurve &nc) {
  if (!nc.convert_to_hermite(this)) {
    parametrics_cat->warning()
      << "Cannot make a Hermite from the indicated curve."
      << endl;
  }
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::Destructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
HermiteCurve::
~HermiteCurve() {
}






////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_num_cvs
//       Access: Published
//  Description: Returns the number of CV's in the curve.
////////////////////////////////////////////////////////////////////
int HermiteCurve::
get_num_cvs() const {
  return _points.size();
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::insert_cv
//       Access: Published
//  Description: Inserts a new CV at the given parametric point along
//               the curve.  If this parametric point is already on
//               the curve, the CV is assigned an index between its
//               two neighbors and the indices of all following CV's
//               are incremented by 1; its in and out tangents are
//               chosen to keep the curve consistent.  If the new
//               parametric point is beyond the end of the existing
//               curve, the curve is extended to meet it and the new
//               CV's position, in tangent, and out tangent are set to
//               zero.
//
//               The index number of the new CV is returned.
////////////////////////////////////////////////////////////////////
int HermiteCurve::
insert_cv(float t) {
  if (!is_valid() || t >= get_max_t()) {
    int n = append_cv(HC_SMOOTH, 0.0f, 0.0f, 0.0f);
    set_cv_tstart(n, t);
    return n;
  }

  t = min(max(t, 0.0f), get_max_t());

  int n = find_cv(t);
  nassertr(n+1<get_num_cvs(), 0);

  HermiteCurveCV cv;
  LVecBase3f tan;
  cv._type = HC_SMOOTH;
  get_pt(t, cv._p, tan);
  cv._out = cv._in = tan * 0.5f;

  _points.insert(_points.begin() + n + 1, cv);
  bool result =
    insert_curveseg(n, new CubicCurveseg, t - get_cv_tstart(n));
  nassertr(result, 0);

  recompute_basis();
  invalidate_all();

  return n + 1;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::append_cv
//       Access: Published
//  Description: Adds a new CV to the end of the curve.  The new CV is
//               given initial in/out tangents of 0.  The return value
//               is the index of the new CV.
////////////////////////////////////////////////////////////////////
int HermiteCurve::
append_cv(int type, float x, float y, float z) {
  HermiteCurveCV cv;
  cv.set_type(type);
  cv.set_point(LVecBase3f(x, y, z));
  cv.set_in(zerovec_3f);
  cv.set_out(zerovec_3f);
  _points.push_back(cv);
  if (_points.size()>1) {
    bool result =
      insert_curveseg(_segs.size(), new CubicCurveseg, 1.0f);
    nassertr(result, 0);
  }

  recompute_basis();
  invalidate_all();

  return _points.size()-1;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::remove_cv
//       Access: Published
//  Description: Removes the given CV from the curve.  Returns true if
//               the CV existed, false otherwise.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
remove_cv(int n) {
  if (n < 0 || n >= (int)_points.size()) {
    return false;
  }

  _points.erase(_points.begin() + n);
  if (_segs.size()>0) {
    remove_curveseg(_segs.size()-1);
  }

  recompute_basis();
  invalidate_all();
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::remove_all_cvs
//       Access: Published
//  Description: Removes all CV's from the curve.
////////////////////////////////////////////////////////////////////
void HermiteCurve::
remove_all_cvs() {
  _points.erase(_points.begin(), _points.end());
  remove_all_curvesegs();

  invalidate_all();
}




////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::set_cv_type
//       Access: Published
//  Description: Changes the given CV's continuity type.  Legal values
//               are HC_CUT, HC_FREE, HC_G1, or HC_SMOOTH.
//
//               Other than HC_CUT, these have no effect on the actual
//               curve; it remains up to user software to impose the
//               constraints these imply.
//
//               HC_CUT implies a disconnection of the curve; HC_FREE
//               imposes no constraints on the tangents; HC_G1
//               forces the tangents to be collinear, and HC_SMOOTH
//               forces the tangents to be identical.  Setting type
//               type to HC_G1 or HC_SMOOTH may adjust the out
//               tangent to match the in tangent.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
set_cv_type(int n, int type) {
  if (n < 0 || n >= (int)_points.size()) {
    return false;
  }

  bool changed_cut = false;

  if (type!=_points[n]._type) {
    changed_cut = (type==HC_CUT || _points[n]._type==HC_CUT);
    _points[n].set_type(type);
  }

  invalidate_cv(n, changed_cut);
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::set_cv_point
//       Access: Published
//  Description: Changes the given CV's position.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
set_cv_point(int n, float x, float y, float z) {
  if (n < 0 || n >= (int)_points.size()) {
    return false;
  }
  _points[n].set_point(LVecBase3f(x, y, z));
  invalidate_cv(n, false);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::set_cv_in
//       Access: Published
//  Description: Changes the given CV's in tangent.  Depending on the
//               continuity type, this may also adjust the out
//               tangent.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
set_cv_in(int n, float x, float y, float z) {
  if (n < 0 || n >= (int)_points.size()) {
    return false;
  }
  _points[n].set_in(LVecBase3f(x, y, z));
  invalidate_cv(n, false);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::set_cv_out
//       Access: Published
//  Description: Changes the given CV's out tangent.  Depending on the
//               continuity type, this may also adjust the in
//               tangent.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
set_cv_out(int n, float x, float y, float z) {
  if (n < 0 || n >= (int)_points.size()) {
    return false;
  }
  _points[n].set_out(LVecBase3f(x, y, z));
  invalidate_cv(n, false);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::set_cv_tstart
//       Access: Published
//  Description: Changes the given CV's parametric starting time.
//               This may affect the shape of the curve.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
set_cv_tstart(int n, float tstart) {
  if (n <= 0 || n >= (int)_points.size()) {
    return false;
  }
  if (fabs(tstart - get_cv_tstart(n)) > 0.0001f) {
    set_tlength(n-1, tstart - get_tstart(n-1));
    recompute_basis();
    invalidate_all();
  }
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::set_cv_name
//       Access: Published
//  Description: Changes the name associated with a particular CV.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
set_cv_name(int n, const char *name) {
  if (n < 0 || n >= (int)_points.size()) {
    return false;
  }
  _points[n].set_name(name);
  return true;
}



////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_cv_type
//       Access: Published
//  Description: Returns the given CV's continuity type, HC_CUT,
//               HC_FREE, HC_G1, or HC_SMOOTH, or 0 if there is
//               no such CV.
////////////////////////////////////////////////////////////////////
int HermiteCurve::
get_cv_type(int n) const {
  if (n < 0 || n >= (int)_points.size()) {
    return 0;
  }

  return _points[n]._type;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_cv_point
//       Access: Published
//  Description: Returns the position of the given CV.
////////////////////////////////////////////////////////////////////
const LVecBase3f &HermiteCurve::
get_cv_point(int n) const {
  if (n < 0 || n >= (int)_points.size()) {
    return zerovec_3f;
  }

  return _points[n]._p;
}
void HermiteCurve::
get_cv_point(int n, LVecBase3f &v) const {
  v = get_cv_point(n);
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_cv_in
//       Access: Published
//  Description: Returns the in tangent of the given CV.
////////////////////////////////////////////////////////////////////
const LVecBase3f &HermiteCurve::
get_cv_in(int n) const {
  if (n < 0 || n >= (int)_points.size() || _points[n-1]._type==HC_CUT) {
    return zerovec_3f;
  }

  return _points[n]._in;
}
void HermiteCurve::
get_cv_in(int n, LVecBase3f &v) const {
  v = get_cv_in(n);
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_cv_out
//       Access: Published
//  Description: Returns the out tangent of the given CV.
////////////////////////////////////////////////////////////////////
const LVecBase3f &HermiteCurve::
get_cv_out(int n) const {
  if (n < 0 || n >= (int)_points.size() || _points[n]._type==HC_CUT) {
    return zerovec_3f;
  }

  return _points[n]._out;
}
void HermiteCurve::
get_cv_out(int n, LVecBase3f &v) const {
  v = get_cv_out(n);
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_cv_tstart
//       Access: Published
//  Description: Returns the starting point in parametric space of the
//               given CV.
////////////////////////////////////////////////////////////////////
float HermiteCurve::
get_cv_tstart(int n) const {
  if (n<0) {
    return 0.0f;
  } else if (n >= (int)_points.size()) {
    return get_max_t();
  }

  return get_tstart(n);
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::get_cv_name
//       Access: Published
//  Description: Returns the name of the given CV, or NULL.
////////////////////////////////////////////////////////////////////
string HermiteCurve::
get_cv_name(int n) const {
  if (n < 0 || n >= (int)_points.size()) {
    return string();
  }

  return _points[n]._name;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::output
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
void HermiteCurve::
output(ostream &out) const {
  PiecewiseCurve::output(out);

  out << " (";
  switch (get_curve_type()) {
  case PCT_T:
    out << "in T, ";
    break;

  case PCT_XYZ:
    out << "in XYZ, ";
    break;

  case PCT_HPR:
    out << "in HPR, ";
    break;

  default:
    break;
  }

  out << get_num_cvs() << " CV's)";
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::write_cv
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void HermiteCurve::
write_cv(ostream &out, int n) const {
  out << "CV";
  if (!get_cv_name(n).empty()) {
    out << " " << get_cv_name(n);
  }

  out << " at t = " << get_cv_tstart(n)
    << "\npoint = " << get_cv_point(n)
    << "\nin = " << get_cv_in(n) << " out = " << get_cv_out(n)
    << "\ncontinuity type = ";

  switch (get_cv_type(n)) {
  case HC_CUT:
    out << "Cut";
    break;

  case HC_FREE:
    out << "Free";
    break;

  case HC_G1:
    out << "G1";
    break;

  case HC_SMOOTH:
    out << "Smooth";
    break;

  default:
    break;
  }

  out << "\n";
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::rebuild_curveseg
//       Access: Public, Virtual
//  Description: Rebuilds the current curve segment (as selected by
//               the most recent call to find_curve()) according to
//               the specified properties (see
//               CubicCurveseg::compute_seg).  Returns true if
//               possible, false if something goes horribly wrong.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
rebuild_curveseg(int, float, const LVecBase4f &,
                 int, float, const LVecBase4f &,
                 int, float, const LVecBase4f &,
                 int, float, const LVecBase4f &) {
  cerr << "rebuild_curveseg not implemented for this curve type.\n";
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::format_egg
//       Access: Public
//  Description: Formats the Hermite curve for output to an Egg file.
////////////////////////////////////////////////////////////////////
bool HermiteCurve::
format_egg(ostream &out, const string &name, const string &curve_type,
           int indent_level) const {
  indent(out, indent_level)
    << "<VertexPool> " << name << ".pool {\n";

  int i;
  for (i = 0; i < (int)_points.size(); i++) {
    bool show_in = (i != 0);
    bool show_out = (i != (int)_points.size()-1);
    _points[i].format_egg(out, indent_level + 2, _num_dimensions,
                          show_in, show_out,
                          show_in ? get_tlength(i-1) : 0.0f,
                          show_out ? get_tlength(i) : 0.0f);
  }
  indent(out, indent_level) << "}\n";

  indent(out, indent_level) << "<BezierCurve> " << name << " {\n";

  if (!curve_type.empty()) {
    indent(out, indent_level+2)
      << "<Scalar> type { " << curve_type << " }\n";
  }

  indent(out, indent_level+2) << "<TLengths> {";
  if (_points.size() > 1) {
    for (i = 0; i < (int)_segs.size(); i++) {
      if (i%10 == 1) {
        out << "\n";
        indent(out, indent_level+3);
      }
      out << " " << get_tlength(i);
    }
  }
  out << "\n";
  indent(out, indent_level+2) << "}\n";

  indent(out, indent_level+2) << "<VertexRef> {";
  for (i = 1; i <= (int)_points.size() * 3 - 2; i++) {
    if (i%10 == 1) {
      out << "\n";
      indent(out, indent_level+3);
    }
    out << " " << i;
  }
  out << "\n";
  indent(out, indent_level+4) << "<Ref> { " << name << ".pool }\n";
  indent(out, indent_level+2) << "}\n";

  indent(out, indent_level) << "}\n";

  return true;
}


static void
wrap_hpr(const LVecBase3f &hpr1, LVecBase3f &hpr2) {
  for (int i = 0; i < 3; i++) {
    while ((hpr2[i] - hpr1[i]) > 180.0) {
      hpr2[i] -= 360.0;
    }

    while ((hpr2[i] - hpr1[i]) < -180.0) {
      hpr2[i] += 360.0;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::invalidate_cv
//       Access: Protected
//  Description: Recomputes the CV and its neighbors appropriately
//               after a change.
////////////////////////////////////////////////////////////////////
void HermiteCurve::
invalidate_cv(int n, bool redo_all) {
  float t1 = 0.0f, t2 = get_max_t();
  if (n>0 && _points[n-1]._type!=HC_CUT) {
    const HermiteCurveCV &p1 = _points[n-1];
    HermiteCurveCV p2(_points[n]);
    if (_curve_type == PCT_HPR) {
      wrap_hpr(p1._p, p2._p);
    }
    get_curveseg(n-1)->hermite_basis(p1, p2, get_tlength(n-1));
    t1 = get_cv_tstart(n-1);
  }

  if (n+1 < (int)_points.size()) {
    if (_points[n]._type==HC_CUT) {
      BezierSeg seg;
      seg._v[0] = seg._v[1] = seg._v[2] = seg._v[3] = _points[n]._p;
      get_curveseg(n)->bezier_basis(seg);
    } else {
      const HermiteCurveCV &p1 = _points[n];
      HermiteCurveCV p2(_points[n+1]);
      if (_curve_type == PCT_HPR) {
        wrap_hpr(p1._p, p2._p);
      }
      get_curveseg(n)->hermite_basis(p1, p2, get_tlength(n));
      t2 = get_cv_tstart(n+2);
    }
  }

  if (is_valid()) {
    if (redo_all) {
      invalidate_all();
    } else {
      invalidate(t1, t2);
    }
  }
}



////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::find_cv
//       Access: Protected
//  Description: Finds the CV immediately preceding the given value of
//               t.
////////////////////////////////////////////////////////////////////
int HermiteCurve::
find_cv(float t) {
  nassertr(is_valid(), 0);

  int n;
  for (n = 0; n < (int)_segs.size(); n++) {
    if (_segs[n]._tend+0.00001 > t) {
      break;
    }
  }

  return n;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::recompute_basis
//       Access: Protected
//  Description: Recomputes the coefficients for all the CV's in the
//               curve.  This is intended to be called whenever the
//               CV's have been changed in some drastic way, and it's
//               safest just to recompute everything.
////////////////////////////////////////////////////////////////////
void HermiteCurve::
recompute_basis() {
  int n;
  for (n = 0; n < (int)_segs.size(); n++) {
    if (_points[n]._type==HC_CUT) {
      BezierSeg seg;
      seg._v[0] = seg._v[1] = seg._v[2] = seg._v[3] = _points[n]._p;
      get_curveseg(n)->bezier_basis(seg);
    } else {
      const HermiteCurveCV &p1 = _points[n];
      HermiteCurveCV p2(_points[n+1]);
      if (_curve_type == PCT_HPR) {
        wrap_hpr(p1._p, p2._p);
      }
      get_curveseg(n)->hermite_basis(p1, p2, get_tlength(n));
    }
  }
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::register_with_factory
//       Access: Public, Static
//  Description: Initializes the factory for reading these things from
//               Bam files.
////////////////////////////////////////////////////////////////////
void HermiteCurve::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_HermiteCurve);
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::make_HermiteCurve
//       Access: Protected
//  Description: Factory method to generate an object of this type.
////////////////////////////////////////////////////////////////////
TypedWritable *HermiteCurve::
make_HermiteCurve(const FactoryParams &params) {
  HermiteCurve *me = new HermiteCurve;
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  me->fillin(scan, manager);
  return me;
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::write_datagram
//       Access: Protected, Virtual
//  Description: Function to write the important information in
//               the particular object to a Datagram
////////////////////////////////////////////////////////////////////
void HermiteCurve::
write_datagram(BamWriter *manager, Datagram &me) {
  PiecewiseCurve::write_datagram(manager, me);

  me.add_uint32(_points.size());
  size_t i;
  for (i = 0; i < _points.size(); i++) {
    _points[i].write_datagram(manager, me);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurve::fillin
//       Access: Protected
//  Description: Function that reads out of the datagram (or asks
//               manager to read) all of the data that is needed to
//               re-create this object and stores it in the appropiate
//               place
////////////////////////////////////////////////////////////////////
void HermiteCurve::
fillin(DatagramIterator &scan, BamReader *manager) {
  PiecewiseCurve::fillin(scan, manager);

  size_t num_points = scan.get_uint32();
  _points.reserve(num_points);
  size_t i;
  for (i = 0; i < num_points; i++) {
    HermiteCurveCV cv;
    cv.fillin(scan, manager);
    _points.push_back(cv);
  }
}
