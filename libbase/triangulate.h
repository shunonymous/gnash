// triangulate.h	-- Thatcher Ulrich 2004

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code to triangulate arbitrary 2D polygonal regions.


#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include "container.h"
#include "tu_types.h"

namespace triangulate
{
	// Input is a list of closed paths that form the overall
	// region.  The region does not need to be connected, and may
	// contain holes/islands.  The paths should not intersect each
	// other, or themselves.
	// 
	// The paths should be in counter-clockwise orientation,
	// meaning that the inside of the shape is to the left of the
	// directed path.  So a closed region is in CCW order, while
	// an island inside a closed region is in CW (reverse) order.
	//
	// The output is a triangle list; each tri consists of 6
	// coords: (x0, y0, x1, y1, x2, y2).

	// Version using float coords
	void	compute(
		std::vector<float>* results,
		int path_count,
		const std::vector<float> paths[],
		int debug_halt_step = -1,
		std::vector<float>* debug_remaining_loop = NULL);

	// Version using short coords
	void	compute(
		std::vector<sint16>* results,	// indexed trilist
		int path_count,
		const std::vector<sint16> paths[],
		int debug_halt_step = -1,
		std::vector<sint16>* debug_remaining_loop = NULL);

	// Version using int coords
	void	compute(
		std::vector<sint32>* results,	// indexed trilist
		int path_count,
		const std::vector<sint32> paths[],
		int debug_halt_step = -1,
		std::vector<sint32>* debug_remaining_loop = NULL);
}


#endif // TRIANGULATE
