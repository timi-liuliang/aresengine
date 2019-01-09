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

#ifndef _GLLOADER_EGL_H
#define _GLLOADER_EGL_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EGL_ANGLE_d3d_share_handle_client_buffer
#define EGL_ANGLE_d3d_share_handle_client_buffer 1
#endif

#ifndef EGL_ANGLE_query_surface_pointer
#define EGL_ANGLE_query_surface_pointer 1
#endif

#ifndef EGL_ANGLE_surface_d3d_texture_2d_share_handle
#define EGL_ANGLE_surface_d3d_texture_2d_share_handle 1
#endif

#ifndef EGL_EXT_create_context_robustness
#define EGL_EXT_create_context_robustness 1
#endif

#ifndef EGL_EXT_multiview_window
#define EGL_EXT_multiview_window 1
#endif

#ifndef EGL_HI_clientpixmap
#define EGL_HI_clientpixmap 1
#endif

#ifndef EGL_HI_colorformats
#define EGL_HI_colorformats 1
#endif

#ifndef EGL_IMG_context_priority
#define EGL_IMG_context_priority 1
#endif

#ifndef EGL_KHR_config_attribs
#define EGL_KHR_config_attribs 1
#endif

#ifndef EGL_KHR_create_context
#define EGL_KHR_create_context 1
#endif

#ifndef EGL_KHR_fence_sync
#define EGL_KHR_fence_sync 1
#endif

#ifndef EGL_KHR_gl_texture_2D_image
#define EGL_KHR_gl_texture_2D_image 1
#endif

#ifndef EGL_KHR_image
#define EGL_KHR_image 1
#endif

#ifndef EGL_KHR_image_base
#define EGL_KHR_image_base 1
#endif

#ifndef EGL_KHR_image_pixmap
#define EGL_KHR_image_pixmap 1
#endif

#ifndef EGL_KHR_lock_surface
#define EGL_KHR_lock_surface 1
#endif

#ifndef EGL_KHR_reusable_sync
#define EGL_KHR_reusable_sync 1
#endif

#ifndef EGL_KHR_stream
#define EGL_KHR_stream 1
#endif

#ifndef EGL_KHR_stream_consumer_gltexture
#define EGL_KHR_stream_consumer_gltexture 1
#endif

#ifndef EGL_KHR_stream_cross_process_fd
#define EGL_KHR_stream_cross_process_fd 1
#endif

#ifndef EGL_KHR_stream_fifo
#define EGL_KHR_stream_fifo 1
#endif

#ifndef EGL_KHR_stream_producer_aldatalocator
#define EGL_KHR_stream_producer_aldatalocator 1
#endif

#ifndef EGL_KHR_stream_producer_eglsurface
#define EGL_KHR_stream_producer_eglsurface 1
#endif

#ifndef EGL_KHR_surfaceless_context
#define EGL_KHR_surfaceless_context 1
#endif

#ifndef EGL_KHR_vg_parent_image
#define EGL_KHR_vg_parent_image 1
#endif

#ifndef EGL_KHR_wait_sync
#define EGL_KHR_wait_sync 1
#endif

#ifndef EGL_MESA_drm_image
#define EGL_MESA_drm_image 1
#endif

#ifndef EGL_NV_coverage_sample
#define EGL_NV_coverage_sample 1
#endif

#ifndef EGL_NV_coverage_sample_resolve
#define EGL_NV_coverage_sample_resolve 1
#endif

#ifndef EGL_NV_depth_nonlinear
#define EGL_NV_depth_nonlinear 1
#endif

#ifndef EGL_NV_post_sub_buffer
#define EGL_NV_post_sub_buffer 1
#endif

#ifndef EGL_NV_sync
#define EGL_NV_sync 1
#endif

#ifndef EGL_NV_system_time
#define EGL_NV_system_time 1
#endif

#ifndef EGL_VERSION_1_1
#define EGL_VERSION_1_1 1
#endif

#ifndef EGL_VERSION_1_2
#define EGL_VERSION_1_2 1
#endif

#ifndef EGL_VERSION_1_3
#define EGL_VERSION_1_3 1
#endif

#ifndef EGL_VERSION_1_4
#define EGL_VERSION_1_4 1
#endif

#ifdef EGL_ANGLE_d3d_share_handle_client_buffer

#define EGL_D3D_TEXTURE_2D_SHARE_HANDLE_ANGLE 0x3200

#endif

#ifdef EGL_ANGLE_surface_d3d_texture_2d_share_handle


#endif

#ifdef EGL_EXT_create_context_robustness

#define EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT 0x30BF
#define EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT 0x3138
#define EGL_NO_RESET_NOTIFICATION_EXT 0x31BE
#define EGL_LOSE_CONTEXT_ON_RESET_EXT 0x31BF

#endif

#ifdef EGL_EXT_multiview_window

#define EGL_MULTIVIEW_VIEW_COUNT_EXT 0x3134

#endif

#ifdef EGL_HI_colorformats

#define EGL_COLOR_FORMAT_HI 0x8F70
#define EGL_COLOR_RGB_HI 0x8F71
#define EGL_COLOR_RGBA_HI 0x8F72
#define EGL_COLOR_ARGB_HI 0x8F73

#endif

#ifdef EGL_IMG_context_priority

#define EGL_CONTEXT_PRIORITY_LEVEL_IMG 0x3100
#define EGL_CONTEXT_PRIORITY_HIGH_IMG 0x3101
#define EGL_CONTEXT_PRIORITY_MEDIUM_IMG 0x3102
#define EGL_CONTEXT_PRIORITY_LOW_IMG 0x3103

#endif

#ifdef EGL_KHR_config_attribs

