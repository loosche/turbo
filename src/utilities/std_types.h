#ifndef __STD_TYPES_H__
#define __STD_TYPES_H__

typedef signed short	int16;
typedef signed int		int32;

typedef float			real32;

typedef unsigned int	uint32;

#define IS_VALID_INDEX(index, max_index)	((index >= 0) && (index < max_index))

#endif __STD_TYPES_H__