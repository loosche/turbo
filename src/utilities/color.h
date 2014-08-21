#ifndef __COLOR_H__
#define __COLOR_H__

#include "std_types.h"

const int32 k_color_channel_count= 4;

class c_color
{
public:

	c_color();
	c_color(real32 r, real32 g, real32 b, real32 a);
	~c_color();

	inline real32 a() { return m_values[3]; }
	
	inline real32 b() { return m_values[2]; }
	
	inline real32 g() { return m_values[1]; }

	inline real32 operator[] (int32 index);

	inline real32 r() { return m_values[0]; }

	inline const real32 *values() const { return m_values; }

private:

	real32	m_values[k_color_channel_count];

};

const c_color k_color_blue(0,0,1,1);

#endif __COLOR_H__