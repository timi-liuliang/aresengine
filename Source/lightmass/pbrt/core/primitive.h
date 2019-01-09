
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

#ifndef PBRT_CORE_PRIMITIVE_H
#define PBRT_CORE_PRIMITIVE_H

// core/primitive.h*
#include "pbrt.h"
#include "shape.h"
#include "material.h"

//--------------------------------------
// Primitive Declarations 2012-3-21 阅
// connects geometry and shading.
//--------------------------------------
class Primitive : public ReferenceCounted 
{
public:
    // 构造函数
    Primitive() : primitiveId(nextprimitiveId++) { }

	// 析构函数
    virtual ~Primitive();

	// geometry相关
public:
	// returns a box that encloses the primitive geometry in world space
    virtual BBox WorldBound() const = 0;

	// 能否进行相交检测
    virtual bool CanIntersect() const;

	// 射线检测, 返回相交信息
    virtual bool Intersect(const Ray& r, Intersection *in) const = 0;

	// 快速射线检测,仅判断是否相交
    virtual bool IntersectP(const Ray& r) const = 0;

	// Refind
    virtual void Refine(vector<Reference<Primitive> > &refined) const;

	// 完全Refined
    void FullyRefine(vector<Reference<Primitive> > &refined) const;

	// 材质相关
public:
	// returns a pointer to the AreaLight that describes the primitive’s 
	// emission distribution, if the primitive is itself a light source. 
	// If the primitive is not emissive, this method should return NULL.
    virtual const AreaLight *GetAreaLight() const = 0;

	// returns a BSDF object that describes local light-scattering properties at the intersection point.
    virtual BSDF *GetBSDF(const DifferentialGeometry &dg, const Transform &ObjectToWorld, MemoryArena &arena) const = 0;
    
	// BSSRDF
	virtual BSSRDF *GetBSSRDF(const DifferentialGeometry &dg, const Transform &ObjectToWorld, MemoryArena &arena) const = 0;

public:
    const uint32_t primitiveId;		// Primitive Public Data

protected:   
    static uint32_t nextprimitiveId;// Primitive Protected Data
};

//---------------------------------------------------------
// 几何元素 2012-3-21 阅
// The GeometricPrimitve class represents a single shape
// in the scene. Each geometricPrimitives holds a reference
// to a shape and its material.
//---------------------------------------------------------
class GeometricPrimitive : public Primitive 
{
public:
	// 构造函数
	GeometricPrimitive(const Reference<Shape> &s, const Reference<Material> &m, AreaLight *a);

    // 是否可相交
    bool CanIntersect() const;
    void Refine(vector<Reference<Primitive> > &refined) const;
    virtual BBox WorldBound() const;

	// 相交检测
    virtual bool Intersect(const Ray &r, Intersection *isect) const;
    virtual bool IntersectP(const Ray &r) const;
    const AreaLight *GetAreaLight() const;
    BSDF *GetBSDF(const DifferentialGeometry &dg,
                  const Transform &ObjectToWorld, MemoryArena &arena) const;
    BSSRDF *GetBSSRDF(const DifferentialGeometry &dg,
                      const Transform &ObjectToWorld, MemoryArena &arena) const;

private:
    Reference<Shape>	shape;		// 几何形状
    Reference<Material> material;	// 材质
    AreaLight*			areaLight;	// 是否发光
};

//-------------------------------------------------
// TransformedPrimitive Declarations
//-------------------------------------------------
class TransformedPrimitive : public Primitive
{
public:
    // 构造函数
    TransformedPrimitive( Reference<Primitive> &prim,  const AnimatedTransform &w2p)
        : primitive(prim), WorldToPrimitive(w2p) 
	{ }

	// 相交检测
    bool Intersect(const Ray &r, Intersection *in) const;

	// 快速相交检测
    bool IntersectP(const Ray &r) const;

	// 世界包围盒
	BBox WorldBound() const 
	{
		return WorldToPrimitive.MotionBounds(primitive->WorldBound(), true);
	}

private:
	// 获取AresLight
    const AreaLight *GetAreaLight() const { return NULL; }

	// GetBSDF
    BSDF *GetBSDF(const DifferentialGeometry &dg, const Transform &ObjectToWorld, MemoryArena &arena) const { return NULL;}

	// GetBSSRDF
    BSSRDF *GetBSSRDF(const DifferentialGeometry &dg, const Transform &ObjectToWorld, MemoryArena &arena) const { return NULL;}

private:
    Reference<Primitive>	primitive;			// 几何体
    const AnimatedTransform WorldToPrimitive;	// 世界到几体体空间
};

//-----------------------------------------
// Aggregate Declarations
//-----------------------------------------
class Aggregate : public Primitive 
{
public:
    // Aggregate Public Methods
    const AreaLight *GetAreaLight() const;

	// BSDF
    BSDF *GetBSDF(const DifferentialGeometry &dg, const Transform &, MemoryArena &) const;

	// BSRDF
    BSSRDF *GetBSSRDF(const DifferentialGeometry &dg, const Transform &, MemoryArena &) const;
};



#endif // PBRT_CORE_PRIMITIVE_H
