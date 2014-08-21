#include <assert.h>
#include <string.h>

#include "utilities/color.h"

c_color::c_color()
{
	memset(&m_values[0], 0, k_color_channel_count * sizeof(real32));
}

c_color::c_color(
	real32	r,
	real32	g,
	real32	b,
	real32	a)
{
	m_values[0]= r;
	m_values[1]= g;
	m_values[2]= b;
	m_values[3]= a;
}

c_color::~c_color()
{

}

real32 c_color::operator[](int32 index)
{
	assert(IS_VALID_INDEX(index, k_color_channel_count));
	return m_values[index];
}