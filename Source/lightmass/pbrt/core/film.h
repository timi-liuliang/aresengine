
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

#ifndef PBRT_CORE_FILM_H
#define PBRT_CORE_FILM_H

// core/film.h*
#include "pbrt.h"

// Film Declarations
class Film 
{
public:
    // 构造函数 x方向与y方向上的分辨率
    Film(int xres, int yres): xResolution(xres), yResolution(yres) { }

	// 析构函数
    virtual ~Film();

public:
	// 以一个采样及其对应的相机光线，辐射亮度值和alpha值做为参数来更新图象
    virtual void AddSample(const CameraSample &sample, const Spectrum &L) = 0;

    virtual void Splat(const CameraSample &sample, const Spectrum &L) = 0;

	// 决定采样器进行采样所需要的整数像素值的范围
    virtual void GetSampleExtent(int *xstart, int *xend, int *ystart, int *yend) const = 0;

    virtual void GetPixelExtent(int *xstart, int *xend, int *ystart, int *yend) const = 0;

    virtual void UpdateDisplay(int x0, int y0, int x1, int y1, float splatScale = 1.f);

	// 该函数允许对图像做任意的处理工作，然后再显示或存盘
    virtual void WriteImage(float splatScale = 1.f) = 0;

public:
    const int xResolution;	// x方向分辨率
	const int yResolution;	// y方向分辨率
};



#endif // PBRT_CORE_FILM_H
