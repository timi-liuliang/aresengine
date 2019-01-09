/*
// glloader
// Copyright (C) 2004-2009 Minmin Gong
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <glloader/glloader.h>
#include "utils.h"

#ifdef GLLOADER_EGL

#ifdef __cplusplus
extern "C"
{
#endif

char _EGL_ANGLE_d3d_share_handle_client_buffer = 0;
char _EGL_ANGLE_query_surface_pointer = 0;
char _EGL_ANGLE_surface_d3d_texture_2d_share_handle = 0;
char _EGL_EXT_create_context_robustness = 0;
char _EGL_EXT_multiview_window = 0;
char _EGL_HI_clientpixmap = 0;
char _EGL_HI_colorformats = 0;
char _EGL_IMG_context_priority = 0;
char _EGL_KHR_config_attribs = 0;
char _EGL_KHR_create_context = 0;
char _EGL_KHR_fence_sync = 0;
char _EGL_KHR_gl_texture_2D_image = 0;
char _EGL_KHR_image = 0;
char _EGL_KHR_image_base = 0;
char _EGL_KHR_image_pixmap = 0;
char _EGL_KHR_lock_surface = 0;
char _EGL_KHR_reusable_sync = 0;
char _EGL_KHR_stream = 0;
char _EGL_KHR_stream_consumer_gltexture = 0;
char _EGL_KHR_stream_cross_process_fd = 0;
char _EGL_KHR_stream_fifo = 0;
char _EGL_KHR_stream_producer_aldatalocator = 0;
char _EGL_KHR_stream_producer_eglsurface = 0;
char _EGL_KHR_surfaceless_context = 0;
char _EGL_KHR_vg_parent_image = 0;
char _EGL_KHR_wait_sync = 0;
char _EGL_MESA_drm_image = 0;
char _EGL_NV_coverage_sample = 0;
char _EGL_NV_coverage_sample_resolve = 0;
char _EGL_NV_depth_nonlinear = 0;
char _EGL_NV_post_sub_buffer = 0;
char _EGL_NV_sync = 0;
char _EGL_NV_system_time = 0;
char _EGL_VERSION_1_1 = 0;
char _EGL_VERSION_1_2 = 0;
char _EGL_VERSION_1_3 = 0;
char _EGL_VERSION_1_4 = 0;

static char GLLOADER_APIENTRY _glloader_EGL_ANGLE_d3d_share_handle_client_buffer()
{
	return _EGL_ANGLE_d3d_share_handle_client_buffer;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_ANGLE_d3d_share_handle_client_buffer()
{
	glloader_init();
	return glloader_EGL_ANGLE_d3d_share_handle_client_buffer();
}
glloader_EGL_ANGLE_d3d_share_handle_client_bufferFUNC glloader_EGL_ANGLE_d3d_share_handle_client_buffer = self_init_glloader_EGL_ANGLE_d3d_share_handle_client_buffer;

static char GLLOADER_APIENTRY _glloader_EGL_ANGLE_query_surface_pointer()
{
	return _EGL_ANGLE_query_surface_pointer;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_ANGLE_query_surface_pointer()
{
	glloader_init();
	return glloader_EGL_ANGLE_query_surface_pointer();
}
glloader_EGL_ANGLE_query_surface_pointerFUNC glloader_EGL_ANGLE_query_surface_pointer = self_init_glloader_EGL_ANGLE_query_surface_pointer;

#ifdef EGL_ANGLE_query_surface_pointer

static EGLBoolean GLLOADER_APIENTRY self_init_eglQuerySurfacePointerANGLE(EGLDisplay dpy, EGLSurface surface, EGLint attribute, void** value)
{
	glloader_init();
	return eglQuerySurfacePointerANGLE(dpy, surface, attribute, value);
}

eglQuerySurfacePointerANGLEFUNC eglQuerySurfacePointerANGLE = self_init_eglQuerySurfacePointerANGLE;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle()
{
	return _EGL_ANGLE_surface_d3d_texture_2d_share_handle;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle()
{
	glloader_init();
	return glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle();
}
glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handleFUNC glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle = self_init_glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle;

static char GLLOADER_APIENTRY _glloader_EGL_EXT_create_context_robustness()
{
	return _EGL_EXT_create_context_robustness;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_EXT_create_context_robustness()
{
	glloader_init();
	return glloader_EGL_EXT_create_context_robustness();
}
glloader_EGL_EXT_create_context_robustnessFUNC glloader_EGL_EXT_create_context_robustness = self_init_glloader_EGL_EXT_create_context_robustness;

static char GLLOADER_APIENTRY _glloader_EGL_EXT_multiview_window()
{
	return _EGL_EXT_multiview_window;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_EXT_multiview_window()
{
	glloader_init();
	return glloader_EGL_EXT_multiview_window();
}
glloader_EGL_EXT_multiview_windowFUNC glloader_EGL_EXT_multiview_window = self_init_glloader_EGL_EXT_multiview_window;

static char GLLOADER_APIENTRY _glloader_EGL_HI_clientpixmap()
{
	return _EGL_HI_clientpixmap;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_HI_clientpixmap()
{
	glloader_init();
	return glloader_EGL_HI_clientpixmap();
}
glloader_EGL_HI_clientpixmapFUNC glloader_EGL_HI_clientpixmap = self_init_glloader_EGL_HI_clientpixmap;

#ifdef EGL_HI_clientpixmap

static void GLLOADER_APIENTRY self_init_eglCreatePixmapSurfaceHI(EGLDisplay dpy, EGLConfig config, EGLClientPixmapHI* pixmap)
{
	glloader_init();
	eglCreatePixmapSurfaceHI(dpy, config, pixmap);
}

eglCreatePixmapSurfaceHIFUNC eglCreatePixmapSurfaceHI = self_init_eglCreatePixmapSurfaceHI;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_HI_colorformats()
{
	return _EGL_HI_colorformats;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_HI_colorformats()
{
	glloader_init();
	return glloader_EGL_HI_colorformats();
}
glloader_EGL_HI_colorformatsFUNC glloader_EGL_HI_colorformats = self_init_glloader_EGL_HI_colorformats;

static char GLLOADER_APIENTRY _glloader_EGL_IMG_context_priority()
{
	return _EGL_IMG_context_priority;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_IMG_context_priority()
{
	glloader_init();
	return glloader_EGL_IMG_context_priority();
}
glloader_EGL_IMG_context_priorityFUNC glloader_EGL_IMG_context_priority = self_init_glloader_EGL_IMG_context_priority;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_config_attribs()
{
	return _EGL_KHR_config_attribs;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_config_attribs()
{
	glloader_init();
	return glloader_EGL_KHR_config_attribs();
}
glloader_EGL_KHR_config_attribsFUNC glloader_EGL_KHR_config_attribs = self_init_glloader_EGL_KHR_config_attribs;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_create_context()
{
	return _EGL_KHR_create_context;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_create_context()
{
	glloader_init();
	return glloader_EGL_KHR_create_context();
}
glloader_EGL_KHR_create_contextFUNC glloader_EGL_KHR_create_context = self_init_glloader_EGL_KHR_create_context;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_fence_sync()
{
	return _EGL_KHR_fence_sync;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_fence_sync()
{
	glloader_init();
	return glloader_EGL_KHR_fence_sync();
}
glloader_EGL_KHR_fence_syncFUNC glloader_EGL_KHR_fence_sync = self_init_glloader_EGL_KHR_fence_sync;

#ifdef EGL_KHR_fence_sync

static EGLSyncKHR GLLOADER_APIENTRY self_init_eglCreateSyncKHR(EGLDisplay dpy, EGLenum type, const EGLint* attrib_list)
{
	glloader_init();
	return eglCreateSyncKHR(dpy, type, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglDestroySyncKHR(EGLDisplay dpy, EGLSyncKHR sync)
{
	glloader_init();
	return eglDestroySyncKHR(dpy, sync);
}
static EGLint GLLOADER_APIENTRY self_init_eglClientWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout)
{
	glloader_init();
	return eglClientWaitSyncKHR(dpy, sync, flags, timeout);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglGetSyncAttribKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint* value)
{
	glloader_init();
	return eglGetSyncAttribKHR(dpy, sync, attribute, value);
}

eglCreateSyncKHRFUNC eglCreateSyncKHR = self_init_eglCreateSyncKHR;
eglDestroySyncKHRFUNC eglDestroySyncKHR = self_init_eglDestroySyncKHR;
eglClientWaitSyncKHRFUNC eglClientWaitSyncKHR = self_init_eglClientWaitSyncKHR;
eglGetSyncAttribKHRFUNC eglGetSyncAttribKHR = self_init_eglGetSyncAttribKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_gl_texture_2D_image()
{
	return _EGL_KHR_gl_texture_2D_image;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_gl_texture_2D_image()
{
	glloader_init();
	return glloader_EGL_KHR_gl_texture_2D_image();
}
glloader_EGL_KHR_gl_texture_2D_imageFUNC glloader_EGL_KHR_gl_texture_2D_image = self_init_glloader_EGL_KHR_gl_texture_2D_image;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_image()
{
	return _EGL_KHR_image;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_image()
{
	glloader_init();
	return glloader_EGL_KHR_image();
}
glloader_EGL_KHR_imageFUNC glloader_EGL_KHR_image = self_init_glloader_EGL_KHR_image;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_image_base()
{
	return _EGL_KHR_image_base;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_image_base()
{
	glloader_init();
	return glloader_EGL_KHR_image_base();
}
glloader_EGL_KHR_image_baseFUNC glloader_EGL_KHR_image_base = self_init_glloader_EGL_KHR_image_base;

#ifdef EGL_KHR_image_base

static EGLImageKHR GLLOADER_APIENTRY self_init_eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint* attrib_list)
{
	glloader_init();
	return eglCreateImageKHR(dpy, ctx, target, buffer, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image)
{
	glloader_init();
	return eglDestroyImageKHR(dpy, image);
}

eglCreateImageKHRFUNC eglCreateImageKHR = self_init_eglCreateImageKHR;
eglDestroyImageKHRFUNC eglDestroyImageKHR = self_init_eglDestroyImageKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_image_pixmap()
{
	return _EGL_KHR_image_pixmap;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_image_pixmap()
{
	glloader_init();
	return glloader_EGL_KHR_image_pixmap();
}
glloader_EGL_KHR_image_pixmapFUNC glloader_EGL_KHR_image_pixmap = self_init_glloader_EGL_KHR_image_pixmap;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_lock_surface()
{
	return _EGL_KHR_lock_surface;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_lock_surface()
{
	glloader_init();
	return glloader_EGL_KHR_lock_surface();
}
glloader_EGL_KHR_lock_surfaceFUNC glloader_EGL_KHR_lock_surface = self_init_glloader_EGL_KHR_lock_surface;

#ifdef EGL_KHR_lock_surface

static EGLBoolean GLLOADER_APIENTRY self_init_eglLockSurfaceKHR(EGLDisplay display, EGLSurface surface, const EGLint* attrib_list)
{
	glloader_init();
	return eglLockSurfaceKHR(display, surface, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglUnlockSurfaceKHR(EGLDisplay display, EGLSurface surface)
{
	glloader_init();
	return eglUnlockSurfaceKHR(display, surface);
}

eglLockSurfaceKHRFUNC eglLockSurfaceKHR = self_init_eglLockSurfaceKHR;
eglUnlockSurfaceKHRFUNC eglUnlockSurfaceKHR = self_init_eglUnlockSurfaceKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_reusable_sync()
{
	return _EGL_KHR_reusable_sync;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_reusable_sync()
{
	glloader_init();
	return glloader_EGL_KHR_reusable_sync();
}
glloader_EGL_KHR_reusable_syncFUNC glloader_EGL_KHR_reusable_sync = self_init_glloader_EGL_KHR_reusable_sync;

#ifdef EGL_KHR_reusable_sync

static EGLBoolean GLLOADER_APIENTRY self_init_eglSignalSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode)
{
	glloader_init();
	return eglSignalSyncKHR(dpy, sync, mode);
}

eglSignalSyncKHRFUNC eglSignalSyncKHR = self_init_eglSignalSyncKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_stream()
{
	return _EGL_KHR_stream;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_stream()
{
	glloader_init();
	return glloader_EGL_KHR_stream();
}
glloader_EGL_KHR_streamFUNC glloader_EGL_KHR_stream = self_init_glloader_EGL_KHR_stream;

#ifdef EGL_KHR_stream

static EGLStreamKHR GLLOADER_APIENTRY self_init_eglCreateStreamKHR(EGLDisplay dpy, const EGLint* attrib_list)
{
	glloader_init();
	return eglCreateStreamKHR(dpy, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglDestroyStreamKHR(EGLDisplay dpy, EGLStreamKHR stream)
{
	glloader_init();
	return eglDestroyStreamKHR(dpy, stream);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglStreamAttribKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint value)
{
	glloader_init();
	return eglStreamAttribKHR(dpy, stream, attribute, value);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglQueryStreamKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint* value)
{
	glloader_init();
	return eglQueryStreamKHR(dpy, stream, attribute, value);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglQueryStreamu64KHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLuint64KHR* value)
{
	glloader_init();
	return eglQueryStreamu64KHR(dpy, stream, attribute, value);
}

eglCreateStreamKHRFUNC eglCreateStreamKHR = self_init_eglCreateStreamKHR;
eglDestroyStreamKHRFUNC eglDestroyStreamKHR = self_init_eglDestroyStreamKHR;
eglStreamAttribKHRFUNC eglStreamAttribKHR = self_init_eglStreamAttribKHR;
eglQueryStreamKHRFUNC eglQueryStreamKHR = self_init_eglQueryStreamKHR;
eglQueryStreamu64KHRFUNC eglQueryStreamu64KHR = self_init_eglQueryStreamu64KHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_stream_consumer_gltexture()
{
	return _EGL_KHR_stream_consumer_gltexture;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_stream_consumer_gltexture()
{
	glloader_init();
	return glloader_EGL_KHR_stream_consumer_gltexture();
}
glloader_EGL_KHR_stream_consumer_gltextureFUNC glloader_EGL_KHR_stream_consumer_gltexture = self_init_glloader_EGL_KHR_stream_consumer_gltexture;

#ifdef EGL_KHR_stream_consumer_gltexture

static EGLBoolean GLLOADER_APIENTRY self_init_eglStreamConsumerGLTextureExternalKHR(EGLDisplay dpy, EGLStreamKHR stream)
{
	glloader_init();
	return eglStreamConsumerGLTextureExternalKHR(dpy, stream);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglStreamConsumerAcquireKHR(EGLDisplay dpy, EGLStreamKHR stream)
{
	glloader_init();
	return eglStreamConsumerAcquireKHR(dpy, stream);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglStreamConsumerReleaseKHR(EGLDisplay dpy, EGLStreamKHR stream)
{
	glloader_init();
	return eglStreamConsumerReleaseKHR(dpy, stream);
}

eglStreamConsumerGLTextureExternalKHRFUNC eglStreamConsumerGLTextureExternalKHR = self_init_eglStreamConsumerGLTextureExternalKHR;
eglStreamConsumerAcquireKHRFUNC eglStreamConsumerAcquireKHR = self_init_eglStreamConsumerAcquireKHR;
eglStreamConsumerReleaseKHRFUNC eglStreamConsumerReleaseKHR = self_init_eglStreamConsumerReleaseKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_stream_cross_process_fd()
{
	return _EGL_KHR_stream_cross_process_fd;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_stream_cross_process_fd()
{
	glloader_init();
	return glloader_EGL_KHR_stream_cross_process_fd();
}
glloader_EGL_KHR_stream_cross_process_fdFUNC glloader_EGL_KHR_stream_cross_process_fd = self_init_glloader_EGL_KHR_stream_cross_process_fd;

#ifdef EGL_KHR_stream_cross_process_fd

static EGLNativeFileDescriptorKHR GLLOADER_APIENTRY self_init_eglGetStreamFileDescriptorKHR(EGLDisplay dpy, EGLStreamKHR stream)
{
	glloader_init();
	return eglGetStreamFileDescriptorKHR(dpy, stream);
}
static EGLStreamKHR GLLOADER_APIENTRY self_init_eglCreateStreamFromFileDescriptorKHR(EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor)
{
	glloader_init();
	return eglCreateStreamFromFileDescriptorKHR(dpy, file_descriptor);
}

eglGetStreamFileDescriptorKHRFUNC eglGetStreamFileDescriptorKHR = self_init_eglGetStreamFileDescriptorKHR;
eglCreateStreamFromFileDescriptorKHRFUNC eglCreateStreamFromFileDescriptorKHR = self_init_eglCreateStreamFromFileDescriptorKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_stream_fifo()
{
	return _EGL_KHR_stream_fifo;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_stream_fifo()
{
	glloader_init();
	return glloader_EGL_KHR_stream_fifo();
}
glloader_EGL_KHR_stream_fifoFUNC glloader_EGL_KHR_stream_fifo = self_init_glloader_EGL_KHR_stream_fifo;

#ifdef EGL_KHR_stream_fifo

static EGLBoolean GLLOADER_APIENTRY self_init_eglQueryStreamTimeKHR(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLTimeKHR* value)
{
	glloader_init();
	return eglQueryStreamTimeKHR(dpy, stream, attribute, value);
}

eglQueryStreamTimeKHRFUNC eglQueryStreamTimeKHR = self_init_eglQueryStreamTimeKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_stream_producer_aldatalocator()
{
	return _EGL_KHR_stream_producer_aldatalocator;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_stream_producer_aldatalocator()
{
	glloader_init();
	return glloader_EGL_KHR_stream_producer_aldatalocator();
}
glloader_EGL_KHR_stream_producer_aldatalocatorFUNC glloader_EGL_KHR_stream_producer_aldatalocator = self_init_glloader_EGL_KHR_stream_producer_aldatalocator;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_stream_producer_eglsurface()
{
	return _EGL_KHR_stream_producer_eglsurface;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_stream_producer_eglsurface()
{
	glloader_init();
	return glloader_EGL_KHR_stream_producer_eglsurface();
}
glloader_EGL_KHR_stream_producer_eglsurfaceFUNC glloader_EGL_KHR_stream_producer_eglsurface = self_init_glloader_EGL_KHR_stream_producer_eglsurface;

#ifdef EGL_KHR_stream_producer_eglsurface

static EGLSurface GLLOADER_APIENTRY self_init_eglCreateStreamProducerSurface(EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream, const EGLint* attrib_list)
{
	glloader_init();
	return eglCreateStreamProducerSurface(dpy, config, stream, attrib_list);
}

eglCreateStreamProducerSurfaceFUNC eglCreateStreamProducerSurface = self_init_eglCreateStreamProducerSurface;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_KHR_surfaceless_context()
{
	return _EGL_KHR_surfaceless_context;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_surfaceless_context()
{
	glloader_init();
	return glloader_EGL_KHR_surfaceless_context();
}
glloader_EGL_KHR_surfaceless_contextFUNC glloader_EGL_KHR_surfaceless_context = self_init_glloader_EGL_KHR_surfaceless_context;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_vg_parent_image()
{
	return _EGL_KHR_vg_parent_image;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_vg_parent_image()
{
	glloader_init();
	return glloader_EGL_KHR_vg_parent_image();
}
glloader_EGL_KHR_vg_parent_imageFUNC glloader_EGL_KHR_vg_parent_image = self_init_glloader_EGL_KHR_vg_parent_image;

static char GLLOADER_APIENTRY _glloader_EGL_KHR_wait_sync()
{
	return _EGL_KHR_wait_sync;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_KHR_wait_sync()
{
	glloader_init();
	return glloader_EGL_KHR_wait_sync();
}
glloader_EGL_KHR_wait_syncFUNC glloader_EGL_KHR_wait_sync = self_init_glloader_EGL_KHR_wait_sync;

#ifdef EGL_KHR_wait_sync

static EGLint GLLOADER_APIENTRY self_init_eglWaitSyncKHR(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags)
{
	glloader_init();
	return eglWaitSyncKHR(dpy, sync, flags);
}

eglWaitSyncKHRFUNC eglWaitSyncKHR = self_init_eglWaitSyncKHR;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_MESA_drm_image()
{
	return _EGL_MESA_drm_image;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_MESA_drm_image()
{
	glloader_init();
	return glloader_EGL_MESA_drm_image();
}
glloader_EGL_MESA_drm_imageFUNC glloader_EGL_MESA_drm_image = self_init_glloader_EGL_MESA_drm_image;

#ifdef EGL_MESA_drm_image

static EGLImageKHR GLLOADER_APIENTRY self_init_eglCreateDRMImageMESA(EGLDisplay dpy, const EGLint* attrib_list)
{
	glloader_init();
	return eglCreateDRMImageMESA(dpy, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglExportDRMImageMESA(EGLDisplay dpy, EGLImageKHR image, EGLint* name, EGLint* handle, EGLint* stride)
{
	glloader_init();
	return eglExportDRMImageMESA(dpy, image, name, handle, stride);
}

eglCreateDRMImageMESAFUNC eglCreateDRMImageMESA = self_init_eglCreateDRMImageMESA;
eglExportDRMImageMESAFUNC eglExportDRMImageMESA = self_init_eglExportDRMImageMESA;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_NV_coverage_sample()
{
	return _EGL_NV_coverage_sample;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_NV_coverage_sample()
{
	glloader_init();
	return glloader_EGL_NV_coverage_sample();
}
glloader_EGL_NV_coverage_sampleFUNC glloader_EGL_NV_coverage_sample = self_init_glloader_EGL_NV_coverage_sample;

static char GLLOADER_APIENTRY _glloader_EGL_NV_coverage_sample_resolve()
{
	return _EGL_NV_coverage_sample_resolve;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_NV_coverage_sample_resolve()
{
	glloader_init();
	return glloader_EGL_NV_coverage_sample_resolve();
}
glloader_EGL_NV_coverage_sample_resolveFUNC glloader_EGL_NV_coverage_sample_resolve = self_init_glloader_EGL_NV_coverage_sample_resolve;

static char GLLOADER_APIENTRY _glloader_EGL_NV_depth_nonlinear()
{
	return _EGL_NV_depth_nonlinear;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_NV_depth_nonlinear()
{
	glloader_init();
	return glloader_EGL_NV_depth_nonlinear();
}
glloader_EGL_NV_depth_nonlinearFUNC glloader_EGL_NV_depth_nonlinear = self_init_glloader_EGL_NV_depth_nonlinear;

static char GLLOADER_APIENTRY _glloader_EGL_NV_post_sub_buffer()
{
	return _EGL_NV_post_sub_buffer;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_NV_post_sub_buffer()
{
	glloader_init();
	return glloader_EGL_NV_post_sub_buffer();
}
glloader_EGL_NV_post_sub_bufferFUNC glloader_EGL_NV_post_sub_buffer = self_init_glloader_EGL_NV_post_sub_buffer;

#ifdef EGL_NV_post_sub_buffer

static EGLBoolean GLLOADER_APIENTRY self_init_eglPostSubBufferNV(EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height)
{
	glloader_init();
	return eglPostSubBufferNV(dpy, surface, x, y, width, height);
}

eglPostSubBufferNVFUNC eglPostSubBufferNV = self_init_eglPostSubBufferNV;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_NV_sync()
{
	return _EGL_NV_sync;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_NV_sync()
{
	glloader_init();
	return glloader_EGL_NV_sync();
}
glloader_EGL_NV_syncFUNC glloader_EGL_NV_sync = self_init_glloader_EGL_NV_sync;

#ifdef EGL_NV_sync

static EGLSyncNV GLLOADER_APIENTRY self_init_eglCreateFenceSyncNV(EGLDisplay dpy, EGLenum condition, const EGLint* attrib_list)
{
	glloader_init();
	return eglCreateFenceSyncNV(dpy, condition, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglDestroySyncNV(EGLSyncNV sync)
{
	glloader_init();
	return eglDestroySyncNV(sync);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglFenceNV(EGLSyncNV sync)
{
	glloader_init();
	return eglFenceNV(sync);
}
static EGLint GLLOADER_APIENTRY self_init_eglClientWaitSyncNV(EGLSyncNV sync, EGLint flags, EGLTimeNV timeout)
{
	glloader_init();
	return eglClientWaitSyncNV(sync, flags, timeout);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglSignalSyncNV(EGLSyncNV sync, EGLenum mode)
{
	glloader_init();
	return eglSignalSyncNV(sync, mode);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglGetSyncAttribNV(EGLSyncNV sync, EGLint attribute, EGLint* value)
{
	glloader_init();
	return eglGetSyncAttribNV(sync, attribute, value);
}

eglCreateFenceSyncNVFUNC eglCreateFenceSyncNV = self_init_eglCreateFenceSyncNV;
eglDestroySyncNVFUNC eglDestroySyncNV = self_init_eglDestroySyncNV;
eglFenceNVFUNC eglFenceNV = self_init_eglFenceNV;
eglClientWaitSyncNVFUNC eglClientWaitSyncNV = self_init_eglClientWaitSyncNV;
eglSignalSyncNVFUNC eglSignalSyncNV = self_init_eglSignalSyncNV;
eglGetSyncAttribNVFUNC eglGetSyncAttribNV = self_init_eglGetSyncAttribNV;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_NV_system_time()
{
	return _EGL_NV_system_time;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_NV_system_time()
{
	glloader_init();
	return glloader_EGL_NV_system_time();
}
glloader_EGL_NV_system_timeFUNC glloader_EGL_NV_system_time = self_init_glloader_EGL_NV_system_time;

#ifdef EGL_NV_system_time

static EGLuint64NV GLLOADER_APIENTRY self_init_eglGetSystemTimeFrequencyNV()
{
	glloader_init();
	return eglGetSystemTimeFrequencyNV();
}
static EGLuint64NV GLLOADER_APIENTRY self_init_eglGetSystemTimeNV()
{
	glloader_init();
	return eglGetSystemTimeNV();
}

eglGetSystemTimeFrequencyNVFUNC eglGetSystemTimeFrequencyNV = self_init_eglGetSystemTimeFrequencyNV;
eglGetSystemTimeNVFUNC eglGetSystemTimeNV = self_init_eglGetSystemTimeNV;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_VERSION_1_1()
{
	return _EGL_VERSION_1_1;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_VERSION_1_1()
{
	glloader_init();
	return glloader_EGL_VERSION_1_1();
}
glloader_EGL_VERSION_1_1FUNC glloader_EGL_VERSION_1_1 = self_init_glloader_EGL_VERSION_1_1;

#ifdef EGL_VERSION_1_1

static EGLint GLLOADER_APIENTRY self_init_eglGetError()
{
	LOAD_FUNC1(eglGetError);
	return eglGetError();
}
static EGLDisplay GLLOADER_APIENTRY self_init_eglGetDisplay(EGLNativeDisplayType display_id)
{
	LOAD_FUNC1(eglGetDisplay);
	return eglGetDisplay(display_id);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglInitialize(EGLDisplay dpy, EGLint* major, EGLint* minor)
{
	LOAD_FUNC1(eglInitialize);
	return eglInitialize(dpy, major, minor);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglTerminate(EGLDisplay dpy)
{
	LOAD_FUNC1(eglTerminate);
	return eglTerminate(dpy);
}
static const char* GLLOADER_APIENTRY self_init_eglQueryString(EGLDisplay dpy, EGLint name)
{
	LOAD_FUNC1(eglQueryString);
	return eglQueryString(dpy, name);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglGetConfigs(EGLDisplay dpy, EGLConfig* configs, EGLint config_size, EGLint* num_config)
{
	LOAD_FUNC1(eglGetConfigs);
	return eglGetConfigs(dpy, configs, config_size, num_config);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglChooseConfig(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config)
{
	LOAD_FUNC1(eglChooseConfig);
	return eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig configs, EGLint attribute, EGLint* value)
{
	LOAD_FUNC1(eglGetConfigAttrib);
	return eglGetConfigAttrib(dpy, configs, attribute, value);
}
static EGLSurface GLLOADER_APIENTRY self_init_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig configs, EGLNativeWindowType win, const EGLint* attrib_list)
{
	LOAD_FUNC1(eglCreateWindowSurface);
	return eglCreateWindowSurface(dpy, configs, win, attrib_list);
}
static EGLSurface GLLOADER_APIENTRY self_init_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig configs, const EGLint* attrib_list)
{
	LOAD_FUNC1(eglCreatePbufferSurface);
	return eglCreatePbufferSurface(dpy, configs, attrib_list);
}
static EGLSurface GLLOADER_APIENTRY self_init_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig configs, EGLNativePixmapType pixmap, const EGLint* attrib_list)
{
	LOAD_FUNC1(eglCreatePixmapSurface);
	return eglCreatePixmapSurface(dpy, configs, pixmap, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	LOAD_FUNC1(eglDestroySurface);
	return eglDestroySurface(dpy, surface);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value)
{
	LOAD_FUNC1(eglQuerySurface);
	return eglQuerySurface(dpy, surface, attribute, value);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
	LOAD_FUNC1(eglSurfaceAttrib);
	return eglSurfaceAttrib(dpy, surface, attribute, value);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	LOAD_FUNC1(eglBindTexImage);
	return eglBindTexImage(dpy, surface, buffer);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
	LOAD_FUNC1(eglReleaseTexImage);
	return eglReleaseTexImage(dpy, surface, buffer);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
	LOAD_FUNC1(eglSwapInterval);
	return eglSwapInterval(dpy, interval);
}
static EGLContext GLLOADER_APIENTRY self_init_eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint* attrib_list)
{
	LOAD_FUNC1(eglCreateContext);
	return eglCreateContext(dpy, config, share_context, attrib_list);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
	LOAD_FUNC1(eglDestroyContext);
	return eglDestroyContext(dpy, ctx);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
	LOAD_FUNC1(eglMakeCurrent);
	return eglMakeCurrent(dpy, draw, read, ctx);
}
static EGLContext GLLOADER_APIENTRY self_init_eglGetCurrentContext()
{
	LOAD_FUNC1(eglGetCurrentContext);
	return eglGetCurrentContext();
}
static EGLSurface GLLOADER_APIENTRY self_init_eglGetCurrentSurface(EGLint readdraw)
{
	LOAD_FUNC1(eglGetCurrentSurface);
	return eglGetCurrentSurface(readdraw);
}
static EGLDisplay GLLOADER_APIENTRY self_init_eglGetCurrentDisplay()
{
	LOAD_FUNC1(eglGetCurrentDisplay);
	return eglGetCurrentDisplay();
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value)
{
	LOAD_FUNC1(eglQueryContext);
	return eglQueryContext(dpy, ctx, attribute, value);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglWaitGL()
{
	LOAD_FUNC1(eglWaitGL);
	return eglWaitGL();
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglWaitNative(EGLint engine)
{
	LOAD_FUNC1(eglWaitNative);
	return eglWaitNative(engine);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	LOAD_FUNC1(eglSwapBuffers);
	return eglSwapBuffers(dpy, surface);
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
	LOAD_FUNC1(eglCopyBuffers);
	return eglCopyBuffers(dpy, surface, target);
}
static __eglMustCastToProperFunctionPointerType GLLOADER_APIENTRY self_init_eglGetProcAddress(const char* procname)
{
	LOAD_FUNC1(eglGetProcAddress);
	return eglGetProcAddress(procname);
}

eglGetErrorFUNC eglGetError = self_init_eglGetError;
eglGetDisplayFUNC eglGetDisplay = self_init_eglGetDisplay;
eglInitializeFUNC eglInitialize = self_init_eglInitialize;
eglTerminateFUNC eglTerminate = self_init_eglTerminate;
eglQueryStringFUNC eglQueryString = self_init_eglQueryString;
eglGetConfigsFUNC eglGetConfigs = self_init_eglGetConfigs;
eglChooseConfigFUNC eglChooseConfig = self_init_eglChooseConfig;
eglGetConfigAttribFUNC eglGetConfigAttrib = self_init_eglGetConfigAttrib;
eglCreateWindowSurfaceFUNC eglCreateWindowSurface = self_init_eglCreateWindowSurface;
eglCreatePbufferSurfaceFUNC eglCreatePbufferSurface = self_init_eglCreatePbufferSurface;
eglCreatePixmapSurfaceFUNC eglCreatePixmapSurface = self_init_eglCreatePixmapSurface;
eglDestroySurfaceFUNC eglDestroySurface = self_init_eglDestroySurface;
eglQuerySurfaceFUNC eglQuerySurface = self_init_eglQuerySurface;
eglSurfaceAttribFUNC eglSurfaceAttrib = self_init_eglSurfaceAttrib;
eglBindTexImageFUNC eglBindTexImage = self_init_eglBindTexImage;
eglReleaseTexImageFUNC eglReleaseTexImage = self_init_eglReleaseTexImage;
eglSwapIntervalFUNC eglSwapInterval = self_init_eglSwapInterval;
eglCreateContextFUNC eglCreateContext = self_init_eglCreateContext;
eglDestroyContextFUNC eglDestroyContext = self_init_eglDestroyContext;
eglMakeCurrentFUNC eglMakeCurrent = self_init_eglMakeCurrent;
eglGetCurrentContextFUNC eglGetCurrentContext = self_init_eglGetCurrentContext;
eglGetCurrentSurfaceFUNC eglGetCurrentSurface = self_init_eglGetCurrentSurface;
eglGetCurrentDisplayFUNC eglGetCurrentDisplay = self_init_eglGetCurrentDisplay;
eglQueryContextFUNC eglQueryContext = self_init_eglQueryContext;
eglWaitGLFUNC eglWaitGL = self_init_eglWaitGL;
eglWaitNativeFUNC eglWaitNative = self_init_eglWaitNative;
eglSwapBuffersFUNC eglSwapBuffers = self_init_eglSwapBuffers;
eglCopyBuffersFUNC eglCopyBuffers = self_init_eglCopyBuffers;
eglGetProcAddressFUNC eglGetProcAddress = self_init_eglGetProcAddress;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_VERSION_1_2()
{
	return _EGL_VERSION_1_2;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_VERSION_1_2()
{
	glloader_init();
	return glloader_EGL_VERSION_1_2();
}
glloader_EGL_VERSION_1_2FUNC glloader_EGL_VERSION_1_2 = self_init_glloader_EGL_VERSION_1_2;

#ifdef EGL_VERSION_1_2

static EGLBoolean GLLOADER_APIENTRY self_init_eglBindAPI(EGLenum api)
{
	LOAD_FUNC1(eglBindAPI);
	return eglBindAPI(api);
}
static EGLenum GLLOADER_APIENTRY self_init_eglQueryAPI()
{
	LOAD_FUNC1(eglQueryAPI);
	return eglQueryAPI();
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglWaitClient()
{
	LOAD_FUNC1(eglWaitClient);
	return eglWaitClient();
}
static EGLBoolean GLLOADER_APIENTRY self_init_eglReleaseThread()
{
	LOAD_FUNC1(eglReleaseThread);
	return eglReleaseThread();
}
static EGLSurface GLLOADER_APIENTRY self_init_eglCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint* attrib_list)
{
	LOAD_FUNC1(eglCreatePbufferFromClientBuffer);
	return eglCreatePbufferFromClientBuffer(dpy, buftype, buffer, config, attrib_list);
}

eglBindAPIFUNC eglBindAPI = self_init_eglBindAPI;
eglQueryAPIFUNC eglQueryAPI = self_init_eglQueryAPI;
eglWaitClientFUNC eglWaitClient = self_init_eglWaitClient;
eglReleaseThreadFUNC eglReleaseThread = self_init_eglReleaseThread;
eglCreatePbufferFromClientBufferFUNC eglCreatePbufferFromClientBuffer = self_init_eglCreatePbufferFromClientBuffer;

#endif

static char GLLOADER_APIENTRY _glloader_EGL_VERSION_1_3()
{
	return _EGL_VERSION_1_3;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_VERSION_1_3()
{
	glloader_init();
	return glloader_EGL_VERSION_1_3();
}
glloader_EGL_VERSION_1_3FUNC glloader_EGL_VERSION_1_3 = self_init_glloader_EGL_VERSION_1_3;

static char GLLOADER_APIENTRY _glloader_EGL_VERSION_1_4()
{
	return _EGL_VERSION_1_4;
}

static char GLLOADER_APIENTRY self_init_glloader_EGL_VERSION_1_4()
{
	glloader_init();
	return glloader_EGL_VERSION_1_4();
}
glloader_EGL_VERSION_1_4FUNC glloader_EGL_VERSION_1_4 = self_init_glloader_EGL_VERSION_1_4;

void init_EGL_ANGLE_d3d_share_handle_client_buffer()
{
	glloader_EGL_ANGLE_d3d_share_handle_client_buffer = _glloader_EGL_ANGLE_d3d_share_handle_client_buffer;

	_EGL_ANGLE_d3d_share_handle_client_buffer = 0;
	if (glloader_is_supported("EGL_ANGLE_d3d_share_handle_client_buffer"))
	{
		_EGL_ANGLE_d3d_share_handle_client_buffer = 1;
	}
}

void init_EGL_ANGLE_query_surface_pointer()
{
	glloader_EGL_ANGLE_query_surface_pointer = _glloader_EGL_ANGLE_query_surface_pointer;

	{
		eglQuerySurfacePointerANGLE = NULL;
	}

	_EGL_ANGLE_query_surface_pointer = 0;
	if (glloader_is_supported("EGL_ANGLE_query_surface_pointer"))
	{
		_EGL_ANGLE_query_surface_pointer = 1;

		LOAD_FUNC1(eglQuerySurfacePointerANGLE);
	}
}

void init_EGL_ANGLE_surface_d3d_texture_2d_share_handle()
{
	glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle = _glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle;

	_EGL_ANGLE_surface_d3d_texture_2d_share_handle = 0;
	if (glloader_is_supported("EGL_ANGLE_surface_d3d_texture_2d_share_handle"))
	{
		_EGL_ANGLE_surface_d3d_texture_2d_share_handle = 1;
	}
}

void init_EGL_EXT_create_context_robustness()
{
	glloader_EGL_EXT_create_context_robustness = _glloader_EGL_EXT_create_context_robustness;

	_EGL_EXT_create_context_robustness = 0;
	if (glloader_is_supported("EGL_EXT_create_context_robustness"))
	{
		_EGL_EXT_create_context_robustness = 1;
	}
}

void init_EGL_EXT_multiview_window()
{
	glloader_EGL_EXT_multiview_window = _glloader_EGL_EXT_multiview_window;

	_EGL_EXT_multiview_window = 0;
	if (glloader_is_supported("EGL_EXT_multiview_window"))
	{
		_EGL_EXT_multiview_window = 1;
	}
}

void init_EGL_HI_clientpixmap()
{
	glloader_EGL_HI_clientpixmap = _glloader_EGL_HI_clientpixmap;

	{
		eglCreatePixmapSurfaceHI = NULL;
	}

	_EGL_HI_clientpixmap = 0;
	if (glloader_is_supported("EGL_HI_clientpixmap"))
	{
		_EGL_HI_clientpixmap = 1;

		LOAD_FUNC1(eglCreatePixmapSurfaceHI);
	}
}

void init_EGL_HI_colorformats()
{
	glloader_EGL_HI_colorformats = _glloader_EGL_HI_colorformats;

	_EGL_HI_colorformats = 0;
	if (glloader_is_supported("EGL_HI_colorformats"))
	{
		_EGL_HI_colorformats = 1;
	}
}

void init_EGL_IMG_context_priority()
{
	glloader_EGL_IMG_context_priority = _glloader_EGL_IMG_context_priority;

	_EGL_IMG_context_priority = 0;
	if (glloader_is_supported("EGL_IMG_context_priority"))
	{
		_EGL_IMG_context_priority = 1;
	}
}

void init_EGL_KHR_config_attribs()
{
	glloader_EGL_KHR_config_attribs = _glloader_EGL_KHR_config_attribs;

	_EGL_KHR_config_attribs = 0;
	if (glloader_is_supported("EGL_KHR_config_attribs"))
	{
		_EGL_KHR_config_attribs = 1;
	}
}

void init_EGL_KHR_create_context()
{
	glloader_EGL_KHR_create_context = _glloader_EGL_KHR_create_context;

	_EGL_KHR_create_context = 0;
	if (glloader_is_supported("EGL_KHR_create_context"))
	{
		_EGL_KHR_create_context = 1;
	}
}

void init_EGL_KHR_fence_sync()
{
	glloader_EGL_KHR_fence_sync = _glloader_EGL_KHR_fence_sync;

	{
		eglCreateSyncKHR = NULL;
		eglDestroySyncKHR = NULL;
		eglClientWaitSyncKHR = NULL;
		eglGetSyncAttribKHR = NULL;
	}

	_EGL_KHR_fence_sync = 0;
	if (glloader_is_supported("EGL_KHR_fence_sync"))
	{
		_EGL_KHR_fence_sync = 1;

		LOAD_FUNC1(eglCreateSyncKHR);
		LOAD_FUNC1(eglDestroySyncKHR);
		LOAD_FUNC1(eglClientWaitSyncKHR);
		LOAD_FUNC1(eglGetSyncAttribKHR);
	}
}

void init_EGL_KHR_gl_texture_2D_image()
{
	glloader_EGL_KHR_gl_texture_2D_image = _glloader_EGL_KHR_gl_texture_2D_image;

	_EGL_KHR_gl_texture_2D_image = 0;
	if (glloader_is_supported("EGL_KHR_gl_texture_2D_image"))
	{
		_EGL_KHR_gl_texture_2D_image = 1;
	}
}

void init_EGL_KHR_image()
{
	glloader_EGL_KHR_image = _glloader_EGL_KHR_image;

	_EGL_KHR_image = 0;
	if (glloader_is_supported("EGL_KHR_image"))
	{
		_EGL_KHR_image = 1;
	}
}

void init_EGL_KHR_image_base()
{
	glloader_EGL_KHR_image_base = _glloader_EGL_KHR_image_base;

	{
		eglCreateImageKHR = NULL;
		eglDestroyImageKHR = NULL;
	}

	_EGL_KHR_image_base = 0;
	if (glloader_is_supported("EGL_KHR_image_base"))
	{
		_EGL_KHR_image_base = 1;

		LOAD_FUNC1(eglCreateImageKHR);
		LOAD_FUNC1(eglDestroyImageKHR);
	}
}

void init_EGL_KHR_image_pixmap()
{
	glloader_EGL_KHR_image_pixmap = _glloader_EGL_KHR_image_pixmap;

	_EGL_KHR_image_pixmap = 0;
	if (glloader_is_supported("EGL_KHR_image_pixmap"))
	{
		_EGL_KHR_image_pixmap = 1;
	}
}

void init_EGL_KHR_lock_surface()
{
	glloader_EGL_KHR_lock_surface = _glloader_EGL_KHR_lock_surface;

	{
		eglLockSurfaceKHR = NULL;
		eglUnlockSurfaceKHR = NULL;
	}

	_EGL_KHR_lock_surface = 0;
	if (glloader_is_supported("EGL_KHR_lock_surface"))
	{
		_EGL_KHR_lock_surface = 1;

		LOAD_FUNC1(eglLockSurfaceKHR);
		LOAD_FUNC1(eglUnlockSurfaceKHR);
	}
}

void init_EGL_KHR_reusable_sync()
{
	glloader_EGL_KHR_reusable_sync = _glloader_EGL_KHR_reusable_sync;

	{
		eglCreateSyncKHR = NULL;
		eglDestroySyncKHR = NULL;
		eglClientWaitSyncKHR = NULL;
		eglSignalSyncKHR = NULL;
		eglGetSyncAttribKHR = NULL;
	}

	_EGL_KHR_reusable_sync = 0;
	if (glloader_is_supported("EGL_KHR_reusable_sync"))
	{
		_EGL_KHR_reusable_sync = 1;

		LOAD_FUNC1(eglCreateSyncKHR);
		LOAD_FUNC1(eglDestroySyncKHR);
		LOAD_FUNC1(eglClientWaitSyncKHR);
		LOAD_FUNC1(eglSignalSyncKHR);
		LOAD_FUNC1(eglGetSyncAttribKHR);
	}
}

void init_EGL_KHR_stream()
{
	glloader_EGL_KHR_stream = _glloader_EGL_KHR_stream;

	{
		eglCreateStreamKHR = NULL;
		eglDestroyStreamKHR = NULL;
		eglStreamAttribKHR = NULL;
		eglQueryStreamKHR = NULL;
		eglQueryStreamu64KHR = NULL;
	}

	_EGL_KHR_stream = 0;
	if (glloader_is_supported("EGL_KHR_stream"))
	{
		_EGL_KHR_stream = 1;

		LOAD_FUNC1(eglCreateStreamKHR);
		LOAD_FUNC1(eglDestroyStreamKHR);
		LOAD_FUNC1(eglStreamAttribKHR);
		LOAD_FUNC1(eglQueryStreamKHR);
		LOAD_FUNC1(eglQueryStreamu64KHR);
	}
}

void init_EGL_KHR_stream_consumer_gltexture()
{
	glloader_EGL_KHR_stream_consumer_gltexture = _glloader_EGL_KHR_stream_consumer_gltexture;

	{
		eglStreamConsumerGLTextureExternalKHR = NULL;
		eglStreamConsumerAcquireKHR = NULL;
		eglStreamConsumerReleaseKHR = NULL;
	}

	_EGL_KHR_stream_consumer_gltexture = 0;
	if (glloader_is_supported("EGL_KHR_stream_consumer_gltexture"))
	{
		_EGL_KHR_stream_consumer_gltexture = 1;

		LOAD_FUNC1(eglStreamConsumerGLTextureExternalKHR);
		LOAD_FUNC1(eglStreamConsumerAcquireKHR);
		LOAD_FUNC1(eglStreamConsumerReleaseKHR);
	}
}

void init_EGL_KHR_stream_cross_process_fd()
{
	glloader_EGL_KHR_stream_cross_process_fd = _glloader_EGL_KHR_stream_cross_process_fd;

	{
		eglGetStreamFileDescriptorKHR = NULL;
		eglCreateStreamFromFileDescriptorKHR = NULL;
	}

	_EGL_KHR_stream_cross_process_fd = 0;
	if (glloader_is_supported("EGL_KHR_stream_cross_process_fd"))
	{
		_EGL_KHR_stream_cross_process_fd = 1;

		LOAD_FUNC1(eglGetStreamFileDescriptorKHR);
		LOAD_FUNC1(eglCreateStreamFromFileDescriptorKHR);
	}
}

void init_EGL_KHR_stream_fifo()
{
	glloader_EGL_KHR_stream_fifo = _glloader_EGL_KHR_stream_fifo;

	{
		eglQueryStreamTimeKHR = NULL;
	}

	_EGL_KHR_stream_fifo = 0;
	if (glloader_is_supported("EGL_KHR_stream_fifo"))
	{
		_EGL_KHR_stream_fifo = 1;

		LOAD_FUNC1(eglQueryStreamTimeKHR);
	}
}

void init_EGL_KHR_stream_producer_aldatalocator()
{
	glloader_EGL_KHR_stream_producer_aldatalocator = _glloader_EGL_KHR_stream_producer_aldatalocator;

	_EGL_KHR_stream_producer_aldatalocator = 0;
	if (glloader_is_supported("EGL_KHR_stream_producer_aldatalocator"))
	{
		_EGL_KHR_stream_producer_aldatalocator = 1;
	}
}

void init_EGL_KHR_stream_producer_eglsurface()
{
	glloader_EGL_KHR_stream_producer_eglsurface = _glloader_EGL_KHR_stream_producer_eglsurface;

	{
		eglCreateStreamProducerSurface = NULL;
	}

	_EGL_KHR_stream_producer_eglsurface = 0;
	if (glloader_is_supported("EGL_KHR_stream_producer_eglsurface"))
	{
		_EGL_KHR_stream_producer_eglsurface = 1;

		LOAD_FUNC1(eglCreateStreamProducerSurface);
	}
}

void init_EGL_KHR_surfaceless_context()
{
	glloader_EGL_KHR_surfaceless_context = _glloader_EGL_KHR_surfaceless_context;

	_EGL_KHR_surfaceless_context = 0;
	if (glloader_is_supported("EGL_KHR_surfaceless_context"))
	{
		_EGL_KHR_surfaceless_context = 1;
	}
}

void init_EGL_KHR_vg_parent_image()
{
	glloader_EGL_KHR_vg_parent_image = _glloader_EGL_KHR_vg_parent_image;

	_EGL_KHR_vg_parent_image = 0;
	if (glloader_is_supported("EGL_KHR_vg_parent_image"))
	{
		_EGL_KHR_vg_parent_image = 1;
	}
}

void init_EGL_KHR_wait_sync()
{
	glloader_EGL_KHR_wait_sync = _glloader_EGL_KHR_wait_sync;

	{
		eglWaitSyncKHR = NULL;
	}

	_EGL_KHR_wait_sync = 0;
	if (glloader_is_supported("EGL_KHR_wait_sync"))
	{
		_EGL_KHR_wait_sync = 1;

		LOAD_FUNC1(eglWaitSyncKHR);
	}
}

void init_EGL_MESA_drm_image()
{
	glloader_EGL_MESA_drm_image = _glloader_EGL_MESA_drm_image;

	{
		eglCreateDRMImageMESA = NULL;
		eglExportDRMImageMESA = NULL;
	}

	_EGL_MESA_drm_image = 0;
	if (glloader_is_supported("EGL_MESA_drm_image"))
	{
		_EGL_MESA_drm_image = 1;

		LOAD_FUNC1(eglCreateDRMImageMESA);
		LOAD_FUNC1(eglExportDRMImageMESA);
	}
}

void init_EGL_NV_coverage_sample()
{
	glloader_EGL_NV_coverage_sample = _glloader_EGL_NV_coverage_sample;

	_EGL_NV_coverage_sample = 0;
	if (glloader_is_supported("EGL_NV_coverage_sample"))
	{
		_EGL_NV_coverage_sample = 1;
	}
}

void init_EGL_NV_coverage_sample_resolve()
{
	glloader_EGL_NV_coverage_sample_resolve = _glloader_EGL_NV_coverage_sample_resolve;

	_EGL_NV_coverage_sample_resolve = 0;
	if (glloader_is_supported("EGL_NV_coverage_sample_resolve"))
	{
		_EGL_NV_coverage_sample_resolve = 1;
	}
}

void init_EGL_NV_depth_nonlinear()
{
	glloader_EGL_NV_depth_nonlinear = _glloader_EGL_NV_depth_nonlinear;

	_EGL_NV_depth_nonlinear = 0;
	if (glloader_is_supported("EGL_NV_depth_nonlinear"))
	{
		_EGL_NV_depth_nonlinear = 1;
	}
}

void init_EGL_NV_post_sub_buffer()
{
	glloader_EGL_NV_post_sub_buffer = _glloader_EGL_NV_post_sub_buffer;

	{
		eglPostSubBufferNV = NULL;
	}

	_EGL_NV_post_sub_buffer = 0;
	if (glloader_is_supported("EGL_NV_post_sub_buffer"))
	{
		_EGL_NV_post_sub_buffer = 1;

		LOAD_FUNC1(eglPostSubBufferNV);
	}
}

void init_EGL_NV_sync()
{
	glloader_EGL_NV_sync = _glloader_EGL_NV_sync;

	{
		eglCreateFenceSyncNV = NULL;
		eglDestroySyncNV = NULL;
		eglFenceNV = NULL;
		eglClientWaitSyncNV = NULL;
		eglSignalSyncNV = NULL;
		eglGetSyncAttribNV = NULL;
	}

	_EGL_NV_sync = 0;
	if (glloader_is_supported("EGL_NV_sync"))
	{
		_EGL_NV_sync = 1;

		LOAD_FUNC1(eglCreateFenceSyncNV);
		LOAD_FUNC1(eglDestroySyncNV);
		LOAD_FUNC1(eglFenceNV);
		LOAD_FUNC1(eglClientWaitSyncNV);
		LOAD_FUNC1(eglSignalSyncNV);
		LOAD_FUNC1(eglGetSyncAttribNV);
	}
}

void init_EGL_NV_system_time()
{
	glloader_EGL_NV_system_time = _glloader_EGL_NV_system_time;

	{
		eglGetSystemTimeFrequencyNV = NULL;
		eglGetSystemTimeNV = NULL;
	}

	_EGL_NV_system_time = 0;
	if (glloader_is_supported("EGL_NV_system_time"))
	{
		_EGL_NV_system_time = 1;

		LOAD_FUNC1(eglGetSystemTimeFrequencyNV);
		LOAD_FUNC1(eglGetSystemTimeNV);
	}
}

void init_EGL_VERSION_1_1()
{
	glloader_EGL_VERSION_1_1 = _glloader_EGL_VERSION_1_1;

	_EGL_VERSION_1_1 = 0;
	if (glloader_is_supported("EGL_VERSION_1_1"))
	{
		_EGL_VERSION_1_1 = 1;

		LOAD_FUNC1(eglGetError);
		LOAD_FUNC1(eglGetDisplay);
		LOAD_FUNC1(eglInitialize);
		LOAD_FUNC1(eglTerminate);
		LOAD_FUNC1(eglQueryString);
		LOAD_FUNC1(eglGetConfigs);
		LOAD_FUNC1(eglChooseConfig);
		LOAD_FUNC1(eglGetConfigAttrib);
		LOAD_FUNC1(eglCreateWindowSurface);
		LOAD_FUNC1(eglCreatePbufferSurface);
		LOAD_FUNC1(eglCreatePixmapSurface);
		LOAD_FUNC1(eglDestroySurface);
		LOAD_FUNC1(eglQuerySurface);
		LOAD_FUNC1(eglSurfaceAttrib);
		LOAD_FUNC1(eglBindTexImage);
		LOAD_FUNC1(eglReleaseTexImage);
		LOAD_FUNC1(eglSwapInterval);
		LOAD_FUNC1(eglCreateContext);
		LOAD_FUNC1(eglDestroyContext);
		LOAD_FUNC1(eglMakeCurrent);
		LOAD_FUNC1(eglGetCurrentContext);
		LOAD_FUNC1(eglGetCurrentSurface);
		LOAD_FUNC1(eglGetCurrentDisplay);
		LOAD_FUNC1(eglQueryContext);
		LOAD_FUNC1(eglWaitGL);
		LOAD_FUNC1(eglWaitNative);
		LOAD_FUNC1(eglSwapBuffers);
		LOAD_FUNC1(eglCopyBuffers);
		LOAD_FUNC1(eglGetProcAddress);
	}
}

void init_EGL_VERSION_1_2()
{
	glloader_EGL_VERSION_1_2 = _glloader_EGL_VERSION_1_2;

	_EGL_VERSION_1_2 = 0;
	if (glloader_is_supported("EGL_VERSION_1_2"))
	{
		_EGL_VERSION_1_2 = 1;

		LOAD_FUNC1(eglBindAPI);
		LOAD_FUNC1(eglQueryAPI);
		LOAD_FUNC1(eglWaitClient);
		LOAD_FUNC1(eglReleaseThread);
		LOAD_FUNC1(eglCreatePbufferFromClientBuffer);
	}
}

void init_EGL_VERSION_1_3()
{
	glloader_EGL_VERSION_1_3 = _glloader_EGL_VERSION_1_3;

	_EGL_VERSION_1_3 = 0;
	if (glloader_is_supported("EGL_VERSION_1_3"))
	{
		_EGL_VERSION_1_3 = 1;
	}
}

void init_EGL_VERSION_1_4()
{
	glloader_EGL_VERSION_1_4 = _glloader_EGL_VERSION_1_4;

	_EGL_VERSION_1_4 = 0;
	if (glloader_is_supported("EGL_VERSION_1_4"))
	{
		_EGL_VERSION_1_4 = 1;
	}
}


void egl_init()
{
	init_EGL_ANGLE_d3d_share_handle_client_buffer();
	init_EGL_ANGLE_query_surface_pointer();
	init_EGL_ANGLE_surface_d3d_texture_2d_share_handle();
	init_EGL_EXT_create_context_robustness();
	init_EGL_EXT_multiview_window();
	init_EGL_HI_clientpixmap();
	init_EGL_HI_colorformats();
	init_EGL_IMG_context_priority();
	init_EGL_KHR_config_attribs();
	init_EGL_KHR_create_context();
	init_EGL_KHR_fence_sync();
	init_EGL_KHR_gl_texture_2D_image();
	init_EGL_KHR_image();
	init_EGL_KHR_image_base();
	init_EGL_KHR_image_pixmap();
	init_EGL_KHR_lock_surface();
	init_EGL_KHR_reusable_sync();
	init_EGL_KHR_stream();
	init_EGL_KHR_stream_consumer_gltexture();
	init_EGL_KHR_stream_cross_process_fd();
	init_EGL_KHR_stream_fifo();
	init_EGL_KHR_stream_producer_aldatalocator();
	init_EGL_KHR_stream_producer_eglsurface();
	init_EGL_KHR_surfaceless_context();
	init_EGL_KHR_vg_parent_image();
	init_EGL_KHR_wait_sync();
	init_EGL_MESA_drm_image();
	init_EGL_NV_coverage_sample();
	init_EGL_NV_coverage_sample_resolve();
	init_EGL_NV_depth_nonlinear();
	init_EGL_NV_post_sub_buffer();
	init_EGL_NV_sync();
	init_EGL_NV_system_time();
	init_EGL_VERSION_1_1();
	init_EGL_VERSION_1_2();
	init_EGL_VERSION_1_3();
	init_EGL_VERSION_1_4();
}

#ifdef __cplusplus
}
#endif

#endif		/* GLLOADER_EGL */
