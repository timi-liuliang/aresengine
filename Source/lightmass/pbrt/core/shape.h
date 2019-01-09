
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

#ifndef PBRT_CORE_SHAPE_H
#define PBRT_CORE_SHAPE_H

// core/shape.h*
#include "pbrt.h"
#include "geometry.h"
#include "transform.h"
#include "diffgeom.h"
#include "memory.h"

// Shape Declarations
class Shape : public ReferenceCounted 
{
public:
    // constructor
    Shape(const Transform *o2w, const Transform *w2o, bool ro);

	// destructor
    virtual ~Shape();

	// 本地空间包围盒
    virtual BBox ObjectBound() const = 0;

	// 世界空间中的包围盒
    virtual BBox WorldBound() const;

	//  是否可相交
    virtual bool CanIntersect() const;

	// 细分
    virtual void Refine(vector<Reference<Shape> > &refined) const;

	// 射线检测(detail)
    virtual bool Intersect(const Ray &ray, float *tHit,float *rayEpsilon, DifferentialGeometry *dg) const;

	// 快速检测
    virtual bool IntersectP(const Ray &ray) const;

	// returns the shading geometry corresponding to the differentialGeometry
    virtual void GetShadingGeometry(const Transform &obj2world, const DifferentialGeometry &dg,  DifferentialGeometry *dgShading) const {  *dgShading = dg;}

	// 求表面积，arealight使用
    virtual float Area() const;

	// 采样
    virtual Point Sample(float u1, float u2, Normal *Ns) const { Severe("Unimplemented Shape::Sample() method called");  return Point(); }
   
	// not sure
	virtual float Pdf(const Point &Pshape) const { return 1.f / Area(); }

	// 采样
    virtual Point Sample(const Point &P, float u1, float u2, Normal *Ns) const { return Sample(u1, u2, Ns);}
    
	// pdf
	virtual float Pdf(const Point &p, const Vector &wi) const;

public:
    const Transform *ObjectToWorld, *WorldToObject;
    const bool ReverseOrientation, TransformSwapsHandedness;
    const uint32_t shapeId;
    static uint32_t nextshapeId;
};



#endif // PBRT_CORE_SHAPE_H