#define EGL_CONFORMANT_KHR 0x3042
#define EGL_VG_COLORSPACE_LINEAR_BIT_KHR 0x0020
#define EGL_VG_ALPHA_FORMAT_PRE_BIT_KHR 0x0040

#endif

#ifdef EGL_KHR_create_context

#define EGL_CONTEXT_MAJOR_VERSION_KHR 0x3098
#define EGL_CONTEXT_MINOR_VERSION_KHR 0x30FB
#define EGL_CONTEXT_FLAGS_KHR 0x30FC
#define EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR 0x30FD
#define EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR 0x31BD
#define EGL_NO_RESET_NOTIFICATION_KHR 0x31BE
#define EGL_LOSE_CONTEXT_ON_RESET_KHR 0x31BF
#define EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR 0x00000001
#define EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR 0x00000002
#define EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR 0x00000004
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR 0x00000001
#define EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR 0x00000002

#endif

#ifdef EGL_KHR_fence_sync

#define EGL_SYNC_FENCE_KHR 0x30F9
#define EGL_SYNC_TYPE_KHR 0x30F7
#define EGL_SYNC_STATUS_KHR 0x30F1
#define EGL_SYNC_CONDITION_KHR 0x30F8
#define EGL_SIGNALED_KHR 0x30F2
#define EGL_UNSIGNALED_KHR 0x30F3
#define EGL_SYNC_PRIOR_COMMANDS_COMPLETE_KHR 0x30F0
#define EGL_SYNC_FLUSH_COMMANDS_BIT_KHR 0x0001
#define EGL_FOREVER_KHR 0xFFFFFFFFFFFFFFFFull
#define EGL_TIMEOUT_EXPIRED_KHR 0x30F5
#define EGL_CONDITION_SATISFIED_KHR 0x30F6
#define EGL_NO_SYNC_KHR ((EGLSyncKHR)0)

#endif

#ifdef EGL_KHR_gl_texture_2D_image

#define EGL_GL_TEXTURE_2D_KHR 0x30B1
#define EGL_GL_TEXTURE_LEVEL_KHR 0x30BC
#define EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR 0x30B3
#define EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR 0x30B4
#define EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR 0x30B5
#define EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR 0x30B6
#define EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR 0x30B7
#define EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR 0x30B8
#define EGL_GL_TEXTURE_3D_KHR 0x30B2
#define EGL_GL_TEXTURE_ZOFFSET_KHR 0x30BD
#define EGL_GL_RENDERBUFFER_KHR 0x30B9

#endif

#ifdef EGL_KHR_image_base

#define EGL_NO_IMAGE_KHR ((EGLImageKHR)0)
#define EGL_IMAGE_PRESERVED_KHR 0x30D2

#endif

#ifdef EGL_KHR_image_pixmap

#define EGL_NATIVE_PIXMAP_KHR 0x30B0

#endif

#ifdef EGL_KHR_lock_surface

#define EGL_LOCK_SURFACE_BIT_KHR 0x0080
#define EGL_OPTIMAL_FORMAT_BIT_KHR 0x0100
#define EGL_MATCH_FORMAT_KHR 0x3043
#define EGL_FORMAT_RGB_565_EXACT_KHR 0x30C0
#define EGL_FORMAT_RGB_565_KHR 0x30C1
#define EGL_FORMAT_RGBA_8888_EXACT_KHR 0x30C2
#define EGL_FORMAT_RGBA_8888_KHR 0x30C3
#define EGL_MAP_PRESERVE_PIXELS_KHR 0x30C4
#define EGL_LOCK_USAGE_HINT_KHR 0x30C5
#define EGL_READ_SURFACE_BIT_KHR 0x0001
#define EGL_WRITE_SURFACE_BIT_KHR 0x0002
#define EGL_BITMAP_POINTER_KHR 0x30C6
#define EGL_BITMAP_PITCH_KHR 0x30C7
#define EGL_BITMAP_ORIGIN_KHR 0x30C8
#define EGL_BITMAP_PIXEL_RED_OFFSET_KHR 0x30C9
#define EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR 0x30CA
#define EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR 0x30CB
#define EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR 0x30CC
#define EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR 0x30CD
#define EGL_LOWER_LEFT_KHR 0x30CE
#define EGL_UPPER_LEFT_KHR 0x30CF

#endif

#ifdef EGL_KHR_reusable_sync

#define EGL_SYNC_REUSABLE_KHR 0x30FA

#endif

#ifdef EGL_KHR_stream

#define EGL_NO_STREAM_KHR 0
#define EGL_CONSUMER_LATENCY_USEC_KHR 0x3210
#define EGL_PRODUCER_FRAME_KHR 0x3212
#define EGL_CONSUMER_FRAME_KHR 0x3213
#define EGL_STREAM_STATE_KHR 0x3214
#define EGL_STREAM_STATE_CREATED_KHR 0x3215
#define EGL_STREAM_STATE_CONNECTING_KHR 0x3216
#define EGL_STREAM_STATE_EMPTY_KHR 0x3217
#define EGL_STREAM_STATE_NEW_FRAME_AVAILABLE_KHR 0x3218
#define EGL_STREAM_STATE_OLD_FRAME_AVAILABLE_KHR 0x3219
#define EGL_STREAM_STATE_DISCONNECTED_KHR 0x321A
#define EGL_BAD_STREAM_KHR 0x321B
#define EGL_BAD_STATE_KHR 0x321C

#endif

#ifdef EGL_KHR_stream_consumer_gltexture

#define EGL_CONSUMER_ACQUIRE_TIMEOUT_USEC_KHR 0x321E

#endif

#ifdef EGL_KHR_stream_cross_process_fd

#define EGL_NO_FILE_DESCRIPTOR_KHR ((EGLNativeFileDescriptorKHR)(-1))

#endif

#ifdef EGL_KHR_stream_fifo

