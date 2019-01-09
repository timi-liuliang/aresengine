
/*
    pbrt source code Copyright(c) 1998-2010 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    pbrt is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.  Note that the text contents of
    the book "Physically Based Rendering" are *not* licensed under the
    GNU GPL.

    pbrt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef PBRT_CORE_DIFFGEOM_H
#define PBRT_CORE_DIFFGEOM_H

// core/diffgeom.h*
#include "pbrt.h"
#include "geometry.h"

//------------------------------------------
// DifferentialGeometry Declarations
//------------------------------------------
struct DifferentialGeometry 
{
	Point			p;						// The position of the 3d point
	Normal			nn;						// The surface normal at the point
	float			u, v;					// (u,v) coordinates from the parameterization of the surface
	Vector			dpdu, dpdv;				// The parametric partial derivatives ∂p/∂u and ∂p/∂v
	Normal			dndu, dndv;				// The partial derivatives of the change in surface normal ∂n/∂u and ∂n/∂v
	mutable Vector	dpdx, dpdy;
	mutable float	dudx, dvdx, dudy, dvdy;
	const Shape*	shape;					// A pointer to the shape that the differential geometry lies on

	// constructor
    DifferentialGeometry();

    // constructor
    DifferentialGeometry(const Point &P, const Vector &DPDU, const Vector &DPDV, const Normal &DNDU, const Normal &DNDV, float uu, float vv, const Shape *sh);

	// compute differentials
    void ComputeDifferentials(const RayDifferential &r) const;
};

#endif // PBRT_CORE_DIFFGEOM_H
