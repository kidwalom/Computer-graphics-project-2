

// Ensure the header file is included only once in multi-file projects
#ifndef MATERIAL_H
#define MATERIAL_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:



class mat : public GlObjects {

public:
	GsColor amb, dif, spe;
	GsVec pos;
	float shine;

	mat(const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s, float sn)
		:pos(p), amb(a), dif(d), spe(s), shine(sn) {}

	mat() {};
};
#endif // SO_LIGHT_H

#pragma once