#define EGL_STREAM_FIFO_LENGTH_KHR 0x31FC
#define EGL_STREAM_TIME_NOW_KHR 0x31FD
#define EGL_STREAM_TIME_CONSUMER_KHR 0x31FE
#define EGL_STREAM_TIME_PRODUCER_KHR 0x31FF

#endif

#ifdef EGL_KHR_stream_producer_eglsurface

#define EGL_STREAM_BIT_KHR 0x0800

#endif

#ifdef EGL_KHR_vg_parent_image

#define EGL_VG_PARENT_IMAGE_KHR 0x30BA

#endif

#ifdef EGL_MESA_drm_image

#define EGL_DRM_BUFFER_FORMAT_MESA 0x31D0
#define EGL_DRM_BUFFER_USE_MESA 0x31D1
#define EGL_DRM_BUFFER_FORMAT_ARGB32_MESA 0x31D2
#define EGL_DRM_BUFFER_USE_SCANOUT_MESA 0x0001
#define EGL_DRM_BUFFER_USE_SHARE_MESA 0x0002
#define EGL_DRM_BUFFER_MESA 0x31D3
#define EGL_DRM_BUFFER_STRIDE_MESA 0x31D4

#endif

#ifdef EGL_NV_coverage_sample

#define EGL_COVERAGE_BUFFERS_NV 0x30E0
#define EGL_COVERAGE_SAMPLES_NV 0x30E1

#endif

#ifdef EGL_NV_coverage_sample_resolve

#define EGL_COVERAGE_SAMPLE_RESOLVE_NV 0x3131
#define EGL_COVERAGE_SAMPLE_RESOLVE_DEFAULT_NV 0x3132
#define EGL_COVERAGE_SAMPLE_RESOLVE_NONE_NV 0x3133

#endif

#ifdef EGL_NV_depth_nonlinear

#define EGL_DEPTH_ENCODING_NV 0x30E2
#define EGL_DEPTH_ENCODING_NONE_NV 0
#define EGL_DEPTH_ENCODING_NONLINEAR_NV 0x30E3

#endif

#ifdef EGL_NV_post_sub_buffer

#define EGL_POST_SUB_BUFFER_SUPPORTED_NV 0x30BE

#endif

#ifdef EGL_NV_sync

#define EGL_SYNC_PRIOR_COMMANDS_COMPLETE_NV 0x30E6
#define EGL_SYNC_STATUS_NV 0x30E7
#define EGL_SIGNALED_NV 0x30E8
#define EGL_UNSIGNALED_NV 0x30E9
#define EGL_SYNC_FLUSH_COMMANDS_BIT_NV 0x0001
#define EGL_FOREVER_NV 0xFFFFFFFFFFFFFFFFull
#define EGL_ALREADY_SIGNALED_NV 0x30EA
#define EGL_TIMEOUT_EXPIRED_NV 0x30EB
#define EGL_CONDITION_SATISFIED_NV 0x30EC
#define EGL_SYNC_TYPE_NV 0x30ED
#define EGL_SYNC_CONDITION_NV 0x30EE
#define EGL_SYNC_FENCE_NV 0x30EF
#define EGL_NO_SYNC_NV ((EGLSyncNV)0)

#endif

#ifdef EGL_VERSION_1_1

#define EGL_FALSE 0
#define EGL_TRUE 1
#define EGL_DEFAULT_DISPLAY ((EGLNativeDisplayType)0)
#define EGL_NO_CONTEXT ((EGLContext)0)
#define EGL_NO_DISPLAY ((EGLDisplay)0)
#define EGL_NO_SURFACE ((EGLSurface)0)
#define EGL_DONT_CARE -1
#define EGL_SUCCESS 0x3000
#define EGL_NOT_INITIALIZED 0x3001
#define EGL_BAD_ACCESS 0x3002
#define EGL_BAD_ATTRIBUTE 0x3004
#define EGL_BAD_CONFIG 0x3005
#define EGL_BAD_CONTEXT 0x3006
#define EGL_BAD_CURRENT_SURFACE 0x3007
#define EGL_BAD_DISPLAY 0x3008
#define EGL_BAD_MATCH 0x3009
#define EGL_BAD_NATIVE_PIXMAP 0x300A
#define EGL_BAD_NATIVE_WINDOW 0x300B
#define EGL_BAD_PARAMETER 0x300C
#define EGL_BAD_SURFACE 0x300D
#define EGL_CONTEXT_LOST 0x300E
#define EGL_BUFFER_SIZE 0x3020
#define EGL_ALPHA_SIZE 0x3021
#define EGL_BLUE_SIZE 0x3022
#define EGL_GREEN_SIZE 0x3023
#define EGL_RED_SIZE 0x3024
#define EGL_DEPTH_SIZE 0x3025
#define EGL_STENCIL_SIZE 0x3026
#define EGL_CONFIG_CAVEAT 0x3027
#define EGL_CONFIG_ID 0x3028
#define EGL_LEVEL 0x3029
#define EGL_MAX_PBUFFER_HEIGHT 0x302A
#define EGL_MAX_PBUFFER_PIXELS 0x302B
#define EGL_MAX_PBUFFER_WIDTH 0x302C
#define EGL_NATIVE_RENDERABLE 0x302D
#define EGL_NATIVE_VISUAL_ID 0x302E
#define EGL_NATIVE_VISUAL_TYPE 0x302F
#define EGL_SAMPLES 0x3031
#define EGL_SAMPLE_BUFFERS 0x3032
#define EGL_SURFACE_TYPE 0x3033
#define EGL_TRANSPARENT_TYPE 0x3034
#define EGL_TRANSPARENT_BLUE_VALUE 0x3035
#define EGL_TRANSPARENT_GREEN_VALUE 0x3036
#define EGL_TRANSPARENT_RED_VALUE 0x3037
#define EGL_NONE 0x3038
#define EGL_BIND_TO_TEXTURE_RGB 0x3039
#define EGL_BIND_TO_TEXTURE_RGBA 0x303A
#define EGL_MIN_SWAP_INTERVAL 0x303B
#define EGL_MAX_SWAP_INTERVAL 0x303C
#define EGL_SLOW_CONFIG 0x3050
#define EGL_NON_CONFORMANT_CONFIG 0x3051
#define EGL_TRANSPARENT_RGB 0x3052
#define EGL_NO_TEXTURE 0x305C
#define EGL_TEXTURE_RGB 0x305D
#define EGL_TEXTURE_RGBA 0x305E
#define EGL_TEXTURE_2D 0x305F
#define EGL_PBUFFER_BIT 0x0001
#define EGL_PIXMAP_BIT 0x0002
#define EGL_WINDOW_BIT 0x0004
#define EGL_VENDOR 0x3053
#define EGL_VERSION 0x3054
#define EGL_EXTENSIONS 0x3055
#define EGL_HEIGHT 0x3056
#define EGL_WIDTH 0x3057
#define EGL_LARGEST_PBUFFER 0x3058
#define EGL_TEXTURE_FORMAT 0x3080
#define EGL_TEXTURE_TARGET 0x3081
#define EGL_MIPMAP_TEXTURE 0x3082
#define EGL_MIPMAP_LEVEL 0x3083
#define EGL_BACK_BUFFER 0x3084
#define EGL_DRAW 0x3059
#define EGL_READ 0x305A
#define EGL_CORE_NATIVE_ENGINE 0x305B

