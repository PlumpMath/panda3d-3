// Filename: ropeNode.cxx
// Created by:  drose (04Dec02)
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

#include "ropeNode.h"
#include "cullTraverser.h"
#include "cullTraverserData.h"
#include "cullableObject.h"
#include "cullHandler.h"
#include "renderState.h"
#include "renderModeAttrib.h"
#include "colorAttrib.h"
#include "bamWriter.h"
#include "bamReader.h"
#include "datagram.h"
#include "datagramIterator.h"
#include "pStatTimer.h"
#include "geom.h"
#include "geomLinestrips.h"
#include "geomTristrips.h"
#include "geomVertexWriter.h"
#include "boundingSphere.h"

TypeHandle RopeNode::_type_handle;

PStatCollector RopeNode::_rope_node_pcollector("*:RopeNode");

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::CData::make_copy
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CycleData *RopeNode::CData::
make_copy() const {
  return new CData(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::CData::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void RopeNode::CData::
write_datagram(BamWriter *writer, Datagram &dg) const {
  // For now, we write a NULL pointer.  Eventually we will write out
  // the NurbsCurveEvaluator pointer.
  writer->write_pointer(dg, (TypedWritable *)NULL);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::CData::fillin
//       Access: Public, Virtual
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new RopeNode.
////////////////////////////////////////////////////////////////////
void RopeNode::CData::
fillin(DatagramIterator &scan, BamReader *reader) {
  // For now, we skip over the NULL pointer that we wrote out.
  reader->skip_pointer(scan);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
RopeNode::
RopeNode(const string &name) :
  PandaNode(name)
{
  set_cull_callback();
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::Copy Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
RopeNode::
RopeNode(const RopeNode &copy) :
  PandaNode(copy),
  _cycler(copy._cycler)
{
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::make_copy
//       Access: Public, Virtual
//  Description: Returns a newly-allocated Node that is a shallow copy
//               of this one.  It will be a different Node pointer,
//               but its internal data may or may not be shared with
//               that of the original Node.
////////////////////////////////////////////////////////////////////
PandaNode *RopeNode::
make_copy() const {
  return new RopeNode(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::safe_to_transform
//       Access: Public, Virtual
//  Description: Returns true if it is generally safe to transform
//               this particular kind of Node by calling the xform()
//               method, false otherwise.  For instance, it's usually
//               a bad idea to attempt to xform a RopeNode.
////////////////////////////////////////////////////////////////////
bool RopeNode::
safe_to_transform() const {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::cull_callback
//       Access: Public, Virtual
//  Description: This function will be called during the cull
//               traversal to perform any additional operations that
//               should be performed at cull time.  This may include
//               additional manipulation of render state or additional
//               visible/invisible decisions, or any other arbitrary
//               operation.
//
//               Note that this function will *not* be called unless
//               set_cull_callback() is called in the constructor of
//               the derived class.  It is necessary to call
//               set_cull_callback() to indicated that we require
//               cull_callback() to be called.
//
//               By the time this function is called, the node has
//               already passed the bounding-volume test for the
//               viewing frustum, and the node's transform and state
//               have already been applied to the indicated
//               CullTraverserData object.
//
//               The return value is true if this node should be
//               visible, or false if it should be culled.
////////////////////////////////////////////////////////////////////
bool RopeNode::
cull_callback(CullTraverser *trav, CullTraverserData &data) {
  // Statistics
  PStatTimer timer(_rope_node_pcollector);

  // Create some geometry on-the-fly to render the rope.
  if (get_num_subdiv() > 0) {
    NurbsCurveEvaluator *curve = get_curve();
    if (curve != (NurbsCurveEvaluator *)NULL) {
      PT(NurbsCurveResult) result;
      if (has_matrix()) {
        result = curve->evaluate(data._node_path.get_node_path(), get_matrix());
      } else {
        result = curve->evaluate(data._node_path.get_node_path());
      }

      if (result->get_num_segments() > 0) {
        switch (get_render_mode()) {
        case RM_thread:
          render_thread(trav, data, result);
          break;
          
        case RM_tape:
          render_tape(trav, data, result);
          break;
          
        case RM_billboard:
          render_billboard(trav, data, result);
          break;
          
        case RM_tube:
          render_tube(trav, data, result);
          break;
        }
      }
    }
  }
  
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::is_renderable
//       Access: Public, Virtual
//  Description: Returns true if there is some value to visiting this
//               particular node during the cull traversal for any
//               camera, false otherwise.  This will be used to
//               optimize the result of get_net_draw_show_mask(), so
//               that any subtrees that contain only nodes for which
//               is_renderable() is false need not be visited.
////////////////////////////////////////////////////////////////////
bool RopeNode::
is_renderable() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::output
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void RopeNode::
output(ostream &out) const {
  PandaNode::output(out);
  NurbsCurveEvaluator *curve = get_curve();
  if (curve != (NurbsCurveEvaluator *)NULL) {
    out << " " << *curve;
  } else {
    out << " (no curve)";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::write
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void RopeNode::
write(ostream &out, int indent_level) const {
  PandaNode::write(out, indent_level);
  indent(out, indent_level) << *get_curve() << "\n";
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::reset_bound
//       Access: Published
//  Description: Recomputes the bounding volume.  This is normally
//               called automatically, but it must occasionally be
//               called explicitly when the curve has changed
//               properties outside of this node's knowledge.
////////////////////////////////////////////////////////////////////
void RopeNode::
reset_bound(const NodePath &rel_to) {
  Thread *current_thread = Thread::get_current_thread();
  int pipeline_stage = current_thread->get_pipeline_stage();
  do_recompute_bounds(rel_to, pipeline_stage, current_thread);
  mark_internal_bounds_stale(current_thread);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::compute_internal_bounds
//       Access: Protected, Virtual
//  Description: Called when needed to recompute the node's
//               _internal_bound object.  Nodes that contain anything
//               of substance should redefine this to do the right
//               thing.
////////////////////////////////////////////////////////////////////
void RopeNode::
compute_internal_bounds(CPT(BoundingVolume) &internal_bounds,
                        int &internal_vertices,
                        int pipeline_stage,
                        Thread *current_thread) const {
  PT(BoundingVolume) bounds = 
    do_recompute_bounds(NodePath((PandaNode *)this), pipeline_stage, 
                        current_thread);

  internal_bounds = bounds;
  internal_vertices = 0;  // TODO--estimate this better.
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::get_format
//       Access: Private
//  Description: Returns the appropriate GeomVertexFormat for
//               rendering, according to the user-specified
//               requirements.
////////////////////////////////////////////////////////////////////
CPT(GeomVertexFormat) RopeNode::
get_format(bool support_normals) const {
  PT(GeomVertexArrayFormat) array_format = new GeomVertexArrayFormat
    (InternalName::get_vertex(), 3, Geom::NT_float32,
     Geom::C_point);

  if (support_normals && get_normal_mode() == NM_vertex) {
    array_format->add_column
      (InternalName::get_normal(), 3, Geom::NT_float32,
       Geom::C_vector);
  }
  if (get_use_vertex_color()) {
    array_format->add_column
      (InternalName::get_color(), 1, Geom::NT_packed_dabc,
       Geom::C_color);
  }
  if (get_uv_mode() != UV_none) {
    array_format->add_column
      (InternalName::get_texcoord(), 2, Geom::NT_float32,
       Geom::C_texcoord);
  }

  return GeomVertexFormat::register_format(array_format);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::do_recompute_bounds
//       Access: Private
//  Description: Does the actual internal recompute.
////////////////////////////////////////////////////////////////////
PT(BoundingVolume) RopeNode::
do_recompute_bounds(const NodePath &rel_to, int pipeline_stage, 
                    Thread *current_thread) const {
  // TODO: fix the bounds so that it properly reflects the indicated
  // pipeline stage.  At the moment, we cheat and get some of the
  // properties from the current pipeline stage, the lazy way.

  // First, get ourselves a fresh, empty bounding volume.
  PT(BoundingVolume) bound = new BoundingSphere;
  
  NurbsCurveEvaluator *curve = get_curve();
  if (curve != (NurbsCurveEvaluator *)NULL) {
    pvector<LPoint3f> verts;
    get_curve()->get_vertices(verts, rel_to);

    if (has_matrix()) {
      // And then apply the indicated matrix.
      const LMatrix4f &mat = get_matrix();
      pvector<LPoint3f>::iterator vi;
      for (vi = verts.begin(); vi != verts.end(); ++vi) {
        (*vi) = (*vi) * mat;
      }
    }
    
    GeometricBoundingVolume *gbv;
    DCAST_INTO_R(gbv, bound, bound);
    gbv->around(&verts[0], &verts[0] + verts.size());
  }
  return bound;
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::render_thread
//       Access: Private
//  Description: Draws the rope in RM_thread mode.  This uses a
//               GeomLinestrip to draw the rope in the simplest
//               possible method, generally resulting in a
//               one-pixel-wide curve.
//
//               In this mode, the thickness parameter represents a
//               thickness in pixels, and is passed to the linestrip.
//               However, you should be aware the DirectX does not
//               support line thickness.  This mode does not support
//               per-vertex thickness.
////////////////////////////////////////////////////////////////////
void RopeNode::
render_thread(CullTraverser *trav, CullTraverserData &data, 
              NurbsCurveResult *result) const {
  CurveSegments curve_segments;
  get_connected_segments(curve_segments, result);

  // Now we have stored one or more sequences of vertices down the
  // center strips.  Go back through and calculate the vertices on
  // either side.
  PT(GeomVertexData) vdata = new GeomVertexData
    ("rope", get_format(false), Geom::UH_stream);
  
  compute_thread_vertices(vdata, curve_segments);
  
  PT(GeomLinestrips) strip = new GeomLinestrips(Geom::UH_stream);
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    
    strip->add_next_vertices(segment.size());
    strip->close_primitive();
  }
  
  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(strip);
  
  CPT(RenderAttrib) thick = RenderModeAttrib::make(RenderModeAttrib::M_unchanged, get_thickness());
  CPT(RenderState) state = data._state->add_attrib(thick);
  if (get_use_vertex_color()) {
    state = state->add_attrib(ColorAttrib::make_vertex());
  }
  
  CullableObject *object = 
    new CullableObject(geom, state,
                       data.get_net_transform(trav),
                       data.get_modelview_transform(trav),
                       trav->get_gsg());
  trav->get_cull_handler()->record_object(object, trav);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::render_tape
//       Access: Private
//  Description: Draws the rope in RM_tape mode.  This draws a
//               series of triangle strips oriented to be
//               perpendicular to the tube_up vector.
//
//               In this mode, thickness is in spatial units, and
//               determines the width of the triangle strips.
////////////////////////////////////////////////////////////////////
void RopeNode::
render_tape(CullTraverser *trav, CullTraverserData &data, 
            NurbsCurveResult *result) const {
  CurveSegments curve_segments;
  get_connected_segments(curve_segments, result);

  // Now we have stored one or more sequences of vertices down the
  // center strips.  Go back through and calculate the vertices on
  // either side.
  PT(GeomVertexData) vdata = new GeomVertexData
    ("rope", get_format(false), Geom::UH_stream);
  
  compute_billboard_vertices(vdata, -get_tube_up(), 
                             curve_segments, result);
  
  PT(GeomTristrips) strip = new GeomTristrips(Geom::UH_stream);
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    
    strip->add_next_vertices(segment.size() * 2);
    strip->close_primitive();
  }
  
  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(strip);

  CPT(RenderState) state = data._state;
  if (get_use_vertex_color()) {
    state = state->add_attrib(ColorAttrib::make_vertex());
  }
  
  CullableObject *object = 
    new CullableObject(geom, state,
                       data.get_net_transform(trav),
                       data.get_modelview_transform(trav),
                       trav->get_gsg());
  trav->get_cull_handler()->record_object(object, trav);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::render_billboard
//       Access: Private
//  Description: Draws the rope in RM_billboard mode.  This draws a
//               series of triangle strips oriented to be
//               perpendicular to the camera plane.
//
//               In this mode, thickness is in spatial units, and
//               determines the width of the triangle strips.
////////////////////////////////////////////////////////////////////
void RopeNode::
render_billboard(CullTraverser *trav, CullTraverserData &data, 
                 NurbsCurveResult *result) const {
  const TransformState *net_transform = data.get_net_transform(trav);
  const TransformState *camera_transform = trav->get_camera_transform();

  CPT(TransformState) rel_transform =
    net_transform->invert_compose(camera_transform);
  LVector3f camera_vec = LVector3f::forward() * rel_transform->get_mat();

  CurveSegments curve_segments;
  get_connected_segments(curve_segments, result);

  // Now we have stored one or more sequences of vertices down the
  // center strips.  Go back through and calculate the vertices on
  // either side.
  PT(GeomVertexData) vdata = new GeomVertexData
    ("rope", get_format(false), Geom::UH_stream);
  
  compute_billboard_vertices(vdata, camera_vec, 
                             curve_segments, result);
  
  PT(GeomTristrips) strip = new GeomTristrips(Geom::UH_stream);
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    
    strip->add_next_vertices(segment.size() * 2);
    strip->close_primitive();
  }
  
  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(strip);

  CPT(RenderState) state = data._state;
  if (get_use_vertex_color()) {
    state = state->add_attrib(ColorAttrib::make_vertex());
  }
  
  CullableObject *object = 
    new CullableObject(geom, state,
                       data.get_net_transform(trav),
                       data.get_modelview_transform(trav),
                       trav->get_gsg());
  trav->get_cull_handler()->record_object(object, trav);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::render_tube
//       Access: Private
//  Description: Draws the rope in RM_tube mode.  This draws a hollow
//               tube centered around the string.
//
//               In this mode, thickness is in spatial units, and
//               determines the diameter of the tube.
////////////////////////////////////////////////////////////////////
void RopeNode::
render_tube(CullTraverser *trav, CullTraverserData &data, 
            NurbsCurveResult *result) const {
  CurveSegments curve_segments;
  get_connected_segments(curve_segments, result);

  // Now, we build up a table of vertices, in a series of rings
  // around the circumference of the tube.

  int num_slices = get_num_slices();
  int num_verts_per_slice;

  PT(GeomVertexData) vdata = new GeomVertexData
    ("rope", get_format(true), Geom::UH_stream);
  
  compute_tube_vertices(vdata, num_verts_per_slice, 
                        curve_segments, result);
  
  PT(GeomTristrips) strip = new GeomTristrips(Geom::UH_stream);
  // Finally, go through build up the index array, to tie all the
  // triangle strips together.
  int vi = 0;
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    
    for (int s = 0; s < num_slices; ++s) {
      int s1 = (s + 1) % num_verts_per_slice;
      
      for (size_t j = 0; j < segment.size(); ++j) {
        strip->add_vertex((vi + j) * num_verts_per_slice + s);
        strip->add_vertex((vi + j) * num_verts_per_slice + s1);
      }
      
      strip->close_primitive();
    }
    vi += (int)segment.size();
  }
  
  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(strip);

  CPT(RenderState) state = data._state;
  if (get_use_vertex_color()) {
    state = state->add_attrib(ColorAttrib::make_vertex());
  }
  
  CullableObject *object = 
    new CullableObject(geom, state,
                       data.get_net_transform(trav),
                       data.get_modelview_transform(trav),
                       trav->get_gsg());
  trav->get_cull_handler()->record_object(object, trav);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::get_connected_segments
//       Access: Private
//  Description: Evaluates the string of vertices along the curve, and
//               also breaks them up into connected segments.
//
//               Since the NurbsCurveEvaluator describes the curve as
//               a sequence of possibly-connected piecewise continuous
//               segments, this means joining together some adjacent
//               segments from the NurbsCurveEvaluator into a single
//               CurveSegment, if they happen to be connected (as most
//               will be).
////////////////////////////////////////////////////////////////////
void RopeNode::
get_connected_segments(RopeNode::CurveSegments &curve_segments,
                       const NurbsCurveResult *result) const {
  int num_verts = get_num_subdiv() + 1;
  int num_segments = result->get_num_segments();
  bool use_vertex_color = get_use_vertex_color();
  bool use_vertex_thickness = get_use_vertex_thickness();

  CurveSegment *curve_segment = NULL;
  LPoint3f last_point;

  for (int segment = 0; segment < num_segments; ++segment) {
    LPoint3f point;
    result->eval_segment_point(segment, 0.0f, point);

    if (curve_segment == (CurveSegment *)NULL || 
        !point.almost_equal(last_point)) {
      // If the first point of this segment is different from the last
      // point of the previous segment, end the previous segment and
      // begin a new one.
      curve_segments.push_back(CurveSegment());
      curve_segment = &curve_segments.back();

      CurveVertex vtx;
      vtx._p = point;
      vtx._t = result->get_segment_t(segment, 0.0f);
      if (use_vertex_color) {
        result->eval_segment_extended_points(segment, 0.0f, 
                                             get_vertex_color_dimension(), 
                                             &vtx._c[0], 4);
      }
      if (use_vertex_thickness) {
        vtx._thickness = 
          result->eval_segment_extended_point(segment, 0.0f, 
                                              get_vertex_thickness_dimension());
      }

      curve_segment->push_back(vtx);
    }

    // Store all the remaining points in this segment.
    for (int i = 1; i < num_verts; ++i) {
      float t = (float)i / (float)(num_verts - 1);

      CurveVertex vtx;
      result->eval_segment_point(segment, t, vtx._p);
      vtx._t = result->get_segment_t(segment, t);
      if (use_vertex_color) {
        result->eval_segment_extended_points(segment, t, 
                                             get_vertex_color_dimension(),
                                             &vtx._c[0], 4);
      }
      if (use_vertex_thickness) {
        vtx._thickness = 
          result->eval_segment_extended_point(segment, t, 
                                              get_vertex_thickness_dimension());
      }

      curve_segment->push_back(vtx);

      last_point = vtx._p;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::compute_thread_vertices
//       Access: Private
//  Description: Calculates the vertices for a RM_thread render.  This
//               just copies the vertices more-or-less directly into
//               the array.
////////////////////////////////////////////////////////////////////
void RopeNode::
compute_thread_vertices(GeomVertexData *vdata,
                        const RopeNode::CurveSegments &curve_segments) const {
  GeomVertexWriter vertex(vdata, InternalName::get_vertex());
  GeomVertexWriter color(vdata, InternalName::get_color());
  GeomVertexWriter texcoord(vdata, InternalName::get_texcoord());

  UVMode uv_mode = get_uv_mode();
  float uv_scale = get_uv_scale();
  bool u_dominant = get_uv_direction();
  bool use_vertex_color = get_use_vertex_color();

  float dist = 0.0f;
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    for (size_t j = 0; j < segment.size(); ++j) {
      vertex.add_data3f(segment[j]._p);

      if (use_vertex_color) {
        color.add_data4f(segment[j]._c);
      }

      float uv_t = compute_uv_t(dist, uv_mode, uv_scale, segment, j);

      if (uv_mode != UV_none) {
        if (u_dominant) {
          texcoord.add_data2f(uv_t, 0.0f);
        } else {
          texcoord.add_data2f(0.0f, uv_t);
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::compute_billboard_vertices
//       Access: Private
//  Description: Calculates the vertices for a RM_billboard render.  This
//               puts a pair of vertices on either side of each
//               computed point in curve_segments.
////////////////////////////////////////////////////////////////////
void RopeNode::
compute_billboard_vertices(GeomVertexData *vdata,
                           const LVector3f &camera_vec,
                           const RopeNode::CurveSegments &curve_segments,
                           NurbsCurveResult *result) const {
  GeomVertexWriter vertex(vdata, InternalName::get_vertex());
  GeomVertexWriter color(vdata, InternalName::get_color());
  GeomVertexWriter texcoord(vdata, InternalName::get_texcoord());

  float thickness = get_thickness();
  float overall_radius = thickness * 0.5f;
  float radius = overall_radius;
  UVMode uv_mode = get_uv_mode();
  float uv_scale = get_uv_scale();
  bool u_dominant = get_uv_direction();
  bool use_vertex_color = get_use_vertex_color();
  bool use_vertex_thickness = get_use_vertex_thickness();

  float dist = 0.0f;
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    for (size_t j = 0; j < segment.size(); ++j) {
      LVector3f tangent;
      compute_tangent(tangent, segment, j, result);

      LVector3f norm = cross(tangent, camera_vec);
      norm.normalize();

      if (use_vertex_thickness) {
        radius = overall_radius * segment[j]._thickness;
      }

      vertex.add_data3f(segment[j]._p + norm * radius);
      vertex.add_data3f(segment[j]._p - norm * radius);

      if (use_vertex_color) {
        color.add_data4f(segment[j]._c);
        color.add_data4f(segment[j]._c);
      }

      float uv_t = compute_uv_t(dist, uv_mode, uv_scale, segment, j);

      if (uv_mode != UV_none) {
        if (u_dominant) {
          texcoord.add_data2f(uv_t, 1.0f);
          texcoord.add_data2f(uv_t, 0.0f);
        } else {
          texcoord.add_data2f(1.0f, uv_t);
          texcoord.add_data2f(0.0f, uv_t);
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::compute_tube_vertices
//       Access: Private
//  Description: Calculates the vertices for a RM_tube render.  This
//               puts a ring of vertices around each computed point in
//               curve_segments.
////////////////////////////////////////////////////////////////////
void RopeNode::
compute_tube_vertices(GeomVertexData *vdata,
                      int &num_verts_per_slice,
                      const RopeNode::CurveSegments &curve_segments,
                      NurbsCurveResult *result) const {
  GeomVertexWriter vertex(vdata, InternalName::get_vertex());
  GeomVertexWriter normal(vdata, InternalName::get_normal());
  GeomVertexWriter color(vdata, InternalName::get_color());
  GeomVertexWriter texcoord(vdata, InternalName::get_texcoord());

  int num_slices = get_num_slices();
  num_verts_per_slice = num_slices;

  float thickness = get_thickness();
  float overall_radius = thickness * 0.5f;
  float radius = overall_radius;
  UVMode uv_mode = get_uv_mode();
  float uv_scale = get_uv_scale();
  bool u_dominant = get_uv_direction();
  NormalMode normal_mode = get_normal_mode();
  bool use_vertex_color = get_use_vertex_color();
  bool use_vertex_thickness = get_use_vertex_thickness();

  // If we are generating UV's, we will need to duplicate the vertices
  // along the seam so that the UV's go through the whole range of
  // 0..1 instead of reflecting in the last polygon before the seam.
  if (uv_mode != UV_none) {
    ++num_verts_per_slice;
  }

  LVector3f up = get_tube_up();

  float dist = 0.0f;
  CurveSegments::const_iterator si;
  for (si = curve_segments.begin(); si != curve_segments.end(); ++si) {
    const CurveSegment &segment = (*si);
    for (size_t j = 0; j < segment.size(); ++j) {
      LVector3f tangent;
      compute_tangent(tangent, segment, j, result);

      LVector3f norm = cross(tangent, up);
      norm.normalize();
      up = cross(norm, tangent);

      LMatrix3f rotate = LMatrix3f::rotate_mat(360.0f / (float)num_slices,
                                               tangent);

      float uv_t = compute_uv_t(dist, uv_mode, uv_scale, segment, j);

      for (int s = 0; s < num_verts_per_slice; ++s) {
        if (use_vertex_thickness) {
          radius = overall_radius * segment[j]._thickness;
        }

        vertex.add_data3f(segment[j]._p + norm * radius);

        if (normal_mode == NM_vertex) {
          normal.add_data3f(norm);
        }

        if (use_vertex_color) {
          color.add_data4f(segment[j]._c);
        }

        norm = norm * rotate;

        if (uv_mode != UV_none) {
          float uv_s = (float)s / (float)num_slices;
          if (u_dominant) {
            texcoord.add_data2f(uv_t, uv_s);
          } else {
            texcoord.add_data2f(uv_s, uv_t);
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::compute_tangent
//       Access: Private, Static
//  Description: Computes the tangent to the curve at the indicated
//               point in the segment.
////////////////////////////////////////////////////////////////////
void RopeNode::
compute_tangent(LVector3f &tangent, const RopeNode::CurveSegment &segment, 
                size_t j, NurbsCurveResult *result) {
  // First, try to evaluate the tangent at the curve.  This gives
  // better results at the ends at the endpoints where the tangent
  // does not go to zero.

  /*
    Actually, on second thought this looks terrible.

  if (result->eval_tangent(segment[j]._t, tangent)) {
    if (!tangent.almost_equal(LVector3f::zero())) {
      return;
    }
  }
  */

  // If that failed (or produced a zero tangent), then derive the
  // tangent from the neighboring points instead.
  if (j == 0) {
    tangent = segment[j + 1]._p - segment[j]._p;
  } else if (j == segment.size() - 1) {
    tangent = segment[j]._p - segment[j - 1]._p;
  } else {
    tangent = segment[j + 1]._p - segment[j - 1]._p;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::compute_uv_t
//       Access: Private, Static
//  Description: Computes the texture coordinate along the curve for
//               the indicated point in the segment.
////////////////////////////////////////////////////////////////////
float RopeNode::
compute_uv_t(float &dist, const RopeNode::UVMode &uv_mode,
             float uv_scale, const RopeNode::CurveSegment &segment,
             size_t j) {
  switch (uv_mode) {
  case UV_none:
    return 0.0f;
    
  case UV_parametric:
    return segment[j]._t * uv_scale;
    
  case UV_distance:
    if (j != 0) {
      LVector3f vec = segment[j]._p - segment[j - 1]._p;
      dist += vec.length();
    }
    return dist * uv_scale;
    
  case UV_distance2:
    if (j != 0) {
      LVector3f vec = segment[j]._p - segment[j - 1]._p;
      dist += vec.length_squared();
    }
    return dist * uv_scale;
  }

  return 0.0f;
}
  
////////////////////////////////////////////////////////////////////
//     Function: RopeNode::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               RopeNode.
////////////////////////////////////////////////////////////////////
void RopeNode::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void RopeNode::
write_datagram(BamWriter *manager, Datagram &dg) {
  PandaNode::write_datagram(manager, dg);
  manager->write_cdata(dg, _cycler);
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type RopeNode is encountered
//               in the Bam file.  It should create the RopeNode
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *RopeNode::
make_from_bam(const FactoryParams &params) {
  RopeNode *node = new RopeNode("");
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: RopeNode::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new RopeNode.
////////////////////////////////////////////////////////////////////
void RopeNode::
fillin(DatagramIterator &scan, BamReader *manager) {
  PandaNode::fillin(scan, manager);
  manager->read_cdata(scan, _cycler);
}
