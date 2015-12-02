#pragma once


// Ensure the header file is included only once in multi-file projects
#ifndef SO_LIGHT_H
#define SO_LIGHT_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:


class light : public GlObjects {
public:
	GsVec pos;
	GsColor amb, dif, spe;



	light(const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s)
		:pos(p), amb(a), dif(d), spe(s) {}


	light() {};



};
#endif // SO_LIGHT_H

#pragma once