#endif

#ifdef EGL_VERSION_1_2

#define EGL_LUMINANCE_SIZE 0x303D
#define EGL_ALPHA_MASK_SIZE 0x303E
#define EGL_COLOR_BUFFER_TYPE 0x303F
#define EGL_RENDERABLE_TYPE 0x3040
#define EGL_RGB_BUFFER 0x308E
#define EGL_LUMINANCE_BUFFER 0x308F
#define EGL_OPENGL_ES_BIT 0x0001
#define EGL_OPENVG_BIT 0x0002
#define EGL_CLIENT_APIS 0x308D
#define EGL_RENDER_BUFFER 0x3086
#define EGL_COLORSPACE EGL_VG_COLORSPACE
#define EGL_ALPHA_FORMAT EGL_VG_ALPHA_FORMAT
#define EGL_COLORSPACE_sRGB EGL_VG_COLORSPACE_sRGB
#define EGL_COLORSPACE_LINEAR EGL_VG_COLORSPACE_LINEAR
#define EGL_ALPHA_FORMAT_NONPRE EGL_VG_ALPHA_FORMAT_NONPRE
#define EGL_ALPHA_FORMAT_PRE EGL_VG_ALPHA_FORMAT_PRE
#define EGL_HORIZONTAL_RESOLUTION 0x3090
#define EGL_VERTICAL_RESOLUTION 0x3091
#define EGL_PIXEL_ASPECT_RATIO 0x3092
#define EGL_SWAP_BEHAVIOR 0x3093
#define EGL_SINGLE_BUFFER 0x3085
#define EGL_DISPLAY_SCALING 10000
#define EGL_UNKNOWN -1
#define EGL_BUFFER_PRESERVED 0x3094
#define EGL_BUFFER_DESTROYED 0x3095
#define EGL_OPENVG_IMAGE 0x3096
#define EGL_CONTEXT_CLIENT_TYPE 0x3097
#define EGL_OPENGL_ES_API 0x30A0
#define EGL_OPENVG_API 0x30A1

#endif

#ifdef EGL_VERSION_1_3

#define EGL_CONFORMANT 0x3042
#define EGL_VG_COLORSPACE_LINEAR_BIT 0x0020
#define EGL_VG_ALPHA_FORMAT_PRE_BIT 0x0040
#define EGL_OPENGL_ES2_BIT 0x0004
#define EGL_VG_COLORSPACE 0x3087
#define EGL_VG_ALPHA_FORMAT 0x3088
#define EGL_VG_COLORSPACE_sRGB 0x3089
#define EGL_VG_COLORSPACE_LINEAR 0x308A
#define EGL_VG_ALPHA_FORMAT_NONPRE 0x308B
#define EGL_VG_ALPHA_FORMAT_PRE 0x308C
#define EGL_CONTEXT_CLIENT_VERSION 0x3098

#endif

#ifdef EGL_VERSION_1_4

#define EGL_MATCH_NATIVE_PIXMAP 0x3041
#define EGL_OPENGL_BIT 0x0008
#define EGL_MULTISAMPLE_RESOLVE 0x3099
#define EGL_OPENGL_API 0x30A2

#endif

#ifdef EGL_HI_clientpixmap

typedef struct _EGLClientPixmapHI { void* pData; int iWidth; int iHeight; int iStride; } EGLClientPixmapHI;

#endif

#ifdef EGL_KHR_fence_sync

typedef void* EGLSyncKHR;
typedef GLuint64 EGLTimeKHR;

#endif

#ifdef EGL_KHR_image_base

typedef void* EGLImageKHR;

#endif

#ifdef EGL_KHR_reusable_sync


#endif

#ifdef EGL_KHR_stream

typedef void* EGLStreamKHR;
typedef GLuint64 EGLuint64KHR;

#endif

#ifdef EGL_KHR_stream_cross_process_fd

typedef int EGLNativeFileDescriptorKHR;

#endif

#ifdef EGL_KHR_stream_fifo


#endif

#ifdef EGL_MESA_drm_image


#endif

#ifdef EGL_NV_sync

typedef void* EGLSyncNV;
typedef unsigned long long EGLTimeNV;

