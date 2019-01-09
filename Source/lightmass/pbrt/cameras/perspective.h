
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

#ifndef PBRT_CAMERAS_PERSPECTIVE_H
#define PBRT_CAMERAS_PERSPECTIVE_H

// cameras/perspective.h*
#include "pbrt.h"
#include "camera.h"
#include "film.h"

// PerspectiveCamera Declarations
class PerspectiveCamera : public ProjectiveCamera 
{
public:
    // 构造函数
    PerspectiveCamera(const AnimatedTransform &cam2world, const float screenWindow[4], float sopen, float sclose, float lensr, float focald, float fov, Film *film);

	// 生成射线
    float GenerateRay(const CameraSample &sample, Ray *) const;

	// 生成射线
    float GenerateRayDifferential(const CameraSample &sample, RayDifferential *ray) const;

private:
    Vector dxCamera;	// PerspectiveCamera Private Data 
	Vector dyCamera;
};


PerspectiveCamera *CreatePerspectiveCamera(const ParamSet &params, const AnimatedTransform &cam2world, Film *film);

#endif // PBRT_CAMERAS_PERSPECTIVE_H
