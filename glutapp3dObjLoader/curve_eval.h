#pragma once
#pragma once
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_poly.h"
# include "math.h"

void eval_bezier(int n, GsArray<GsVec>& curve, const GsArray<GsVec>& ctrlpnts);
void eval_lagrange(int n, GsArray<GsVec>& curve, const GsArray<GsVec>& ctrlpnts);
GsVec eval_bsspline(int n, int k, float t, const GsArray<GsVec>& ctrlpnts);
GsVec eval_crspline(float i, int n, const GsArray<GsVec>& ctrlpnts);
GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts);
GsVec eval_bospline(float t, const GsArray<GsVec>& ctrlpnts);
static float N(int i, int k, float u);