#endif

#ifdef EGL_NV_system_time

typedef GLint64 EGLint64NV;
typedef GLuint64 EGLuint64NV;

#endif

#ifdef EGL_VERSION_1_1

typedef unsigned int EGLBoolean;
typedef int EGLint;
typedef void* EGLDisplay;
typedef void* EGLConfig;
typedef void* EGLSurface;
typedef void* EGLContext;
typedef void (*__eglMustCastToProperFunctionPointerType)(void);

#endif

#ifdef EGL_VERSION_1_2

typedef unsigned int EGLenum;
typedef void* EGLClientBuffer;

#endif

#ifdef EGL_ANGLE_query_surface_pointer

typedef EGLBoolean (GLLOADER_APIENTRY *eglQuerySurfacePointerANGLEFUNC)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, void** value);

extern GLLOADER_API eglQuerySurfacePointerANGLEFUNC eglQuerySurfacePointerANGLE;

#endif

#ifdef EGL_HI_clientpixmap

typedef void (GLLOADER_APIENTRY *eglCreatePixmapSurfaceHIFUNC)(EGLDisplay dpy, EGLConfig config, EGLClientPixmapHI* pixmap);

extern GLLOADER_API eglCreatePixmapSurfaceHIFUNC eglCreatePixmapSurfaceHI;

#endif

#ifdef EGL_KHR_fence_sync

typedef EGLSyncKHR (GLLOADER_APIENTRY *eglCreateSyncKHRFUNC)(EGLDisplay dpy, EGLenum type, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglDestroySyncKHRFUNC)(EGLDisplay dpy, EGLSyncKHR sync);
typedef EGLint (GLLOADER_APIENTRY *eglClientWaitSyncKHRFUNC)(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout);
typedef EGLBoolean (GLLOADER_APIENTRY *eglGetSyncAttribKHRFUNC)(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint* value);

extern GLLOADER_API eglCreateSyncKHRFUNC eglCreateSyncKHR;
extern GLLOADER_API eglDestroySyncKHRFUNC eglDestroySyncKHR;
extern GLLOADER_API eglClientWaitSyncKHRFUNC eglClientWaitSyncKHR;
extern GLLOADER_API eglGetSyncAttribKHRFUNC eglGetSyncAttribKHR;

#endif

#ifdef EGL_KHR_image_base

typedef EGLImageKHR (GLLOADER_APIENTRY *eglCreateImageKHRFUNC)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglDestroyImageKHRFUNC)(EGLDisplay dpy, EGLImageKHR image);

extern GLLOADER_API eglCreateImageKHRFUNC eglCreateImageKHR;
extern GLLOADER_API eglDestroyImageKHRFUNC eglDestroyImageKHR;

#endif

#ifdef EGL_KHR_lock_surface

typedef EGLBoolean (GLLOADER_APIENTRY *eglLockSurfaceKHRFUNC)(EGLDisplay display, EGLSurface surface, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglUnlockSurfaceKHRFUNC)(EGLDisplay display, EGLSurface surface);

extern GLLOADER_API eglLockSurfaceKHRFUNC eglLockSurfaceKHR;
extern GLLOADER_API eglUnlockSurfaceKHRFUNC eglUnlockSurfaceKHR;

#endif

#ifdef EGL_KHR_reusable_sync

typedef EGLBoolean (GLLOADER_APIENTRY *eglSignalSyncKHRFUNC)(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode);

extern GLLOADER_API eglSignalSyncKHRFUNC eglSignalSyncKHR;

#endif

#ifdef EGL_KHR_stream

typedef EGLStreamKHR (GLLOADER_APIENTRY *eglCreateStreamKHRFUNC)(EGLDisplay dpy, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglDestroyStreamKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream);
typedef EGLBoolean (GLLOADER_APIENTRY *eglStreamAttribKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint value);
typedef EGLBoolean (GLLOADER_APIENTRY *eglQueryStreamKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint* value);
typedef EGLBoolean (GLLOADER_APIENTRY *eglQueryStreamu64KHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLuint64KHR* value);

extern GLLOADER_API eglCreateStreamKHRFUNC eglCreateStreamKHR;
extern GLLOADER_API eglDestroyStreamKHRFUNC eglDestroyStreamKHR;
extern GLLOADER_API eglStreamAttribKHRFUNC eglStreamAttribKHR;
extern GLLOADER_API eglQueryStreamKHRFUNC eglQueryStreamKHR;
extern GLLOADER_API eglQueryStreamu64KHRFUNC eglQueryStreamu64KHR;

#endif

#ifdef EGL_KHR_stream_consumer_gltexture

typedef EGLBoolean (GLLOADER_APIENTRY *eglStreamConsumerGLTextureExternalKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream);
typedef EGLBoolean (GLLOADER_APIENTRY *eglStreamConsumerAcquireKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream);
typedef EGLBoolean (GLLOADER_APIENTRY *eglStreamConsumerReleaseKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream);

extern GLLOADER_API eglStreamConsumerGLTextureExternalKHRFUNC eglStreamConsumerGLTextureExternalKHR;
extern GLLOADER_API eglStreamConsumerAcquireKHRFUNC eglStreamConsumerAcquireKHR;
extern GLLOADER_API eglStreamConsumerReleaseKHRFUNC eglStreamConsumerReleaseKHR;

#endif

#ifdef EGL_KHR_stream_cross_process_fd

typedef EGLNativeFileDescriptorKHR (GLLOADER_APIENTRY *eglGetStreamFileDescriptorKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream);
typedef EGLStreamKHR (GLLOADER_APIENTRY *eglCreateStreamFromFileDescriptorKHRFUNC)(EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor);

