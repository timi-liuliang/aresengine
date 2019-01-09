
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

#ifndef PBRT_CORE_SCENE_H
#define PBRT_CORE_SCENE_H

// core/scene.h*
#include "pbrt.h"
#include "primitive.h"
#include "integrator.h"

// Scene Declarations
class Scene 
{
public:
    // 构造函数
    Scene(Primitive *accel, const vector<Light *> &lts, VolumeRegion *vr);

	// 析构函数
    ~Scene();

	// 射线跟踪
    bool Intersect(const Ray &ray, Intersection *isect) const;

	// 相交判断
    bool IntersectP(const Ray &ray) const;

	// 返回世界包围盒
    const BBox &WorldBound() const;

public:
	// lightmap Baking
	bool IntersectBakingSurface( const Ray& uvspaceRay, Intersection* isect) const;

public:
    Primitive*      aggregate;		// 场景中几体体对象
    vector<Light*>  lights;			// 光源对象
    VolumeRegion*	volumeRegion;	// 体积区域,大气,尘埃等。
    BBox			bound;			// 场景范围(有向包围盒)
};

#endif // PBRT_CORE_SCENE_H