extern GLLOADER_API eglGetStreamFileDescriptorKHRFUNC eglGetStreamFileDescriptorKHR;
extern GLLOADER_API eglCreateStreamFromFileDescriptorKHRFUNC eglCreateStreamFromFileDescriptorKHR;

#endif

#ifdef EGL_KHR_stream_fifo

typedef EGLBoolean (GLLOADER_APIENTRY *eglQueryStreamTimeKHRFUNC)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLTimeKHR* value);

extern GLLOADER_API eglQueryStreamTimeKHRFUNC eglQueryStreamTimeKHR;

#endif

#ifdef EGL_KHR_stream_producer_eglsurface

typedef EGLSurface (GLLOADER_APIENTRY *eglCreateStreamProducerSurfaceFUNC)(EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream, const EGLint* attrib_list);

extern GLLOADER_API eglCreateStreamProducerSurfaceFUNC eglCreateStreamProducerSurface;

#endif

#ifdef EGL_KHR_wait_sync

typedef EGLint (GLLOADER_APIENTRY *eglWaitSyncKHRFUNC)(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags);

extern GLLOADER_API eglWaitSyncKHRFUNC eglWaitSyncKHR;

#endif

#ifdef EGL_MESA_drm_image

typedef EGLImageKHR (GLLOADER_APIENTRY *eglCreateDRMImageMESAFUNC)(EGLDisplay dpy, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglExportDRMImageMESAFUNC)(EGLDisplay dpy, EGLImageKHR image, EGLint* name, EGLint* handle, EGLint* stride);

extern GLLOADER_API eglCreateDRMImageMESAFUNC eglCreateDRMImageMESA;
extern GLLOADER_API eglExportDRMImageMESAFUNC eglExportDRMImageMESA;

#endif

#ifdef EGL_NV_post_sub_buffer

typedef EGLBoolean (GLLOADER_APIENTRY *eglPostSubBufferNVFUNC)(EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height);

extern GLLOADER_API eglPostSubBufferNVFUNC eglPostSubBufferNV;

#endif

#ifdef EGL_NV_sync

typedef EGLSyncNV (GLLOADER_APIENTRY *eglCreateFenceSyncNVFUNC)(EGLDisplay dpy, EGLenum condition, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglDestroySyncNVFUNC)(EGLSyncNV sync);
typedef EGLBoolean (GLLOADER_APIENTRY *eglFenceNVFUNC)(EGLSyncNV sync);
typedef EGLint (GLLOADER_APIENTRY *eglClientWaitSyncNVFUNC)(EGLSyncNV sync, EGLint flags, EGLTimeNV timeout);
typedef EGLBoolean (GLLOADER_APIENTRY *eglSignalSyncNVFUNC)(EGLSyncNV sync, EGLenum mode);
typedef EGLBoolean (GLLOADER_APIENTRY *eglGetSyncAttribNVFUNC)(EGLSyncNV sync, EGLint attribute, EGLint* value);

extern GLLOADER_API eglCreateFenceSyncNVFUNC eglCreateFenceSyncNV;
extern GLLOADER_API eglDestroySyncNVFUNC eglDestroySyncNV;
extern GLLOADER_API eglFenceNVFUNC eglFenceNV;
extern GLLOADER_API eglClientWaitSyncNVFUNC eglClientWaitSyncNV;
extern GLLOADER_API eglSignalSyncNVFUNC eglSignalSyncNV;
extern GLLOADER_API eglGetSyncAttribNVFUNC eglGetSyncAttribNV;

#endif

#ifdef EGL_NV_system_time

typedef EGLuint64NV (GLLOADER_APIENTRY *eglGetSystemTimeFrequencyNVFUNC)();
typedef EGLuint64NV (GLLOADER_APIENTRY *eglGetSystemTimeNVFUNC)();

extern GLLOADER_API eglGetSystemTimeFrequencyNVFUNC eglGetSystemTimeFrequencyNV;
extern GLLOADER_API eglGetSystemTimeNVFUNC eglGetSystemTimeNV;

#endif

#ifdef EGL_VERSION_1_1

typedef EGLint (GLLOADER_APIENTRY *eglGetErrorFUNC)();
typedef EGLDisplay (GLLOADER_APIENTRY *eglGetDisplayFUNC)(EGLNativeDisplayType display_id);
typedef EGLBoolean (GLLOADER_APIENTRY *eglInitializeFUNC)(EGLDisplay dpy, EGLint* major, EGLint* minor);
typedef EGLBoolean (GLLOADER_APIENTRY *eglTerminateFUNC)(EGLDisplay dpy);
typedef const char* (GLLOADER_APIENTRY *eglQueryStringFUNC)(EGLDisplay dpy, EGLint name);
typedef EGLBoolean (GLLOADER_APIENTRY *eglGetConfigsFUNC)(EGLDisplay dpy, EGLConfig* configs, EGLint config_size, EGLint* num_config);
typedef EGLBoolean (GLLOADER_APIENTRY *eglChooseConfigFUNC)(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config);
typedef EGLBoolean (GLLOADER_APIENTRY *eglGetConfigAttribFUNC)(EGLDisplay dpy, EGLConfig configs, EGLint attribute, EGLint* value);
typedef EGLSurface (GLLOADER_APIENTRY *eglCreateWindowSurfaceFUNC)(EGLDisplay dpy, EGLConfig configs, EGLNativeWindowType win, const EGLint* attrib_list);
typedef EGLSurface (GLLOADER_APIENTRY *eglCreatePbufferSurfaceFUNC)(EGLDisplay dpy, EGLConfig configs, const EGLint* attrib_list);
typedef EGLSurface (GLLOADER_APIENTRY *eglCreatePixmapSurfaceFUNC)(EGLDisplay dpy, EGLConfig configs, EGLNativePixmapType pixmap, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglDestroySurfaceFUNC)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean (GLLOADER_APIENTRY *eglQuerySurfaceFUNC)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value);
typedef EGLBoolean (GLLOADER_APIENTRY *eglSurfaceAttribFUNC)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
typedef EGLBoolean (GLLOADER_APIENTRY *eglBindTexImageFUNC)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean (GLLOADER_APIENTRY *eglReleaseTexImageFUNC)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean (GLLOADER_APIENTRY *eglSwapIntervalFUNC)(EGLDisplay dpy, EGLint interval);
typedef EGLContext (GLLOADER_APIENTRY *eglCreateContextFUNC)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint* attrib_list);
typedef EGLBoolean (GLLOADER_APIENTRY *eglDestroyContextFUNC)(EGLDisplay dpy, EGLContext ctx);
typedef EGLBoolean (GLLOADER_APIENTRY *eglMakeCurrentFUNC)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
typedef EGLContext (GLLOADER_APIENTRY *eglGetCurrentContextFUNC)();
typedef EGLSurface (GLLOADER_APIENTRY *eglGetCurrentSurfaceFUNC)(EGLint readdraw);
typedef EGLDisplay (GLLOADER_APIENTRY *eglGetCurrentDisplayFUNC)();
typedef EGLBoolean (GLLOADER_APIENTRY *eglQueryContextFUNC)(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value);
typedef EGLBoolean (GLLOADER_APIENTRY *eglWaitGLFUNC)();
typedef EGLBoolean (GLLOADER_APIENTRY *eglWaitNativeFUNC)(EGLint engine);
typedef EGLBoolean (GLLOADER_APIENTRY *eglSwapBuffersFUNC)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean (GLLOADER_APIENTRY *eglCopyBuffersFUNC)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
typedef __eglMustCastToProperFunctionPointerType (GLLOADER_APIENTRY *eglGetProcAddressFUNC)(const char* procname);

extern GLLOADER_API eglGetErrorFUNC eglGetError;
extern GLLOADER_API eglGetDisplayFUNC eglGetDisplay;
extern GLLOADER_API eglInitializeFUNC eglInitialize;
extern GLLOADER_API eglTerminateFUNC eglTerminate;
extern GLLOADER_API eglQueryStringFUNC eglQueryString;
extern GLLOADER_API eglGetConfigsFUNC eglGetConfigs;
extern GLLOADER_API eglChooseConfigFUNC eglChooseConfig;
extern GLLOADER_API eglGetConfigAttribFUNC eglGetConfigAttrib;
extern GLLOADER_API eglCreateWindowSurfaceFUNC eglCreateWindowSurface;
extern GLLOADER_API eglCreatePbufferSurfaceFUNC eglCreatePbufferSurface;
extern GLLOADER_API eglCreatePixmapSurfaceFUNC eglCreatePixmapSurface;
extern GLLOADER_API eglDestroySurfaceFUNC eglDestroySurface;
extern GLLOADER_API eglQuerySurfaceFUNC eglQuerySurface;
extern GLLOADER_API eglSurfaceAttribFUNC eglSurfaceAttrib;
extern GLLOADER_API eglBindTexImageFUNC eglBindTexImage;
extern GLLOADER_API eglReleaseTexImageFUNC eglReleaseTexImage;
extern GLLOADER_API eglSwapIntervalFUNC eglSwapInterval;
extern GLLOADER_API eglCreateContextFUNC eglCreateContext;
extern GLLOADER_API eglDestroyContextFUNC eglDestroyContext;
extern GLLOADER_API eglMakeCurrentFUNC eglMakeCurrent;
extern GLLOADER_API eglGetCurrentContextFUNC eglGetCurrentContext;
extern GLLOADER_API eglGetCurrentSurfaceFUNC eglGetCurrentSurface;
extern GLLOADER_API eglGetCurrentDisplayFUNC eglGetCurrentDisplay;
extern GLLOADER_API eglQueryContextFUNC eglQueryContext;
extern GLLOADER_API eglWaitGLFUNC eglWaitGL;
extern GLLOADER_API eglWaitNativeFUNC eglWaitNative;
extern GLLOADER_API eglSwapBuffersFUNC eglSwapBuffers;
extern GLLOADER_API eglCopyBuffersFUNC eglCopyBuffers;
extern GLLOADER_API eglGetProcAddressFUNC eglGetProcAddress;

#endif

#ifdef EGL_VERSION_1_2

typedef EGLBoolean (GLLOADER_APIENTRY *eglBindAPIFUNC)(EGLenum api);
typedef EGLenum (GLLOADER_APIENTRY *eglQueryAPIFUNC)();
typedef EGLBoolean (GLLOADER_APIENTRY *eglWaitClientFUNC)();
typedef EGLBoolean (GLLOADER_APIENTRY *eglReleaseThreadFUNC)();
typedef EGLSurface (GLLOADER_APIENTRY *eglCreatePbufferFromClientBufferFUNC)(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint* attrib_list);

extern GLLOADER_API eglBindAPIFUNC eglBindAPI;
extern GLLOADER_API eglQueryAPIFUNC eglQueryAPI;
extern GLLOADER_API eglWaitClientFUNC eglWaitClient;
extern GLLOADER_API eglReleaseThreadFUNC eglReleaseThread;
extern GLLOADER_API eglCreatePbufferFromClientBufferFUNC eglCreatePbufferFromClientBuffer;

#endif

typedef char (GLLOADER_APIENTRY *glloader_EGL_ANGLE_d3d_share_handle_client_bufferFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_ANGLE_query_surface_pointerFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handleFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_EXT_create_context_robustnessFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_EXT_multiview_windowFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_HI_clientpixmapFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_HI_colorformatsFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_IMG_context_priorityFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_config_attribsFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_create_contextFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_fence_syncFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_gl_texture_2D_imageFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_imageFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_image_baseFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_image_pixmapFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_lock_surfaceFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_reusable_syncFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_streamFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_stream_consumer_gltextureFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_stream_cross_process_fdFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_stream_fifoFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_stream_producer_aldatalocatorFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_stream_producer_eglsurfaceFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_surfaceless_contextFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_vg_parent_imageFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_KHR_wait_syncFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_MESA_drm_imageFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_NV_coverage_sampleFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_NV_coverage_sample_resolveFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_NV_depth_nonlinearFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_NV_post_sub_bufferFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_NV_syncFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_NV_system_timeFUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_VERSION_1_1FUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_VERSION_1_2FUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_VERSION_1_3FUNC)();
typedef char (GLLOADER_APIENTRY *glloader_EGL_VERSION_1_4FUNC)();

extern GLLOADER_API glloader_EGL_ANGLE_d3d_share_handle_client_bufferFUNC glloader_EGL_ANGLE_d3d_share_handle_client_buffer;
extern GLLOADER_API glloader_EGL_ANGLE_query_surface_pointerFUNC glloader_EGL_ANGLE_query_surface_pointer;
extern GLLOADER_API glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handleFUNC glloader_EGL_ANGLE_surface_d3d_texture_2d_share_handle;
extern GLLOADER_API glloader_EGL_EXT_create_context_robustnessFUNC glloader_EGL_EXT_create_context_robustness;
extern GLLOADER_API glloader_EGL_EXT_multiview_windowFUNC glloader_EGL_EXT_multiview_window;
extern GLLOADER_API glloader_EGL_HI_clientpixmapFUNC glloader_EGL_HI_clientpixmap;
extern GLLOADER_API glloader_EGL_HI_colorformatsFUNC glloader_EGL_HI_colorformats;
extern GLLOADER_API glloader_EGL_IMG_context_priorityFUNC glloader_EGL_IMG_context_priority;
extern GLLOADER_API glloader_EGL_KHR_config_attribsFUNC glloader_EGL_KHR_config_attribs;
extern GLLOADER_API glloader_EGL_KHR_create_contextFUNC glloader_EGL_KHR_create_context;
extern GLLOADER_API glloader_EGL_KHR_fence_syncFUNC glloader_EGL_KHR_fence_sync;
extern GLLOADER_API glloader_EGL_KHR_gl_texture_2D_imageFUNC glloader_EGL_KHR_gl_texture_2D_image;
extern GLLOADER_API glloader_EGL_KHR_imageFUNC glloader_EGL_KHR_image;
extern GLLOADER_API glloader_EGL_KHR_image_baseFUNC glloader_EGL_KHR_image_base;
extern GLLOADER_API glloader_EGL_KHR_image_pixmapFUNC glloader_EGL_KHR_image_pixmap;
extern GLLOADER_API glloader_EGL_KHR_lock_surfaceFUNC glloader_EGL_KHR_lock_surface;
extern GLLOADER_API glloader_EGL_KHR_reusable_syncFUNC glloader_EGL_KHR_reusable_sync;
extern GLLOADER_API glloader_EGL_KHR_streamFUNC glloader_EGL_KHR_stream;
extern GLLOADER_API glloader_EGL_KHR_stream_consumer_gltextureFUNC glloader_EGL_KHR_stream_consumer_gltexture;
extern GLLOADER_API glloader_EGL_KHR_stream_cross_process_fdFUNC glloader_EGL_KHR_stream_cross_process_fd;
extern GLLOADER_API glloader_EGL_KHR_stream_fifoFUNC glloader_EGL_KHR_stream_fifo;
extern GLLOADER_API glloader_EGL_KHR_stream_producer_aldatalocatorFUNC glloader_EGL_KHR_stream_producer_aldatalocator;
extern GLLOADER_API glloader_EGL_KHR_stream_producer_eglsurfaceFUNC glloader_EGL_KHR_stream_producer_eglsurface;
extern GLLOADER_API glloader_EGL_KHR_surfaceless_contextFUNC glloader_EGL_KHR_surfaceless_context;
extern GLLOADER_API glloader_EGL_KHR_vg_parent_imageFUNC glloader_EGL_KHR_vg_parent_image;
extern GLLOADER_API glloader_EGL_KHR_wait_syncFUNC glloader_EGL_KHR_wait_sync;
extern GLLOADER_API glloader_EGL_MESA_drm_imageFUNC glloader_EGL_MESA_drm_image;
extern GLLOADER_API glloader_EGL_NV_coverage_sampleFUNC glloader_EGL_NV_coverage_sample;
extern GLLOADER_API glloader_EGL_NV_coverage_sample_resolveFUNC glloader_EGL_NV_coverage_sample_resolve;
extern GLLOADER_API glloader_EGL_NV_depth_nonlinearFUNC glloader_EGL_NV_depth_nonlinear;
extern GLLOADER_API glloader_EGL_NV_post_sub_bufferFUNC glloader_EGL_NV_post_sub_buffer;
extern GLLOADER_API glloader_EGL_NV_syncFUNC glloader_EGL_NV_sync;
extern GLLOADER_API glloader_EGL_NV_system_timeFUNC glloader_EGL_NV_system_time;
extern GLLOADER_API glloader_EGL_VERSION_1_1FUNC glloader_EGL_VERSION_1_1;
extern GLLOADER_API glloader_EGL_VERSION_1_2FUNC glloader_EGL_VERSION_1_2;
extern GLLOADER_API glloader_EGL_VERSION_1_3FUNC glloader_EGL_VERSION_1_3;
extern GLLOADER_API glloader_EGL_VERSION_1_4FUNC glloader_EGL_VERSION_1_4;

#ifdef __cplusplus
}
#endif

#endif		/* _GLLOADER_EGL_H */
