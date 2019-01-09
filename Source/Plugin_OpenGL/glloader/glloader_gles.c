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

#ifdef GLLOADER_GLES

#ifdef __cplusplus
extern "C"
{
#endif

char _GLES_AMD_compressed_3DC_texture = 0;
char _GLES_AMD_compressed_ATC_texture = 0;
char _GLES_AMD_performance_monitor = 0;
char _GLES_AMD_program_binary_Z400 = 0;
char _GLES_ANGLE_framebuffer_blit = 0;
char _GLES_ANGLE_framebuffer_multisample = 0;
char _GLES_ANGLE_instanced_arrays = 0;
char _GLES_ANGLE_pack_reverse_row_order = 0;
char _GLES_ANGLE_texture_compression_dxt3 = 0;
char _GLES_ANGLE_texture_compression_dxt5 = 0;
char _GLES_ANGLE_texture_usage = 0;
char _GLES_ANGLE_translated_shader_source = 0;
char _GLES_APPLE_copy_texture_levels = 0;
char _GLES_APPLE_framebuffer_multisample = 0;
char _GLES_APPLE_rgb_422 = 0;
char _GLES_APPLE_sync = 0;
char _GLES_APPLE_texture_2D_limited_npot = 0;
char _GLES_APPLE_texture_format_BGRA8888 = 0;
char _GLES_APPLE_texture_max_level = 0;
char _GLES_ARM_mali_shader_binary = 0;
char _GLES_ARM_rgba8 = 0;
char _GLES_EXT_blend_minmax = 0;
char _GLES_EXT_color_buffer_half_float = 0;
char _GLES_EXT_debug_label = 0;
char _GLES_EXT_debug_marker = 0;
char _GLES_EXT_discard_framebuffer = 0;
char _GLES_EXT_frag_depth = 0;
char _GLES_EXT_map_buffer_range = 0;
char _GLES_EXT_multisampled_render_to_texture = 0;
char _GLES_EXT_multiview_draw_buffers = 0;
char _GLES_EXT_multi_draw_arrays = 0;
char _GLES_EXT_occlusion_query_boolean = 0;
char _GLES_read_format_bgra = 0;
char _GL_EXT_robustness = 0;
char _GLES_EXT_separate_shader_objects = 0;
char _GLES_EXT_shader_framebuffer_fetch = 0;
char _GLES_EXT_shader_texture_lod = 0;
char _GLES_EXT_shadow_samplers = 0;
char _GLES_EXT_sRGB = 0;
char _GLES_EXT_texture_compression_dxt1 = 0;
char _GLES_EXT_texture_compression_latc = 0;
char _GLES_EXT_texture_compression_s3tc = 0;
char _GLES_EXT_texture_filter_anisotropic = 0;
char _GLES_EXT_texture_format_BGRA8888 = 0;
char _GLES_EXT_texture_lod_bias = 0;
char _GLES_EXT_texture_rg = 0;
char _GLES_EXT_texture_storage = 0;
char _GLES_EXT_texture_type_2_10_10_10_REV = 0;
char _GLES_EXT_unpack_subimage = 0;
char _GLES_FJ_shader_binary_GCCSO = 0;
char _GLES_IMG_multisampled_render_to_texture = 0;
char _GLES_IMG_program_binary = 0;
char _GLES_IMG_read_format = 0;
char _GLES_shader_binary = 0;
char _GLES_IMG_texture_compression_pvrtc = 0;
char _GLES_IMG_texture_env_enhanced_fixed_function = 0;
char _GLES_IMG_user_clip_plane = 0;
char _GLES_NV_coverage_sample = 0;
char _GLES_NV_depth_nonlinear = 0;
char _GLES_NV_draw_buffers = 0;
char _GLES_NV_draw_texture = 0;
char _GLES_NV_EGL_stream_consumer_external = 0;
char _GLES_NV_fbo_color_attachments = 0;
char _GLES_NV_fence = 0;
char _GLES_NV_read_buffer = 0;
char _GLES_NV_read_depth = 0;
char _GLES_NV_read_depth_stencil = 0;
char _GLES_NV_read_stencil = 0;
char _GLES_NV_texture_compression_s3tc_update = 0;
char _GLES_NV_texture_npot_2D_mipmap = 0;
char _GLES_OES_blend_equation_separate = 0;
char _GLES_OES_blend_func_separate = 0;
char _GLES_OES_blend_subtract = 0;
char _GLES_OES_byte_coordinates = 0;
char _GLES_OES_compressed_ETC1_RGB8_texture = 0;
char _GLES_OES_compressed_paletted_texture = 0;
char _GLES_OES_depth24 = 0;
char _GLES_OES_depth32 = 0;
char _GLES_OES_depth_texture = 0;
char _GLES_OES_draw_texture = 0;
char _GLES_OES_EGL_image = 0;
char _GLES_OES_EGL_image_external = 0;
char _GLES_OES_element_index_uint = 0;
char _GLES_OES_extended_matrix_palette = 0;
char _GLES_OES_fbo_render_mipmap = 0;
char _GLES_OES_fixed_point = 0;
char _GLES_OES_fragment_precision_high = 0;
char _GLES_OES_framebuffer_object = 0;
char _GLES_OES_get_program_binary = 0;
char _GLES_OES_mapbuffer = 0;
char _GLES_OES_matrix_get = 0;
char _GLES_OES_matrix_palette = 0;
char _GLES_OES_packed_depth_stencil = 0;
char _GLES_OES_point_size_array = 0;
char _GLES_OES_point_sprite = 0;
char _GLES_OES_query_matrix = 0;
char _GLES_OES_read_format = 0;
char _GLES_OES_required_internalformat = 0;
char _GLES_OES_rgb8_rgba8 = 0;
char _GLES_OES_shader_binary = 0;
char _GLES_OES_shader_source = 0;
char _GLES_OES_single_precision = 0;
char _GLES_OES_standard_derivatives = 0;
char _GLES_OES_stencil1 = 0;
char _GLES_OES_stencil4 = 0;
char _GLES_OES_stencil8 = 0;
char _GLES_OES_stencil_wrap = 0;
char _GLES_OES_texture_3D = 0;
char _GLES_OES_texture_cube_map = 0;
char _GLES_OES_texture_env_crossbar = 0;
char _GLES_OES_texture_float = 0;
char _GLES_OES_texture_float_linear = 0;
char _GLES_OES_texture_half_float = 0;
char _GLES_OES_texture_half_float_linear = 0;
char _GLES_OES_texture_mirrored_repeat = 0;
char _GLES_OES_texture_npot = 0;
char _GLES_OES_vertex_array_object = 0;
char _GLES_OES_vertex_half_float = 0;
char _GLES_OES_vertex_type_10_10_10_2 = 0;
char _GLES_QCOM_driver_control = 0;
char _GLES_QCOM_extended_get = 0;
char _GLES_QCOM_extended_get2 = 0;
char _GLES_QCOM_performance_monitor_global_mode = 0;
char _GLES_QCOM_tiled_rendering = 0;
char _GLES_QCOM_writeonly_rendering = 0;
char _GLES_VERSION_1_0 = 0;
char _GLES_VERSION_1_1 = 0;
char _GLES_VERSION_2_0 = 0;
char _GLES_VIV_shader_binary = 0;

static char GLLOADER_APIENTRY _glloader_GLES_AMD_compressed_3DC_texture()
{
	return _GLES_AMD_compressed_3DC_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_AMD_compressed_3DC_texture()
{
	glloader_init();
	return glloader_GLES_AMD_compressed_3DC_texture();
}
glloader_GLES_AMD_compressed_3DC_textureFUNC glloader_GLES_AMD_compressed_3DC_texture = self_init_glloader_GLES_AMD_compressed_3DC_texture;

static char GLLOADER_APIENTRY _glloader_GLES_AMD_compressed_ATC_texture()
{
	return _GLES_AMD_compressed_ATC_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_AMD_compressed_ATC_texture()
{
	glloader_init();
	return glloader_GLES_AMD_compressed_ATC_texture();
}
glloader_GLES_AMD_compressed_ATC_textureFUNC glloader_GLES_AMD_compressed_ATC_texture = self_init_glloader_GLES_AMD_compressed_ATC_texture;

static char GLLOADER_APIENTRY _glloader_GLES_AMD_performance_monitor()
{
	return _GLES_AMD_performance_monitor;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_AMD_performance_monitor()
{
	glloader_init();
	return glloader_GLES_AMD_performance_monitor();
}
glloader_GLES_AMD_performance_monitorFUNC glloader_GLES_AMD_performance_monitor = self_init_glloader_GLES_AMD_performance_monitor;

#ifdef GLES_AMD_performance_monitor

static void GLLOADER_APIENTRY self_init_glGetPerfMonitorGroupsAMD(GLint* numGroups, GLsizei groupsSize, GLuint* groups)
{
	glloader_init();
	glGetPerfMonitorGroupsAMD(numGroups, groupsSize, groups);
}
static void GLLOADER_APIENTRY self_init_glGetPerfMonitorCountersAMD(GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei countersSize, GLuint* counters)
{
	glloader_init();
	glGetPerfMonitorCountersAMD(group, numCounters, maxActiveCounters, countersSize, counters);
}
static void GLLOADER_APIENTRY self_init_glGetPerfMonitorGroupStringAMD(GLuint group, GLsizei bufSize, GLsizei* length, GLchar* groupString)
{
	glloader_init();
	glGetPerfMonitorGroupStringAMD(group, bufSize, length, groupString);
}
static void GLLOADER_APIENTRY self_init_glGetPerfMonitorCounterStringAMD(GLuint group, GLuint count, GLsizei bufSize, GLsizei* length, GLchar* counterString)
{
	glloader_init();
	glGetPerfMonitorCounterStringAMD(group, count, bufSize, length, counterString);
}
static void GLLOADER_APIENTRY self_init_glGetPerfMonitorCounterInfoAMD(GLuint group, GLuint counter, GLenum pname, GLvoid* data)
{
	glloader_init();
	glGetPerfMonitorCounterInfoAMD(group, counter, pname, data);
}
static void GLLOADER_APIENTRY self_init_glGenPerfMonitorsAMD(GLsizei n, GLuint* monitors)
{
	glloader_init();
	glGenPerfMonitorsAMD(n, monitors);
}
static void GLLOADER_APIENTRY self_init_glDeletePerfMonitorsAMD(GLsizei n, GLuint* monitors)
{
	glloader_init();
	glDeletePerfMonitorsAMD(n, monitors);
}
static void GLLOADER_APIENTRY self_init_glSelectPerfMonitorCountersAMD(GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* counterList)
{
	glloader_init();
	glSelectPerfMonitorCountersAMD(monitor, enable, group, numCounters, counterList);
}
static void GLLOADER_APIENTRY self_init_glBeginPerfMonitorAMD(GLuint monitor)
{
	glloader_init();
	glBeginPerfMonitorAMD(monitor);
}
static void GLLOADER_APIENTRY self_init_glEndPerfMonitorAMD(GLuint monitor)
{
	glloader_init();
	glEndPerfMonitorAMD(monitor);
}
static void GLLOADER_APIENTRY self_init_glGetPerfMonitorCounterDataAMD(GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten)
{
	glloader_init();
	glGetPerfMonitorCounterDataAMD(monitor, pname, dataSize, data, bytesWritten);
}

glGetPerfMonitorGroupsAMDFUNC glGetPerfMonitorGroupsAMD = self_init_glGetPerfMonitorGroupsAMD;
glGetPerfMonitorCountersAMDFUNC glGetPerfMonitorCountersAMD = self_init_glGetPerfMonitorCountersAMD;
glGetPerfMonitorGroupStringAMDFUNC glGetPerfMonitorGroupStringAMD = self_init_glGetPerfMonitorGroupStringAMD;
glGetPerfMonitorCounterStringAMDFUNC glGetPerfMonitorCounterStringAMD = self_init_glGetPerfMonitorCounterStringAMD;
glGetPerfMonitorCounterInfoAMDFUNC glGetPerfMonitorCounterInfoAMD = self_init_glGetPerfMonitorCounterInfoAMD;
glGenPerfMonitorsAMDFUNC glGenPerfMonitorsAMD = self_init_glGenPerfMonitorsAMD;
glDeletePerfMonitorsAMDFUNC glDeletePerfMonitorsAMD = self_init_glDeletePerfMonitorsAMD;
glSelectPerfMonitorCountersAMDFUNC glSelectPerfMonitorCountersAMD = self_init_glSelectPerfMonitorCountersAMD;
glBeginPerfMonitorAMDFUNC glBeginPerfMonitorAMD = self_init_glBeginPerfMonitorAMD;
glEndPerfMonitorAMDFUNC glEndPerfMonitorAMD = self_init_glEndPerfMonitorAMD;
glGetPerfMonitorCounterDataAMDFUNC glGetPerfMonitorCounterDataAMD = self_init_glGetPerfMonitorCounterDataAMD;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_AMD_program_binary_Z400()
{
	return _GLES_AMD_program_binary_Z400;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_AMD_program_binary_Z400()
{
	glloader_init();
	return glloader_GLES_AMD_program_binary_Z400();
}
glloader_GLES_AMD_program_binary_Z400FUNC glloader_GLES_AMD_program_binary_Z400 = self_init_glloader_GLES_AMD_program_binary_Z400;

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_framebuffer_blit()
{
	return _GLES_ANGLE_framebuffer_blit;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_framebuffer_blit()
{
	glloader_init();
	return glloader_GLES_ANGLE_framebuffer_blit();
}
glloader_GLES_ANGLE_framebuffer_blitFUNC glloader_GLES_ANGLE_framebuffer_blit = self_init_glloader_GLES_ANGLE_framebuffer_blit;

#ifdef GLES_ANGLE_framebuffer_blit

static void GLLOADER_APIENTRY self_init_glBlitFramebufferANGLE(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
	glloader_init();
	glBlitFramebufferANGLE(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

glBlitFramebufferANGLEFUNC glBlitFramebufferANGLE = self_init_glBlitFramebufferANGLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_framebuffer_multisample()
{
	return _GLES_ANGLE_framebuffer_multisample;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_framebuffer_multisample()
{
	glloader_init();
	return glloader_GLES_ANGLE_framebuffer_multisample();
}
glloader_GLES_ANGLE_framebuffer_multisampleFUNC glloader_GLES_ANGLE_framebuffer_multisample = self_init_glloader_GLES_ANGLE_framebuffer_multisample;

#ifdef GLES_ANGLE_framebuffer_multisample

static void GLLOADER_APIENTRY self_init_glRenderbufferStorageMultisampleANGLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glRenderbufferStorageMultisampleANGLE(target, samples, internalformat, width, height);
}

glRenderbufferStorageMultisampleANGLEFUNC glRenderbufferStorageMultisampleANGLE = self_init_glRenderbufferStorageMultisampleANGLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_instanced_arrays()
{
	return _GLES_ANGLE_instanced_arrays;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_instanced_arrays()
{
	glloader_init();
	return glloader_GLES_ANGLE_instanced_arrays();
}
glloader_GLES_ANGLE_instanced_arraysFUNC glloader_GLES_ANGLE_instanced_arrays = self_init_glloader_GLES_ANGLE_instanced_arrays;

#ifdef GLES_ANGLE_instanced_arrays

static void GLLOADER_APIENTRY self_init_glDrawArraysInstancedANGLE(GLenum mode, GLint first, GLsizei count, GLsizei primcount)
{
	glloader_init();
	glDrawArraysInstancedANGLE(mode, first, count, primcount);
}
static void GLLOADER_APIENTRY self_init_glDrawElementsInstancedANGLE(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount)
{
	glloader_init();
	glDrawElementsInstancedANGLE(mode, count, type, indices, primcount);
}
static void GLLOADER_APIENTRY self_init_glVertexAttribDivisorANGLE(GLuint index, GLuint divisor)
{
	glloader_init();
	glVertexAttribDivisorANGLE(index, divisor);
}

glDrawArraysInstancedANGLEFUNC glDrawArraysInstancedANGLE = self_init_glDrawArraysInstancedANGLE;
glDrawElementsInstancedANGLEFUNC glDrawElementsInstancedANGLE = self_init_glDrawElementsInstancedANGLE;
glVertexAttribDivisorANGLEFUNC glVertexAttribDivisorANGLE = self_init_glVertexAttribDivisorANGLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_pack_reverse_row_order()
{
	return _GLES_ANGLE_pack_reverse_row_order;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_pack_reverse_row_order()
{
	glloader_init();
	return glloader_GLES_ANGLE_pack_reverse_row_order();
}
glloader_GLES_ANGLE_pack_reverse_row_orderFUNC glloader_GLES_ANGLE_pack_reverse_row_order = self_init_glloader_GLES_ANGLE_pack_reverse_row_order;

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_texture_compression_dxt3()
{
	return _GLES_ANGLE_texture_compression_dxt3;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_texture_compression_dxt3()
{
	glloader_init();
	return glloader_GLES_ANGLE_texture_compression_dxt3();
}
glloader_GLES_ANGLE_texture_compression_dxt3FUNC glloader_GLES_ANGLE_texture_compression_dxt3 = self_init_glloader_GLES_ANGLE_texture_compression_dxt3;

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_texture_compression_dxt5()
{
	return _GLES_ANGLE_texture_compression_dxt5;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_texture_compression_dxt5()
{
	glloader_init();
	return glloader_GLES_ANGLE_texture_compression_dxt5();
}
glloader_GLES_ANGLE_texture_compression_dxt5FUNC glloader_GLES_ANGLE_texture_compression_dxt5 = self_init_glloader_GLES_ANGLE_texture_compression_dxt5;

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_texture_usage()
{
	return _GLES_ANGLE_texture_usage;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_texture_usage()
{
	glloader_init();
	return glloader_GLES_ANGLE_texture_usage();
}
glloader_GLES_ANGLE_texture_usageFUNC glloader_GLES_ANGLE_texture_usage = self_init_glloader_GLES_ANGLE_texture_usage;

static char GLLOADER_APIENTRY _glloader_GLES_ANGLE_translated_shader_source()
{
	return _GLES_ANGLE_translated_shader_source;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ANGLE_translated_shader_source()
{
	glloader_init();
	return glloader_GLES_ANGLE_translated_shader_source();
}
glloader_GLES_ANGLE_translated_shader_sourceFUNC glloader_GLES_ANGLE_translated_shader_source = self_init_glloader_GLES_ANGLE_translated_shader_source;

#ifdef GLES_ANGLE_translated_shader_source

static void GLLOADER_APIENTRY self_init_glGetTranslatedShaderSourceANGLE(GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source)
{
	glloader_init();
	glGetTranslatedShaderSourceANGLE(shader, bufsize, length, source);
}

glGetTranslatedShaderSourceANGLEFUNC glGetTranslatedShaderSourceANGLE = self_init_glGetTranslatedShaderSourceANGLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_copy_texture_levels()
{
	return _GLES_APPLE_copy_texture_levels;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_copy_texture_levels()
{
	glloader_init();
	return glloader_GLES_APPLE_copy_texture_levels();
}
glloader_GLES_APPLE_copy_texture_levelsFUNC glloader_GLES_APPLE_copy_texture_levels = self_init_glloader_GLES_APPLE_copy_texture_levels;

#ifdef GLES_APPLE_copy_texture_levels

static void GLLOADER_APIENTRY self_init_glCopyTextureLevelsAPPLE(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount)
{
	glloader_init();
	glCopyTextureLevelsAPPLE(destinationTexture, sourceTexture, sourceBaseLevel, sourceLevelCount);
}

glCopyTextureLevelsAPPLEFUNC glCopyTextureLevelsAPPLE = self_init_glCopyTextureLevelsAPPLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_framebuffer_multisample()
{
	return _GLES_APPLE_framebuffer_multisample;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_framebuffer_multisample()
{
	glloader_init();
	return glloader_GLES_APPLE_framebuffer_multisample();
}
glloader_GLES_APPLE_framebuffer_multisampleFUNC glloader_GLES_APPLE_framebuffer_multisample = self_init_glloader_GLES_APPLE_framebuffer_multisample;

#ifdef GLES_APPLE_framebuffer_multisample

static void GLLOADER_APIENTRY self_init_glRenderbufferStorageMultisampleAPPLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glRenderbufferStorageMultisampleAPPLE(target, samples, internalformat, width, height);
}

glRenderbufferStorageMultisampleAPPLEFUNC glRenderbufferStorageMultisampleAPPLE = self_init_glRenderbufferStorageMultisampleAPPLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_rgb_422()
{
	return _GLES_APPLE_rgb_422;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_rgb_422()
{
	glloader_init();
	return glloader_GLES_APPLE_rgb_422();
}
glloader_GLES_APPLE_rgb_422FUNC glloader_GLES_APPLE_rgb_422 = self_init_glloader_GLES_APPLE_rgb_422;

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_sync()
{
	return _GLES_APPLE_sync;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_sync()
{
	glloader_init();
	return glloader_GLES_APPLE_sync();
}
glloader_GLES_APPLE_syncFUNC glloader_GLES_APPLE_sync = self_init_glloader_GLES_APPLE_sync;

#ifdef GLES_APPLE_sync

static GLsync GLLOADER_APIENTRY self_init_glFenceSyncAPPLE(GLenum condition, GLbitfield flags)
{
	glloader_init();
	return glFenceSyncAPPLE(condition, flags);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsSyncAPPLE(GLsync sync)
{
	glloader_init();
	return glIsSyncAPPLE(sync);
}
static void GLLOADER_APIENTRY self_init_glDeleteSyncAPPLE(GLsync sync)
{
	glloader_init();
	glDeleteSyncAPPLE(sync);
}
static GLenum GLLOADER_APIENTRY self_init_glClientWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	glloader_init();
	return glClientWaitSyncAPPLE(sync, flags, timeout);
}
static void GLLOADER_APIENTRY self_init_glWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	glloader_init();
	glWaitSyncAPPLE(sync, flags, timeout);
}
static void GLLOADER_APIENTRY self_init_glGetInteger64vAPPLE(GLenum pname, GLint64* params)
{
	glloader_init();
	glGetInteger64vAPPLE(pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetSyncivAPPLE(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values)
{
	glloader_init();
	glGetSyncivAPPLE(sync, pname, bufSize, length, values);
}

glFenceSyncAPPLEFUNC glFenceSyncAPPLE = self_init_glFenceSyncAPPLE;
glIsSyncAPPLEFUNC glIsSyncAPPLE = self_init_glIsSyncAPPLE;
glDeleteSyncAPPLEFUNC glDeleteSyncAPPLE = self_init_glDeleteSyncAPPLE;
glClientWaitSyncAPPLEFUNC glClientWaitSyncAPPLE = self_init_glClientWaitSyncAPPLE;
glWaitSyncAPPLEFUNC glWaitSyncAPPLE = self_init_glWaitSyncAPPLE;
glGetInteger64vAPPLEFUNC glGetInteger64vAPPLE = self_init_glGetInteger64vAPPLE;
glGetSyncivAPPLEFUNC glGetSyncivAPPLE = self_init_glGetSyncivAPPLE;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_texture_2D_limited_npot()
{
	return _GLES_APPLE_texture_2D_limited_npot;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_texture_2D_limited_npot()
{
	glloader_init();
	return glloader_GLES_APPLE_texture_2D_limited_npot();
}
glloader_GLES_APPLE_texture_2D_limited_npotFUNC glloader_GLES_APPLE_texture_2D_limited_npot = self_init_glloader_GLES_APPLE_texture_2D_limited_npot;

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_texture_format_BGRA8888()
{
	return _GLES_APPLE_texture_format_BGRA8888;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_texture_format_BGRA8888()
{
	glloader_init();
	return glloader_GLES_APPLE_texture_format_BGRA8888();
}
glloader_GLES_APPLE_texture_format_BGRA8888FUNC glloader_GLES_APPLE_texture_format_BGRA8888 = self_init_glloader_GLES_APPLE_texture_format_BGRA8888;

static char GLLOADER_APIENTRY _glloader_GLES_APPLE_texture_max_level()
{
	return _GLES_APPLE_texture_max_level;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_APPLE_texture_max_level()
{
	glloader_init();
	return glloader_GLES_APPLE_texture_max_level();
}
glloader_GLES_APPLE_texture_max_levelFUNC glloader_GLES_APPLE_texture_max_level = self_init_glloader_GLES_APPLE_texture_max_level;

static char GLLOADER_APIENTRY _glloader_GLES_ARM_mali_shader_binary()
{
	return _GLES_ARM_mali_shader_binary;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ARM_mali_shader_binary()
{
	glloader_init();
	return glloader_GLES_ARM_mali_shader_binary();
}
glloader_GLES_ARM_mali_shader_binaryFUNC glloader_GLES_ARM_mali_shader_binary = self_init_glloader_GLES_ARM_mali_shader_binary;

static char GLLOADER_APIENTRY _glloader_GLES_ARM_rgba8()
{
	return _GLES_ARM_rgba8;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_ARM_rgba8()
{
	glloader_init();
	return glloader_GLES_ARM_rgba8();
}
glloader_GLES_ARM_rgba8FUNC glloader_GLES_ARM_rgba8 = self_init_glloader_GLES_ARM_rgba8;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_blend_minmax()
{
	return _GLES_EXT_blend_minmax;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_blend_minmax()
{
	glloader_init();
	return glloader_GLES_EXT_blend_minmax();
}
glloader_GLES_EXT_blend_minmaxFUNC glloader_GLES_EXT_blend_minmax = self_init_glloader_GLES_EXT_blend_minmax;

#ifdef GLES_EXT_blend_minmax

static void GLLOADER_APIENTRY self_init_glBlendEquationEXT(GLenum mode)
{
	glloader_init();
	glBlendEquationEXT(mode);
}

glBlendEquationEXTFUNC glBlendEquationEXT = self_init_glBlendEquationEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_color_buffer_half_float()
{
	return _GLES_EXT_color_buffer_half_float;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_color_buffer_half_float()
{
	glloader_init();
	return glloader_GLES_EXT_color_buffer_half_float();
}
glloader_GLES_EXT_color_buffer_half_floatFUNC glloader_GLES_EXT_color_buffer_half_float = self_init_glloader_GLES_EXT_color_buffer_half_float;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_debug_label()
{
	return _GLES_EXT_debug_label;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_debug_label()
{
	glloader_init();
	return glloader_GLES_EXT_debug_label();
}
glloader_GLES_EXT_debug_labelFUNC glloader_GLES_EXT_debug_label = self_init_glloader_GLES_EXT_debug_label;

#ifdef GLES_EXT_debug_label

static void GLLOADER_APIENTRY self_init_glLabelObjectEXT(GLenum type, GLuint object, GLsizei length, const GLchar* label)
{
	glloader_init();
	glLabelObjectEXT(type, object, length, label);
}
static void GLLOADER_APIENTRY self_init_glGetObjectLabelEXT(GLenum type, GLuint object, GLsizei bufSize, GLsizei* length, GLchar* label)
{
	glloader_init();
	glGetObjectLabelEXT(type, object, bufSize, length, label);
}

glLabelObjectEXTFUNC glLabelObjectEXT = self_init_glLabelObjectEXT;
glGetObjectLabelEXTFUNC glGetObjectLabelEXT = self_init_glGetObjectLabelEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_debug_marker()
{
	return _GLES_EXT_debug_marker;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_debug_marker()
{
	glloader_init();
	return glloader_GLES_EXT_debug_marker();
}
glloader_GLES_EXT_debug_markerFUNC glloader_GLES_EXT_debug_marker = self_init_glloader_GLES_EXT_debug_marker;

#ifdef GLES_EXT_debug_marker

static void GLLOADER_APIENTRY self_init_glInsertEventMarkerEXT(GLsizei length, const GLchar* marker)
{
	glloader_init();
	glInsertEventMarkerEXT(length, marker);
}
static void GLLOADER_APIENTRY self_init_glPushGroupMarkerEXT(GLsizei length, const GLchar* marker)
{
	glloader_init();
	glPushGroupMarkerEXT(length, marker);
}
static void GLLOADER_APIENTRY self_init_glPopGroupMarkerEXT()
{
	glloader_init();
	glPopGroupMarkerEXT();
}

glInsertEventMarkerEXTFUNC glInsertEventMarkerEXT = self_init_glInsertEventMarkerEXT;
glPushGroupMarkerEXTFUNC glPushGroupMarkerEXT = self_init_glPushGroupMarkerEXT;
glPopGroupMarkerEXTFUNC glPopGroupMarkerEXT = self_init_glPopGroupMarkerEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_discard_framebuffer()
{
	return _GLES_EXT_discard_framebuffer;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_discard_framebuffer()
{
	glloader_init();
	return glloader_GLES_EXT_discard_framebuffer();
}
glloader_GLES_EXT_discard_framebufferFUNC glloader_GLES_EXT_discard_framebuffer = self_init_glloader_GLES_EXT_discard_framebuffer;

#ifdef GLES_EXT_discard_framebuffer

static void GLLOADER_APIENTRY self_init_glDiscardFramebufferEXT(GLenum target, GLsizei numAttachments, const GLenum* attachments)
{
	glloader_init();
	glDiscardFramebufferEXT(target, numAttachments, attachments);
}

glDiscardFramebufferEXTFUNC glDiscardFramebufferEXT = self_init_glDiscardFramebufferEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_frag_depth()
{
	return _GLES_EXT_frag_depth;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_frag_depth()
{
	glloader_init();
	return glloader_GLES_EXT_frag_depth();
}
glloader_GLES_EXT_frag_depthFUNC glloader_GLES_EXT_frag_depth = self_init_glloader_GLES_EXT_frag_depth;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_map_buffer_range()
{
	return _GLES_EXT_map_buffer_range;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_map_buffer_range()
{
	glloader_init();
	return glloader_GLES_EXT_map_buffer_range();
}
glloader_GLES_EXT_map_buffer_rangeFUNC glloader_GLES_EXT_map_buffer_range = self_init_glloader_GLES_EXT_map_buffer_range;

#ifdef GLES_EXT_map_buffer_range

static void* GLLOADER_APIENTRY self_init_glMapBufferRangeEXT(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	glloader_init();
	return glMapBufferRangeEXT(target, offset, length, access);
}
static void GLLOADER_APIENTRY self_init_glFlushMappedBufferRangeEXT(GLenum target, GLintptr offset, GLsizeiptr length)
{
	glloader_init();
	glFlushMappedBufferRangeEXT(target, offset, length);
}

glMapBufferRangeEXTFUNC glMapBufferRangeEXT = self_init_glMapBufferRangeEXT;
glFlushMappedBufferRangeEXTFUNC glFlushMappedBufferRangeEXT = self_init_glFlushMappedBufferRangeEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_multisampled_render_to_texture()
{
	return _GLES_EXT_multisampled_render_to_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_multisampled_render_to_texture()
{
	glloader_init();
	return glloader_GLES_EXT_multisampled_render_to_texture();
}
glloader_GLES_EXT_multisampled_render_to_textureFUNC glloader_GLES_EXT_multisampled_render_to_texture = self_init_glloader_GLES_EXT_multisampled_render_to_texture;

#ifdef GLES_EXT_multisampled_render_to_texture

static void GLLOADER_APIENTRY self_init_glRenderbufferStorageMultisampleEXT(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glRenderbufferStorageMultisampleEXT(target, samples, internalformat, width, height);
}
static void GLLOADER_APIENTRY self_init_glFramebufferTexture2DMultisampleEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
	glloader_init();
	glFramebufferTexture2DMultisampleEXT(target, attachment, textarget, texture, level, samples);
}

glRenderbufferStorageMultisampleEXTFUNC glRenderbufferStorageMultisampleEXT = self_init_glRenderbufferStorageMultisampleEXT;
glFramebufferTexture2DMultisampleEXTFUNC glFramebufferTexture2DMultisampleEXT = self_init_glFramebufferTexture2DMultisampleEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_multiview_draw_buffers()
{
	return _GLES_EXT_multiview_draw_buffers;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_multiview_draw_buffers()
{
	glloader_init();
	return glloader_GLES_EXT_multiview_draw_buffers();
}
glloader_GLES_EXT_multiview_draw_buffersFUNC glloader_GLES_EXT_multiview_draw_buffers = self_init_glloader_GLES_EXT_multiview_draw_buffers;

#ifdef GLES_EXT_multiview_draw_buffers

static void GLLOADER_APIENTRY self_init_glReadBufferIndexedEXT(GLenum src, GLint index)
{
	glloader_init();
	glReadBufferIndexedEXT(src, index);
}
static void GLLOADER_APIENTRY self_init_glDrawBuffersIndexedEXT(GLint n, const GLenum* location, const GLint* indices)
{
	glloader_init();
	glDrawBuffersIndexedEXT(n, location, indices);
}
static void GLLOADER_APIENTRY self_init_GetIntegeri_vEXT(GLenum target, GLuint index, GLint* data)
{
	glloader_init();
	GetIntegeri_vEXT(target, index, data);
}

glReadBufferIndexedEXTFUNC glReadBufferIndexedEXT = self_init_glReadBufferIndexedEXT;
glDrawBuffersIndexedEXTFUNC glDrawBuffersIndexedEXT = self_init_glDrawBuffersIndexedEXT;
GetIntegeri_vEXTFUNC GetIntegeri_vEXT = self_init_GetIntegeri_vEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_multi_draw_arrays()
{
	return _GLES_EXT_multi_draw_arrays;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_multi_draw_arrays()
{
	glloader_init();
	return glloader_GLES_EXT_multi_draw_arrays();
}
glloader_GLES_EXT_multi_draw_arraysFUNC glloader_GLES_EXT_multi_draw_arrays = self_init_glloader_GLES_EXT_multi_draw_arrays;

#ifdef GLES_EXT_multi_draw_arrays

static void GLLOADER_APIENTRY self_init_glMultiDrawArraysEXT(GLenum mode, GLint* first, GLsizei* count, GLsizei primcount)
{
	glloader_init();
	glMultiDrawArraysEXT(mode, first, count, primcount);
}
static void GLLOADER_APIENTRY self_init_glMultiDrawElementsEXT(GLenum mode, GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount)
{
	glloader_init();
	glMultiDrawElementsEXT(mode, count, type, indices, primcount);
}

glMultiDrawArraysEXTFUNC glMultiDrawArraysEXT = self_init_glMultiDrawArraysEXT;
glMultiDrawElementsEXTFUNC glMultiDrawElementsEXT = self_init_glMultiDrawElementsEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_occlusion_query_boolean()
{
	return _GLES_EXT_occlusion_query_boolean;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_occlusion_query_boolean()
{
	glloader_init();
	return glloader_GLES_EXT_occlusion_query_boolean();
}
glloader_GLES_EXT_occlusion_query_booleanFUNC glloader_GLES_EXT_occlusion_query_boolean = self_init_glloader_GLES_EXT_occlusion_query_boolean;

#ifdef GLES_EXT_occlusion_query_boolean

static void GLLOADER_APIENTRY self_init_glGenQueriesEXT(GLsizei n, GLuint* ids)
{
	glloader_init();
	glGenQueriesEXT(n, ids);
}
static void GLLOADER_APIENTRY self_init_glDeleteQueriesEXT(GLsizei n, const GLuint* ids)
{
	glloader_init();
	glDeleteQueriesEXT(n, ids);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsQueryEXT(GLuint id)
{
	glloader_init();
	return glIsQueryEXT(id);
}
static void GLLOADER_APIENTRY self_init_glBeginQueryEXT(GLenum target, GLuint id)
{
	glloader_init();
	glBeginQueryEXT(target, id);
}
static void GLLOADER_APIENTRY self_init_glEndQueryEXT(GLenum target)
{
	glloader_init();
	glEndQueryEXT(target);
}
static void GLLOADER_APIENTRY self_init_glGetQueryivEXT(GLenum target, GLenum pname, GLint* param)
{
	glloader_init();
	glGetQueryivEXT(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glGetQueryObjectuivEXT(GLuint id, GLenum pname, GLuint* params)
{
	glloader_init();
	glGetQueryObjectuivEXT(id, pname, params);
}

glGenQueriesEXTFUNC glGenQueriesEXT = self_init_glGenQueriesEXT;
glDeleteQueriesEXTFUNC glDeleteQueriesEXT = self_init_glDeleteQueriesEXT;
glIsQueryEXTFUNC glIsQueryEXT = self_init_glIsQueryEXT;
glBeginQueryEXTFUNC glBeginQueryEXT = self_init_glBeginQueryEXT;
glEndQueryEXTFUNC glEndQueryEXT = self_init_glEndQueryEXT;
glGetQueryivEXTFUNC glGetQueryivEXT = self_init_glGetQueryivEXT;
glGetQueryObjectuivEXTFUNC glGetQueryObjectuivEXT = self_init_glGetQueryObjectuivEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_read_format_bgra()
{
	return _GLES_read_format_bgra;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_read_format_bgra()
{
	glloader_init();
	return glloader_GLES_read_format_bgra();
}
glloader_GLES_read_format_bgraFUNC glloader_GLES_read_format_bgra = self_init_glloader_GLES_read_format_bgra;

static char GLLOADER_APIENTRY _glloader_GL_EXT_robustness()
{
	return _GL_EXT_robustness;
}

static char GLLOADER_APIENTRY self_init_glloader_GL_EXT_robustness()
{
	glloader_init();
	return glloader_GL_EXT_robustness();
}
glloader_GL_EXT_robustnessFUNC glloader_GL_EXT_robustness = self_init_glloader_GL_EXT_robustness;

#ifdef GL_EXT_robustness

static GLenum GLLOADER_APIENTRY self_init_glGetGraphicsResetStatusEXT()
{
	glloader_init();
	return glGetGraphicsResetStatusEXT();
}
static void GLLOADER_APIENTRY self_init_glReadnPixelsEXT(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data)
{
	glloader_init();
	glReadnPixelsEXT(x, y, width, height, format, type, bufSize, data);
}
static void GLLOADER_APIENTRY self_init_glGetnUniformfvEXT(GLuint program, GLint location, GLsizei bufSize, GLfloat* params)
{
	glloader_init();
	glGetnUniformfvEXT(program, location, bufSize, params);
}
static void GLLOADER_APIENTRY self_init_glGetnUniformivEXT(GLuint program, GLint location, GLsizei bufSize, GLint* params)
{
	glloader_init();
	glGetnUniformivEXT(program, location, bufSize, params);
}

glGetGraphicsResetStatusEXTFUNC glGetGraphicsResetStatusEXT = self_init_glGetGraphicsResetStatusEXT;
glReadnPixelsEXTFUNC glReadnPixelsEXT = self_init_glReadnPixelsEXT;
glGetnUniformfvEXTFUNC glGetnUniformfvEXT = self_init_glGetnUniformfvEXT;
glGetnUniformivEXTFUNC glGetnUniformivEXT = self_init_glGetnUniformivEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_separate_shader_objects()
{
	return _GLES_EXT_separate_shader_objects;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_separate_shader_objects()
{
	glloader_init();
	return glloader_GLES_EXT_separate_shader_objects();
}
glloader_GLES_EXT_separate_shader_objectsFUNC glloader_GLES_EXT_separate_shader_objects = self_init_glloader_GLES_EXT_separate_shader_objects;

#ifdef GLES_EXT_separate_shader_objects

static void GLLOADER_APIENTRY self_init_glUseShaderProgramEXT(GLenum type, GLuint program)
{
	glloader_init();
	glUseShaderProgramEXT(type, program);
}
static void GLLOADER_APIENTRY self_init_glActiveProgramEXT(GLuint program)
{
	glloader_init();
	glActiveProgramEXT(program);
}
static GLuint GLLOADER_APIENTRY self_init_glCreateShaderProgramEXT(GLenum type, const GLchar* string)
{
	glloader_init();
	return glCreateShaderProgramEXT(type, string);
}
static GLboolean GLLOADER_APIENTRY self_init_glBindProgramPipelineEXT(GLuint pipeline)
{
	glloader_init();
	return glBindProgramPipelineEXT(pipeline);
}
static GLboolean GLLOADER_APIENTRY self_init_glDeleteProgramPipelinesEXT(GLsizei n, const GLuint* pipelines)
{
	glloader_init();
	return glDeleteProgramPipelinesEXT(n, pipelines);
}
static GLboolean GLLOADER_APIENTRY self_init_glGenProgramPipelinesEXT(GLsizei n, GLuint* pipelines)
{
	glloader_init();
	return glGenProgramPipelinesEXT(n, pipelines);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsProgramPipelineEXT(GLuint pipeline)
{
	glloader_init();
	return glIsProgramPipelineEXT(pipeline);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramParameteriEXT(GLuint program, GLenum pname, GLint value)
{
	glloader_init();
	return glProgramParameteriEXT(program, pname, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glGetProgramPipelineivEXT(GLuint pipeline, GLenum pname, GLint* params)
{
	glloader_init();
	return glGetProgramPipelineivEXT(pipeline, pname, params);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform1iEXT(GLuint program, GLint location, GLint x)
{
	glloader_init();
	return glProgramUniform1iEXT(program, location, x);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform2iEXT(GLuint program, GLint location, GLint x, GLint y)
{
	glloader_init();
	return glProgramUniform2iEXT(program, location, x, y);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform3iEXT(GLuint program, GLint location, GLint x, GLint y, GLint z)
{
	glloader_init();
	return glProgramUniform3iEXT(program, location, x, y, z);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform4iEXT(GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w)
{
	glloader_init();
	return glProgramUniform4iEXT(program, location, x, y, z, w);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform1fEXT(GLuint program, GLint location, GLfloat x)
{
	glloader_init();
	return glProgramUniform1fEXT(program, location, x);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform2fEXT(GLuint program, GLint location, GLfloat x, GLfloat y)
{
	glloader_init();
	return glProgramUniform2fEXT(program, location, x, y);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform3fEXT(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	glloader_init();
	return glProgramUniform3fEXT(program, location, x, y, z);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform4fEXT(GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glloader_init();
	return glProgramUniform4fEXT(program, location, x, y, z, w);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform1ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	glloader_init();
	return glProgramUniform1ivEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform2ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	glloader_init();
	return glProgramUniform2ivEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform3ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	glloader_init();
	return glProgramUniform3ivEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform4ivEXT(GLuint program, GLint location, GLsizei count, const GLint* value)
{
	glloader_init();
	return glProgramUniform4ivEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform1fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	glloader_init();
	return glProgramUniform1fvEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform2fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	glloader_init();
	return glProgramUniform2fvEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform3fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	glloader_init();
	return glProgramUniform3fvEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniform4fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat* value)
{
	glloader_init();
	return glProgramUniform4fvEXT(program, location, count, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniformMatrix2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glloader_init();
	return glProgramUniformMatrix2fvEXT(program, location, count, transpose, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniformMatrix3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glloader_init();
	return glProgramUniformMatrix3fvEXT(program, location, count, transpose, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glProgramUniformMatrix4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glloader_init();
	return glProgramUniformMatrix4fvEXT(program, location, count, transpose, value);
}
static GLboolean GLLOADER_APIENTRY self_init_glValidateProgramPipelineEXT(GLuint pipeline)
{
	glloader_init();
	return glValidateProgramPipelineEXT(pipeline);
}
static GLboolean GLLOADER_APIENTRY self_init_glGetProgramPipelineInfoLogEXT(GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	glloader_init();
	return glGetProgramPipelineInfoLogEXT(pipeline, bufSize, length, infoLog);
}

glUseShaderProgramEXTFUNC glUseShaderProgramEXT = self_init_glUseShaderProgramEXT;
glActiveProgramEXTFUNC glActiveProgramEXT = self_init_glActiveProgramEXT;
glCreateShaderProgramEXTFUNC glCreateShaderProgramEXT = self_init_glCreateShaderProgramEXT;
glBindProgramPipelineEXTFUNC glBindProgramPipelineEXT = self_init_glBindProgramPipelineEXT;
glDeleteProgramPipelinesEXTFUNC glDeleteProgramPipelinesEXT = self_init_glDeleteProgramPipelinesEXT;
glGenProgramPipelinesEXTFUNC glGenProgramPipelinesEXT = self_init_glGenProgramPipelinesEXT;
glIsProgramPipelineEXTFUNC glIsProgramPipelineEXT = self_init_glIsProgramPipelineEXT;
glProgramParameteriEXTFUNC glProgramParameteriEXT = self_init_glProgramParameteriEXT;
glGetProgramPipelineivEXTFUNC glGetProgramPipelineivEXT = self_init_glGetProgramPipelineivEXT;
glProgramUniform1iEXTFUNC glProgramUniform1iEXT = self_init_glProgramUniform1iEXT;
glProgramUniform2iEXTFUNC glProgramUniform2iEXT = self_init_glProgramUniform2iEXT;
glProgramUniform3iEXTFUNC glProgramUniform3iEXT = self_init_glProgramUniform3iEXT;
glProgramUniform4iEXTFUNC glProgramUniform4iEXT = self_init_glProgramUniform4iEXT;
glProgramUniform1fEXTFUNC glProgramUniform1fEXT = self_init_glProgramUniform1fEXT;
glProgramUniform2fEXTFUNC glProgramUniform2fEXT = self_init_glProgramUniform2fEXT;
glProgramUniform3fEXTFUNC glProgramUniform3fEXT = self_init_glProgramUniform3fEXT;
glProgramUniform4fEXTFUNC glProgramUniform4fEXT = self_init_glProgramUniform4fEXT;
glProgramUniform1ivEXTFUNC glProgramUniform1ivEXT = self_init_glProgramUniform1ivEXT;
glProgramUniform2ivEXTFUNC glProgramUniform2ivEXT = self_init_glProgramUniform2ivEXT;
glProgramUniform3ivEXTFUNC glProgramUniform3ivEXT = self_init_glProgramUniform3ivEXT;
glProgramUniform4ivEXTFUNC glProgramUniform4ivEXT = self_init_glProgramUniform4ivEXT;
glProgramUniform1fvEXTFUNC glProgramUniform1fvEXT = self_init_glProgramUniform1fvEXT;
glProgramUniform2fvEXTFUNC glProgramUniform2fvEXT = self_init_glProgramUniform2fvEXT;
glProgramUniform3fvEXTFUNC glProgramUniform3fvEXT = self_init_glProgramUniform3fvEXT;
glProgramUniform4fvEXTFUNC glProgramUniform4fvEXT = self_init_glProgramUniform4fvEXT;
glProgramUniformMatrix2fvEXTFUNC glProgramUniformMatrix2fvEXT = self_init_glProgramUniformMatrix2fvEXT;
glProgramUniformMatrix3fvEXTFUNC glProgramUniformMatrix3fvEXT = self_init_glProgramUniformMatrix3fvEXT;
glProgramUniformMatrix4fvEXTFUNC glProgramUniformMatrix4fvEXT = self_init_glProgramUniformMatrix4fvEXT;
glValidateProgramPipelineEXTFUNC glValidateProgramPipelineEXT = self_init_glValidateProgramPipelineEXT;
glGetProgramPipelineInfoLogEXTFUNC glGetProgramPipelineInfoLogEXT = self_init_glGetProgramPipelineInfoLogEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_shader_framebuffer_fetch()
{
	return _GLES_EXT_shader_framebuffer_fetch;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_shader_framebuffer_fetch()
{
	glloader_init();
	return glloader_GLES_EXT_shader_framebuffer_fetch();
}
glloader_GLES_EXT_shader_framebuffer_fetchFUNC glloader_GLES_EXT_shader_framebuffer_fetch = self_init_glloader_GLES_EXT_shader_framebuffer_fetch;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_shader_texture_lod()
{
	return _GLES_EXT_shader_texture_lod;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_shader_texture_lod()
{
	glloader_init();
	return glloader_GLES_EXT_shader_texture_lod();
}
glloader_GLES_EXT_shader_texture_lodFUNC glloader_GLES_EXT_shader_texture_lod = self_init_glloader_GLES_EXT_shader_texture_lod;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_shadow_samplers()
{
	return _GLES_EXT_shadow_samplers;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_shadow_samplers()
{
	glloader_init();
	return glloader_GLES_EXT_shadow_samplers();
}
glloader_GLES_EXT_shadow_samplersFUNC glloader_GLES_EXT_shadow_samplers = self_init_glloader_GLES_EXT_shadow_samplers;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_sRGB()
{
	return _GLES_EXT_sRGB;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_sRGB()
{
	glloader_init();
	return glloader_GLES_EXT_sRGB();
}
glloader_GLES_EXT_sRGBFUNC glloader_GLES_EXT_sRGB = self_init_glloader_GLES_EXT_sRGB;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_compression_dxt1()
{
	return _GLES_EXT_texture_compression_dxt1;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_compression_dxt1()
{
	glloader_init();
	return glloader_GLES_EXT_texture_compression_dxt1();
}
glloader_GLES_EXT_texture_compression_dxt1FUNC glloader_GLES_EXT_texture_compression_dxt1 = self_init_glloader_GLES_EXT_texture_compression_dxt1;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_compression_latc()
{
	return _GLES_EXT_texture_compression_latc;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_compression_latc()
{
	glloader_init();
	return glloader_GLES_EXT_texture_compression_latc();
}
glloader_GLES_EXT_texture_compression_latcFUNC glloader_GLES_EXT_texture_compression_latc = self_init_glloader_GLES_EXT_texture_compression_latc;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_compression_s3tc()
{
	return _GLES_EXT_texture_compression_s3tc;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_compression_s3tc()
{
	glloader_init();
	return glloader_GLES_EXT_texture_compression_s3tc();
}
glloader_GLES_EXT_texture_compression_s3tcFUNC glloader_GLES_EXT_texture_compression_s3tc = self_init_glloader_GLES_EXT_texture_compression_s3tc;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_filter_anisotropic()
{
	return _GLES_EXT_texture_filter_anisotropic;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_filter_anisotropic()
{
	glloader_init();
	return glloader_GLES_EXT_texture_filter_anisotropic();
}
glloader_GLES_EXT_texture_filter_anisotropicFUNC glloader_GLES_EXT_texture_filter_anisotropic = self_init_glloader_GLES_EXT_texture_filter_anisotropic;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_format_BGRA8888()
{
	return _GLES_EXT_texture_format_BGRA8888;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_format_BGRA8888()
{
	glloader_init();
	return glloader_GLES_EXT_texture_format_BGRA8888();
}
glloader_GLES_EXT_texture_format_BGRA8888FUNC glloader_GLES_EXT_texture_format_BGRA8888 = self_init_glloader_GLES_EXT_texture_format_BGRA8888;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_lod_bias()
{
	return _GLES_EXT_texture_lod_bias;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_lod_bias()
{
	glloader_init();
	return glloader_GLES_EXT_texture_lod_bias();
}
glloader_GLES_EXT_texture_lod_biasFUNC glloader_GLES_EXT_texture_lod_bias = self_init_glloader_GLES_EXT_texture_lod_bias;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_rg()
{
	return _GLES_EXT_texture_rg;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_rg()
{
	glloader_init();
	return glloader_GLES_EXT_texture_rg();
}
glloader_GLES_EXT_texture_rgFUNC glloader_GLES_EXT_texture_rg = self_init_glloader_GLES_EXT_texture_rg;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_storage()
{
	return _GLES_EXT_texture_storage;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_storage()
{
	glloader_init();
	return glloader_GLES_EXT_texture_storage();
}
glloader_GLES_EXT_texture_storageFUNC glloader_GLES_EXT_texture_storage = self_init_glloader_GLES_EXT_texture_storage;

#ifdef GLES_EXT_texture_storage

static void GLLOADER_APIENTRY self_init_glTexStorage1DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)
{
	glloader_init();
	glTexStorage1DEXT(target, levels, internalformat, width);
}
static void GLLOADER_APIENTRY self_init_glTexStorage2DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glTexStorage2DEXT(target, levels, internalformat, width, height);
}
static void GLLOADER_APIENTRY self_init_glTexStorage3DEXT(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	glloader_init();
	glTexStorage3DEXT(target, levels, internalformat, width, height, depth);
}
static void GLLOADER_APIENTRY self_init_glTextureStorage1DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)
{
	glloader_init();
	glTextureStorage1DEXT(texture, target, levels, internalformat, width);
}
static void GLLOADER_APIENTRY self_init_glTextureStorage2DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glTextureStorage2DEXT(texture, target, levels, internalformat, width, height);
}
static void GLLOADER_APIENTRY self_init_glTextureStorage3DEXT(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	glloader_init();
	glTextureStorage3DEXT(texture, target, levels, internalformat, width, height, depth);
}

glTexStorage1DEXTFUNC glTexStorage1DEXT = self_init_glTexStorage1DEXT;
glTexStorage2DEXTFUNC glTexStorage2DEXT = self_init_glTexStorage2DEXT;
glTexStorage3DEXTFUNC glTexStorage3DEXT = self_init_glTexStorage3DEXT;
glTextureStorage1DEXTFUNC glTextureStorage1DEXT = self_init_glTextureStorage1DEXT;
glTextureStorage2DEXTFUNC glTextureStorage2DEXT = self_init_glTextureStorage2DEXT;
glTextureStorage3DEXTFUNC glTextureStorage3DEXT = self_init_glTextureStorage3DEXT;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_EXT_texture_type_2_10_10_10_REV()
{
	return _GLES_EXT_texture_type_2_10_10_10_REV;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_texture_type_2_10_10_10_REV()
{
	glloader_init();
	return glloader_GLES_EXT_texture_type_2_10_10_10_REV();
}
glloader_GLES_EXT_texture_type_2_10_10_10_REVFUNC glloader_GLES_EXT_texture_type_2_10_10_10_REV = self_init_glloader_GLES_EXT_texture_type_2_10_10_10_REV;

static char GLLOADER_APIENTRY _glloader_GLES_EXT_unpack_subimage()
{
	return _GLES_EXT_unpack_subimage;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_EXT_unpack_subimage()
{
	glloader_init();
	return glloader_GLES_EXT_unpack_subimage();
}
glloader_GLES_EXT_unpack_subimageFUNC glloader_GLES_EXT_unpack_subimage = self_init_glloader_GLES_EXT_unpack_subimage;

static char GLLOADER_APIENTRY _glloader_GLES_FJ_shader_binary_GCCSO()
{
	return _GLES_FJ_shader_binary_GCCSO;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_FJ_shader_binary_GCCSO()
{
	glloader_init();
	return glloader_GLES_FJ_shader_binary_GCCSO();
}
glloader_GLES_FJ_shader_binary_GCCSOFUNC glloader_GLES_FJ_shader_binary_GCCSO = self_init_glloader_GLES_FJ_shader_binary_GCCSO;

static char GLLOADER_APIENTRY _glloader_GLES_IMG_multisampled_render_to_texture()
{
	return _GLES_IMG_multisampled_render_to_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_IMG_multisampled_render_to_texture()
{
	glloader_init();
	return glloader_GLES_IMG_multisampled_render_to_texture();
}
glloader_GLES_IMG_multisampled_render_to_textureFUNC glloader_GLES_IMG_multisampled_render_to_texture = self_init_glloader_GLES_IMG_multisampled_render_to_texture;

#ifdef GLES_IMG_multisampled_render_to_texture

static void GLLOADER_APIENTRY self_init_glRenderbufferStorageMultisampleIMG(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glRenderbufferStorageMultisampleIMG(target, samples, internalformat, width, height);
}
static void GLLOADER_APIENTRY self_init_glFramebufferTexture2DMultisampleIMG(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)
{
	glloader_init();
	glFramebufferTexture2DMultisampleIMG(target, attachment, textarget, texture, level, samples);
}

glRenderbufferStorageMultisampleIMGFUNC glRenderbufferStorageMultisampleIMG = self_init_glRenderbufferStorageMultisampleIMG;
glFramebufferTexture2DMultisampleIMGFUNC glFramebufferTexture2DMultisampleIMG = self_init_glFramebufferTexture2DMultisampleIMG;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_IMG_program_binary()
{
	return _GLES_IMG_program_binary;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_IMG_program_binary()
{
	glloader_init();
	return glloader_GLES_IMG_program_binary();
}
glloader_GLES_IMG_program_binaryFUNC glloader_GLES_IMG_program_binary = self_init_glloader_GLES_IMG_program_binary;

static char GLLOADER_APIENTRY _glloader_GLES_IMG_read_format()
{
	return _GLES_IMG_read_format;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_IMG_read_format()
{
	glloader_init();
	return glloader_GLES_IMG_read_format();
}
glloader_GLES_IMG_read_formatFUNC glloader_GLES_IMG_read_format = self_init_glloader_GLES_IMG_read_format;

static char GLLOADER_APIENTRY _glloader_GLES_shader_binary()
{
	return _GLES_shader_binary;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_shader_binary()
{
	glloader_init();
	return glloader_GLES_shader_binary();
}
glloader_GLES_shader_binaryFUNC glloader_GLES_shader_binary = self_init_glloader_GLES_shader_binary;

static char GLLOADER_APIENTRY _glloader_GLES_IMG_texture_compression_pvrtc()
{
	return _GLES_IMG_texture_compression_pvrtc;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_IMG_texture_compression_pvrtc()
{
	glloader_init();
	return glloader_GLES_IMG_texture_compression_pvrtc();
}
glloader_GLES_IMG_texture_compression_pvrtcFUNC glloader_GLES_IMG_texture_compression_pvrtc = self_init_glloader_GLES_IMG_texture_compression_pvrtc;

static char GLLOADER_APIENTRY _glloader_GLES_IMG_texture_env_enhanced_fixed_function()
{
	return _GLES_IMG_texture_env_enhanced_fixed_function;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_IMG_texture_env_enhanced_fixed_function()
{
	glloader_init();
	return glloader_GLES_IMG_texture_env_enhanced_fixed_function();
}
glloader_GLES_IMG_texture_env_enhanced_fixed_functionFUNC glloader_GLES_IMG_texture_env_enhanced_fixed_function = self_init_glloader_GLES_IMG_texture_env_enhanced_fixed_function;

static char GLLOADER_APIENTRY _glloader_GLES_IMG_user_clip_plane()
{
	return _GLES_IMG_user_clip_plane;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_IMG_user_clip_plane()
{
	glloader_init();
	return glloader_GLES_IMG_user_clip_plane();
}
glloader_GLES_IMG_user_clip_planeFUNC glloader_GLES_IMG_user_clip_plane = self_init_glloader_GLES_IMG_user_clip_plane;

#ifdef GLES_IMG_user_clip_plane

static void GLLOADER_APIENTRY self_init_glClipPlanefIMG(GLenum p, GLfloat* eqn)
{
	glloader_init();
	glClipPlanefIMG(p, eqn);
}
static void GLLOADER_APIENTRY self_init_glClipPlanexIMG(GLenum p, GLfixed* eqn)
{
	glloader_init();
	glClipPlanexIMG(p, eqn);
}

glClipPlanefIMGFUNC glClipPlanefIMG = self_init_glClipPlanefIMG;
glClipPlanexIMGFUNC glClipPlanexIMG = self_init_glClipPlanexIMG;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_NV_coverage_sample()
{
	return _GLES_NV_coverage_sample;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_coverage_sample()
{
	glloader_init();
	return glloader_GLES_NV_coverage_sample();
}
glloader_GLES_NV_coverage_sampleFUNC glloader_GLES_NV_coverage_sample = self_init_glloader_GLES_NV_coverage_sample;

#ifdef GLES_NV_coverage_sample

static void GLLOADER_APIENTRY self_init_glCoverageMaskNV(GLboolean mask)
{
	glloader_init();
	glCoverageMaskNV(mask);
}
static void GLLOADER_APIENTRY self_init_glCoverageOperationNV(GLenum operation)
{
	glloader_init();
	glCoverageOperationNV(operation);
}

glCoverageMaskNVFUNC glCoverageMaskNV = self_init_glCoverageMaskNV;
glCoverageOperationNVFUNC glCoverageOperationNV = self_init_glCoverageOperationNV;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_NV_depth_nonlinear()
{
	return _GLES_NV_depth_nonlinear;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_depth_nonlinear()
{
	glloader_init();
	return glloader_GLES_NV_depth_nonlinear();
}
glloader_GLES_NV_depth_nonlinearFUNC glloader_GLES_NV_depth_nonlinear = self_init_glloader_GLES_NV_depth_nonlinear;

static char GLLOADER_APIENTRY _glloader_GLES_NV_draw_buffers()
{
	return _GLES_NV_draw_buffers;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_draw_buffers()
{
	glloader_init();
	return glloader_GLES_NV_draw_buffers();
}
glloader_GLES_NV_draw_buffersFUNC glloader_GLES_NV_draw_buffers = self_init_glloader_GLES_NV_draw_buffers;

#ifdef GLES_NV_draw_buffers

static void GLLOADER_APIENTRY self_init_glDrawBuffersNV(GLsizei n, const GLenum* bufs)
{
	glloader_init();
	glDrawBuffersNV(n, bufs);
}

glDrawBuffersNVFUNC glDrawBuffersNV = self_init_glDrawBuffersNV;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_NV_draw_texture()
{
	return _GLES_NV_draw_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_draw_texture()
{
	glloader_init();
	return glloader_GLES_NV_draw_texture();
}
glloader_GLES_NV_draw_textureFUNC glloader_GLES_NV_draw_texture = self_init_glloader_GLES_NV_draw_texture;

#ifdef GLES_NV_draw_texture

static void GLLOADER_APIENTRY self_init_glDrawTextureNV(GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1)
{
	glloader_init();
	glDrawTextureNV(texture, sampler, x0, y0, x1, y1, z, s0, t0, s1, t1);
}

glDrawTextureNVFUNC glDrawTextureNV = self_init_glDrawTextureNV;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_NV_EGL_stream_consumer_external()
{
	return _GLES_NV_EGL_stream_consumer_external;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_EGL_stream_consumer_external()
{
	glloader_init();
	return glloader_GLES_NV_EGL_stream_consumer_external();
}
glloader_GLES_NV_EGL_stream_consumer_externalFUNC glloader_GLES_NV_EGL_stream_consumer_external = self_init_glloader_GLES_NV_EGL_stream_consumer_external;

static char GLLOADER_APIENTRY _glloader_GLES_NV_fbo_color_attachments()
{
	return _GLES_NV_fbo_color_attachments;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_fbo_color_attachments()
{
	glloader_init();
	return glloader_GLES_NV_fbo_color_attachments();
}
glloader_GLES_NV_fbo_color_attachmentsFUNC glloader_GLES_NV_fbo_color_attachments = self_init_glloader_GLES_NV_fbo_color_attachments;

static char GLLOADER_APIENTRY _glloader_GLES_NV_fence()
{
	return _GLES_NV_fence;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_fence()
{
	glloader_init();
	return glloader_GLES_NV_fence();
}
glloader_GLES_NV_fenceFUNC glloader_GLES_NV_fence = self_init_glloader_GLES_NV_fence;

#ifdef GLES_NV_fence

static void GLLOADER_APIENTRY self_init_glDeleteFencesNV(GLsizei n, const GLuint* fences)
{
	glloader_init();
	glDeleteFencesNV(n, fences);
}
static void GLLOADER_APIENTRY self_init_glGenFencesNV(GLsizei n, GLuint* fences)
{
	glloader_init();
	glGenFencesNV(n, fences);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsFenceNV(GLuint fence)
{
	glloader_init();
	return glIsFenceNV(fence);
}
static GLboolean GLLOADER_APIENTRY self_init_glTestFenceNV(GLuint fence)
{
	glloader_init();
	return glTestFenceNV(fence);
}
static void GLLOADER_APIENTRY self_init_glGetFenceivNV(GLuint fence, GLenum pname, GLint* params)
{
	glloader_init();
	glGetFenceivNV(fence, pname, params);
}
static void GLLOADER_APIENTRY self_init_glFinishFenceNV(GLuint fence)
{
	glloader_init();
	glFinishFenceNV(fence);
}
static void GLLOADER_APIENTRY self_init_glSetFenceNV(GLuint fence, GLenum condition)
{
	glloader_init();
	glSetFenceNV(fence, condition);
}

glDeleteFencesNVFUNC glDeleteFencesNV = self_init_glDeleteFencesNV;
glGenFencesNVFUNC glGenFencesNV = self_init_glGenFencesNV;
glIsFenceNVFUNC glIsFenceNV = self_init_glIsFenceNV;
glTestFenceNVFUNC glTestFenceNV = self_init_glTestFenceNV;
glGetFenceivNVFUNC glGetFenceivNV = self_init_glGetFenceivNV;
glFinishFenceNVFUNC glFinishFenceNV = self_init_glFinishFenceNV;
glSetFenceNVFUNC glSetFenceNV = self_init_glSetFenceNV;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_NV_read_buffer()
{
	return _GLES_NV_read_buffer;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_read_buffer()
{
	glloader_init();
	return glloader_GLES_NV_read_buffer();
}
glloader_GLES_NV_read_bufferFUNC glloader_GLES_NV_read_buffer = self_init_glloader_GLES_NV_read_buffer;

#ifdef GLES_NV_read_buffer

static void GLLOADER_APIENTRY self_init_glReadBufferNV(GLenum mode)
{
	glloader_init();
	glReadBufferNV(mode);
}

glReadBufferNVFUNC glReadBufferNV = self_init_glReadBufferNV;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_NV_read_depth()
{
	return _GLES_NV_read_depth;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_read_depth()
{
	glloader_init();
	return glloader_GLES_NV_read_depth();
}
glloader_GLES_NV_read_depthFUNC glloader_GLES_NV_read_depth = self_init_glloader_GLES_NV_read_depth;

static char GLLOADER_APIENTRY _glloader_GLES_NV_read_depth_stencil()
{
	return _GLES_NV_read_depth_stencil;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_read_depth_stencil()
{
	glloader_init();
	return glloader_GLES_NV_read_depth_stencil();
}
glloader_GLES_NV_read_depth_stencilFUNC glloader_GLES_NV_read_depth_stencil = self_init_glloader_GLES_NV_read_depth_stencil;

static char GLLOADER_APIENTRY _glloader_GLES_NV_read_stencil()
{
	return _GLES_NV_read_stencil;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_read_stencil()
{
	glloader_init();
	return glloader_GLES_NV_read_stencil();
}
glloader_GLES_NV_read_stencilFUNC glloader_GLES_NV_read_stencil = self_init_glloader_GLES_NV_read_stencil;

static char GLLOADER_APIENTRY _glloader_GLES_NV_texture_compression_s3tc_update()
{
	return _GLES_NV_texture_compression_s3tc_update;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_texture_compression_s3tc_update()
{
	glloader_init();
	return glloader_GLES_NV_texture_compression_s3tc_update();
}
glloader_GLES_NV_texture_compression_s3tc_updateFUNC glloader_GLES_NV_texture_compression_s3tc_update = self_init_glloader_GLES_NV_texture_compression_s3tc_update;

static char GLLOADER_APIENTRY _glloader_GLES_NV_texture_npot_2D_mipmap()
{
	return _GLES_NV_texture_npot_2D_mipmap;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_NV_texture_npot_2D_mipmap()
{
	glloader_init();
	return glloader_GLES_NV_texture_npot_2D_mipmap();
}
glloader_GLES_NV_texture_npot_2D_mipmapFUNC glloader_GLES_NV_texture_npot_2D_mipmap = self_init_glloader_GLES_NV_texture_npot_2D_mipmap;

static char GLLOADER_APIENTRY _glloader_GLES_OES_blend_equation_separate()
{
	return _GLES_OES_blend_equation_separate;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_blend_equation_separate()
{
	glloader_init();
	return glloader_GLES_OES_blend_equation_separate();
}
glloader_GLES_OES_blend_equation_separateFUNC glloader_GLES_OES_blend_equation_separate = self_init_glloader_GLES_OES_blend_equation_separate;

#ifdef GLES_OES_blend_equation_separate

static void GLLOADER_APIENTRY self_init_glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha)
{
	glloader_init();
	glBlendEquationSeparateOES(modeRGB, modeAlpha);
}

glBlendEquationSeparateOESFUNC glBlendEquationSeparateOES = self_init_glBlendEquationSeparateOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_blend_func_separate()
{
	return _GLES_OES_blend_func_separate;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_blend_func_separate()
{
	glloader_init();
	return glloader_GLES_OES_blend_func_separate();
}
glloader_GLES_OES_blend_func_separateFUNC glloader_GLES_OES_blend_func_separate = self_init_glloader_GLES_OES_blend_func_separate;

#ifdef GLES_OES_blend_func_separate

static void GLLOADER_APIENTRY self_init_glBlendFuncSeparateOES(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
	glloader_init();
	glBlendFuncSeparateOES(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

glBlendFuncSeparateOESFUNC glBlendFuncSeparateOES = self_init_glBlendFuncSeparateOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_blend_subtract()
{
	return _GLES_OES_blend_subtract;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_blend_subtract()
{
	glloader_init();
	return glloader_GLES_OES_blend_subtract();
}
glloader_GLES_OES_blend_subtractFUNC glloader_GLES_OES_blend_subtract = self_init_glloader_GLES_OES_blend_subtract;

#ifdef GLES_OES_blend_subtract

static void GLLOADER_APIENTRY self_init_glBlendEquationOES(GLenum mode)
{
	glloader_init();
	glBlendEquationOES(mode);
}

glBlendEquationOESFUNC glBlendEquationOES = self_init_glBlendEquationOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_byte_coordinates()
{
	return _GLES_OES_byte_coordinates;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_byte_coordinates()
{
	glloader_init();
	return glloader_GLES_OES_byte_coordinates();
}
glloader_GLES_OES_byte_coordinatesFUNC glloader_GLES_OES_byte_coordinates = self_init_glloader_GLES_OES_byte_coordinates;

#ifdef GLES_OES_byte_coordinates

static void GLLOADER_APIENTRY self_init_glVertex2bOES(GLbyte x, GLbyte y)
{
	glloader_init();
	glVertex2bOES(x, y);
}
static void GLLOADER_APIENTRY self_init_glVertex3bOES(GLbyte x, GLbyte y, GLbyte z)
{
	glloader_init();
	glVertex3bOES(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glVertex4bOES(GLbyte x, GLbyte y, GLbyte z, GLbyte w)
{
	glloader_init();
	glVertex4bOES(x, y, z, w);
}
static void GLLOADER_APIENTRY self_init_glVertex2bvOES(GLbyte* v)
{
	glloader_init();
	glVertex2bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glVertex3bvOES(GLbyte* v)
{
	glloader_init();
	glVertex3bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glVertex4bvOES(GLbyte* v)
{
	glloader_init();
	glVertex4bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glTexCoord1bOES(GLbyte s)
{
	glloader_init();
	glTexCoord1bOES(s);
}
static void GLLOADER_APIENTRY self_init_glTexCoord2bOES(GLbyte s, GLbyte t)
{
	glloader_init();
	glTexCoord2bOES(s, t);
}
static void GLLOADER_APIENTRY self_init_glTexCoord3bOES(GLbyte s, GLbyte t, GLbyte r)
{
	glloader_init();
	glTexCoord3bOES(s, t, r);
}
static void GLLOADER_APIENTRY self_init_glTexCoord4bOES(GLbyte s, GLbyte t, GLbyte r, GLbyte q)
{
	glloader_init();
	glTexCoord4bOES(s, t, r, q);
}
static void GLLOADER_APIENTRY self_init_glTexCoord1bvOES(GLbyte* v)
{
	glloader_init();
	glTexCoord1bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glTexCoord2bvOES(GLbyte* v)
{
	glloader_init();
	glTexCoord2bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glTexCoord3bvOES(GLbyte* v)
{
	glloader_init();
	glTexCoord3bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glTexCoord4bvOES(GLbyte* v)
{
	glloader_init();
	glTexCoord4bvOES(v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord1bOES(GLenum target, GLbyte s)
{
	glloader_init();
	glMultiTexCoord1bOES(target, s);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord2bOES(GLenum target, GLbyte s, GLbyte t)
{
	glloader_init();
	glMultiTexCoord2bOES(target, s, t);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord3bOES(GLenum target, GLbyte s, GLbyte t, GLbyte r)
{
	glloader_init();
	glMultiTexCoord3bOES(target, s, t, r);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord4bOES(GLenum target, GLbyte s, GLbyte t, GLbyte r, GLbyte q)
{
	glloader_init();
	glMultiTexCoord4bOES(target, s, t, r, q);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord1bvOES(GLenum target, GLbyte* v)
{
	glloader_init();
	glMultiTexCoord1bvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord2bvOES(GLenum target, GLbyte* v)
{
	glloader_init();
	glMultiTexCoord2bvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord3bvOES(GLenum target, GLbyte* v)
{
	glloader_init();
	glMultiTexCoord3bvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord4bvOES(GLenum target, GLbyte* v)
{
	glloader_init();
	glMultiTexCoord4bvOES(target, v);
}

glVertex2bOESFUNC glVertex2bOES = self_init_glVertex2bOES;
glVertex3bOESFUNC glVertex3bOES = self_init_glVertex3bOES;
glVertex4bOESFUNC glVertex4bOES = self_init_glVertex4bOES;
glVertex2bvOESFUNC glVertex2bvOES = self_init_glVertex2bvOES;
glVertex3bvOESFUNC glVertex3bvOES = self_init_glVertex3bvOES;
glVertex4bvOESFUNC glVertex4bvOES = self_init_glVertex4bvOES;
glTexCoord1bOESFUNC glTexCoord1bOES = self_init_glTexCoord1bOES;
glTexCoord2bOESFUNC glTexCoord2bOES = self_init_glTexCoord2bOES;
glTexCoord3bOESFUNC glTexCoord3bOES = self_init_glTexCoord3bOES;
glTexCoord4bOESFUNC glTexCoord4bOES = self_init_glTexCoord4bOES;
glTexCoord1bvOESFUNC glTexCoord1bvOES = self_init_glTexCoord1bvOES;
glTexCoord2bvOESFUNC glTexCoord2bvOES = self_init_glTexCoord2bvOES;
glTexCoord3bvOESFUNC glTexCoord3bvOES = self_init_glTexCoord3bvOES;
glTexCoord4bvOESFUNC glTexCoord4bvOES = self_init_glTexCoord4bvOES;
glMultiTexCoord1bOESFUNC glMultiTexCoord1bOES = self_init_glMultiTexCoord1bOES;
glMultiTexCoord2bOESFUNC glMultiTexCoord2bOES = self_init_glMultiTexCoord2bOES;
glMultiTexCoord3bOESFUNC glMultiTexCoord3bOES = self_init_glMultiTexCoord3bOES;
glMultiTexCoord4bOESFUNC glMultiTexCoord4bOES = self_init_glMultiTexCoord4bOES;
glMultiTexCoord1bvOESFUNC glMultiTexCoord1bvOES = self_init_glMultiTexCoord1bvOES;
glMultiTexCoord2bvOESFUNC glMultiTexCoord2bvOES = self_init_glMultiTexCoord2bvOES;
glMultiTexCoord3bvOESFUNC glMultiTexCoord3bvOES = self_init_glMultiTexCoord3bvOES;
glMultiTexCoord4bvOESFUNC glMultiTexCoord4bvOES = self_init_glMultiTexCoord4bvOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_compressed_ETC1_RGB8_texture()
{
	return _GLES_OES_compressed_ETC1_RGB8_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_compressed_ETC1_RGB8_texture()
{
	glloader_init();
	return glloader_GLES_OES_compressed_ETC1_RGB8_texture();
}
glloader_GLES_OES_compressed_ETC1_RGB8_textureFUNC glloader_GLES_OES_compressed_ETC1_RGB8_texture = self_init_glloader_GLES_OES_compressed_ETC1_RGB8_texture;

static char GLLOADER_APIENTRY _glloader_GLES_OES_compressed_paletted_texture()
{
	return _GLES_OES_compressed_paletted_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_compressed_paletted_texture()
{
	glloader_init();
	return glloader_GLES_OES_compressed_paletted_texture();
}
glloader_GLES_OES_compressed_paletted_textureFUNC glloader_GLES_OES_compressed_paletted_texture = self_init_glloader_GLES_OES_compressed_paletted_texture;

static char GLLOADER_APIENTRY _glloader_GLES_OES_depth24()
{
	return _GLES_OES_depth24;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_depth24()
{
	glloader_init();
	return glloader_GLES_OES_depth24();
}
glloader_GLES_OES_depth24FUNC glloader_GLES_OES_depth24 = self_init_glloader_GLES_OES_depth24;

static char GLLOADER_APIENTRY _glloader_GLES_OES_depth32()
{
	return _GLES_OES_depth32;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_depth32()
{
	glloader_init();
	return glloader_GLES_OES_depth32();
}
glloader_GLES_OES_depth32FUNC glloader_GLES_OES_depth32 = self_init_glloader_GLES_OES_depth32;

static char GLLOADER_APIENTRY _glloader_GLES_OES_depth_texture()
{
	return _GLES_OES_depth_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_depth_texture()
{
	glloader_init();
	return glloader_GLES_OES_depth_texture();
}
glloader_GLES_OES_depth_textureFUNC glloader_GLES_OES_depth_texture = self_init_glloader_GLES_OES_depth_texture;

static char GLLOADER_APIENTRY _glloader_GLES_OES_draw_texture()
{
	return _GLES_OES_draw_texture;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_draw_texture()
{
	glloader_init();
	return glloader_GLES_OES_draw_texture();
}
glloader_GLES_OES_draw_textureFUNC glloader_GLES_OES_draw_texture = self_init_glloader_GLES_OES_draw_texture;

#ifdef GLES_OES_draw_texture

static void GLLOADER_APIENTRY self_init_glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
	glloader_init();
	glDrawTexsOES(x, y, z, width, height);
}
static void GLLOADER_APIENTRY self_init_glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
	glloader_init();
	glDrawTexiOES(x, y, z, width, height);
}
static void GLLOADER_APIENTRY self_init_glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
	glloader_init();
	glDrawTexxOES(x, y, z, width, height);
}
static void GLLOADER_APIENTRY self_init_glDrawTexsvOES(const GLshort* coords)
{
	glloader_init();
	glDrawTexsvOES(coords);
}
static void GLLOADER_APIENTRY self_init_glDrawTexivOES(const GLint* coords)
{
	glloader_init();
	glDrawTexivOES(coords);
}
static void GLLOADER_APIENTRY self_init_glDrawTexxvOES(const GLfixed* coords)
{
	glloader_init();
	glDrawTexxvOES(coords);
}
static void GLLOADER_APIENTRY self_init_glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
	glloader_init();
	glDrawTexfOES(x, y, z, width, height);
}
static void GLLOADER_APIENTRY self_init_glDrawTexfvOES(const GLfloat* coords)
{
	glloader_init();
	glDrawTexfvOES(coords);
}

glDrawTexsOESFUNC glDrawTexsOES = self_init_glDrawTexsOES;
glDrawTexiOESFUNC glDrawTexiOES = self_init_glDrawTexiOES;
glDrawTexxOESFUNC glDrawTexxOES = self_init_glDrawTexxOES;
glDrawTexsvOESFUNC glDrawTexsvOES = self_init_glDrawTexsvOES;
glDrawTexivOESFUNC glDrawTexivOES = self_init_glDrawTexivOES;
glDrawTexxvOESFUNC glDrawTexxvOES = self_init_glDrawTexxvOES;
glDrawTexfOESFUNC glDrawTexfOES = self_init_glDrawTexfOES;
glDrawTexfvOESFUNC glDrawTexfvOES = self_init_glDrawTexfvOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_EGL_image()
{
	return _GLES_OES_EGL_image;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_EGL_image()
{
	glloader_init();
	return glloader_GLES_OES_EGL_image();
}
glloader_GLES_OES_EGL_imageFUNC glloader_GLES_OES_EGL_image = self_init_glloader_GLES_OES_EGL_image;

#ifdef GLES_OES_EGL_image

static void GLLOADER_APIENTRY self_init_EGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
	glloader_init();
	EGLImageTargetTexture2DOES(target, image);
}
static void GLLOADER_APIENTRY self_init_EGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image)
{
	glloader_init();
	EGLImageTargetRenderbufferStorageOES(target, image);
}

EGLImageTargetTexture2DOESFUNC EGLImageTargetTexture2DOES = self_init_EGLImageTargetTexture2DOES;
EGLImageTargetRenderbufferStorageOESFUNC EGLImageTargetRenderbufferStorageOES = self_init_EGLImageTargetRenderbufferStorageOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_EGL_image_external()
{
	return _GLES_OES_EGL_image_external;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_EGL_image_external()
{
	glloader_init();
	return glloader_GLES_OES_EGL_image_external();
}
glloader_GLES_OES_EGL_image_externalFUNC glloader_GLES_OES_EGL_image_external = self_init_glloader_GLES_OES_EGL_image_external;

static char GLLOADER_APIENTRY _glloader_GLES_OES_element_index_uint()
{
	return _GLES_OES_element_index_uint;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_element_index_uint()
{
	glloader_init();
	return glloader_GLES_OES_element_index_uint();
}
glloader_GLES_OES_element_index_uintFUNC glloader_GLES_OES_element_index_uint = self_init_glloader_GLES_OES_element_index_uint;

static char GLLOADER_APIENTRY _glloader_GLES_OES_extended_matrix_palette()
{
	return _GLES_OES_extended_matrix_palette;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_extended_matrix_palette()
{
	glloader_init();
	return glloader_GLES_OES_extended_matrix_palette();
}
glloader_GLES_OES_extended_matrix_paletteFUNC glloader_GLES_OES_extended_matrix_palette = self_init_glloader_GLES_OES_extended_matrix_palette;

static char GLLOADER_APIENTRY _glloader_GLES_OES_fbo_render_mipmap()
{
	return _GLES_OES_fbo_render_mipmap;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_fbo_render_mipmap()
{
	glloader_init();
	return glloader_GLES_OES_fbo_render_mipmap();
}
glloader_GLES_OES_fbo_render_mipmapFUNC glloader_GLES_OES_fbo_render_mipmap = self_init_glloader_GLES_OES_fbo_render_mipmap;

static char GLLOADER_APIENTRY _glloader_GLES_OES_fixed_point()
{
	return _GLES_OES_fixed_point;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_fixed_point()
{
	glloader_init();
	return glloader_GLES_OES_fixed_point();
}
glloader_GLES_OES_fixed_pointFUNC glloader_GLES_OES_fixed_point = self_init_glloader_GLES_OES_fixed_point;

#ifdef GLES_OES_fixed_point

static void GLLOADER_APIENTRY self_init_glVertex2xOES(GLfixed x, GLfixed y)
{
	glloader_init();
	glVertex2xOES(x, y);
}
static void GLLOADER_APIENTRY self_init_glVertex3xOES(GLfixed x, GLfixed y, GLfixed z)
{
	glloader_init();
	glVertex3xOES(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glVertex4xOES(GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
	glloader_init();
	glVertex4xOES(x, y, z, w);
}
static void GLLOADER_APIENTRY self_init_glVertex2xvOES(const GLfixed* v)
{
	glloader_init();
	glVertex2xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glVertex3xvOES(const GLfixed* v)
{
	glloader_init();
	glVertex3xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glVertex4xvOES(const GLfixed* v)
{
	glloader_init();
	glVertex4xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glNormal3xOES(GLfixed nx, GLfixed ny, GLfixed nz)
{
	glloader_init();
	glNormal3xOES(nx, ny, nz);
}
static void GLLOADER_APIENTRY self_init_glNormal3xvOES(const GLfixed* v)
{
	glloader_init();
	glNormal3xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glTexCoord1xOES(GLfixed x)
{
	glloader_init();
	glTexCoord1xOES(x);
}
static void GLLOADER_APIENTRY self_init_glTexCoord2xOES(GLfixed x, GLfixed y)
{
	glloader_init();
	glTexCoord2xOES(x, y);
}
static void GLLOADER_APIENTRY self_init_glTexCoord3xOES(GLfixed x, GLfixed y, GLfixed z)
{
	glloader_init();
	glTexCoord3xOES(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glTexCoord4xOES(GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
	glloader_init();
	glTexCoord4xOES(x, y, z, w);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord1xOES(GLenum target, GLfixed s)
{
	glloader_init();
	glMultiTexCoord1xOES(target, s);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord2xOES(GLenum target, GLfixed s, GLfixed t)
{
	glloader_init();
	glMultiTexCoord2xOES(target, s, t);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord3xOES(GLenum target, GLfixed s, GLfixed t, GLfixed r)
{
	glloader_init();
	glMultiTexCoord3xOES(target, s, t, r);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord4xOES(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	glloader_init();
	glMultiTexCoord4xOES(target, s, t, r, q);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord1xvOES(GLenum target, const GLfixed* v)
{
	glloader_init();
	glMultiTexCoord1xvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord2xvOES(GLenum target, const GLfixed* v)
{
	glloader_init();
	glMultiTexCoord2xvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord3xvOES(GLenum target, const GLfixed* v)
{
	glloader_init();
	glMultiTexCoord3xvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord4xvOES(GLenum target, const GLfixed* v)
{
	glloader_init();
	glMultiTexCoord4xvOES(target, v);
}
static void GLLOADER_APIENTRY self_init_glColor3xOES(GLfixed red, GLfixed green, GLfixed blue)
{
	glloader_init();
	glColor3xOES(red, green, blue);
}
static void GLLOADER_APIENTRY self_init_glColor4xOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	glloader_init();
	glColor4xOES(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glColor3xvOES(const GLfixed* v)
{
	glloader_init();
	glColor3xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glColor4xvOES(const GLfixed* v)
{
	glloader_init();
	glColor4xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glIndexxOES(GLfixed c)
{
	glloader_init();
	glIndexxOES(c);
}
static void GLLOADER_APIENTRY self_init_glIndexxvOES(const GLfixed* c)
{
	glloader_init();
	glIndexxvOES(c);
}
static void GLLOADER_APIENTRY self_init_glRectxOES(GLfixed x1, GLfixed y1, GLfixed x2, GLfixed y2)
{
	glloader_init();
	glRectxOES(x1, y1, x2, y2);
}
static void GLLOADER_APIENTRY self_init_glRectxvOES(const GLfixed* v1, const GLfixed* v2)
{
	glloader_init();
	glRectxvOES(v1, v2);
}
static void GLLOADER_APIENTRY self_init_glDepthRangexOES(GLclampx zNear, GLclampx zFar)
{
	glloader_init();
	glDepthRangexOES(zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glLoadMatrixxOES(const GLfixed* m)
{
	glloader_init();
	glLoadMatrixxOES(m);
}
static void GLLOADER_APIENTRY self_init_glMultMatrixxOES(const GLfixed* m)
{
	glloader_init();
	glMultMatrixxOES(m);
}
static void GLLOADER_APIENTRY self_init_glLoadTransposeMatrixxOES(const GLfixed* m)
{
	glloader_init();
	glLoadTransposeMatrixxOES(m);
}
static void GLLOADER_APIENTRY self_init_glMultTransposeMatrixxOES(const GLfixed* m)
{
	glloader_init();
	glMultTransposeMatrixxOES(m);
}
static void GLLOADER_APIENTRY self_init_glRotatexOES(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	glloader_init();
	glRotatexOES(angle, x, y, z);
}
static void GLLOADER_APIENTRY self_init_glScalexOES(GLfixed x, GLfixed y, GLfixed z)
{
	glloader_init();
	glScalexOES(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glTranslatexOES(GLfixed x, GLfixed y, GLfixed z)
{
	glloader_init();
	glTranslatexOES(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glFrustumxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	glloader_init();
	glFrustumxOES(left, right, bottom, top, zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glOrthoxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	glloader_init();
	glOrthoxOES(left, right, bottom, top, zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glTexGenxOES(GLenum coord, GLenum pname, GLfixed param)
{
	glloader_init();
	glTexGenxOES(coord, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed* params)
{
	glloader_init();
	glTexGenxvOES(coord, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetTexGenxvOES(coord, pname, params);
}
static void GLLOADER_APIENTRY self_init_glClipPlanexOES(GLenum plane, const GLfixed* equation)
{
	glloader_init();
	glClipPlanexOES(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glGetClipPlanexOES(GLenum plane, GLfixed* equation)
{
	glloader_init();
	glGetClipPlanexOES(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glRasterPos2xOES(GLfixed x, GLfixed y)
{
	glloader_init();
	glRasterPos2xOES(x, y);
}
static void GLLOADER_APIENTRY self_init_glRasterPos3xOES(GLfixed x, GLfixed y, GLfixed z)
{
	glloader_init();
	glRasterPos3xOES(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glRasterPos4xOES(GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
	glloader_init();
	glRasterPos4xOES(x, y, z, w);
}
static void GLLOADER_APIENTRY self_init_glRasterPos2xvOES(const GLfixed* v)
{
	glloader_init();
	glRasterPos2xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glRasterPos3xvOES(const GLfixed* v)
{
	glloader_init();
	glRasterPos3xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glRasterPos4xvOES(const GLfixed* v)
{
	glloader_init();
	glRasterPos4xvOES(v);
}
static void GLLOADER_APIENTRY self_init_glMaterialxOES(GLenum face, GLenum pname, GLfixed param)
{
	glloader_init();
	glMaterialxOES(face, pname, param);
}
static void GLLOADER_APIENTRY self_init_glMaterialxvOES(GLenum face, GLenum pname, const GLfixed* params)
{
	glloader_init();
	glMaterialxvOES(face, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetMaterialxOES(GLenum face, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetMaterialxOES(face, pname, params);
}
static void GLLOADER_APIENTRY self_init_glLightxOES(GLenum light, GLenum pname, GLfixed param)
{
	glloader_init();
	glLightxOES(light, pname, param);
}
static void GLLOADER_APIENTRY self_init_glLightxvOES(GLenum light, GLenum pname, const GLfixed* params)
{
	glloader_init();
	glLightxvOES(light, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetLightxvOES(GLenum light, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetLightxvOES(light, pname, params);
}
static void GLLOADER_APIENTRY self_init_glLightModelxOES(GLenum pname, GLfixed param)
{
	glloader_init();
	glLightModelxOES(pname, param);
}
static void GLLOADER_APIENTRY self_init_glLightModelxvOES(GLenum pname, const GLfixed* params)
{
	glloader_init();
	glLightModelxvOES(pname, params);
}
static void GLLOADER_APIENTRY self_init_glPointSizexOES(GLfixed size)
{
	glloader_init();
	glPointSizexOES(size);
}
static void GLLOADER_APIENTRY self_init_glPointParameterxvOES(GLenum pname, GLfixed* params)
{
	glloader_init();
	glPointParameterxvOES(pname, params);
}
static void GLLOADER_APIENTRY self_init_glLineWidthxOES(GLfixed width)
{
	glloader_init();
	glLineWidthxOES(width);
}
static void GLLOADER_APIENTRY self_init_glPolygonOffsetxOES(GLfixed factor, GLfixed units)
{
	glloader_init();
	glPolygonOffsetxOES(factor, units);
}
static void GLLOADER_APIENTRY self_init_glPixelStorexOES(GLenum pname, GLfixed param)
{
	glloader_init();
	glPixelStorexOES(pname, param);
}
static void GLLOADER_APIENTRY self_init_glPixelTransferxOES(GLenum pname, GLfixed param)
{
	glloader_init();
	glPixelTransferxOES(pname, param);
}
static void GLLOADER_APIENTRY self_init_glPixelMapxOES(GLenum map, GLsizei mapsize, const GLfixed* values)
{
	glloader_init();
	glPixelMapxOES(map, mapsize, values);
}
static void GLLOADER_APIENTRY self_init_glGetPixelMapxvOES(GLenum map, GLfixed* values)
{
	glloader_init();
	glGetPixelMapxvOES(map, values);
}
static void GLLOADER_APIENTRY self_init_glConvolutionParameterxOES(GLenum target, GLenum pname, GLfixed params)
{
	glloader_init();
	glConvolutionParameterxOES(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glConvolutionParameterxvOES(GLenum target, GLenum pname, const GLfixed* params)
{
	glloader_init();
	glConvolutionParameterxvOES(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetConvolutionParameterxvOES(GLenum target, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetConvolutionParameterxvOES(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetHistogramParameterxvOES(GLenum target, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetHistogramParameterxvOES(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glPixelZoomxOES(GLfixed xfactor, GLfixed yfactor)
{
	glloader_init();
	glPixelZoomxOES(xfactor, yfactor);
}
static void GLLOADER_APIENTRY self_init_glBitmapxOES(GLsizei width, GLsizei height, GLfixed xorig, GLfixed yorig, GLfixed xmove, GLfixed ymove, const GLubyte* bitmap)
{
	glloader_init();
	glBitmapxOES(width, height, xorig, yorig, xmove, ymove, bitmap);
}
static void GLLOADER_APIENTRY self_init_glTexParameterxOES(GLenum target, GLenum pname, GLfixed param)
{
	glloader_init();
	glTexParameterxOES(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glGetTexParameterxvOES(GLenum target, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetTexParameterxvOES(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexLevelParameterxvOES(GLenum target, GLint level, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetTexLevelParameterxvOES(target, level, pname, params);
}
static void GLLOADER_APIENTRY self_init_glPrioritizeTexturesxOES(GLsizei n, const GLuint* textures, const GLclampx* priorities)
{
	glloader_init();
	glPrioritizeTexturesxOES(n, textures, priorities);
}
static void GLLOADER_APIENTRY self_init_glTexEnvxOES(GLenum target, GLenum pname, GLfixed param)
{
	glloader_init();
	glTexEnvxOES(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glGetTexEnvxvOES(GLenum target, GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetTexEnvxvOES(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glFogxOES(GLenum pname, GLfixed param)
{
	glloader_init();
	glFogxOES(pname, param);
}
static void GLLOADER_APIENTRY self_init_glFogxvOES(GLenum pname, const GLfixed* params)
{
	glloader_init();
	glFogxvOES(pname, params);
}
static void GLLOADER_APIENTRY self_init_glSampleCoveragexOES(GLclampx value, GLboolean invert)
{
	glloader_init();
	glSampleCoveragexOES(value, invert);
}
static void GLLOADER_APIENTRY self_init_glAlphaFuncxOES(GLenum func, GLclampx ref)
{
	glloader_init();
	glAlphaFuncxOES(func, ref);
}
static void GLLOADER_APIENTRY self_init_glBlendColorxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	glloader_init();
	glBlendColorxOES(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glClearColorxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	glloader_init();
	glClearColorxOES(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glClearDepthxOES(GLclampx depth)
{
	glloader_init();
	glClearDepthxOES(depth);
}
static void GLLOADER_APIENTRY self_init_glClearAccumxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	glloader_init();
	glClearAccumxOES(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glAccumxOES(GLenum op, GLfixed value)
{
	glloader_init();
	glAccumxOES(op, value);
}
static void GLLOADER_APIENTRY self_init_glMap1xOES(GLenum target, GLfixed u1, GLfixed u2, GLint stride, GLint order, const GLfixed* points)
{
	glloader_init();
	glMap1xOES(target, u1, u2, stride, order, points);
}
static void GLLOADER_APIENTRY self_init_glMap2xOES(GLenum target, GLfixed u1, GLfixed u2, GLint ustride, GLint uorder, GLfixed v1, GLfixed v2, GLint vstride, GLint vorder, const GLfixed* points)
{
	glloader_init();
	glMap2xOES(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}
static void GLLOADER_APIENTRY self_init_glMapGrid1xOES(GLint un, GLfixed u1, GLfixed u2)
{
	glloader_init();
	glMapGrid1xOES(un, u1, u2);
}
static void GLLOADER_APIENTRY self_init_glMapGrid2xOES(GLint un, GLfixed u1, GLfixed u2, GLint vn, GLfixed v1, GLfixed v2)
{
	glloader_init();
	glMapGrid2xOES(un, u1, u2, vn, v1, v2);
}
static void GLLOADER_APIENTRY self_init_glGetMapxvOES(GLenum target, GLenum query, GLfixed* v)
{
	glloader_init();
	glGetMapxvOES(target, query, v);
}
static void GLLOADER_APIENTRY self_init_glEvalCoord1xOES(GLfixed u)
{
	glloader_init();
	glEvalCoord1xOES(u);
}
static void GLLOADER_APIENTRY self_init_glEvalCoord1xvOES(const GLfixed* u)
{
	glloader_init();
	glEvalCoord1xvOES(u);
}
static void GLLOADER_APIENTRY self_init_glEvalCoord2xOES(GLfixed u, GLfixed v)
{
	glloader_init();
	glEvalCoord2xOES(u, v);
}
static void GLLOADER_APIENTRY self_init_glEvalCoord2xvOES(const GLfixed* u)
{
	glloader_init();
	glEvalCoord2xvOES(u);
}
static void GLLOADER_APIENTRY self_init_glFeedbackBufferxOES(GLsizei size, GLenum type, GLfixed* buffer)
{
	glloader_init();
	glFeedbackBufferxOES(size, type, buffer);
}
static void GLLOADER_APIENTRY self_init_glPassThroughxOES(GLfixed token)
{
	glloader_init();
	glPassThroughxOES(token);
}
static void GLLOADER_APIENTRY self_init_glGetFixedvOES(GLenum pname, GLfixed* params)
{
	glloader_init();
	glGetFixedvOES(pname, params);
}

glVertex2xOESFUNC glVertex2xOES = self_init_glVertex2xOES;
glVertex3xOESFUNC glVertex3xOES = self_init_glVertex3xOES;
glVertex4xOESFUNC glVertex4xOES = self_init_glVertex4xOES;
glVertex2xvOESFUNC glVertex2xvOES = self_init_glVertex2xvOES;
glVertex3xvOESFUNC glVertex3xvOES = self_init_glVertex3xvOES;
glVertex4xvOESFUNC glVertex4xvOES = self_init_glVertex4xvOES;
glNormal3xOESFUNC glNormal3xOES = self_init_glNormal3xOES;
glNormal3xvOESFUNC glNormal3xvOES = self_init_glNormal3xvOES;
glTexCoord1xOESFUNC glTexCoord1xOES = self_init_glTexCoord1xOES;
glTexCoord2xOESFUNC glTexCoord2xOES = self_init_glTexCoord2xOES;
glTexCoord3xOESFUNC glTexCoord3xOES = self_init_glTexCoord3xOES;
glTexCoord4xOESFUNC glTexCoord4xOES = self_init_glTexCoord4xOES;
glMultiTexCoord1xOESFUNC glMultiTexCoord1xOES = self_init_glMultiTexCoord1xOES;
glMultiTexCoord2xOESFUNC glMultiTexCoord2xOES = self_init_glMultiTexCoord2xOES;
glMultiTexCoord3xOESFUNC glMultiTexCoord3xOES = self_init_glMultiTexCoord3xOES;
glMultiTexCoord4xOESFUNC glMultiTexCoord4xOES = self_init_glMultiTexCoord4xOES;
glMultiTexCoord1xvOESFUNC glMultiTexCoord1xvOES = self_init_glMultiTexCoord1xvOES;
glMultiTexCoord2xvOESFUNC glMultiTexCoord2xvOES = self_init_glMultiTexCoord2xvOES;
glMultiTexCoord3xvOESFUNC glMultiTexCoord3xvOES = self_init_glMultiTexCoord3xvOES;
glMultiTexCoord4xvOESFUNC glMultiTexCoord4xvOES = self_init_glMultiTexCoord4xvOES;
glColor3xOESFUNC glColor3xOES = self_init_glColor3xOES;
glColor4xOESFUNC glColor4xOES = self_init_glColor4xOES;
glColor3xvOESFUNC glColor3xvOES = self_init_glColor3xvOES;
glColor4xvOESFUNC glColor4xvOES = self_init_glColor4xvOES;
glIndexxOESFUNC glIndexxOES = self_init_glIndexxOES;
glIndexxvOESFUNC glIndexxvOES = self_init_glIndexxvOES;
glRectxOESFUNC glRectxOES = self_init_glRectxOES;
glRectxvOESFUNC glRectxvOES = self_init_glRectxvOES;
glDepthRangexOESFUNC glDepthRangexOES = self_init_glDepthRangexOES;
glLoadMatrixxOESFUNC glLoadMatrixxOES = self_init_glLoadMatrixxOES;
glMultMatrixxOESFUNC glMultMatrixxOES = self_init_glMultMatrixxOES;
glLoadTransposeMatrixxOESFUNC glLoadTransposeMatrixxOES = self_init_glLoadTransposeMatrixxOES;
glMultTransposeMatrixxOESFUNC glMultTransposeMatrixxOES = self_init_glMultTransposeMatrixxOES;
glRotatexOESFUNC glRotatexOES = self_init_glRotatexOES;
glScalexOESFUNC glScalexOES = self_init_glScalexOES;
glTranslatexOESFUNC glTranslatexOES = self_init_glTranslatexOES;
glFrustumxOESFUNC glFrustumxOES = self_init_glFrustumxOES;
glOrthoxOESFUNC glOrthoxOES = self_init_glOrthoxOES;
glTexGenxOESFUNC glTexGenxOES = self_init_glTexGenxOES;
glTexGenxvOESFUNC glTexGenxvOES = self_init_glTexGenxvOES;
glGetTexGenxvOESFUNC glGetTexGenxvOES = self_init_glGetTexGenxvOES;
glClipPlanexOESFUNC glClipPlanexOES = self_init_glClipPlanexOES;
glGetClipPlanexOESFUNC glGetClipPlanexOES = self_init_glGetClipPlanexOES;
glRasterPos2xOESFUNC glRasterPos2xOES = self_init_glRasterPos2xOES;
glRasterPos3xOESFUNC glRasterPos3xOES = self_init_glRasterPos3xOES;
glRasterPos4xOESFUNC glRasterPos4xOES = self_init_glRasterPos4xOES;
glRasterPos2xvOESFUNC glRasterPos2xvOES = self_init_glRasterPos2xvOES;
glRasterPos3xvOESFUNC glRasterPos3xvOES = self_init_glRasterPos3xvOES;
glRasterPos4xvOESFUNC glRasterPos4xvOES = self_init_glRasterPos4xvOES;
glMaterialxOESFUNC glMaterialxOES = self_init_glMaterialxOES;
glMaterialxvOESFUNC glMaterialxvOES = self_init_glMaterialxvOES;
glGetMaterialxOESFUNC glGetMaterialxOES = self_init_glGetMaterialxOES;
glLightxOESFUNC glLightxOES = self_init_glLightxOES;
glLightxvOESFUNC glLightxvOES = self_init_glLightxvOES;
glGetLightxvOESFUNC glGetLightxvOES = self_init_glGetLightxvOES;
glLightModelxOESFUNC glLightModelxOES = self_init_glLightModelxOES;
glLightModelxvOESFUNC glLightModelxvOES = self_init_glLightModelxvOES;
glPointSizexOESFUNC glPointSizexOES = self_init_glPointSizexOES;
glPointParameterxvOESFUNC glPointParameterxvOES = self_init_glPointParameterxvOES;
glLineWidthxOESFUNC glLineWidthxOES = self_init_glLineWidthxOES;
glPolygonOffsetxOESFUNC glPolygonOffsetxOES = self_init_glPolygonOffsetxOES;
glPixelStorexOESFUNC glPixelStorexOES = self_init_glPixelStorexOES;
glPixelTransferxOESFUNC glPixelTransferxOES = self_init_glPixelTransferxOES;
glPixelMapxOESFUNC glPixelMapxOES = self_init_glPixelMapxOES;
glGetPixelMapxvOESFUNC glGetPixelMapxvOES = self_init_glGetPixelMapxvOES;
glConvolutionParameterxOESFUNC glConvolutionParameterxOES = self_init_glConvolutionParameterxOES;
glConvolutionParameterxvOESFUNC glConvolutionParameterxvOES = self_init_glConvolutionParameterxvOES;
glGetConvolutionParameterxvOESFUNC glGetConvolutionParameterxvOES = self_init_glGetConvolutionParameterxvOES;
glGetHistogramParameterxvOESFUNC glGetHistogramParameterxvOES = self_init_glGetHistogramParameterxvOES;
glPixelZoomxOESFUNC glPixelZoomxOES = self_init_glPixelZoomxOES;
glBitmapxOESFUNC glBitmapxOES = self_init_glBitmapxOES;
glTexParameterxOESFUNC glTexParameterxOES = self_init_glTexParameterxOES;
glGetTexParameterxvOESFUNC glGetTexParameterxvOES = self_init_glGetTexParameterxvOES;
glGetTexLevelParameterxvOESFUNC glGetTexLevelParameterxvOES = self_init_glGetTexLevelParameterxvOES;
glPrioritizeTexturesxOESFUNC glPrioritizeTexturesxOES = self_init_glPrioritizeTexturesxOES;
glTexEnvxOESFUNC glTexEnvxOES = self_init_glTexEnvxOES;
glGetTexEnvxvOESFUNC glGetTexEnvxvOES = self_init_glGetTexEnvxvOES;
glFogxOESFUNC glFogxOES = self_init_glFogxOES;
glFogxvOESFUNC glFogxvOES = self_init_glFogxvOES;
glSampleCoveragexOESFUNC glSampleCoveragexOES = self_init_glSampleCoveragexOES;
glAlphaFuncxOESFUNC glAlphaFuncxOES = self_init_glAlphaFuncxOES;
glBlendColorxOESFUNC glBlendColorxOES = self_init_glBlendColorxOES;
glClearColorxOESFUNC glClearColorxOES = self_init_glClearColorxOES;
glClearDepthxOESFUNC glClearDepthxOES = self_init_glClearDepthxOES;
glClearAccumxOESFUNC glClearAccumxOES = self_init_glClearAccumxOES;
glAccumxOESFUNC glAccumxOES = self_init_glAccumxOES;
glMap1xOESFUNC glMap1xOES = self_init_glMap1xOES;
glMap2xOESFUNC glMap2xOES = self_init_glMap2xOES;
glMapGrid1xOESFUNC glMapGrid1xOES = self_init_glMapGrid1xOES;
glMapGrid2xOESFUNC glMapGrid2xOES = self_init_glMapGrid2xOES;
glGetMapxvOESFUNC glGetMapxvOES = self_init_glGetMapxvOES;
glEvalCoord1xOESFUNC glEvalCoord1xOES = self_init_glEvalCoord1xOES;
glEvalCoord1xvOESFUNC glEvalCoord1xvOES = self_init_glEvalCoord1xvOES;
glEvalCoord2xOESFUNC glEvalCoord2xOES = self_init_glEvalCoord2xOES;
glEvalCoord2xvOESFUNC glEvalCoord2xvOES = self_init_glEvalCoord2xvOES;
glFeedbackBufferxOESFUNC glFeedbackBufferxOES = self_init_glFeedbackBufferxOES;
glPassThroughxOESFUNC glPassThroughxOES = self_init_glPassThroughxOES;
glGetFixedvOESFUNC glGetFixedvOES = self_init_glGetFixedvOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_fragment_precision_high()
{
	return _GLES_OES_fragment_precision_high;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_fragment_precision_high()
{
	glloader_init();
	return glloader_GLES_OES_fragment_precision_high();
}
glloader_GLES_OES_fragment_precision_highFUNC glloader_GLES_OES_fragment_precision_high = self_init_glloader_GLES_OES_fragment_precision_high;

static char GLLOADER_APIENTRY _glloader_GLES_OES_framebuffer_object()
{
	return _GLES_OES_framebuffer_object;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_framebuffer_object()
{
	glloader_init();
	return glloader_GLES_OES_framebuffer_object();
}
glloader_GLES_OES_framebuffer_objectFUNC glloader_GLES_OES_framebuffer_object = self_init_glloader_GLES_OES_framebuffer_object;

#ifdef GLES_OES_framebuffer_object

static GLboolean GLLOADER_APIENTRY self_init_glIsRenderbufferOES(GLuint renderbuffer)
{
	glloader_init();
	return glIsRenderbufferOES(renderbuffer);
}
static void GLLOADER_APIENTRY self_init_glBindRenderbufferOES(GLenum target, GLuint renderbuffer)
{
	glloader_init();
	glBindRenderbufferOES(target, renderbuffer);
}
static void GLLOADER_APIENTRY self_init_glDeleteRenderbuffersOES(GLsizei n, const GLuint* renderbuffers)
{
	glloader_init();
	glDeleteRenderbuffersOES(n, renderbuffers);
}
static void GLLOADER_APIENTRY self_init_glGenRenderbuffersOES(GLsizei n, GLuint* renderbuffers)
{
	glloader_init();
	glGenRenderbuffersOES(n, renderbuffers);
}
static void GLLOADER_APIENTRY self_init_glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
	glloader_init();
	glRenderbufferStorageOES(target, internalformat, width, height);
}
static void GLLOADER_APIENTRY self_init_glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint* params)
{
	glloader_init();
	glGetRenderbufferParameterivOES(target, pname, params);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsFramebufferOES(GLuint framebuffer)
{
	glloader_init();
	return glIsFramebufferOES(framebuffer);
}
static void GLLOADER_APIENTRY self_init_glBindFramebufferOES(GLenum target, GLuint framebuffer)
{
	glloader_init();
	glBindFramebufferOES(target, framebuffer);
}
static void GLLOADER_APIENTRY self_init_glDeleteFramebuffersOES(GLsizei n, const GLuint* framebuffers)
{
	glloader_init();
	glDeleteFramebuffersOES(n, framebuffers);
}
static void GLLOADER_APIENTRY self_init_glGenFramebuffersOES(GLsizei n, GLuint* framebuffers)
{
	glloader_init();
	glGenFramebuffersOES(n, framebuffers);
}
static GLenum GLLOADER_APIENTRY self_init_glCheckFramebufferStatusOES(GLenum target)
{
	glloader_init();
	return glCheckFramebufferStatusOES(target);
}
static void GLLOADER_APIENTRY self_init_glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	glloader_init();
	glFramebufferTexture2DOES(target, attachment, textarget, texture, level);
}
static void GLLOADER_APIENTRY self_init_glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	glloader_init();
	glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer);
}
static void GLLOADER_APIENTRY self_init_glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
	glloader_init();
	glGetFramebufferAttachmentParameterivOES(target, attachment, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGenerateMipmapOES(GLenum target)
{
	glloader_init();
	glGenerateMipmapOES(target);
}

glIsRenderbufferOESFUNC glIsRenderbufferOES = self_init_glIsRenderbufferOES;
glBindRenderbufferOESFUNC glBindRenderbufferOES = self_init_glBindRenderbufferOES;
glDeleteRenderbuffersOESFUNC glDeleteRenderbuffersOES = self_init_glDeleteRenderbuffersOES;
glGenRenderbuffersOESFUNC glGenRenderbuffersOES = self_init_glGenRenderbuffersOES;
glRenderbufferStorageOESFUNC glRenderbufferStorageOES = self_init_glRenderbufferStorageOES;
glGetRenderbufferParameterivOESFUNC glGetRenderbufferParameterivOES = self_init_glGetRenderbufferParameterivOES;
glIsFramebufferOESFUNC glIsFramebufferOES = self_init_glIsFramebufferOES;
glBindFramebufferOESFUNC glBindFramebufferOES = self_init_glBindFramebufferOES;
glDeleteFramebuffersOESFUNC glDeleteFramebuffersOES = self_init_glDeleteFramebuffersOES;
glGenFramebuffersOESFUNC glGenFramebuffersOES = self_init_glGenFramebuffersOES;
glCheckFramebufferStatusOESFUNC glCheckFramebufferStatusOES = self_init_glCheckFramebufferStatusOES;
glFramebufferTexture2DOESFUNC glFramebufferTexture2DOES = self_init_glFramebufferTexture2DOES;
glFramebufferRenderbufferOESFUNC glFramebufferRenderbufferOES = self_init_glFramebufferRenderbufferOES;
glGetFramebufferAttachmentParameterivOESFUNC glGetFramebufferAttachmentParameterivOES = self_init_glGetFramebufferAttachmentParameterivOES;
glGenerateMipmapOESFUNC glGenerateMipmapOES = self_init_glGenerateMipmapOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_get_program_binary()
{
	return _GLES_OES_get_program_binary;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_get_program_binary()
{
	glloader_init();
	return glloader_GLES_OES_get_program_binary();
}
glloader_GLES_OES_get_program_binaryFUNC glloader_GLES_OES_get_program_binary = self_init_glloader_GLES_OES_get_program_binary;

#ifdef GLES_OES_get_program_binary

static void GLLOADER_APIENTRY self_init_glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary)
{
	glloader_init();
	glGetProgramBinaryOES(program, bufSize, length, binaryFormat, binary);
}
static void GLLOADER_APIENTRY self_init_glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void* binary, GLint length)
{
	glloader_init();
	glProgramBinaryOES(program, binaryFormat, binary, length);
}

glGetProgramBinaryOESFUNC glGetProgramBinaryOES = self_init_glGetProgramBinaryOES;
glProgramBinaryOESFUNC glProgramBinaryOES = self_init_glProgramBinaryOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_mapbuffer()
{
	return _GLES_OES_mapbuffer;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_mapbuffer()
{
	glloader_init();
	return glloader_GLES_OES_mapbuffer();
}
glloader_GLES_OES_mapbufferFUNC glloader_GLES_OES_mapbuffer = self_init_glloader_GLES_OES_mapbuffer;

#ifdef GLES_OES_mapbuffer

static void GLLOADER_APIENTRY self_init_glGetBufferPointervOES(GLenum target, GLenum pname, void** params)
{
	glloader_init();
	glGetBufferPointervOES(target, pname, params);
}
static void* GLLOADER_APIENTRY self_init_glMapBufferOES(GLenum target, GLenum access)
{
	glloader_init();
	return glMapBufferOES(target, access);
}
static GLboolean GLLOADER_APIENTRY self_init_glUnmapBufferOES(GLenum target)
{
	glloader_init();
	return glUnmapBufferOES(target);
}

glGetBufferPointervOESFUNC glGetBufferPointervOES = self_init_glGetBufferPointervOES;
glMapBufferOESFUNC glMapBufferOES = self_init_glMapBufferOES;
glUnmapBufferOESFUNC glUnmapBufferOES = self_init_glUnmapBufferOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_matrix_get()
{
	return _GLES_OES_matrix_get;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_matrix_get()
{
	glloader_init();
	return glloader_GLES_OES_matrix_get();
}
glloader_GLES_OES_matrix_getFUNC glloader_GLES_OES_matrix_get = self_init_glloader_GLES_OES_matrix_get;

static char GLLOADER_APIENTRY _glloader_GLES_OES_matrix_palette()
{
	return _GLES_OES_matrix_palette;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_matrix_palette()
{
	glloader_init();
	return glloader_GLES_OES_matrix_palette();
}
glloader_GLES_OES_matrix_paletteFUNC glloader_GLES_OES_matrix_palette = self_init_glloader_GLES_OES_matrix_palette;

#ifdef GLES_OES_matrix_palette

static void GLLOADER_APIENTRY self_init_glCurrentPaletteMatrixOES(GLuint matrixpaletteindex)
{
	glloader_init();
	glCurrentPaletteMatrixOES(matrixpaletteindex);
}
static void GLLOADER_APIENTRY self_init_glLoadPaletteFromModelViewMatrixOES()
{
	glloader_init();
	glLoadPaletteFromModelViewMatrixOES();
}
static void GLLOADER_APIENTRY self_init_glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	glloader_init();
	glMatrixIndexPointerOES(size, type, stride, pointer);
}
static void GLLOADER_APIENTRY self_init_glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	glloader_init();
	glWeightPointerOES(size, type, stride, pointer);
}

glCurrentPaletteMatrixOESFUNC glCurrentPaletteMatrixOES = self_init_glCurrentPaletteMatrixOES;
glLoadPaletteFromModelViewMatrixOESFUNC glLoadPaletteFromModelViewMatrixOES = self_init_glLoadPaletteFromModelViewMatrixOES;
glMatrixIndexPointerOESFUNC glMatrixIndexPointerOES = self_init_glMatrixIndexPointerOES;
glWeightPointerOESFUNC glWeightPointerOES = self_init_glWeightPointerOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_packed_depth_stencil()
{
	return _GLES_OES_packed_depth_stencil;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_packed_depth_stencil()
{
	glloader_init();
	return glloader_GLES_OES_packed_depth_stencil();
}
glloader_GLES_OES_packed_depth_stencilFUNC glloader_GLES_OES_packed_depth_stencil = self_init_glloader_GLES_OES_packed_depth_stencil;

static char GLLOADER_APIENTRY _glloader_GLES_OES_point_size_array()
{
	return _GLES_OES_point_size_array;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_point_size_array()
{
	glloader_init();
	return glloader_GLES_OES_point_size_array();
}
glloader_GLES_OES_point_size_arrayFUNC glloader_GLES_OES_point_size_array = self_init_glloader_GLES_OES_point_size_array;

#ifdef GLES_OES_point_size_array

static void GLLOADER_APIENTRY self_init_glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid* pointer)
{
	glloader_init();
	glPointSizePointerOES(type, stride, pointer);
}

glPointSizePointerOESFUNC glPointSizePointerOES = self_init_glPointSizePointerOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_point_sprite()
{
	return _GLES_OES_point_sprite;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_point_sprite()
{
	glloader_init();
	return glloader_GLES_OES_point_sprite();
}
glloader_GLES_OES_point_spriteFUNC glloader_GLES_OES_point_sprite = self_init_glloader_GLES_OES_point_sprite;

static char GLLOADER_APIENTRY _glloader_GLES_OES_query_matrix()
{
	return _GLES_OES_query_matrix;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_query_matrix()
{
	glloader_init();
	return glloader_GLES_OES_query_matrix();
}
glloader_GLES_OES_query_matrixFUNC glloader_GLES_OES_query_matrix = self_init_glloader_GLES_OES_query_matrix;

#ifdef GLES_OES_query_matrix

static void GLLOADER_APIENTRY self_init_glQueryMatrixxOES(GLfixed* mantissa, GLint* exponent)
{
	glloader_init();
	glQueryMatrixxOES(mantissa, exponent);
}

glQueryMatrixxOESFUNC glQueryMatrixxOES = self_init_glQueryMatrixxOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_read_format()
{
	return _GLES_OES_read_format;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_read_format()
{
	glloader_init();
	return glloader_GLES_OES_read_format();
}
glloader_GLES_OES_read_formatFUNC glloader_GLES_OES_read_format = self_init_glloader_GLES_OES_read_format;

static char GLLOADER_APIENTRY _glloader_GLES_OES_required_internalformat()
{
	return _GLES_OES_required_internalformat;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_required_internalformat()
{
	glloader_init();
	return glloader_GLES_OES_required_internalformat();
}
glloader_GLES_OES_required_internalformatFUNC glloader_GLES_OES_required_internalformat = self_init_glloader_GLES_OES_required_internalformat;

static char GLLOADER_APIENTRY _glloader_GLES_OES_rgb8_rgba8()
{
	return _GLES_OES_rgb8_rgba8;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_rgb8_rgba8()
{
	glloader_init();
	return glloader_GLES_OES_rgb8_rgba8();
}
glloader_GLES_OES_rgb8_rgba8FUNC glloader_GLES_OES_rgb8_rgba8 = self_init_glloader_GLES_OES_rgb8_rgba8;

static char GLLOADER_APIENTRY _glloader_GLES_OES_shader_binary()
{
	return _GLES_OES_shader_binary;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_shader_binary()
{
	glloader_init();
	return glloader_GLES_OES_shader_binary();
}
glloader_GLES_OES_shader_binaryFUNC glloader_GLES_OES_shader_binary = self_init_glloader_GLES_OES_shader_binary;

#ifdef GLES_OES_shader_binary

static void GLLOADER_APIENTRY self_init_glShaderBinaryOES(GLint n, GLuint* shaders, GLenum binaryformat, const void* binary, GLint length)
{
	glloader_init();
	glShaderBinaryOES(n, shaders, binaryformat, binary, length);
}

glShaderBinaryOESFUNC glShaderBinaryOES = self_init_glShaderBinaryOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_shader_source()
{
	return _GLES_OES_shader_source;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_shader_source()
{
	glloader_init();
	return glloader_GLES_OES_shader_source();
}
glloader_GLES_OES_shader_sourceFUNC glloader_GLES_OES_shader_source = self_init_glloader_GLES_OES_shader_source;

#ifdef GLES_OES_shader_source

static void GLLOADER_APIENTRY self_init_glReleaseShaderCompilerOES()
{
	glloader_init();
	glReleaseShaderCompilerOES();
}
static void GLLOADER_APIENTRY self_init_glGetShaderPrecisionFormatOES(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
	glloader_init();
	glGetShaderPrecisionFormatOES(shadertype, precisiontype, range, precision);
}

glReleaseShaderCompilerOESFUNC glReleaseShaderCompilerOES = self_init_glReleaseShaderCompilerOES;
glGetShaderPrecisionFormatOESFUNC glGetShaderPrecisionFormatOES = self_init_glGetShaderPrecisionFormatOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_single_precision()
{
	return _GLES_OES_single_precision;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_single_precision()
{
	glloader_init();
	return glloader_GLES_OES_single_precision();
}
glloader_GLES_OES_single_precisionFUNC glloader_GLES_OES_single_precision = self_init_glloader_GLES_OES_single_precision;

#ifdef GLES_OES_single_precision

static void GLLOADER_APIENTRY self_init_glDepthRangefOES(GLclampf n, GLclampf f)
{
	glloader_init();
	glDepthRangefOES(n, f);
}
static void GLLOADER_APIENTRY self_init_glFrustumfOES(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
	glloader_init();
	glFrustumfOES(l, r, b, t, n, f);
}
static void GLLOADER_APIENTRY self_init_glOrthofOES(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
	glloader_init();
	glOrthofOES(l, r, b, t, n, f);
}
static void GLLOADER_APIENTRY self_init_glClipPlanefOES(GLenum plane, const GLfloat* equation)
{
	glloader_init();
	glClipPlanefOES(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glGetClipPlanefOES(GLenum plane, GLfloat* equation)
{
	glloader_init();
	glGetClipPlanefOES(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glClearDepthfOES(GLclampd depth)
{
	glloader_init();
	glClearDepthfOES(depth);
}

glDepthRangefOESFUNC glDepthRangefOES = self_init_glDepthRangefOES;
glFrustumfOESFUNC glFrustumfOES = self_init_glFrustumfOES;
glOrthofOESFUNC glOrthofOES = self_init_glOrthofOES;
glClipPlanefOESFUNC glClipPlanefOES = self_init_glClipPlanefOES;
glGetClipPlanefOESFUNC glGetClipPlanefOES = self_init_glGetClipPlanefOES;
glClearDepthfOESFUNC glClearDepthfOES = self_init_glClearDepthfOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_standard_derivatives()
{
	return _GLES_OES_standard_derivatives;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_standard_derivatives()
{
	glloader_init();
	return glloader_GLES_OES_standard_derivatives();
}
glloader_GLES_OES_standard_derivativesFUNC glloader_GLES_OES_standard_derivatives = self_init_glloader_GLES_OES_standard_derivatives;

static char GLLOADER_APIENTRY _glloader_GLES_OES_stencil1()
{
	return _GLES_OES_stencil1;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_stencil1()
{
	glloader_init();
	return glloader_GLES_OES_stencil1();
}
glloader_GLES_OES_stencil1FUNC glloader_GLES_OES_stencil1 = self_init_glloader_GLES_OES_stencil1;

static char GLLOADER_APIENTRY _glloader_GLES_OES_stencil4()
{
	return _GLES_OES_stencil4;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_stencil4()
{
	glloader_init();
	return glloader_GLES_OES_stencil4();
}
glloader_GLES_OES_stencil4FUNC glloader_GLES_OES_stencil4 = self_init_glloader_GLES_OES_stencil4;

static char GLLOADER_APIENTRY _glloader_GLES_OES_stencil8()
{
	return _GLES_OES_stencil8;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_stencil8()
{
	glloader_init();
	return glloader_GLES_OES_stencil8();
}
glloader_GLES_OES_stencil8FUNC glloader_GLES_OES_stencil8 = self_init_glloader_GLES_OES_stencil8;

static char GLLOADER_APIENTRY _glloader_GLES_OES_stencil_wrap()
{
	return _GLES_OES_stencil_wrap;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_stencil_wrap()
{
	glloader_init();
	return glloader_GLES_OES_stencil_wrap();
}
glloader_GLES_OES_stencil_wrapFUNC glloader_GLES_OES_stencil_wrap = self_init_glloader_GLES_OES_stencil_wrap;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_3D()
{
	return _GLES_OES_texture_3D;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_3D()
{
	glloader_init();
	return glloader_GLES_OES_texture_3D();
}
glloader_GLES_OES_texture_3DFUNC glloader_GLES_OES_texture_3D = self_init_glloader_GLES_OES_texture_3D;

#ifdef GLES_OES_texture_3D

static void GLLOADER_APIENTRY self_init_glTexImage3DOES(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
	glloader_init();
	glTexImage3DOES(target, level, internalFormat, width, height, depth, border, format, type, pixels);
}
static void GLLOADER_APIENTRY self_init_glTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels)
{
	glloader_init();
	glTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}
static void GLLOADER_APIENTRY self_init_glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	glloader_init();
	glCopyTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, x, y, width, height);
}
static void GLLOADER_APIENTRY self_init_glCompressedTexImage3DOES(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data)
{
	glloader_init();
	glCompressedTexImage3DOES(target, level, internalFormat, width, height, depth, border, imageSize, data);
}
static void GLLOADER_APIENTRY self_init_glCompressedTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data)
{
	glloader_init();
	glCompressedTexSubImage3DOES(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
}
static void GLLOADER_APIENTRY self_init_glFramebufferTexture3DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
	glloader_init();
	glFramebufferTexture3DOES(target, attachment, textarget, texture, level, zoffset);
}

glTexImage3DOESFUNC glTexImage3DOES = self_init_glTexImage3DOES;
glTexSubImage3DOESFUNC glTexSubImage3DOES = self_init_glTexSubImage3DOES;
glCopyTexSubImage3DOESFUNC glCopyTexSubImage3DOES = self_init_glCopyTexSubImage3DOES;
glCompressedTexImage3DOESFUNC glCompressedTexImage3DOES = self_init_glCompressedTexImage3DOES;
glCompressedTexSubImage3DOESFUNC glCompressedTexSubImage3DOES = self_init_glCompressedTexSubImage3DOES;
glFramebufferTexture3DOESFUNC glFramebufferTexture3DOES = self_init_glFramebufferTexture3DOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_cube_map()
{
	return _GLES_OES_texture_cube_map;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_cube_map()
{
	glloader_init();
	return glloader_GLES_OES_texture_cube_map();
}
glloader_GLES_OES_texture_cube_mapFUNC glloader_GLES_OES_texture_cube_map = self_init_glloader_GLES_OES_texture_cube_map;

#ifdef GLES_OES_texture_cube_map

static void GLLOADER_APIENTRY self_init_glTexGenfOES(GLenum coord, GLenum pname, GLfloat param)
{
	glloader_init();
	glTexGenfOES(coord, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat* params)
{
	glloader_init();
	glTexGenfvOES(coord, pname, params);
}
static void GLLOADER_APIENTRY self_init_glTexGeniOES(GLenum coord, GLenum pname, GLint param)
{
	glloader_init();
	glTexGeniOES(coord, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexGenivOES(GLenum coord, GLenum pname, const GLint* params)
{
	glloader_init();
	glTexGenivOES(coord, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat* params)
{
	glloader_init();
	glGetTexGenfvOES(coord, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexGenivOES(GLenum coord, GLenum pname, GLint* params)
{
	glloader_init();
	glGetTexGenivOES(coord, pname, params);
}

glTexGenfOESFUNC glTexGenfOES = self_init_glTexGenfOES;
glTexGenfvOESFUNC glTexGenfvOES = self_init_glTexGenfvOES;
glTexGeniOESFUNC glTexGeniOES = self_init_glTexGeniOES;
glTexGenivOESFUNC glTexGenivOES = self_init_glTexGenivOES;
glGetTexGenfvOESFUNC glGetTexGenfvOES = self_init_glGetTexGenfvOES;
glGetTexGenivOESFUNC glGetTexGenivOES = self_init_glGetTexGenivOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_env_crossbar()
{
	return _GLES_OES_texture_env_crossbar;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_env_crossbar()
{
	glloader_init();
	return glloader_GLES_OES_texture_env_crossbar();
}
glloader_GLES_OES_texture_env_crossbarFUNC glloader_GLES_OES_texture_env_crossbar = self_init_glloader_GLES_OES_texture_env_crossbar;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_float()
{
	return _GLES_OES_texture_float;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_float()
{
	glloader_init();
	return glloader_GLES_OES_texture_float();
}
glloader_GLES_OES_texture_floatFUNC glloader_GLES_OES_texture_float = self_init_glloader_GLES_OES_texture_float;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_float_linear()
{
	return _GLES_OES_texture_float_linear;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_float_linear()
{
	glloader_init();
	return glloader_GLES_OES_texture_float_linear();
}
glloader_GLES_OES_texture_float_linearFUNC glloader_GLES_OES_texture_float_linear = self_init_glloader_GLES_OES_texture_float_linear;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_half_float()
{
	return _GLES_OES_texture_half_float;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_half_float()
{
	glloader_init();
	return glloader_GLES_OES_texture_half_float();
}
glloader_GLES_OES_texture_half_floatFUNC glloader_GLES_OES_texture_half_float = self_init_glloader_GLES_OES_texture_half_float;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_half_float_linear()
{
	return _GLES_OES_texture_half_float_linear;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_half_float_linear()
{
	glloader_init();
	return glloader_GLES_OES_texture_half_float_linear();
}
glloader_GLES_OES_texture_half_float_linearFUNC glloader_GLES_OES_texture_half_float_linear = self_init_glloader_GLES_OES_texture_half_float_linear;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_mirrored_repeat()
{
	return _GLES_OES_texture_mirrored_repeat;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_mirrored_repeat()
{
	glloader_init();
	return glloader_GLES_OES_texture_mirrored_repeat();
}
glloader_GLES_OES_texture_mirrored_repeatFUNC glloader_GLES_OES_texture_mirrored_repeat = self_init_glloader_GLES_OES_texture_mirrored_repeat;

static char GLLOADER_APIENTRY _glloader_GLES_OES_texture_npot()
{
	return _GLES_OES_texture_npot;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_texture_npot()
{
	glloader_init();
	return glloader_GLES_OES_texture_npot();
}
glloader_GLES_OES_texture_npotFUNC glloader_GLES_OES_texture_npot = self_init_glloader_GLES_OES_texture_npot;

static char GLLOADER_APIENTRY _glloader_GLES_OES_vertex_array_object()
{
	return _GLES_OES_vertex_array_object;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_vertex_array_object()
{
	glloader_init();
	return glloader_GLES_OES_vertex_array_object();
}
glloader_GLES_OES_vertex_array_objectFUNC glloader_GLES_OES_vertex_array_object = self_init_glloader_GLES_OES_vertex_array_object;

#ifdef GLES_OES_vertex_array_object

static void GLLOADER_APIENTRY self_init_glBindVertexArrayOES(GLuint array)
{
	glloader_init();
	glBindVertexArrayOES(array);
}
static void GLLOADER_APIENTRY self_init_glDeleteVertexArraysOES(GLsizei n, const GLuint* arrays)
{
	glloader_init();
	glDeleteVertexArraysOES(n, arrays);
}
static void GLLOADER_APIENTRY self_init_glGenVertexArraysOES(GLsizei n, GLuint* arrays)
{
	glloader_init();
	glGenVertexArraysOES(n, arrays);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsVertexArrayOES(GLuint array)
{
	glloader_init();
	return glIsVertexArrayOES(array);
}

glBindVertexArrayOESFUNC glBindVertexArrayOES = self_init_glBindVertexArrayOES;
glDeleteVertexArraysOESFUNC glDeleteVertexArraysOES = self_init_glDeleteVertexArraysOES;
glGenVertexArraysOESFUNC glGenVertexArraysOES = self_init_glGenVertexArraysOES;
glIsVertexArrayOESFUNC glIsVertexArrayOES = self_init_glIsVertexArrayOES;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_OES_vertex_half_float()
{
	return _GLES_OES_vertex_half_float;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_vertex_half_float()
{
	glloader_init();
	return glloader_GLES_OES_vertex_half_float();
}
glloader_GLES_OES_vertex_half_floatFUNC glloader_GLES_OES_vertex_half_float = self_init_glloader_GLES_OES_vertex_half_float;

static char GLLOADER_APIENTRY _glloader_GLES_OES_vertex_type_10_10_10_2()
{
	return _GLES_OES_vertex_type_10_10_10_2;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_OES_vertex_type_10_10_10_2()
{
	glloader_init();
	return glloader_GLES_OES_vertex_type_10_10_10_2();
}
glloader_GLES_OES_vertex_type_10_10_10_2FUNC glloader_GLES_OES_vertex_type_10_10_10_2 = self_init_glloader_GLES_OES_vertex_type_10_10_10_2;

static char GLLOADER_APIENTRY _glloader_GLES_QCOM_driver_control()
{
	return _GLES_QCOM_driver_control;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_QCOM_driver_control()
{
	glloader_init();
	return glloader_GLES_QCOM_driver_control();
}
glloader_GLES_QCOM_driver_controlFUNC glloader_GLES_QCOM_driver_control = self_init_glloader_GLES_QCOM_driver_control;

#ifdef GLES_QCOM_driver_control

static void GLLOADER_APIENTRY self_init_glGetDriverControlsQCOM(GLint* num, GLsizei size, GLuint* driverControls)
{
	glloader_init();
	glGetDriverControlsQCOM(num, size, driverControls);
}
static void GLLOADER_APIENTRY self_init_glGetDriverControlStringQCOM(GLuint driverControl, GLsizei bufSize, GLsizei* length, GLchar* driverControlString)
{
	glloader_init();
	glGetDriverControlStringQCOM(driverControl, bufSize, length, driverControlString);
}
static void GLLOADER_APIENTRY self_init_glEnableDriverControlQCOM(GLuint driverControl)
{
	glloader_init();
	glEnableDriverControlQCOM(driverControl);
}
static void GLLOADER_APIENTRY self_init_glDisableDriverControlQCOM(GLuint driverControl)
{
	glloader_init();
	glDisableDriverControlQCOM(driverControl);
}

glGetDriverControlsQCOMFUNC glGetDriverControlsQCOM = self_init_glGetDriverControlsQCOM;
glGetDriverControlStringQCOMFUNC glGetDriverControlStringQCOM = self_init_glGetDriverControlStringQCOM;
glEnableDriverControlQCOMFUNC glEnableDriverControlQCOM = self_init_glEnableDriverControlQCOM;
glDisableDriverControlQCOMFUNC glDisableDriverControlQCOM = self_init_glDisableDriverControlQCOM;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_QCOM_extended_get()
{
	return _GLES_QCOM_extended_get;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_QCOM_extended_get()
{
	glloader_init();
	return glloader_GLES_QCOM_extended_get();
}
glloader_GLES_QCOM_extended_getFUNC glloader_GLES_QCOM_extended_get = self_init_glloader_GLES_QCOM_extended_get;

#ifdef GLES_QCOM_extended_get

static void GLLOADER_APIENTRY self_init_glExtGetTexturesQCOM(GLuint* textures, GLint maxTextures, GLint* numTextures)
{
	glloader_init();
	glExtGetTexturesQCOM(textures, maxTextures, numTextures);
}
static void GLLOADER_APIENTRY self_init_glExtGetBuffersQCOM(GLuint* buffers, GLint maxBuffers, GLint* numBuffers)
{
	glloader_init();
	glExtGetBuffersQCOM(buffers, maxBuffers, numBuffers);
}
static void GLLOADER_APIENTRY self_init_glExtGetRenderbuffersQCOM(GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers)
{
	glloader_init();
	glExtGetRenderbuffersQCOM(renderbuffers, maxRenderbuffers, numRenderbuffers);
}
static void GLLOADER_APIENTRY self_init_glExtGetFramebuffersQCOM(GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers)
{
	glloader_init();
	glExtGetFramebuffersQCOM(framebuffers, maxFramebuffers, numFramebuffers);
}
static void GLLOADER_APIENTRY self_init_glExtGetTexLevelParameterivQCOM(GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params)
{
	glloader_init();
	glExtGetTexLevelParameterivQCOM(texture, face, level, pname, params);
}
static void GLLOADER_APIENTRY self_init_glExtTexObjectStateOverrideiQCOM(GLenum target, GLenum pname, GLint param)
{
	glloader_init();
	glExtTexObjectStateOverrideiQCOM(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glExtGetTexSubImageQCOM(GLenum targe, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void* texels)
{
	glloader_init();
	glExtGetTexSubImageQCOM(targe, level, xoffset, yoffset, zoffset, width, height, depth, format, type, texels);
}
static void GLLOADER_APIENTRY self_init_glExtGetBufferPointervQCOM(GLenum target, void** params)
{
	glloader_init();
	glExtGetBufferPointervQCOM(target, params);
}

glExtGetTexturesQCOMFUNC glExtGetTexturesQCOM = self_init_glExtGetTexturesQCOM;
glExtGetBuffersQCOMFUNC glExtGetBuffersQCOM = self_init_glExtGetBuffersQCOM;
glExtGetRenderbuffersQCOMFUNC glExtGetRenderbuffersQCOM = self_init_glExtGetRenderbuffersQCOM;
glExtGetFramebuffersQCOMFUNC glExtGetFramebuffersQCOM = self_init_glExtGetFramebuffersQCOM;
glExtGetTexLevelParameterivQCOMFUNC glExtGetTexLevelParameterivQCOM = self_init_glExtGetTexLevelParameterivQCOM;
glExtTexObjectStateOverrideiQCOMFUNC glExtTexObjectStateOverrideiQCOM = self_init_glExtTexObjectStateOverrideiQCOM;
glExtGetTexSubImageQCOMFUNC glExtGetTexSubImageQCOM = self_init_glExtGetTexSubImageQCOM;
glExtGetBufferPointervQCOMFUNC glExtGetBufferPointervQCOM = self_init_glExtGetBufferPointervQCOM;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_QCOM_extended_get2()
{
	return _GLES_QCOM_extended_get2;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_QCOM_extended_get2()
{
	glloader_init();
	return glloader_GLES_QCOM_extended_get2();
}
glloader_GLES_QCOM_extended_get2FUNC glloader_GLES_QCOM_extended_get2 = self_init_glloader_GLES_QCOM_extended_get2;

#ifdef GLES_QCOM_extended_get2

static void GLLOADER_APIENTRY self_init_glExtGetShadersQCOM(GLuint* shaders, GLint maxShaders, GLint* numShaders)
{
	glloader_init();
	glExtGetShadersQCOM(shaders, maxShaders, numShaders);
}
static void GLLOADER_APIENTRY self_init_glExtGetProgramsQCOM(GLuint* programs, GLint maxPrograms, GLint* numPrograms)
{
	glloader_init();
	glExtGetProgramsQCOM(programs, maxPrograms, numPrograms);
}
static GLboolean GLLOADER_APIENTRY self_init_glExtIsProgramBinaryQCOM(GLuint program)
{
	glloader_init();
	return glExtIsProgramBinaryQCOM(program);
}
static void GLLOADER_APIENTRY self_init_glExtGetProgramBinarySourceQCOM(GLuint program, GLenum shadertype, GLchar* source, GLint* length)
{
	glloader_init();
	glExtGetProgramBinarySourceQCOM(program, shadertype, source, length);
}

glExtGetShadersQCOMFUNC glExtGetShadersQCOM = self_init_glExtGetShadersQCOM;
glExtGetProgramsQCOMFUNC glExtGetProgramsQCOM = self_init_glExtGetProgramsQCOM;
glExtIsProgramBinaryQCOMFUNC glExtIsProgramBinaryQCOM = self_init_glExtIsProgramBinaryQCOM;
glExtGetProgramBinarySourceQCOMFUNC glExtGetProgramBinarySourceQCOM = self_init_glExtGetProgramBinarySourceQCOM;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_QCOM_performance_monitor_global_mode()
{
	return _GLES_QCOM_performance_monitor_global_mode;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_QCOM_performance_monitor_global_mode()
{
	glloader_init();
	return glloader_GLES_QCOM_performance_monitor_global_mode();
}
glloader_GLES_QCOM_performance_monitor_global_modeFUNC glloader_GLES_QCOM_performance_monitor_global_mode = self_init_glloader_GLES_QCOM_performance_monitor_global_mode;

static char GLLOADER_APIENTRY _glloader_GLES_QCOM_tiled_rendering()
{
	return _GLES_QCOM_tiled_rendering;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_QCOM_tiled_rendering()
{
	glloader_init();
	return glloader_GLES_QCOM_tiled_rendering();
}
glloader_GLES_QCOM_tiled_renderingFUNC glloader_GLES_QCOM_tiled_rendering = self_init_glloader_GLES_QCOM_tiled_rendering;

#ifdef GLES_QCOM_tiled_rendering

static void GLLOADER_APIENTRY self_init_glStartTilingQCOM(GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask)
{
	glloader_init();
	glStartTilingQCOM(x, y, width, height, preserveMask);
}
static void GLLOADER_APIENTRY self_init_glEndTilingQCOM(GLbitfield preserveMask)
{
	glloader_init();
	glEndTilingQCOM(preserveMask);
}

glStartTilingQCOMFUNC glStartTilingQCOM = self_init_glStartTilingQCOM;
glEndTilingQCOMFUNC glEndTilingQCOM = self_init_glEndTilingQCOM;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_QCOM_writeonly_rendering()
{
	return _GLES_QCOM_writeonly_rendering;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_QCOM_writeonly_rendering()
{
	glloader_init();
	return glloader_GLES_QCOM_writeonly_rendering();
}
glloader_GLES_QCOM_writeonly_renderingFUNC glloader_GLES_QCOM_writeonly_rendering = self_init_glloader_GLES_QCOM_writeonly_rendering;

static char GLLOADER_APIENTRY _glloader_GLES_VERSION_1_0()
{
	return _GLES_VERSION_1_0;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_VERSION_1_0()
{
	glloader_init();
	return glloader_GLES_VERSION_1_0();
}
glloader_GLES_VERSION_1_0FUNC glloader_GLES_VERSION_1_0 = self_init_glloader_GLES_VERSION_1_0;

#ifdef GLES_VERSION_1_0

static void GLLOADER_APIENTRY self_init_glActiveTexture(GLenum texture)
{
	LOAD_FUNC1(glActiveTexture);
	glActiveTexture(texture);
}
static void GLLOADER_APIENTRY self_init_glAlphaFunc(GLenum func, GLclampf ref)
{
	LOAD_FUNC1(glAlphaFunc);
	glAlphaFunc(func, ref);
}
static void GLLOADER_APIENTRY self_init_glAlphaFuncx(GLenum func, GLclampx ref)
{
	LOAD_FUNC1(glAlphaFuncx);
	glAlphaFuncx(func, ref);
}
static void GLLOADER_APIENTRY self_init_glBindTexture(GLenum target, GLuint texture)
{
	LOAD_FUNC1(glBindTexture);
	glBindTexture(target, texture);
}
static void GLLOADER_APIENTRY self_init_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	LOAD_FUNC1(glBlendFunc);
	glBlendFunc(sfactor, dfactor);
}
static void GLLOADER_APIENTRY self_init_glClear(GLbitfield mask)
{
	LOAD_FUNC1(glClear);
	glClear(mask);
}
static void GLLOADER_APIENTRY self_init_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	LOAD_FUNC1(glClearColor);
	glClearColor(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
	LOAD_FUNC1(glClearColorx);
	glClearColorx(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glClearDepthf(GLclampf depth)
{
	LOAD_FUNC1(glClearDepthf);
	glClearDepthf(depth);
}
static void GLLOADER_APIENTRY self_init_glClearDepthx(GLclampx depth)
{
	LOAD_FUNC1(glClearDepthx);
	glClearDepthx(depth);
}
static void GLLOADER_APIENTRY self_init_glClearStencil(GLint s)
{
	LOAD_FUNC1(glClearStencil);
	glClearStencil(s);
}
static void GLLOADER_APIENTRY self_init_glClientActiveTexture(GLenum texture)
{
	LOAD_FUNC1(glClientActiveTexture);
	glClientActiveTexture(texture);
}
static void GLLOADER_APIENTRY self_init_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	LOAD_FUNC1(glColor4f);
	glColor4f(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
	LOAD_FUNC1(glColor4x);
	glColor4x(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	LOAD_FUNC1(glColorMask);
	glColorMask(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	LOAD_FUNC1(glColorPointer);
	glColorPointer(size, type, stride, pointer);
}
static void GLLOADER_APIENTRY self_init_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data)
{
	LOAD_FUNC1(glCompressedTexImage2D);
	glCompressedTexImage2D(target, level, internalFormat, width, height, border, imageSize, data);
}
static void GLLOADER_APIENTRY self_init_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data)
{
	LOAD_FUNC1(glCompressedTexSubImage2D);
	glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}
static void GLLOADER_APIENTRY self_init_glCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	LOAD_FUNC1(glCopyTexImage2D);
	glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);
}
static void GLLOADER_APIENTRY self_init_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	LOAD_FUNC1(glCopyTexSubImage2D);
	glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}
static void GLLOADER_APIENTRY self_init_glCullFace(GLenum mode)
{
	LOAD_FUNC1(glCullFace);
	glCullFace(mode);
}
static void GLLOADER_APIENTRY self_init_glDeleteTextures(GLsizei n, const GLuint* textures)
{
	LOAD_FUNC1(glDeleteTextures);
	glDeleteTextures(n, textures);
}
static void GLLOADER_APIENTRY self_init_glDepthFunc(GLenum func)
{
	LOAD_FUNC1(glDepthFunc);
	glDepthFunc(func);
}
static void GLLOADER_APIENTRY self_init_glDepthMask(GLboolean flag)
{
	LOAD_FUNC1(glDepthMask);
	glDepthMask(flag);
}
static void GLLOADER_APIENTRY self_init_glDepthRangef(GLclampf zNear, GLclampf zFar)
{
	LOAD_FUNC1(glDepthRangef);
	glDepthRangef(zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glDepthRangex(GLclampx zNear, GLclampx zFar)
{
	LOAD_FUNC1(glDepthRangex);
	glDepthRangex(zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glDisable(GLenum cap)
{
	LOAD_FUNC1(glDisable);
	glDisable(cap);
}
static void GLLOADER_APIENTRY self_init_glDisableClientState(GLenum array)
{
	LOAD_FUNC1(glDisableClientState);
	glDisableClientState(array);
}
static void GLLOADER_APIENTRY self_init_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	LOAD_FUNC1(glDrawArrays);
	glDrawArrays(mode, first, count);
}
static void GLLOADER_APIENTRY self_init_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	LOAD_FUNC1(glDrawElements);
	glDrawElements(mode, count, type, indices);
}
static void GLLOADER_APIENTRY self_init_glEnable(GLenum cap)
{
	LOAD_FUNC1(glEnable);
	glEnable(cap);
}
static void GLLOADER_APIENTRY self_init_glEnableClientState(GLenum array)
{
	LOAD_FUNC1(glEnableClientState);
	glEnableClientState(array);
}
static void GLLOADER_APIENTRY self_init_glFinish()
{
	LOAD_FUNC1(glFinish);
	glFinish();
}
static void GLLOADER_APIENTRY self_init_glFlush()
{
	LOAD_FUNC1(glFlush);
	glFlush();
}
static void GLLOADER_APIENTRY self_init_glFogf(GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glFogf);
	glFogf(pname, param);
}
static void GLLOADER_APIENTRY self_init_glFogfv(GLenum pname, const GLfloat* params)
{
	LOAD_FUNC1(glFogfv);
	glFogfv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glFogx(GLenum pname, GLfixed param)
{
	LOAD_FUNC1(glFogx);
	glFogx(pname, param);
}
static void GLLOADER_APIENTRY self_init_glFogxv(GLenum pname, const GLfixed* params)
{
	LOAD_FUNC1(glFogxv);
	glFogxv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glFrontFace(GLenum mode)
{
	LOAD_FUNC1(glFrontFace);
	glFrontFace(mode);
}
static void GLLOADER_APIENTRY self_init_glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	LOAD_FUNC1(glFrustumf);
	glFrustumf(left, right, bottom, top, zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	LOAD_FUNC1(glFrustumx);
	glFrustumx(left, right, bottom, top, zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glGenTextures(GLsizei n, GLuint* textures)
{
	LOAD_FUNC1(glGenTextures);
	glGenTextures(n, textures);
}
static GLenum GLLOADER_APIENTRY self_init_glGetError()
{
	LOAD_FUNC1(glGetError);
	return glGetError();
}
static void GLLOADER_APIENTRY self_init_glGetIntegerv(GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetIntegerv);
	glGetIntegerv(pname, params);
}
static const GLubyte* GLLOADER_APIENTRY self_init_glGetString(GLenum name)
{
	LOAD_FUNC1(glGetString);
	return glGetString(name);
}
static void GLLOADER_APIENTRY self_init_glHint(GLenum target, GLenum mode)
{
	LOAD_FUNC1(glHint);
	glHint(target, mode);
}
static void GLLOADER_APIENTRY self_init_glLightModelf(GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glLightModelf);
	glLightModelf(pname, param);
}
static void GLLOADER_APIENTRY self_init_glLightModelfv(GLenum pname, const GLfloat* params)
{
	LOAD_FUNC1(glLightModelfv);
	glLightModelfv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glLightModelx(GLenum pname, GLfixed param)
{
	LOAD_FUNC1(glLightModelx);
	glLightModelx(pname, param);
}
static void GLLOADER_APIENTRY self_init_glLightModelxv(GLenum pname, const GLfixed* params)
{
	LOAD_FUNC1(glLightModelxv);
	glLightModelxv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glLightf(GLenum light, GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glLightf);
	glLightf(light, pname, param);
}
static void GLLOADER_APIENTRY self_init_glLightfv(GLenum light, GLenum pname, const GLfloat* params)
{
	LOAD_FUNC1(glLightfv);
	glLightfv(light, pname, params);
}
static void GLLOADER_APIENTRY self_init_glLightx(GLenum light, GLenum pname, GLfixed param)
{
	LOAD_FUNC1(glLightx);
	glLightx(light, pname, param);
}
static void GLLOADER_APIENTRY self_init_glLightxv(GLenum light, GLenum pname, const GLfixed* params)
{
	LOAD_FUNC1(glLightxv);
	glLightxv(light, pname, params);
}
static void GLLOADER_APIENTRY self_init_glLineWidth(GLfloat width)
{
	LOAD_FUNC1(glLineWidth);
	glLineWidth(width);
}
static void GLLOADER_APIENTRY self_init_glLineWidthx(GLfixed width)
{
	LOAD_FUNC1(glLineWidthx);
	glLineWidthx(width);
}
static void GLLOADER_APIENTRY self_init_glLoadIdentity()
{
	LOAD_FUNC1(glLoadIdentity);
	glLoadIdentity();
}
static void GLLOADER_APIENTRY self_init_glLoadMatrixf(const GLfloat* m)
{
	LOAD_FUNC1(glLoadMatrixf);
	glLoadMatrixf(m);
}
static void GLLOADER_APIENTRY self_init_glLoadMatrixx(const GLfixed* m)
{
	LOAD_FUNC1(glLoadMatrixx);
	glLoadMatrixx(m);
}
static void GLLOADER_APIENTRY self_init_glLogicOp(GLenum opcode)
{
	LOAD_FUNC1(glLogicOp);
	glLogicOp(opcode);
}
static void GLLOADER_APIENTRY self_init_glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glMaterialf);
	glMaterialf(face, pname, param);
}
static void GLLOADER_APIENTRY self_init_glMaterialfv(GLenum face, GLenum pname, const GLfloat* params)
{
	LOAD_FUNC1(glMaterialfv);
	glMaterialfv(face, pname, params);
}
static void GLLOADER_APIENTRY self_init_glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
	LOAD_FUNC1(glMaterialx);
	glMaterialx(face, pname, param);
}
static void GLLOADER_APIENTRY self_init_glMaterialxv(GLenum face, GLenum pname, const GLfixed* params)
{
	LOAD_FUNC1(glMaterialxv);
	glMaterialxv(face, pname, params);
}
static void GLLOADER_APIENTRY self_init_glMatrixMode(GLenum mode)
{
	LOAD_FUNC1(glMatrixMode);
	glMatrixMode(mode);
}
static void GLLOADER_APIENTRY self_init_glMultMatrixf(const GLfloat* m)
{
	LOAD_FUNC1(glMultMatrixf);
	glMultMatrixf(m);
}
static void GLLOADER_APIENTRY self_init_glMultMatrixx(const GLfixed* m)
{
	LOAD_FUNC1(glMultMatrixx);
	glMultMatrixx(m);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	LOAD_FUNC1(glMultiTexCoord4f);
	glMultiTexCoord4f(target, s, t, r, q);
}
static void GLLOADER_APIENTRY self_init_glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
	LOAD_FUNC1(glMultiTexCoord4x);
	glMultiTexCoord4x(target, s, t, r, q);
}
static void GLLOADER_APIENTRY self_init_glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	LOAD_FUNC1(glNormal3f);
	glNormal3f(nx, ny, nz);
}
static void GLLOADER_APIENTRY self_init_glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
	LOAD_FUNC1(glNormal3x);
	glNormal3x(nx, ny, nz);
}
static void GLLOADER_APIENTRY self_init_glNormalPointer(GLenum type, GLsizei stride, const GLvoid* pointer)
{
	LOAD_FUNC1(glNormalPointer);
	glNormalPointer(type, stride, pointer);
}
static void GLLOADER_APIENTRY self_init_glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	LOAD_FUNC1(glOrthof);
	glOrthof(left, right, bottom, top, zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
	LOAD_FUNC1(glOrthox);
	glOrthox(left, right, bottom, top, zNear, zFar);
}
static void GLLOADER_APIENTRY self_init_glPixelStorei(GLenum pname, GLint param)
{
	LOAD_FUNC1(glPixelStorei);
	glPixelStorei(pname, param);
}
static void GLLOADER_APIENTRY self_init_glPointSize(GLfloat size)
{
	LOAD_FUNC1(glPointSize);
	glPointSize(size);
}
static void GLLOADER_APIENTRY self_init_glPointSizex(GLfixed size)
{
	LOAD_FUNC1(glPointSizex);
	glPointSizex(size);
}
static void GLLOADER_APIENTRY self_init_glPolygonOffset(GLfloat factor, GLfloat units)
{
	LOAD_FUNC1(glPolygonOffset);
	glPolygonOffset(factor, units);
}
static void GLLOADER_APIENTRY self_init_glPolygonOffsetx(GLfixed factor, GLfixed units)
{
	LOAD_FUNC1(glPolygonOffsetx);
	glPolygonOffsetx(factor, units);
}
static void GLLOADER_APIENTRY self_init_glPopMatrix()
{
	LOAD_FUNC1(glPopMatrix);
	glPopMatrix();
}
static void GLLOADER_APIENTRY self_init_glPushMatrix()
{
	LOAD_FUNC1(glPushMatrix);
	glPushMatrix();
}
static void GLLOADER_APIENTRY self_init_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
	LOAD_FUNC1(glReadPixels);
	glReadPixels(x, y, width, height, format, type, pixels);
}
static void GLLOADER_APIENTRY self_init_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	LOAD_FUNC1(glRotatef);
	glRotatef(angle, x, y, z);
}
static void GLLOADER_APIENTRY self_init_glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
	LOAD_FUNC1(glRotatex);
	glRotatex(angle, x, y, z);
}
static void GLLOADER_APIENTRY self_init_glSampleCoverage(GLclampf value, GLboolean invert)
{
	LOAD_FUNC1(glSampleCoverage);
	glSampleCoverage(value, invert);
}
static void GLLOADER_APIENTRY self_init_glSampleCoveragex(GLclampx value, GLboolean invert)
{
	LOAD_FUNC1(glSampleCoveragex);
	glSampleCoveragex(value, invert);
}
static void GLLOADER_APIENTRY self_init_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	LOAD_FUNC1(glScalef);
	glScalef(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glScalex(GLfixed x, GLfixed y, GLfixed z)
{
	LOAD_FUNC1(glScalex);
	glScalex(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	LOAD_FUNC1(glScissor);
	glScissor(x, y, width, height);
}
static void GLLOADER_APIENTRY self_init_glShadeModel(GLenum mode)
{
	LOAD_FUNC1(glShadeModel);
	glShadeModel(mode);
}
static void GLLOADER_APIENTRY self_init_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	LOAD_FUNC1(glStencilFunc);
	glStencilFunc(func, ref, mask);
}
static void GLLOADER_APIENTRY self_init_glStencilMask(GLuint mask)
{
	LOAD_FUNC1(glStencilMask);
	glStencilMask(mask);
}
static void GLLOADER_APIENTRY self_init_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	LOAD_FUNC1(glStencilOp);
	glStencilOp(fail, zfail, zpass);
}
static void GLLOADER_APIENTRY self_init_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	LOAD_FUNC1(glTexCoordPointer);
	glTexCoordPointer(size, type, stride, pointer);
}
static void GLLOADER_APIENTRY self_init_glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glTexEnvf);
	glTexEnvf(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexEnvfv(GLenum target, GLenum pname, const GLfloat* params)
{
	LOAD_FUNC1(glTexEnvfv);
	glTexEnvfv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
	LOAD_FUNC1(glTexEnvx);
	glTexEnvx(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexEnvxv(GLenum target, GLenum pname, const GLfixed* params)
{
	LOAD_FUNC1(glTexEnvxv);
	glTexEnvxv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
	LOAD_FUNC1(glTexImage2D);
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}
static void GLLOADER_APIENTRY self_init_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glTexParameterf);
	glTexParameterf(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
	LOAD_FUNC1(glTexParameterx);
	glTexParameterx(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
	LOAD_FUNC1(glTexSubImage2D);
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}
static void GLLOADER_APIENTRY self_init_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	LOAD_FUNC1(glTranslatef);
	glTranslatef(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	LOAD_FUNC1(glVertexPointer);
	glVertexPointer(size, type, stride, pointer);
}
static void GLLOADER_APIENTRY self_init_glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
	LOAD_FUNC1(glTranslatex);
	glTranslatex(x, y, z);
}
static void GLLOADER_APIENTRY self_init_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	LOAD_FUNC1(glViewport);
	glViewport(x, y, width, height);
}

glActiveTextureFUNC glActiveTexture = self_init_glActiveTexture;
glAlphaFuncFUNC glAlphaFunc = self_init_glAlphaFunc;
glAlphaFuncxFUNC glAlphaFuncx = self_init_glAlphaFuncx;
glBindTextureFUNC glBindTexture = self_init_glBindTexture;
glBlendFuncFUNC glBlendFunc = self_init_glBlendFunc;
glClearFUNC glClear = self_init_glClear;
glClearColorFUNC glClearColor = self_init_glClearColor;
glClearColorxFUNC glClearColorx = self_init_glClearColorx;
glClearDepthfFUNC glClearDepthf = self_init_glClearDepthf;
glClearDepthxFUNC glClearDepthx = self_init_glClearDepthx;
glClearStencilFUNC glClearStencil = self_init_glClearStencil;
glClientActiveTextureFUNC glClientActiveTexture = self_init_glClientActiveTexture;
glColor4fFUNC glColor4f = self_init_glColor4f;
glColor4xFUNC glColor4x = self_init_glColor4x;
glColorMaskFUNC glColorMask = self_init_glColorMask;
glColorPointerFUNC glColorPointer = self_init_glColorPointer;
glCompressedTexImage2DFUNC glCompressedTexImage2D = self_init_glCompressedTexImage2D;
glCompressedTexSubImage2DFUNC glCompressedTexSubImage2D = self_init_glCompressedTexSubImage2D;
glCopyTexImage2DFUNC glCopyTexImage2D = self_init_glCopyTexImage2D;
glCopyTexSubImage2DFUNC glCopyTexSubImage2D = self_init_glCopyTexSubImage2D;
glCullFaceFUNC glCullFace = self_init_glCullFace;
glDeleteTexturesFUNC glDeleteTextures = self_init_glDeleteTextures;
glDepthFuncFUNC glDepthFunc = self_init_glDepthFunc;
glDepthMaskFUNC glDepthMask = self_init_glDepthMask;
glDepthRangefFUNC glDepthRangef = self_init_glDepthRangef;
glDepthRangexFUNC glDepthRangex = self_init_glDepthRangex;
glDisableFUNC glDisable = self_init_glDisable;
glDisableClientStateFUNC glDisableClientState = self_init_glDisableClientState;
glDrawArraysFUNC glDrawArrays = self_init_glDrawArrays;
glDrawElementsFUNC glDrawElements = self_init_glDrawElements;
glEnableFUNC glEnable = self_init_glEnable;
glEnableClientStateFUNC glEnableClientState = self_init_glEnableClientState;
glFinishFUNC glFinish = self_init_glFinish;
glFlushFUNC glFlush = self_init_glFlush;
glFogfFUNC glFogf = self_init_glFogf;
glFogfvFUNC glFogfv = self_init_glFogfv;
glFogxFUNC glFogx = self_init_glFogx;
glFogxvFUNC glFogxv = self_init_glFogxv;
glFrontFaceFUNC glFrontFace = self_init_glFrontFace;
glFrustumfFUNC glFrustumf = self_init_glFrustumf;
glFrustumxFUNC glFrustumx = self_init_glFrustumx;
glGenTexturesFUNC glGenTextures = self_init_glGenTextures;
glGetErrorFUNC glGetError = self_init_glGetError;
glGetIntegervFUNC glGetIntegerv = self_init_glGetIntegerv;
glGetStringFUNC glGetString = self_init_glGetString;
glHintFUNC glHint = self_init_glHint;
glLightModelfFUNC glLightModelf = self_init_glLightModelf;
glLightModelfvFUNC glLightModelfv = self_init_glLightModelfv;
glLightModelxFUNC glLightModelx = self_init_glLightModelx;
glLightModelxvFUNC glLightModelxv = self_init_glLightModelxv;
glLightfFUNC glLightf = self_init_glLightf;
glLightfvFUNC glLightfv = self_init_glLightfv;
glLightxFUNC glLightx = self_init_glLightx;
glLightxvFUNC glLightxv = self_init_glLightxv;
glLineWidthFUNC glLineWidth = self_init_glLineWidth;
glLineWidthxFUNC glLineWidthx = self_init_glLineWidthx;
glLoadIdentityFUNC glLoadIdentity = self_init_glLoadIdentity;
glLoadMatrixfFUNC glLoadMatrixf = self_init_glLoadMatrixf;
glLoadMatrixxFUNC glLoadMatrixx = self_init_glLoadMatrixx;
glLogicOpFUNC glLogicOp = self_init_glLogicOp;
glMaterialfFUNC glMaterialf = self_init_glMaterialf;
glMaterialfvFUNC glMaterialfv = self_init_glMaterialfv;
glMaterialxFUNC glMaterialx = self_init_glMaterialx;
glMaterialxvFUNC glMaterialxv = self_init_glMaterialxv;
glMatrixModeFUNC glMatrixMode = self_init_glMatrixMode;
glMultMatrixfFUNC glMultMatrixf = self_init_glMultMatrixf;
glMultMatrixxFUNC glMultMatrixx = self_init_glMultMatrixx;
glMultiTexCoord4fFUNC glMultiTexCoord4f = self_init_glMultiTexCoord4f;
glMultiTexCoord4xFUNC glMultiTexCoord4x = self_init_glMultiTexCoord4x;
glNormal3fFUNC glNormal3f = self_init_glNormal3f;
glNormal3xFUNC glNormal3x = self_init_glNormal3x;
glNormalPointerFUNC glNormalPointer = self_init_glNormalPointer;
glOrthofFUNC glOrthof = self_init_glOrthof;
glOrthoxFUNC glOrthox = self_init_glOrthox;
glPixelStoreiFUNC glPixelStorei = self_init_glPixelStorei;
glPointSizeFUNC glPointSize = self_init_glPointSize;
glPointSizexFUNC glPointSizex = self_init_glPointSizex;
glPolygonOffsetFUNC glPolygonOffset = self_init_glPolygonOffset;
glPolygonOffsetxFUNC glPolygonOffsetx = self_init_glPolygonOffsetx;
glPopMatrixFUNC glPopMatrix = self_init_glPopMatrix;
glPushMatrixFUNC glPushMatrix = self_init_glPushMatrix;
glReadPixelsFUNC glReadPixels = self_init_glReadPixels;
glRotatefFUNC glRotatef = self_init_glRotatef;
glRotatexFUNC glRotatex = self_init_glRotatex;
glSampleCoverageFUNC glSampleCoverage = self_init_glSampleCoverage;
glSampleCoveragexFUNC glSampleCoveragex = self_init_glSampleCoveragex;
glScalefFUNC glScalef = self_init_glScalef;
glScalexFUNC glScalex = self_init_glScalex;
glScissorFUNC glScissor = self_init_glScissor;
glShadeModelFUNC glShadeModel = self_init_glShadeModel;
glStencilFuncFUNC glStencilFunc = self_init_glStencilFunc;
glStencilMaskFUNC glStencilMask = self_init_glStencilMask;
glStencilOpFUNC glStencilOp = self_init_glStencilOp;
glTexCoordPointerFUNC glTexCoordPointer = self_init_glTexCoordPointer;
glTexEnvfFUNC glTexEnvf = self_init_glTexEnvf;
glTexEnvfvFUNC glTexEnvfv = self_init_glTexEnvfv;
glTexEnvxFUNC glTexEnvx = self_init_glTexEnvx;
glTexEnvxvFUNC glTexEnvxv = self_init_glTexEnvxv;
glTexImage2DFUNC glTexImage2D = self_init_glTexImage2D;
glTexParameterfFUNC glTexParameterf = self_init_glTexParameterf;
glTexParameterxFUNC glTexParameterx = self_init_glTexParameterx;
glTexSubImage2DFUNC glTexSubImage2D = self_init_glTexSubImage2D;
glTranslatefFUNC glTranslatef = self_init_glTranslatef;
glVertexPointerFUNC glVertexPointer = self_init_glVertexPointer;
glTranslatexFUNC glTranslatex = self_init_glTranslatex;
glViewportFUNC glViewport = self_init_glViewport;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_VERSION_1_1()
{
	return _GLES_VERSION_1_1;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_VERSION_1_1()
{
	glloader_init();
	return glloader_GLES_VERSION_1_1();
}
glloader_GLES_VERSION_1_1FUNC glloader_GLES_VERSION_1_1 = self_init_glloader_GLES_VERSION_1_1;

#ifdef GLES_VERSION_1_1

static void GLLOADER_APIENTRY self_init_glClipPlanef(GLenum plane, const GLfloat* equation)
{
	LOAD_FUNC1(glClipPlanef);
	glClipPlanef(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glGetClipPlanef(GLenum plane, GLfloat* equation)
{
	LOAD_FUNC1(glGetClipPlanef);
	glGetClipPlanef(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glGetFloatv(GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glGetFloatv);
	glGetFloatv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetLightfv(GLenum light, GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glGetLightfv);
	glGetLightfv(light, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetMaterialfv(GLenum face, GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glGetMaterialfv);
	glGetMaterialfv(face, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexEnvfv(GLenum target, GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glGetTexEnvfv);
	glGetTexEnvfv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glGetTexParameterfv);
	glGetTexParameterfv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glPointParameterf(GLenum pname, GLfloat param)
{
	LOAD_FUNC1(glPointParameterf);
	glPointParameterf(pname, param);
}
static void GLLOADER_APIENTRY self_init_glPointParameterfv(GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glPointParameterfv);
	glPointParameterfv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
	LOAD_FUNC1(glTexParameterfv);
	glTexParameterfv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glBindBuffer(GLenum target, GLuint buffer)
{
	LOAD_FUNC1(glBindBuffer);
	glBindBuffer(target, buffer);
}
static void GLLOADER_APIENTRY self_init_glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
	LOAD_FUNC1(glBufferData);
	glBufferData(target, size, data, usage);
}
static void GLLOADER_APIENTRY self_init_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
	LOAD_FUNC1(glBufferSubData);
	glBufferSubData(target, offset, size, data);
}
static void GLLOADER_APIENTRY self_init_glClipPlanex(GLenum plane, const GLfixed* equation)
{
	LOAD_FUNC1(glClipPlanex);
	glClipPlanex(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	LOAD_FUNC1(glColor4ub);
	glColor4ub(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
	LOAD_FUNC1(glDeleteBuffers);
	glDeleteBuffers(n, buffers);
}
static void GLLOADER_APIENTRY self_init_glGetBooleanv(GLenum pname, GLboolean* params)
{
	LOAD_FUNC1(glGetBooleanv);
	glGetBooleanv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetBufferParameteriv);
	glGetBufferParameteriv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetClipPlanex(GLenum plane, GLfixed* equation)
{
	LOAD_FUNC1(glGetClipPlanex);
	glGetClipPlanex(plane, equation);
}
static void GLLOADER_APIENTRY self_init_glGenBuffers(GLsizei n, GLuint* buffers)
{
	LOAD_FUNC1(glGenBuffers);
	glGenBuffers(n, buffers);
}
static void GLLOADER_APIENTRY self_init_glGetFixedv(GLenum pname, GLfixed* params)
{
	LOAD_FUNC1(glGetFixedv);
	glGetFixedv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetLightxv(GLenum light, GLenum pname, GLfixed* params)
{
	LOAD_FUNC1(glGetLightxv);
	glGetLightxv(light, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetMaterialxv(GLenum face, GLenum pname, GLfixed* params)
{
	LOAD_FUNC1(glGetMaterialxv);
	glGetMaterialxv(face, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetPointerv(GLenum pname, GLvoid ** params)
{
	LOAD_FUNC1(glGetPointerv);
	glGetPointerv(pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexEnviv(GLenum target, GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetTexEnviv);
	glGetTexEnviv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexEnvxv(GLenum target, GLenum pname, GLfixed* params)
{
	LOAD_FUNC1(glGetTexEnvxv);
	glGetTexEnvxv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetTexParameteriv);
	glGetTexParameteriv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetTexParameterxv(GLenum target, GLenum pname, GLfixed* params)
{
	LOAD_FUNC1(glGetTexParameterxv);
	glGetTexParameterxv(target, pname, params);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsBuffer(GLuint buffer)
{
	LOAD_FUNC1(glIsBuffer);
	return glIsBuffer(buffer);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsEnabled(GLenum cap)
{
	LOAD_FUNC1(glIsEnabled);
	return glIsEnabled(cap);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsTexture(GLuint texture)
{
	LOAD_FUNC1(glIsTexture);
	return glIsTexture(texture);
}
static void GLLOADER_APIENTRY self_init_glTexEnvi(GLenum target, GLenum pname, GLint param)
{
	LOAD_FUNC1(glTexEnvi);
	glTexEnvi(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexEnviv(GLenum target, GLenum pname, const GLint* params)
{
	LOAD_FUNC1(glTexEnviv);
	glTexEnviv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
	LOAD_FUNC1(glTexParameteri);
	glTexParameteri(target, pname, param);
}
static void GLLOADER_APIENTRY self_init_glTexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
	LOAD_FUNC1(glTexParameteriv);
	glTexParameteriv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glTexParameterxv(GLenum target, GLenum pname, const GLfixed* params)
{
	LOAD_FUNC1(glTexParameterxv);
	glTexParameterxv(target, pname, params);
}

glClipPlanefFUNC glClipPlanef = self_init_glClipPlanef;
glGetClipPlanefFUNC glGetClipPlanef = self_init_glGetClipPlanef;
glGetFloatvFUNC glGetFloatv = self_init_glGetFloatv;
glGetLightfvFUNC glGetLightfv = self_init_glGetLightfv;
glGetMaterialfvFUNC glGetMaterialfv = self_init_glGetMaterialfv;
glGetTexEnvfvFUNC glGetTexEnvfv = self_init_glGetTexEnvfv;
glGetTexParameterfvFUNC glGetTexParameterfv = self_init_glGetTexParameterfv;
glPointParameterfFUNC glPointParameterf = self_init_glPointParameterf;
glPointParameterfvFUNC glPointParameterfv = self_init_glPointParameterfv;
glTexParameterfvFUNC glTexParameterfv = self_init_glTexParameterfv;
glBindBufferFUNC glBindBuffer = self_init_glBindBuffer;
glBufferDataFUNC glBufferData = self_init_glBufferData;
glBufferSubDataFUNC glBufferSubData = self_init_glBufferSubData;
glClipPlanexFUNC glClipPlanex = self_init_glClipPlanex;
glColor4ubFUNC glColor4ub = self_init_glColor4ub;
glDeleteBuffersFUNC glDeleteBuffers = self_init_glDeleteBuffers;
glGetBooleanvFUNC glGetBooleanv = self_init_glGetBooleanv;
glGetBufferParameterivFUNC glGetBufferParameteriv = self_init_glGetBufferParameteriv;
glGetClipPlanexFUNC glGetClipPlanex = self_init_glGetClipPlanex;
glGenBuffersFUNC glGenBuffers = self_init_glGenBuffers;
glGetFixedvFUNC glGetFixedv = self_init_glGetFixedv;
glGetLightxvFUNC glGetLightxv = self_init_glGetLightxv;
glGetMaterialxvFUNC glGetMaterialxv = self_init_glGetMaterialxv;
glGetPointervFUNC glGetPointerv = self_init_glGetPointerv;
glGetTexEnvivFUNC glGetTexEnviv = self_init_glGetTexEnviv;
glGetTexEnvxvFUNC glGetTexEnvxv = self_init_glGetTexEnvxv;
glGetTexParameterivFUNC glGetTexParameteriv = self_init_glGetTexParameteriv;
glGetTexParameterxvFUNC glGetTexParameterxv = self_init_glGetTexParameterxv;
glIsBufferFUNC glIsBuffer = self_init_glIsBuffer;
glIsEnabledFUNC glIsEnabled = self_init_glIsEnabled;
glIsTextureFUNC glIsTexture = self_init_glIsTexture;
glTexEnviFUNC glTexEnvi = self_init_glTexEnvi;
glTexEnvivFUNC glTexEnviv = self_init_glTexEnviv;
glTexParameteriFUNC glTexParameteri = self_init_glTexParameteri;
glTexParameterivFUNC glTexParameteriv = self_init_glTexParameteriv;
glTexParameterxvFUNC glTexParameterxv = self_init_glTexParameterxv;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_VERSION_2_0()
{
	return _GLES_VERSION_2_0;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_VERSION_2_0()
{
	glloader_init();
	return glloader_GLES_VERSION_2_0();
}
glloader_GLES_VERSION_2_0FUNC glloader_GLES_VERSION_2_0 = self_init_glloader_GLES_VERSION_2_0;

#ifdef GLES_VERSION_2_0

static void GLLOADER_APIENTRY self_init_glAttachShader(GLuint program, GLuint shader)
{
	LOAD_FUNC1(glAttachShader);
	glAttachShader(program, shader);
}
static void GLLOADER_APIENTRY self_init_glBindAttribLocation(GLuint program, GLuint index, const GLchar* name)
{
	LOAD_FUNC1(glBindAttribLocation);
	glBindAttribLocation(program, index, name);
}
static void GLLOADER_APIENTRY self_init_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
	LOAD_FUNC1(glBindFramebuffer);
	glBindFramebuffer(target, framebuffer);
}
static void GLLOADER_APIENTRY self_init_glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	LOAD_FUNC1(glBindRenderbuffer);
	glBindRenderbuffer(target, renderbuffer);
}
static void GLLOADER_APIENTRY self_init_glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	LOAD_FUNC1(glBlendColor);
	glBlendColor(red, green, blue, alpha);
}
static void GLLOADER_APIENTRY self_init_glBlendEquation(GLenum mode)
{
	LOAD_FUNC1(glBlendEquation);
	glBlendEquation(mode);
}
static void GLLOADER_APIENTRY self_init_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	LOAD_FUNC1(glBlendEquationSeparate);
	glBlendEquationSeparate(modeRGB, modeAlpha);
}
static void GLLOADER_APIENTRY self_init_glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
	LOAD_FUNC1(glBlendFuncSeparate);
	glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}
static GLenum GLLOADER_APIENTRY self_init_glCheckFramebufferStatus(GLenum target)
{
	LOAD_FUNC1(glCheckFramebufferStatus);
	return glCheckFramebufferStatus(target);
}
static void GLLOADER_APIENTRY self_init_glCompileShader(GLuint shader)
{
	LOAD_FUNC1(glCompileShader);
	glCompileShader(shader);
}
static GLuint GLLOADER_APIENTRY self_init_glCreateProgram()
{
	LOAD_FUNC1(glCreateProgram);
	return glCreateProgram();
}
static GLuint GLLOADER_APIENTRY self_init_glCreateShader(GLenum type)
{
	LOAD_FUNC1(glCreateShader);
	return glCreateShader(type);
}
static void GLLOADER_APIENTRY self_init_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
	LOAD_FUNC1(glDeleteFramebuffers);
	glDeleteFramebuffers(n, framebuffers);
}
static void GLLOADER_APIENTRY self_init_glDeleteProgram(GLuint program)
{
	LOAD_FUNC1(glDeleteProgram);
	glDeleteProgram(program);
}
static void GLLOADER_APIENTRY self_init_glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
	LOAD_FUNC1(glDeleteRenderbuffers);
	glDeleteRenderbuffers(n, renderbuffers);
}
static void GLLOADER_APIENTRY self_init_glDeleteShader(GLuint shader)
{
	LOAD_FUNC1(glDeleteShader);
	glDeleteShader(shader);
}
static void GLLOADER_APIENTRY self_init_glDetachShader(GLuint program, GLuint shader)
{
	LOAD_FUNC1(glDetachShader);
	glDetachShader(program, shader);
}
static void GLLOADER_APIENTRY self_init_glDisableVertexAttribArray(GLuint index)
{
	LOAD_FUNC1(glDisableVertexAttribArray);
	glDisableVertexAttribArray(index);
}
static void GLLOADER_APIENTRY self_init_glEnableVertexAttribArray(GLuint index)
{
	LOAD_FUNC1(glEnableVertexAttribArray);
	glEnableVertexAttribArray(index);
}
static void GLLOADER_APIENTRY self_init_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	LOAD_FUNC1(glFramebufferRenderbuffer);
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}
static void GLLOADER_APIENTRY self_init_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	LOAD_FUNC1(glFramebufferTexture2D);
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
}
static void GLLOADER_APIENTRY self_init_glGenerateMipmap(GLenum target)
{
	LOAD_FUNC1(glGenerateMipmap);
	glGenerateMipmap(target);
}
static void GLLOADER_APIENTRY self_init_glGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
	LOAD_FUNC1(glGenFramebuffers);
	glGenFramebuffers(n, framebuffers);
}
static void GLLOADER_APIENTRY self_init_glGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
	LOAD_FUNC1(glGenRenderbuffers);
	glGenRenderbuffers(n, renderbuffers);
}
static void GLLOADER_APIENTRY self_init_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
	LOAD_FUNC1(glGetActiveAttrib);
	glGetActiveAttrib(program, index, bufSize, length, size, type, name);
}
static void GLLOADER_APIENTRY self_init_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
	LOAD_FUNC1(glGetActiveUniform);
	glGetActiveUniform(program, index, bufSize, length, size, type, name);
}
static void GLLOADER_APIENTRY self_init_glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shader)
{
	LOAD_FUNC1(glGetAttachedShaders);
	glGetAttachedShaders(program, maxCount, count, shader);
}
static GLint GLLOADER_APIENTRY self_init_glGetAttribLocation(GLuint program, const GLchar* name)
{
	LOAD_FUNC1(glGetAttribLocation);
	return glGetAttribLocation(program, name);
}
static void GLLOADER_APIENTRY self_init_glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetFramebufferAttachmentParameteriv);
	glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetProgramiv(GLuint program, GLenum pname, const GLint* params)
{
	LOAD_FUNC1(glGetProgramiv);
	glGetProgramiv(program, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	LOAD_FUNC1(glGetProgramInfoLog);
	glGetProgramInfoLog(program, bufSize, length, infoLog);
}
static void GLLOADER_APIENTRY self_init_glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetRenderbufferParameteriv);
	glGetRenderbufferParameteriv(target, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetShaderiv(GLuint shader, GLenum pname, const GLint* params)
{
	LOAD_FUNC1(glGetShaderiv);
	glGetShaderiv(shader, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
	LOAD_FUNC1(glGetShaderInfoLog);
	glGetShaderInfoLog(shader, bufSize, length, infoLog);
}
static void GLLOADER_APIENTRY self_init_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
	LOAD_FUNC1(glGetShaderPrecisionFormat);
	glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}
static void GLLOADER_APIENTRY self_init_glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source)
{
	LOAD_FUNC1(glGetShaderSource);
	glGetShaderSource(shader, bufSize, length, source);
}
static void GLLOADER_APIENTRY self_init_glGetUniformfv(GLuint program, GLint location, GLfloat* params)
{
	LOAD_FUNC1(glGetUniformfv);
	glGetUniformfv(program, location, params);
}
static void GLLOADER_APIENTRY self_init_glGetUniformiv(GLuint program, GLint location, GLint* params)
{
	LOAD_FUNC1(glGetUniformiv);
	glGetUniformiv(program, location, params);
}
static GLint GLLOADER_APIENTRY self_init_glGetUniformLocation(GLuint program, const GLchar* name)
{
	LOAD_FUNC1(glGetUniformLocation);
	return glGetUniformLocation(program, name);
}
static void GLLOADER_APIENTRY self_init_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
	LOAD_FUNC1(glGetVertexAttribfv);
	glGetVertexAttribfv(index, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
	LOAD_FUNC1(glGetVertexAttribiv);
	glGetVertexAttribiv(index, pname, params);
}
static void GLLOADER_APIENTRY self_init_glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid** pointer)
{
	LOAD_FUNC1(glGetVertexAttribPointerv);
	glGetVertexAttribPointerv(index, pname, pointer);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsFramebuffer(GLuint framebuffer)
{
	LOAD_FUNC1(glIsFramebuffer);
	return glIsFramebuffer(framebuffer);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsProgram(GLuint program)
{
	LOAD_FUNC1(glIsProgram);
	return glIsProgram(program);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsRenderbuffer(GLuint renderbuffer)
{
	LOAD_FUNC1(glIsRenderbuffer);
	return glIsRenderbuffer(renderbuffer);
}
static GLboolean GLLOADER_APIENTRY self_init_glIsShader(GLuint shader)
{
	LOAD_FUNC1(glIsShader);
	return glIsShader(shader);
}
static void GLLOADER_APIENTRY self_init_glLinkProgram(GLuint program)
{
	LOAD_FUNC1(glLinkProgram);
	glLinkProgram(program);
}
static void GLLOADER_APIENTRY self_init_glReleaseShaderCompiler()
{
	LOAD_FUNC1(glReleaseShaderCompiler);
	glReleaseShaderCompiler();
}
static void GLLOADER_APIENTRY self_init_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
	LOAD_FUNC1(glRenderbufferStorage);
	glRenderbufferStorage(target, internalformat, width, height);
}
static void GLLOADER_APIENTRY self_init_glShaderBinary(GLsizei n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length)
{
	LOAD_FUNC1(glShaderBinary);
	glShaderBinary(n, shaders, binaryformat, binary, length);
}
static void GLLOADER_APIENTRY self_init_glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length)
{
	LOAD_FUNC1(glShaderSource);
	glShaderSource(shader, count, string, length);
}
static void GLLOADER_APIENTRY self_init_glStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)
{
	LOAD_FUNC1(glStencilFuncSeparate);
	glStencilFuncSeparate(frontfunc, backfunc, ref, mask);
}
static void GLLOADER_APIENTRY self_init_glStencilMaskSeparate(GLenum face, GLuint mask)
{
	LOAD_FUNC1(glStencilMaskSeparate);
	glStencilMaskSeparate(face, mask);
}
static void GLLOADER_APIENTRY self_init_glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
	LOAD_FUNC1(glStencilOpSeparate);
	glStencilOpSeparate(face, sfail, dpfail, dppass);
}
static void GLLOADER_APIENTRY self_init_glUniform1f(GLint location, GLfloat v0)
{
	LOAD_FUNC1(glUniform1f);
	glUniform1f(location, v0);
}
static void GLLOADER_APIENTRY self_init_glUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
	LOAD_FUNC1(glUniform2f);
	glUniform2f(location, v0, v1);
}
static void GLLOADER_APIENTRY self_init_glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	LOAD_FUNC1(glUniform3f);
	glUniform3f(location, v0, v1, v2);
}
static void GLLOADER_APIENTRY self_init_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	LOAD_FUNC1(glUniform4f);
	glUniform4f(location, v0, v1, v2, v3);
}
static void GLLOADER_APIENTRY self_init_glUniform1i(GLint location, GLint v0)
{
	LOAD_FUNC1(glUniform1i);
	glUniform1i(location, v0);
}
static void GLLOADER_APIENTRY self_init_glUniform2i(GLint location, GLint v0, GLint v1)
{
	LOAD_FUNC1(glUniform2i);
	glUniform2i(location, v0, v1);
}
static void GLLOADER_APIENTRY self_init_glUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
	LOAD_FUNC1(glUniform3i);
	glUniform3i(location, v0, v1, v2);
}
static void GLLOADER_APIENTRY self_init_glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	LOAD_FUNC1(glUniform4i);
	glUniform4i(location, v0, v1, v2, v3);
}
static void GLLOADER_APIENTRY self_init_glUniform1fv(GLint location, GLsizei count, const GLfloat* value)
{
	LOAD_FUNC1(glUniform1fv);
	glUniform1fv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform2fv(GLint location, GLsizei count, const GLfloat* value)
{
	LOAD_FUNC1(glUniform2fv);
	glUniform2fv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
	LOAD_FUNC1(glUniform3fv);
	glUniform3fv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
	LOAD_FUNC1(glUniform4fv);
	glUniform4fv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform1iv(GLint location, GLsizei count, const GLint* value)
{
	LOAD_FUNC1(glUniform1iv);
	glUniform1iv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform2iv(GLint location, GLsizei count, const GLint* value)
{
	LOAD_FUNC1(glUniform2iv);
	glUniform2iv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform3iv(GLint location, GLsizei count, const GLint* value)
{
	LOAD_FUNC1(glUniform3iv);
	glUniform3iv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniform4iv(GLint location, GLsizei count, const GLint* value)
{
	LOAD_FUNC1(glUniform4iv);
	glUniform4iv(location, count, value);
}
static void GLLOADER_APIENTRY self_init_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	LOAD_FUNC1(glUniformMatrix2fv);
	glUniformMatrix2fv(location, count, transpose, value);
}
static void GLLOADER_APIENTRY self_init_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	LOAD_FUNC1(glUniformMatrix3fv);
	glUniformMatrix3fv(location, count, transpose, value);
}
static void GLLOADER_APIENTRY self_init_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	LOAD_FUNC1(glUniformMatrix4fv);
	glUniformMatrix4fv(location, count, transpose, value);
}
static void GLLOADER_APIENTRY self_init_glUseProgram(GLuint program)
{
	LOAD_FUNC1(glUseProgram);
	glUseProgram(program);
}
static void GLLOADER_APIENTRY self_init_glValidateProgram(GLuint program)
{
	LOAD_FUNC1(glValidateProgram);
	glValidateProgram(program);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib1f(GLuint index, GLfloat x)
{
	LOAD_FUNC1(glVertexAttrib1f);
	glVertexAttrib1f(index, x);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib1fv(GLuint index, const GLfloat* v)
{
	LOAD_FUNC1(glVertexAttrib1fv);
	glVertexAttrib1fv(index, v);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
	LOAD_FUNC1(glVertexAttrib2f);
	glVertexAttrib2f(index, x, y);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib2fv(GLuint index, const GLfloat* v)
{
	LOAD_FUNC1(glVertexAttrib2fv);
	glVertexAttrib2fv(index, v);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
	LOAD_FUNC1(glVertexAttrib3f);
	glVertexAttrib3f(index, x, y, z);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib3fv(GLuint index, const GLfloat* v)
{
	LOAD_FUNC1(glVertexAttrib3fv);
	glVertexAttrib3fv(index, v);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	LOAD_FUNC1(glVertexAttrib4f);
	glVertexAttrib4f(index, x, y, z, w);
}
static void GLLOADER_APIENTRY self_init_glVertexAttrib4fv(GLuint index, const GLfloat* v)
{
	LOAD_FUNC1(glVertexAttrib4fv);
	glVertexAttrib4fv(index, v);
}
static void GLLOADER_APIENTRY self_init_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	LOAD_FUNC1(glVertexAttribPointer);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

glAttachShaderFUNC glAttachShader = self_init_glAttachShader;
glBindAttribLocationFUNC glBindAttribLocation = self_init_glBindAttribLocation;
glBindFramebufferFUNC glBindFramebuffer = self_init_glBindFramebuffer;
glBindRenderbufferFUNC glBindRenderbuffer = self_init_glBindRenderbuffer;
glBlendColorFUNC glBlendColor = self_init_glBlendColor;
glBlendEquationFUNC glBlendEquation = self_init_glBlendEquation;
glBlendEquationSeparateFUNC glBlendEquationSeparate = self_init_glBlendEquationSeparate;
glBlendFuncSeparateFUNC glBlendFuncSeparate = self_init_glBlendFuncSeparate;
glCheckFramebufferStatusFUNC glCheckFramebufferStatus = self_init_glCheckFramebufferStatus;
glCompileShaderFUNC glCompileShader = self_init_glCompileShader;
glCreateProgramFUNC glCreateProgram = self_init_glCreateProgram;
glCreateShaderFUNC glCreateShader = self_init_glCreateShader;
glDeleteFramebuffersFUNC glDeleteFramebuffers = self_init_glDeleteFramebuffers;
glDeleteProgramFUNC glDeleteProgram = self_init_glDeleteProgram;
glDeleteRenderbuffersFUNC glDeleteRenderbuffers = self_init_glDeleteRenderbuffers;
glDeleteShaderFUNC glDeleteShader = self_init_glDeleteShader;
glDetachShaderFUNC glDetachShader = self_init_glDetachShader;
glDisableVertexAttribArrayFUNC glDisableVertexAttribArray = self_init_glDisableVertexAttribArray;
glEnableVertexAttribArrayFUNC glEnableVertexAttribArray = self_init_glEnableVertexAttribArray;
glFramebufferRenderbufferFUNC glFramebufferRenderbuffer = self_init_glFramebufferRenderbuffer;
glFramebufferTexture2DFUNC glFramebufferTexture2D = self_init_glFramebufferTexture2D;
glGenerateMipmapFUNC glGenerateMipmap = self_init_glGenerateMipmap;
glGenFramebuffersFUNC glGenFramebuffers = self_init_glGenFramebuffers;
glGenRenderbuffersFUNC glGenRenderbuffers = self_init_glGenRenderbuffers;
glGetActiveAttribFUNC glGetActiveAttrib = self_init_glGetActiveAttrib;
glGetActiveUniformFUNC glGetActiveUniform = self_init_glGetActiveUniform;
glGetAttachedShadersFUNC glGetAttachedShaders = self_init_glGetAttachedShaders;
glGetAttribLocationFUNC glGetAttribLocation = self_init_glGetAttribLocation;
glGetFramebufferAttachmentParameterivFUNC glGetFramebufferAttachmentParameteriv = self_init_glGetFramebufferAttachmentParameteriv;
glGetProgramivFUNC glGetProgramiv = self_init_glGetProgramiv;
glGetProgramInfoLogFUNC glGetProgramInfoLog = self_init_glGetProgramInfoLog;
glGetRenderbufferParameterivFUNC glGetRenderbufferParameteriv = self_init_glGetRenderbufferParameteriv;
glGetShaderivFUNC glGetShaderiv = self_init_glGetShaderiv;
glGetShaderInfoLogFUNC glGetShaderInfoLog = self_init_glGetShaderInfoLog;
glGetShaderPrecisionFormatFUNC glGetShaderPrecisionFormat = self_init_glGetShaderPrecisionFormat;
glGetShaderSourceFUNC glGetShaderSource = self_init_glGetShaderSource;
glGetUniformfvFUNC glGetUniformfv = self_init_glGetUniformfv;
glGetUniformivFUNC glGetUniformiv = self_init_glGetUniformiv;
glGetUniformLocationFUNC glGetUniformLocation = self_init_glGetUniformLocation;
glGetVertexAttribfvFUNC glGetVertexAttribfv = self_init_glGetVertexAttribfv;
glGetVertexAttribivFUNC glGetVertexAttribiv = self_init_glGetVertexAttribiv;
glGetVertexAttribPointervFUNC glGetVertexAttribPointerv = self_init_glGetVertexAttribPointerv;
glIsFramebufferFUNC glIsFramebuffer = self_init_glIsFramebuffer;
glIsProgramFUNC glIsProgram = self_init_glIsProgram;
glIsRenderbufferFUNC glIsRenderbuffer = self_init_glIsRenderbuffer;
glIsShaderFUNC glIsShader = self_init_glIsShader;
glLinkProgramFUNC glLinkProgram = self_init_glLinkProgram;
glReleaseShaderCompilerFUNC glReleaseShaderCompiler = self_init_glReleaseShaderCompiler;
glRenderbufferStorageFUNC glRenderbufferStorage = self_init_glRenderbufferStorage;
glShaderBinaryFUNC glShaderBinary = self_init_glShaderBinary;
glShaderSourceFUNC glShaderSource = self_init_glShaderSource;
glStencilFuncSeparateFUNC glStencilFuncSeparate = self_init_glStencilFuncSeparate;
glStencilMaskSeparateFUNC glStencilMaskSeparate = self_init_glStencilMaskSeparate;
glStencilOpSeparateFUNC glStencilOpSeparate = self_init_glStencilOpSeparate;
glUniform1fFUNC glUniform1f = self_init_glUniform1f;
glUniform2fFUNC glUniform2f = self_init_glUniform2f;
glUniform3fFUNC glUniform3f = self_init_glUniform3f;
glUniform4fFUNC glUniform4f = self_init_glUniform4f;
glUniform1iFUNC glUniform1i = self_init_glUniform1i;
glUniform2iFUNC glUniform2i = self_init_glUniform2i;
glUniform3iFUNC glUniform3i = self_init_glUniform3i;
glUniform4iFUNC glUniform4i = self_init_glUniform4i;
glUniform1fvFUNC glUniform1fv = self_init_glUniform1fv;
glUniform2fvFUNC glUniform2fv = self_init_glUniform2fv;
glUniform3fvFUNC glUniform3fv = self_init_glUniform3fv;
glUniform4fvFUNC glUniform4fv = self_init_glUniform4fv;
glUniform1ivFUNC glUniform1iv = self_init_glUniform1iv;
glUniform2ivFUNC glUniform2iv = self_init_glUniform2iv;
glUniform3ivFUNC glUniform3iv = self_init_glUniform3iv;
glUniform4ivFUNC glUniform4iv = self_init_glUniform4iv;
glUniformMatrix2fvFUNC glUniformMatrix2fv = self_init_glUniformMatrix2fv;
glUniformMatrix3fvFUNC glUniformMatrix3fv = self_init_glUniformMatrix3fv;
glUniformMatrix4fvFUNC glUniformMatrix4fv = self_init_glUniformMatrix4fv;
glUseProgramFUNC glUseProgram = self_init_glUseProgram;
glValidateProgramFUNC glValidateProgram = self_init_glValidateProgram;
glVertexAttrib1fFUNC glVertexAttrib1f = self_init_glVertexAttrib1f;
glVertexAttrib1fvFUNC glVertexAttrib1fv = self_init_glVertexAttrib1fv;
glVertexAttrib2fFUNC glVertexAttrib2f = self_init_glVertexAttrib2f;
glVertexAttrib2fvFUNC glVertexAttrib2fv = self_init_glVertexAttrib2fv;
glVertexAttrib3fFUNC glVertexAttrib3f = self_init_glVertexAttrib3f;
glVertexAttrib3fvFUNC glVertexAttrib3fv = self_init_glVertexAttrib3fv;
glVertexAttrib4fFUNC glVertexAttrib4f = self_init_glVertexAttrib4f;
glVertexAttrib4fvFUNC glVertexAttrib4fv = self_init_glVertexAttrib4fv;
glVertexAttribPointerFUNC glVertexAttribPointer = self_init_glVertexAttribPointer;

#endif

static char GLLOADER_APIENTRY _glloader_GLES_VIV_shader_binary()
{
	return _GLES_VIV_shader_binary;
}

static char GLLOADER_APIENTRY self_init_glloader_GLES_VIV_shader_binary()
{
	glloader_init();
	return glloader_GLES_VIV_shader_binary();
}
glloader_GLES_VIV_shader_binaryFUNC glloader_GLES_VIV_shader_binary = self_init_glloader_GLES_VIV_shader_binary;

void init_GLES_AMD_compressed_3DC_texture()
{
	glloader_GLES_AMD_compressed_3DC_texture = _glloader_GLES_AMD_compressed_3DC_texture;

	_GLES_AMD_compressed_3DC_texture = 0;
	if (glloader_is_supported("GLES_AMD_compressed_3DC_texture"))
	{
		_GLES_AMD_compressed_3DC_texture = 1;
	}
}

void init_GLES_AMD_compressed_ATC_texture()
{
	glloader_GLES_AMD_compressed_ATC_texture = _glloader_GLES_AMD_compressed_ATC_texture;

	_GLES_AMD_compressed_ATC_texture = 0;
	if (glloader_is_supported("GLES_AMD_compressed_ATC_texture"))
	{
		_GLES_AMD_compressed_ATC_texture = 1;
	}
}

void init_GLES_AMD_performance_monitor()
{
	glloader_GLES_AMD_performance_monitor = _glloader_GLES_AMD_performance_monitor;

	{
		glGetPerfMonitorGroupsAMD = NULL;
		glGetPerfMonitorCountersAMD = NULL;
		glGetPerfMonitorGroupStringAMD = NULL;
		glGetPerfMonitorCounterStringAMD = NULL;
		glGetPerfMonitorCounterInfoAMD = NULL;
		glGenPerfMonitorsAMD = NULL;
		glDeletePerfMonitorsAMD = NULL;
		glSelectPerfMonitorCountersAMD = NULL;
		glBeginPerfMonitorAMD = NULL;
		glEndPerfMonitorAMD = NULL;
		glGetPerfMonitorCounterDataAMD = NULL;
	}

	_GLES_AMD_performance_monitor = 0;
	if (glloader_is_supported("GLES_AMD_performance_monitor"))
	{
		_GLES_AMD_performance_monitor = 1;

		LOAD_FUNC1(glGetPerfMonitorGroupsAMD);
		LOAD_FUNC1(glGetPerfMonitorCountersAMD);
		LOAD_FUNC1(glGetPerfMonitorGroupStringAMD);
		LOAD_FUNC1(glGetPerfMonitorCounterStringAMD);
		LOAD_FUNC1(glGetPerfMonitorCounterInfoAMD);
		LOAD_FUNC1(glGenPerfMonitorsAMD);
		LOAD_FUNC1(glDeletePerfMonitorsAMD);
		LOAD_FUNC1(glSelectPerfMonitorCountersAMD);
		LOAD_FUNC1(glBeginPerfMonitorAMD);
		LOAD_FUNC1(glEndPerfMonitorAMD);
		LOAD_FUNC1(glGetPerfMonitorCounterDataAMD);
	}
}

void init_GLES_AMD_program_binary_Z400()
{
	glloader_GLES_AMD_program_binary_Z400 = _glloader_GLES_AMD_program_binary_Z400;

	_GLES_AMD_program_binary_Z400 = 0;
	if (glloader_is_supported("GLES_AMD_program_binary_Z400"))
	{
		_GLES_AMD_program_binary_Z400 = 1;
	}
}

void init_GLES_ANGLE_framebuffer_blit()
{
	glloader_GLES_ANGLE_framebuffer_blit = _glloader_GLES_ANGLE_framebuffer_blit;

	{
		glBlitFramebufferANGLE = NULL;
	}

	_GLES_ANGLE_framebuffer_blit = 0;
	if (glloader_is_supported("GLES_ANGLE_framebuffer_blit"))
	{
		_GLES_ANGLE_framebuffer_blit = 1;

		LOAD_FUNC1(glBlitFramebufferANGLE);
	}
}

void init_GLES_ANGLE_framebuffer_multisample()
{
	glloader_GLES_ANGLE_framebuffer_multisample = _glloader_GLES_ANGLE_framebuffer_multisample;

	{
		glRenderbufferStorageMultisampleANGLE = NULL;
	}

	_GLES_ANGLE_framebuffer_multisample = 0;
	if (glloader_is_supported("GLES_ANGLE_framebuffer_multisample"))
	{
		_GLES_ANGLE_framebuffer_multisample = 1;

		LOAD_FUNC1(glRenderbufferStorageMultisampleANGLE);
	}
}

void init_GLES_ANGLE_instanced_arrays()
{
	glloader_GLES_ANGLE_instanced_arrays = _glloader_GLES_ANGLE_instanced_arrays;

	{
		glDrawArraysInstancedANGLE = NULL;
		glDrawElementsInstancedANGLE = NULL;
		glVertexAttribDivisorANGLE = NULL;
	}

	_GLES_ANGLE_instanced_arrays = 0;
	if (glloader_is_supported("GLES_ANGLE_instanced_arrays"))
	{
		_GLES_ANGLE_instanced_arrays = 1;

		LOAD_FUNC1(glDrawArraysInstancedANGLE);
		LOAD_FUNC1(glDrawElementsInstancedANGLE);
		LOAD_FUNC1(glVertexAttribDivisorANGLE);
	}
}

void init_GLES_ANGLE_pack_reverse_row_order()
{
	glloader_GLES_ANGLE_pack_reverse_row_order = _glloader_GLES_ANGLE_pack_reverse_row_order;

	_GLES_ANGLE_pack_reverse_row_order = 0;
	if (glloader_is_supported("GLES_ANGLE_pack_reverse_row_order"))
	{
		_GLES_ANGLE_pack_reverse_row_order = 1;
	}
}

void init_GLES_ANGLE_texture_compression_dxt3()
{
	glloader_GLES_ANGLE_texture_compression_dxt3 = _glloader_GLES_ANGLE_texture_compression_dxt3;

	_GLES_ANGLE_texture_compression_dxt3 = 0;
	if (glloader_is_supported("GLES_ANGLE_texture_compression_dxt3"))
	{
		_GLES_ANGLE_texture_compression_dxt3 = 1;
	}
}

void init_GLES_ANGLE_texture_compression_dxt5()
{
	glloader_GLES_ANGLE_texture_compression_dxt5 = _glloader_GLES_ANGLE_texture_compression_dxt5;

	_GLES_ANGLE_texture_compression_dxt5 = 0;
	if (glloader_is_supported("GLES_ANGLE_texture_compression_dxt5"))
	{
		_GLES_ANGLE_texture_compression_dxt5 = 1;
	}
}

void init_GLES_ANGLE_texture_usage()
{
	glloader_GLES_ANGLE_texture_usage = _glloader_GLES_ANGLE_texture_usage;

	_GLES_ANGLE_texture_usage = 0;
	if (glloader_is_supported("GLES_ANGLE_texture_usage"))
	{
		_GLES_ANGLE_texture_usage = 1;
	}
}

void init_GLES_ANGLE_translated_shader_source()
{
	glloader_GLES_ANGLE_translated_shader_source = _glloader_GLES_ANGLE_translated_shader_source;

	{
		glGetTranslatedShaderSourceANGLE = NULL;
	}

	_GLES_ANGLE_translated_shader_source = 0;
	if (glloader_is_supported("GLES_ANGLE_translated_shader_source"))
	{
		_GLES_ANGLE_translated_shader_source = 1;

		LOAD_FUNC1(glGetTranslatedShaderSourceANGLE);
	}
}

void init_GLES_APPLE_copy_texture_levels()
{
	glloader_GLES_APPLE_copy_texture_levels = _glloader_GLES_APPLE_copy_texture_levels;

	{
		glCopyTextureLevelsAPPLE = NULL;
	}

	_GLES_APPLE_copy_texture_levels = 0;
	if (glloader_is_supported("GLES_APPLE_copy_texture_levels"))
	{
		_GLES_APPLE_copy_texture_levels = 1;

		LOAD_FUNC1(glCopyTextureLevelsAPPLE);
	}
}

void init_GLES_APPLE_framebuffer_multisample()
{
	glloader_GLES_APPLE_framebuffer_multisample = _glloader_GLES_APPLE_framebuffer_multisample;

	{
		glRenderbufferStorageMultisampleAPPLE = NULL;
	}

	_GLES_APPLE_framebuffer_multisample = 0;
	if (glloader_is_supported("GLES_APPLE_framebuffer_multisample"))
	{
		_GLES_APPLE_framebuffer_multisample = 1;

		LOAD_FUNC1(glRenderbufferStorageMultisampleAPPLE);
	}
}

void init_GLES_APPLE_rgb_422()
{
	glloader_GLES_APPLE_rgb_422 = _glloader_GLES_APPLE_rgb_422;

	_GLES_APPLE_rgb_422 = 0;
	if (glloader_is_supported("GLES_APPLE_rgb_422"))
	{
		_GLES_APPLE_rgb_422 = 1;
	}
}

void init_GLES_APPLE_sync()
{
	glloader_GLES_APPLE_sync = _glloader_GLES_APPLE_sync;

	{
		glFenceSyncAPPLE = NULL;
		glIsSyncAPPLE = NULL;
		glDeleteSyncAPPLE = NULL;
		glClientWaitSyncAPPLE = NULL;
		glWaitSyncAPPLE = NULL;
		glGetInteger64vAPPLE = NULL;
		glGetSyncivAPPLE = NULL;
	}

	_GLES_APPLE_sync = 0;
	if (glloader_is_supported("GLES_APPLE_sync"))
	{
		_GLES_APPLE_sync = 1;

		LOAD_FUNC1(glFenceSyncAPPLE);
		LOAD_FUNC1(glIsSyncAPPLE);
		LOAD_FUNC1(glDeleteSyncAPPLE);
		LOAD_FUNC1(glClientWaitSyncAPPLE);
		LOAD_FUNC1(glWaitSyncAPPLE);
		LOAD_FUNC1(glGetInteger64vAPPLE);
		LOAD_FUNC1(glGetSyncivAPPLE);
	}
}

void init_GLES_APPLE_texture_2D_limited_npot()
{
	glloader_GLES_APPLE_texture_2D_limited_npot = _glloader_GLES_APPLE_texture_2D_limited_npot;

	_GLES_APPLE_texture_2D_limited_npot = 0;
	if (glloader_is_supported("GLES_APPLE_texture_2D_limited_npot"))
	{
		_GLES_APPLE_texture_2D_limited_npot = 1;
	}
}

void init_GLES_APPLE_texture_format_BGRA8888()
{
	glloader_GLES_APPLE_texture_format_BGRA8888 = _glloader_GLES_APPLE_texture_format_BGRA8888;

	_GLES_APPLE_texture_format_BGRA8888 = 0;
	if (glloader_is_supported("GLES_APPLE_texture_format_BGRA8888"))
	{
		_GLES_APPLE_texture_format_BGRA8888 = 1;
	}
}

void init_GLES_APPLE_texture_max_level()
{
	glloader_GLES_APPLE_texture_max_level = _glloader_GLES_APPLE_texture_max_level;

	_GLES_APPLE_texture_max_level = 0;
	if (glloader_is_supported("GLES_APPLE_texture_max_level"))
	{
		_GLES_APPLE_texture_max_level = 1;
	}
}

void init_GLES_ARM_mali_shader_binary()
{
	glloader_GLES_ARM_mali_shader_binary = _glloader_GLES_ARM_mali_shader_binary;

	_GLES_ARM_mali_shader_binary = 0;
	if (glloader_is_supported("GLES_ARM_mali_shader_binary"))
	{
		_GLES_ARM_mali_shader_binary = 1;
	}
}

void init_GLES_ARM_rgba8()
{
	glloader_GLES_ARM_rgba8 = _glloader_GLES_ARM_rgba8;

	_GLES_ARM_rgba8 = 0;
	if (glloader_is_supported("GLES_ARM_rgba8"))
	{
		_GLES_ARM_rgba8 = 1;
	}
}

void init_GLES_EXT_blend_minmax()
{
	glloader_GLES_EXT_blend_minmax = _glloader_GLES_EXT_blend_minmax;

	{
		glBlendEquationEXT = NULL;
	}

	_GLES_EXT_blend_minmax = 0;
	if (glloader_is_supported("GLES_EXT_blend_minmax"))
	{
		_GLES_EXT_blend_minmax = 1;

		LOAD_FUNC1(glBlendEquationEXT);
	}
}

void init_GLES_EXT_color_buffer_half_float()
{
	glloader_GLES_EXT_color_buffer_half_float = _glloader_GLES_EXT_color_buffer_half_float;

	_GLES_EXT_color_buffer_half_float = 0;
	if (glloader_is_supported("GLES_EXT_color_buffer_half_float"))
	{
		_GLES_EXT_color_buffer_half_float = 1;
	}
}

void init_GLES_EXT_debug_label()
{
	glloader_GLES_EXT_debug_label = _glloader_GLES_EXT_debug_label;

	{
		glLabelObjectEXT = NULL;
		glGetObjectLabelEXT = NULL;
	}

	_GLES_EXT_debug_label = 0;
	if (glloader_is_supported("GLES_EXT_debug_label"))
	{
		_GLES_EXT_debug_label = 1;

		LOAD_FUNC1(glLabelObjectEXT);
		LOAD_FUNC1(glGetObjectLabelEXT);
	}
}

void init_GLES_EXT_debug_marker()
{
	glloader_GLES_EXT_debug_marker = _glloader_GLES_EXT_debug_marker;

	{
		glInsertEventMarkerEXT = NULL;
		glPushGroupMarkerEXT = NULL;
		glPopGroupMarkerEXT = NULL;
	}

	_GLES_EXT_debug_marker = 0;
	if (glloader_is_supported("GLES_EXT_debug_marker"))
	{
		_GLES_EXT_debug_marker = 1;

		LOAD_FUNC1(glInsertEventMarkerEXT);
		LOAD_FUNC1(glPushGroupMarkerEXT);
		LOAD_FUNC1(glPopGroupMarkerEXT);
	}
}

void init_GLES_EXT_discard_framebuffer()
{
	glloader_GLES_EXT_discard_framebuffer = _glloader_GLES_EXT_discard_framebuffer;

	{
		glDiscardFramebufferEXT = NULL;
	}

	_GLES_EXT_discard_framebuffer = 0;
	if (glloader_is_supported("GLES_EXT_discard_framebuffer"))
	{
		_GLES_EXT_discard_framebuffer = 1;

		LOAD_FUNC1(glDiscardFramebufferEXT);
	}
}

void init_GLES_EXT_frag_depth()
{
	glloader_GLES_EXT_frag_depth = _glloader_GLES_EXT_frag_depth;

	_GLES_EXT_frag_depth = 0;
	if (glloader_is_supported("GLES_EXT_frag_depth"))
	{
		_GLES_EXT_frag_depth = 1;
	}
}

void init_GLES_EXT_map_buffer_range()
{
	glloader_GLES_EXT_map_buffer_range = _glloader_GLES_EXT_map_buffer_range;

	{
		glMapBufferRangeEXT = NULL;
		glFlushMappedBufferRangeEXT = NULL;
	}

	_GLES_EXT_map_buffer_range = 0;
	if (glloader_is_supported("GLES_EXT_map_buffer_range"))
	{
		_GLES_EXT_map_buffer_range = 1;

		LOAD_FUNC1(glMapBufferRangeEXT);
		LOAD_FUNC1(glFlushMappedBufferRangeEXT);
	}
}

void init_GLES_EXT_multisampled_render_to_texture()
{
	glloader_GLES_EXT_multisampled_render_to_texture = _glloader_GLES_EXT_multisampled_render_to_texture;

	{
		glRenderbufferStorageMultisampleEXT = NULL;
		glFramebufferTexture2DMultisampleEXT = NULL;
	}

	_GLES_EXT_multisampled_render_to_texture = 0;
	if (glloader_is_supported("GLES_EXT_multisampled_render_to_texture"))
	{
		_GLES_EXT_multisampled_render_to_texture = 1;

		LOAD_FUNC1(glRenderbufferStorageMultisampleEXT);
		LOAD_FUNC1(glFramebufferTexture2DMultisampleEXT);
	}
}

void init_GLES_EXT_multiview_draw_buffers()
{
	glloader_GLES_EXT_multiview_draw_buffers = _glloader_GLES_EXT_multiview_draw_buffers;

	{
		glReadBufferIndexedEXT = NULL;
		glDrawBuffersIndexedEXT = NULL;
		GetIntegeri_vEXT = NULL;
	}

	_GLES_EXT_multiview_draw_buffers = 0;
	if (glloader_is_supported("GLES_EXT_multiview_draw_buffers"))
	{
		_GLES_EXT_multiview_draw_buffers = 1;

		LOAD_FUNC1(glReadBufferIndexedEXT);
		LOAD_FUNC1(glDrawBuffersIndexedEXT);
		LOAD_FUNC1(GetIntegeri_vEXT);
	}
}

void init_GLES_EXT_multi_draw_arrays()
{
	glloader_GLES_EXT_multi_draw_arrays = _glloader_GLES_EXT_multi_draw_arrays;

	{
		glMultiDrawArraysEXT = NULL;
		glMultiDrawElementsEXT = NULL;
	}

	_GLES_EXT_multi_draw_arrays = 0;
	if (glloader_is_supported("GLES_EXT_multi_draw_arrays"))
	{
		_GLES_EXT_multi_draw_arrays = 1;

		LOAD_FUNC1(glMultiDrawArraysEXT);
		LOAD_FUNC1(glMultiDrawElementsEXT);
	}
}

void init_GLES_EXT_occlusion_query_boolean()
{
	glloader_GLES_EXT_occlusion_query_boolean = _glloader_GLES_EXT_occlusion_query_boolean;

	{
		glGenQueriesEXT = NULL;
		glDeleteQueriesEXT = NULL;
		glIsQueryEXT = NULL;
		glBeginQueryEXT = NULL;
		glEndQueryEXT = NULL;
		glGetQueryivEXT = NULL;
		glGetQueryObjectuivEXT = NULL;
	}

	_GLES_EXT_occlusion_query_boolean = 0;
	if (glloader_is_supported("GLES_EXT_occlusion_query_boolean"))
	{
		_GLES_EXT_occlusion_query_boolean = 1;

		LOAD_FUNC1(glGenQueriesEXT);
		LOAD_FUNC1(glDeleteQueriesEXT);
		LOAD_FUNC1(glIsQueryEXT);
		LOAD_FUNC1(glBeginQueryEXT);
		LOAD_FUNC1(glEndQueryEXT);
		LOAD_FUNC1(glGetQueryivEXT);
		LOAD_FUNC1(glGetQueryObjectuivEXT);
	}
}

void init_GLES_read_format_bgra()
{
	glloader_GLES_read_format_bgra = _glloader_GLES_read_format_bgra;

	_GLES_read_format_bgra = 0;
	if (glloader_is_supported("GLES_read_format_bgra"))
	{
		_GLES_read_format_bgra = 1;
	}
}

void init_GL_EXT_robustness()
{
	glloader_GL_EXT_robustness = _glloader_GL_EXT_robustness;

	{
		glGetGraphicsResetStatusEXT = NULL;
		glReadnPixelsEXT = NULL;
		glGetnUniformfvEXT = NULL;
		glGetnUniformivEXT = NULL;
	}

	_GL_EXT_robustness = 0;
	if (glloader_is_supported("GL_EXT_robustness"))
	{
		_GL_EXT_robustness = 1;

		LOAD_FUNC1(glGetGraphicsResetStatusEXT);
		LOAD_FUNC1(glReadnPixelsEXT);
		LOAD_FUNC1(glGetnUniformfvEXT);
		LOAD_FUNC1(glGetnUniformivEXT);
	}
}

void init_GLES_EXT_separate_shader_objects()
{
	glloader_GLES_EXT_separate_shader_objects = _glloader_GLES_EXT_separate_shader_objects;

	{
		glUseShaderProgramEXT = NULL;
		glActiveProgramEXT = NULL;
		glCreateShaderProgramEXT = NULL;
		glBindProgramPipelineEXT = NULL;
		glDeleteProgramPipelinesEXT = NULL;
		glGenProgramPipelinesEXT = NULL;
		glIsProgramPipelineEXT = NULL;
		glProgramParameteriEXT = NULL;
		glGetProgramPipelineivEXT = NULL;
		glProgramUniform1iEXT = NULL;
		glProgramUniform2iEXT = NULL;
		glProgramUniform3iEXT = NULL;
		glProgramUniform4iEXT = NULL;
		glProgramUniform1fEXT = NULL;
		glProgramUniform2fEXT = NULL;
		glProgramUniform3fEXT = NULL;
		glProgramUniform4fEXT = NULL;
		glProgramUniform1ivEXT = NULL;
		glProgramUniform2ivEXT = NULL;
		glProgramUniform3ivEXT = NULL;
		glProgramUniform4ivEXT = NULL;
		glProgramUniform1fvEXT = NULL;
		glProgramUniform2fvEXT = NULL;
		glProgramUniform3fvEXT = NULL;
		glProgramUniform4fvEXT = NULL;
		glProgramUniformMatrix2fvEXT = NULL;
		glProgramUniformMatrix3fvEXT = NULL;
		glProgramUniformMatrix4fvEXT = NULL;
		glValidateProgramPipelineEXT = NULL;
		glGetProgramPipelineInfoLogEXT = NULL;
	}

	_GLES_EXT_separate_shader_objects = 0;
	if (glloader_is_supported("GLES_EXT_separate_shader_objects"))
	{
		_GLES_EXT_separate_shader_objects = 1;

		LOAD_FUNC1(glUseShaderProgramEXT);
		LOAD_FUNC1(glActiveProgramEXT);
		LOAD_FUNC1(glCreateShaderProgramEXT);
		LOAD_FUNC1(glBindProgramPipelineEXT);
		LOAD_FUNC1(glDeleteProgramPipelinesEXT);
		LOAD_FUNC1(glGenProgramPipelinesEXT);
		LOAD_FUNC1(glIsProgramPipelineEXT);
		LOAD_FUNC1(glProgramParameteriEXT);
		LOAD_FUNC1(glGetProgramPipelineivEXT);
		LOAD_FUNC1(glProgramUniform1iEXT);
		LOAD_FUNC1(glProgramUniform2iEXT);
		LOAD_FUNC1(glProgramUniform3iEXT);
		LOAD_FUNC1(glProgramUniform4iEXT);
		LOAD_FUNC1(glProgramUniform1fEXT);
		LOAD_FUNC1(glProgramUniform2fEXT);
		LOAD_FUNC1(glProgramUniform3fEXT);
		LOAD_FUNC1(glProgramUniform4fEXT);
		LOAD_FUNC1(glProgramUniform1ivEXT);
		LOAD_FUNC1(glProgramUniform2ivEXT);
		LOAD_FUNC1(glProgramUniform3ivEXT);
		LOAD_FUNC1(glProgramUniform4ivEXT);
		LOAD_FUNC1(glProgramUniform1fvEXT);
		LOAD_FUNC1(glProgramUniform2fvEXT);
		LOAD_FUNC1(glProgramUniform3fvEXT);
		LOAD_FUNC1(glProgramUniform4fvEXT);
		LOAD_FUNC1(glProgramUniformMatrix2fvEXT);
		LOAD_FUNC1(glProgramUniformMatrix3fvEXT);
		LOAD_FUNC1(glProgramUniformMatrix4fvEXT);
		LOAD_FUNC1(glValidateProgramPipelineEXT);
		LOAD_FUNC1(glGetProgramPipelineInfoLogEXT);
	}
}

void init_GLES_EXT_shader_framebuffer_fetch()
{
	glloader_GLES_EXT_shader_framebuffer_fetch = _glloader_GLES_EXT_shader_framebuffer_fetch;

	_GLES_EXT_shader_framebuffer_fetch = 0;
	if (glloader_is_supported("GLES_EXT_shader_framebuffer_fetch"))
	{
		_GLES_EXT_shader_framebuffer_fetch = 1;
	}
}

void init_GLES_EXT_shader_texture_lod()
{
	glloader_GLES_EXT_shader_texture_lod = _glloader_GLES_EXT_shader_texture_lod;

	_GLES_EXT_shader_texture_lod = 0;
	if (glloader_is_supported("GLES_EXT_shader_texture_lod"))
	{
		_GLES_EXT_shader_texture_lod = 1;
	}
}

void init_GLES_EXT_shadow_samplers()
{
	glloader_GLES_EXT_shadow_samplers = _glloader_GLES_EXT_shadow_samplers;

	_GLES_EXT_shadow_samplers = 0;
	if (glloader_is_supported("GLES_EXT_shadow_samplers"))
	{
		_GLES_EXT_shadow_samplers = 1;
	}
}

void init_GLES_EXT_sRGB()
{
	glloader_GLES_EXT_sRGB = _glloader_GLES_EXT_sRGB;

	_GLES_EXT_sRGB = 0;
	if (glloader_is_supported("GLES_EXT_sRGB"))
	{
		_GLES_EXT_sRGB = 1;
	}
}

void init_GLES_EXT_texture_compression_dxt1()
{
	glloader_GLES_EXT_texture_compression_dxt1 = _glloader_GLES_EXT_texture_compression_dxt1;

	_GLES_EXT_texture_compression_dxt1 = 0;
	if (glloader_is_supported("GLES_EXT_texture_compression_dxt1"))
	{
		_GLES_EXT_texture_compression_dxt1 = 1;
	}
}

void init_GLES_EXT_texture_compression_latc()
{
	glloader_GLES_EXT_texture_compression_latc = _glloader_GLES_EXT_texture_compression_latc;

	_GLES_EXT_texture_compression_latc = 0;
	if (glloader_is_supported("GLES_EXT_texture_compression_latc"))
	{
		_GLES_EXT_texture_compression_latc = 1;
	}
}

void init_GLES_EXT_texture_compression_s3tc()
{
	glloader_GLES_EXT_texture_compression_s3tc = _glloader_GLES_EXT_texture_compression_s3tc;

	_GLES_EXT_texture_compression_s3tc = 0;
	if (glloader_is_supported("GLES_EXT_texture_compression_s3tc"))
	{
		_GLES_EXT_texture_compression_s3tc = 1;
	}
}

void init_GLES_EXT_texture_filter_anisotropic()
{
	glloader_GLES_EXT_texture_filter_anisotropic = _glloader_GLES_EXT_texture_filter_anisotropic;

	_GLES_EXT_texture_filter_anisotropic = 0;
	if (glloader_is_supported("GLES_EXT_texture_filter_anisotropic"))
	{
		_GLES_EXT_texture_filter_anisotropic = 1;
	}
}

void init_GLES_EXT_texture_format_BGRA8888()
{
	glloader_GLES_EXT_texture_format_BGRA8888 = _glloader_GLES_EXT_texture_format_BGRA8888;

	_GLES_EXT_texture_format_BGRA8888 = 0;
	if (glloader_is_supported("GLES_EXT_texture_format_BGRA8888"))
	{
		_GLES_EXT_texture_format_BGRA8888 = 1;
	}
	else
	{
		if (glloader_is_supported("GLES_APPLE_texture_format_BGRA8888"))
		{
			_GLES_EXT_texture_format_BGRA8888 = 1;
			promote_high("GLES_EXT_texture_format_BGRA8888");
		}
	}
}

void init_GLES_EXT_texture_lod_bias()
{
	glloader_GLES_EXT_texture_lod_bias = _glloader_GLES_EXT_texture_lod_bias;

	_GLES_EXT_texture_lod_bias = 0;
	if (glloader_is_supported("GLES_EXT_texture_lod_bias"))
	{
		_GLES_EXT_texture_lod_bias = 1;
	}
}

void init_GLES_EXT_texture_rg()
{
	glloader_GLES_EXT_texture_rg = _glloader_GLES_EXT_texture_rg;

	_GLES_EXT_texture_rg = 0;
	if (glloader_is_supported("GLES_EXT_texture_rg"))
	{
		_GLES_EXT_texture_rg = 1;
	}
}

void init_GLES_EXT_texture_storage()
{
	glloader_GLES_EXT_texture_storage = _glloader_GLES_EXT_texture_storage;

	{
		glTexStorage1DEXT = NULL;
		glTexStorage2DEXT = NULL;
		glTexStorage3DEXT = NULL;
		glTextureStorage1DEXT = NULL;
		glTextureStorage2DEXT = NULL;
		glTextureStorage3DEXT = NULL;
	}

	_GLES_EXT_texture_storage = 0;
	if (glloader_is_supported("GLES_EXT_texture_storage"))
	{
		_GLES_EXT_texture_storage = 1;

		LOAD_FUNC1(glTexStorage1DEXT);
		LOAD_FUNC1(glTexStorage2DEXT);
		LOAD_FUNC1(glTexStorage3DEXT);
		LOAD_FUNC1(glTextureStorage1DEXT);
		LOAD_FUNC1(glTextureStorage2DEXT);
		LOAD_FUNC1(glTextureStorage3DEXT);
	}
}

void init_GLES_EXT_texture_type_2_10_10_10_REV()
{
	glloader_GLES_EXT_texture_type_2_10_10_10_REV = _glloader_GLES_EXT_texture_type_2_10_10_10_REV;

	_GLES_EXT_texture_type_2_10_10_10_REV = 0;
	if (glloader_is_supported("GLES_EXT_texture_type_2_10_10_10_REV"))
	{
		_GLES_EXT_texture_type_2_10_10_10_REV = 1;
	}
}

void init_GLES_EXT_unpack_subimage()
{
	glloader_GLES_EXT_unpack_subimage = _glloader_GLES_EXT_unpack_subimage;

	_GLES_EXT_unpack_subimage = 0;
	if (glloader_is_supported("GLES_EXT_unpack_subimage"))
	{
		_GLES_EXT_unpack_subimage = 1;
	}
}

void init_GLES_FJ_shader_binary_GCCSO()
{
	glloader_GLES_FJ_shader_binary_GCCSO = _glloader_GLES_FJ_shader_binary_GCCSO;

	_GLES_FJ_shader_binary_GCCSO = 0;
	if (glloader_is_supported("GLES_FJ_shader_binary_GCCSO"))
	{
		_GLES_FJ_shader_binary_GCCSO = 1;
	}
}

void init_GLES_IMG_multisampled_render_to_texture()
{
	glloader_GLES_IMG_multisampled_render_to_texture = _glloader_GLES_IMG_multisampled_render_to_texture;

	{
		glRenderbufferStorageMultisampleIMG = NULL;
		glFramebufferTexture2DMultisampleIMG = NULL;
	}

	_GLES_IMG_multisampled_render_to_texture = 0;
	if (glloader_is_supported("GLES_IMG_multisampled_render_to_texture"))
	{
		_GLES_IMG_multisampled_render_to_texture = 1;

		LOAD_FUNC1(glRenderbufferStorageMultisampleIMG);
		LOAD_FUNC1(glFramebufferTexture2DMultisampleIMG);
	}
}

void init_GLES_IMG_program_binary()
{
	glloader_GLES_IMG_program_binary = _glloader_GLES_IMG_program_binary;

	_GLES_IMG_program_binary = 0;
	if (glloader_is_supported("GLES_IMG_program_binary"))
	{
		_GLES_IMG_program_binary = 1;
	}
}

void init_GLES_IMG_read_format()
{
	glloader_GLES_IMG_read_format = _glloader_GLES_IMG_read_format;

	_GLES_IMG_read_format = 0;
	if (glloader_is_supported("GLES_IMG_read_format"))
	{
		_GLES_IMG_read_format = 1;
	}
}

void init_GLES_shader_binary()
{
	glloader_GLES_shader_binary = _glloader_GLES_shader_binary;

	_GLES_shader_binary = 0;
	if (glloader_is_supported("GLES_shader_binary"))
	{
		_GLES_shader_binary = 1;
	}
}

void init_GLES_IMG_texture_compression_pvrtc()
{
	glloader_GLES_IMG_texture_compression_pvrtc = _glloader_GLES_IMG_texture_compression_pvrtc;

	_GLES_IMG_texture_compression_pvrtc = 0;
	if (glloader_is_supported("GLES_IMG_texture_compression_pvrtc"))
	{
		_GLES_IMG_texture_compression_pvrtc = 1;
	}
}

void init_GLES_IMG_texture_env_enhanced_fixed_function()
{
	glloader_GLES_IMG_texture_env_enhanced_fixed_function = _glloader_GLES_IMG_texture_env_enhanced_fixed_function;

	_GLES_IMG_texture_env_enhanced_fixed_function = 0;
	if (glloader_is_supported("GLES_IMG_texture_env_enhanced_fixed_function"))
	{
		_GLES_IMG_texture_env_enhanced_fixed_function = 1;
	}
}

void init_GLES_IMG_user_clip_plane()
{
	glloader_GLES_IMG_user_clip_plane = _glloader_GLES_IMG_user_clip_plane;

	{
		glClipPlanefIMG = NULL;
		glClipPlanexIMG = NULL;
	}

	_GLES_IMG_user_clip_plane = 0;
	if (glloader_is_supported("GLES_IMG_user_clip_plane"))
	{
		_GLES_IMG_user_clip_plane = 1;

		LOAD_FUNC1(glClipPlanefIMG);
		LOAD_FUNC1(glClipPlanexIMG);
	}
}

void init_GLES_NV_coverage_sample()
{
	glloader_GLES_NV_coverage_sample = _glloader_GLES_NV_coverage_sample;

	{
		glCoverageMaskNV = NULL;
		glCoverageOperationNV = NULL;
	}

	_GLES_NV_coverage_sample = 0;
	if (glloader_is_supported("GLES_NV_coverage_sample"))
	{
		_GLES_NV_coverage_sample = 1;

		LOAD_FUNC1(glCoverageMaskNV);
		LOAD_FUNC1(glCoverageOperationNV);
	}
}

void init_GLES_NV_depth_nonlinear()
{
	glloader_GLES_NV_depth_nonlinear = _glloader_GLES_NV_depth_nonlinear;

	_GLES_NV_depth_nonlinear = 0;
	if (glloader_is_supported("GLES_NV_depth_nonlinear"))
	{
		_GLES_NV_depth_nonlinear = 1;
	}
}

void init_GLES_NV_draw_buffers()
{
	glloader_GLES_NV_draw_buffers = _glloader_GLES_NV_draw_buffers;

	{
		glDrawBuffersNV = NULL;
	}

	_GLES_NV_draw_buffers = 0;
	if (glloader_is_supported("GLES_NV_draw_buffers"))
	{
		_GLES_NV_draw_buffers = 1;

		LOAD_FUNC1(glDrawBuffersNV);
	}
}

void init_GLES_NV_draw_texture()
{
	glloader_GLES_NV_draw_texture = _glloader_GLES_NV_draw_texture;

	{
		glDrawTextureNV = NULL;
	}

	_GLES_NV_draw_texture = 0;
	if (glloader_is_supported("GLES_NV_draw_texture"))
	{
		_GLES_NV_draw_texture = 1;

		LOAD_FUNC1(glDrawTextureNV);
	}
}

void init_GLES_NV_EGL_stream_consumer_external()
{
	glloader_GLES_NV_EGL_stream_consumer_external = _glloader_GLES_NV_EGL_stream_consumer_external;

	_GLES_NV_EGL_stream_consumer_external = 0;
	if (glloader_is_supported("GLES_NV_EGL_stream_consumer_external"))
	{
		_GLES_NV_EGL_stream_consumer_external = 1;
	}
}

void init_GLES_NV_fbo_color_attachments()
{
	glloader_GLES_NV_fbo_color_attachments = _glloader_GLES_NV_fbo_color_attachments;

	_GLES_NV_fbo_color_attachments = 0;
	if (glloader_is_supported("GLES_NV_fbo_color_attachments"))
	{
		_GLES_NV_fbo_color_attachments = 1;
	}
}

void init_GLES_NV_fence()
{
	glloader_GLES_NV_fence = _glloader_GLES_NV_fence;

	{
		glDeleteFencesNV = NULL;
		glGenFencesNV = NULL;
		glIsFenceNV = NULL;
		glTestFenceNV = NULL;
		glGetFenceivNV = NULL;
		glFinishFenceNV = NULL;
		glSetFenceNV = NULL;
	}

	_GLES_NV_fence = 0;
	if (glloader_is_supported("GLES_NV_fence"))
	{
		_GLES_NV_fence = 1;

		LOAD_FUNC1(glDeleteFencesNV);
		LOAD_FUNC1(glGenFencesNV);
		LOAD_FUNC1(glIsFenceNV);
		LOAD_FUNC1(glTestFenceNV);
		LOAD_FUNC1(glGetFenceivNV);
		LOAD_FUNC1(glFinishFenceNV);
		LOAD_FUNC1(glSetFenceNV);
	}
}

void init_GLES_NV_read_buffer()
{
	glloader_GLES_NV_read_buffer = _glloader_GLES_NV_read_buffer;

	{
		glReadBufferNV = NULL;
	}

	_GLES_NV_read_buffer = 0;
	if (glloader_is_supported("GLES_NV_read_buffer"))
	{
		_GLES_NV_read_buffer = 1;

		LOAD_FUNC1(glReadBufferNV);
	}
}

void init_GLES_NV_read_depth()
{
	glloader_GLES_NV_read_depth = _glloader_GLES_NV_read_depth;

	_GLES_NV_read_depth = 0;
	if (glloader_is_supported("GLES_NV_read_depth"))
	{
		_GLES_NV_read_depth = 1;
	}
}

void init_GLES_NV_read_depth_stencil()
{
	glloader_GLES_NV_read_depth_stencil = _glloader_GLES_NV_read_depth_stencil;

	_GLES_NV_read_depth_stencil = 0;
	if (glloader_is_supported("GLES_NV_read_depth_stencil"))
	{
		_GLES_NV_read_depth_stencil = 1;
	}
}

void init_GLES_NV_read_stencil()
{
	glloader_GLES_NV_read_stencil = _glloader_GLES_NV_read_stencil;

	_GLES_NV_read_stencil = 0;
	if (glloader_is_supported("GLES_NV_read_stencil"))
	{
		_GLES_NV_read_stencil = 1;
	}
}

void init_GLES_NV_texture_compression_s3tc_update()
{
	glloader_GLES_NV_texture_compression_s3tc_update = _glloader_GLES_NV_texture_compression_s3tc_update;

	_GLES_NV_texture_compression_s3tc_update = 0;
	if (glloader_is_supported("GLES_NV_texture_compression_s3tc_update"))
	{
		_GLES_NV_texture_compression_s3tc_update = 1;
	}
}

void init_GLES_NV_texture_npot_2D_mipmap()
{
	glloader_GLES_NV_texture_npot_2D_mipmap = _glloader_GLES_NV_texture_npot_2D_mipmap;

	_GLES_NV_texture_npot_2D_mipmap = 0;
	if (glloader_is_supported("GLES_NV_texture_npot_2D_mipmap"))
	{
		_GLES_NV_texture_npot_2D_mipmap = 1;
	}
}

void init_GLES_OES_blend_equation_separate()
{
	glloader_GLES_OES_blend_equation_separate = _glloader_GLES_OES_blend_equation_separate;

	{
		glBlendEquationSeparateOES = NULL;
	}

	_GLES_OES_blend_equation_separate = 0;
	if (glloader_is_supported("GLES_OES_blend_equation_separate"))
	{
		_GLES_OES_blend_equation_separate = 1;

		LOAD_FUNC1(glBlendEquationSeparateOES);
	}
}

void init_GLES_OES_blend_func_separate()
{
	glloader_GLES_OES_blend_func_separate = _glloader_GLES_OES_blend_func_separate;

	{
		glBlendFuncSeparateOES = NULL;
	}

	_GLES_OES_blend_func_separate = 0;
	if (glloader_is_supported("GLES_OES_blend_func_separate"))
	{
		_GLES_OES_blend_func_separate = 1;

		LOAD_FUNC1(glBlendFuncSeparateOES);
	}
}

void init_GLES_OES_blend_subtract()
{
	glloader_GLES_OES_blend_subtract = _glloader_GLES_OES_blend_subtract;

	{
		glBlendEquationOES = NULL;
	}

	_GLES_OES_blend_subtract = 0;
	if (glloader_is_supported("GLES_OES_blend_subtract"))
	{
		_GLES_OES_blend_subtract = 1;

		LOAD_FUNC1(glBlendEquationOES);
	}
}

void init_GLES_OES_byte_coordinates()
{
	glloader_GLES_OES_byte_coordinates = _glloader_GLES_OES_byte_coordinates;

	{
		glVertex2bOES = NULL;
		glVertex3bOES = NULL;
		glVertex4bOES = NULL;
		glVertex2bvOES = NULL;
		glVertex3bvOES = NULL;
		glVertex4bvOES = NULL;
		glTexCoord1bOES = NULL;
		glTexCoord2bOES = NULL;
		glTexCoord3bOES = NULL;
		glTexCoord4bOES = NULL;
		glTexCoord1bvOES = NULL;
		glTexCoord2bvOES = NULL;
		glTexCoord3bvOES = NULL;
		glTexCoord4bvOES = NULL;
		glMultiTexCoord1bOES = NULL;
		glMultiTexCoord2bOES = NULL;
		glMultiTexCoord3bOES = NULL;
		glMultiTexCoord4bOES = NULL;
		glMultiTexCoord1bvOES = NULL;
		glMultiTexCoord2bvOES = NULL;
		glMultiTexCoord3bvOES = NULL;
		glMultiTexCoord4bvOES = NULL;
	}

	_GLES_OES_byte_coordinates = 0;
	if (glloader_is_supported("GLES_OES_byte_coordinates"))
	{
		_GLES_OES_byte_coordinates = 1;

		LOAD_FUNC1(glVertex2bOES);
		LOAD_FUNC1(glVertex3bOES);
		LOAD_FUNC1(glVertex4bOES);
		LOAD_FUNC1(glVertex2bvOES);
		LOAD_FUNC1(glVertex3bvOES);
		LOAD_FUNC1(glVertex4bvOES);
		LOAD_FUNC1(glTexCoord1bOES);
		LOAD_FUNC1(glTexCoord2bOES);
		LOAD_FUNC1(glTexCoord3bOES);
		LOAD_FUNC1(glTexCoord4bOES);
		LOAD_FUNC1(glTexCoord1bvOES);
		LOAD_FUNC1(glTexCoord2bvOES);
		LOAD_FUNC1(glTexCoord3bvOES);
		LOAD_FUNC1(glTexCoord4bvOES);
		LOAD_FUNC1(glMultiTexCoord1bOES);
		LOAD_FUNC1(glMultiTexCoord2bOES);
		LOAD_FUNC1(glMultiTexCoord3bOES);
		LOAD_FUNC1(glMultiTexCoord4bOES);
		LOAD_FUNC1(glMultiTexCoord1bvOES);
		LOAD_FUNC1(glMultiTexCoord2bvOES);
		LOAD_FUNC1(glMultiTexCoord3bvOES);
		LOAD_FUNC1(glMultiTexCoord4bvOES);
	}
}

void init_GLES_OES_compressed_ETC1_RGB8_texture()
{
	glloader_GLES_OES_compressed_ETC1_RGB8_texture = _glloader_GLES_OES_compressed_ETC1_RGB8_texture;

	_GLES_OES_compressed_ETC1_RGB8_texture = 0;
	if (glloader_is_supported("GLES_OES_compressed_ETC1_RGB8_texture"))
	{
		_GLES_OES_compressed_ETC1_RGB8_texture = 1;
	}
}

void init_GLES_OES_compressed_paletted_texture()
{
	glloader_GLES_OES_compressed_paletted_texture = _glloader_GLES_OES_compressed_paletted_texture;

	_GLES_OES_compressed_paletted_texture = 0;
	if (glloader_is_supported("GLES_OES_compressed_paletted_texture"))
	{
		_GLES_OES_compressed_paletted_texture = 1;
	}
}

void init_GLES_OES_depth24()
{
	glloader_GLES_OES_depth24 = _glloader_GLES_OES_depth24;

	_GLES_OES_depth24 = 0;
	if (glloader_is_supported("GLES_OES_depth24"))
	{
		_GLES_OES_depth24 = 1;
	}
}

void init_GLES_OES_depth32()
{
	glloader_GLES_OES_depth32 = _glloader_GLES_OES_depth32;

	_GLES_OES_depth32 = 0;
	if (glloader_is_supported("GLES_OES_depth32"))
	{
		_GLES_OES_depth32 = 1;
	}
}

void init_GLES_OES_depth_texture()
{
	glloader_GLES_OES_depth_texture = _glloader_GLES_OES_depth_texture;

	_GLES_OES_depth_texture = 0;
	if (glloader_is_supported("GLES_OES_depth_texture"))
	{
		_GLES_OES_depth_texture = 1;
	}
}

void init_GLES_OES_draw_texture()
{
	glloader_GLES_OES_draw_texture = _glloader_GLES_OES_draw_texture;

	{
		glDrawTexsOES = NULL;
		glDrawTexiOES = NULL;
		glDrawTexxOES = NULL;
		glDrawTexsvOES = NULL;
		glDrawTexivOES = NULL;
		glDrawTexxvOES = NULL;
		glDrawTexfOES = NULL;
		glDrawTexfvOES = NULL;
	}

	_GLES_OES_draw_texture = 0;
	if (glloader_is_supported("GLES_OES_draw_texture"))
	{
		_GLES_OES_draw_texture = 1;

		LOAD_FUNC1(glDrawTexsOES);
		LOAD_FUNC1(glDrawTexiOES);
		LOAD_FUNC1(glDrawTexxOES);
		LOAD_FUNC1(glDrawTexsvOES);
		LOAD_FUNC1(glDrawTexivOES);
		LOAD_FUNC1(glDrawTexxvOES);
		LOAD_FUNC1(glDrawTexfOES);
		LOAD_FUNC1(glDrawTexfvOES);
	}
}

void init_GLES_OES_EGL_image()
{
	glloader_GLES_OES_EGL_image = _glloader_GLES_OES_EGL_image;

	{
		EGLImageTargetTexture2DOES = NULL;
		EGLImageTargetRenderbufferStorageOES = NULL;
	}

	_GLES_OES_EGL_image = 0;
	if (glloader_is_supported("GLES_OES_EGL_image"))
	{
		_GLES_OES_EGL_image = 1;

		LOAD_FUNC1(EGLImageTargetTexture2DOES);
		LOAD_FUNC1(EGLImageTargetRenderbufferStorageOES);
	}
}

void init_GLES_OES_EGL_image_external()
{
	glloader_GLES_OES_EGL_image_external = _glloader_GLES_OES_EGL_image_external;

	_GLES_OES_EGL_image_external = 0;
	if (glloader_is_supported("GLES_OES_EGL_image_external"))
	{
		_GLES_OES_EGL_image_external = 1;
	}
}

void init_GLES_OES_element_index_uint()
{
	glloader_GLES_OES_element_index_uint = _glloader_GLES_OES_element_index_uint;

	_GLES_OES_element_index_uint = 0;
	if (glloader_is_supported("GLES_OES_element_index_uint"))
	{
		_GLES_OES_element_index_uint = 1;
	}
}

void init_GLES_OES_extended_matrix_palette()
{
	glloader_GLES_OES_extended_matrix_palette = _glloader_GLES_OES_extended_matrix_palette;

	_GLES_OES_extended_matrix_palette = 0;
	if (glloader_is_supported("GLES_OES_extended_matrix_palette"))
	{
		_GLES_OES_extended_matrix_palette = 1;
	}
}

void init_GLES_OES_fbo_render_mipmap()
{
	glloader_GLES_OES_fbo_render_mipmap = _glloader_GLES_OES_fbo_render_mipmap;

	_GLES_OES_fbo_render_mipmap = 0;
	if (glloader_is_supported("GLES_OES_fbo_render_mipmap"))
	{
		_GLES_OES_fbo_render_mipmap = 1;
	}
}

void init_GLES_OES_fixed_point()
{
	glloader_GLES_OES_fixed_point = _glloader_GLES_OES_fixed_point;

	{
		glVertex2xOES = NULL;
		glVertex3xOES = NULL;
		glVertex4xOES = NULL;
		glVertex2xvOES = NULL;
		glVertex3xvOES = NULL;
		glVertex4xvOES = NULL;
		glNormal3xOES = NULL;
		glNormal3xvOES = NULL;
		glTexCoord1xOES = NULL;
		glTexCoord2xOES = NULL;
		glTexCoord3xOES = NULL;
		glTexCoord4xOES = NULL;
		glMultiTexCoord1xOES = NULL;
		glMultiTexCoord2xOES = NULL;
		glMultiTexCoord3xOES = NULL;
		glMultiTexCoord4xOES = NULL;
		glMultiTexCoord1xvOES = NULL;
		glMultiTexCoord2xvOES = NULL;
		glMultiTexCoord3xvOES = NULL;
		glMultiTexCoord4xvOES = NULL;
		glColor3xOES = NULL;
		glColor4xOES = NULL;
		glColor3xvOES = NULL;
		glColor4xvOES = NULL;
		glIndexxOES = NULL;
		glIndexxvOES = NULL;
		glRectxOES = NULL;
		glRectxvOES = NULL;
		glDepthRangexOES = NULL;
		glLoadMatrixxOES = NULL;
		glMultMatrixxOES = NULL;
		glLoadTransposeMatrixxOES = NULL;
		glMultTransposeMatrixxOES = NULL;
		glRotatexOES = NULL;
		glScalexOES = NULL;
		glTranslatexOES = NULL;
		glFrustumxOES = NULL;
		glOrthoxOES = NULL;
		glTexGenxOES = NULL;
		glTexGenxvOES = NULL;
		glGetTexGenxvOES = NULL;
		glClipPlanexOES = NULL;
		glGetClipPlanexOES = NULL;
		glRasterPos2xOES = NULL;
		glRasterPos3xOES = NULL;
		glRasterPos4xOES = NULL;
		glRasterPos2xvOES = NULL;
		glRasterPos3xvOES = NULL;
		glRasterPos4xvOES = NULL;
		glMaterialxOES = NULL;
		glMaterialxvOES = NULL;
		glGetMaterialxOES = NULL;
		glLightxOES = NULL;
		glLightxvOES = NULL;
		glGetLightxvOES = NULL;
		glLightModelxOES = NULL;
		glLightModelxvOES = NULL;
		glPointSizexOES = NULL;
		glPointParameterxvOES = NULL;
		glLineWidthxOES = NULL;
		glPolygonOffsetxOES = NULL;
		glPixelStorexOES = NULL;
		glPixelTransferxOES = NULL;
		glPixelMapxOES = NULL;
		glGetPixelMapxvOES = NULL;
		glConvolutionParameterxOES = NULL;
		glConvolutionParameterxvOES = NULL;
		glGetConvolutionParameterxvOES = NULL;
		glGetHistogramParameterxvOES = NULL;
		glPixelZoomxOES = NULL;
		glBitmapxOES = NULL;
		glTexParameterxOES = NULL;
		glGetTexParameterxvOES = NULL;
		glGetTexLevelParameterxvOES = NULL;
		glPrioritizeTexturesxOES = NULL;
		glTexEnvxOES = NULL;
		glGetTexEnvxvOES = NULL;
		glFogxOES = NULL;
		glFogxvOES = NULL;
		glSampleCoveragexOES = NULL;
		glAlphaFuncxOES = NULL;
		glBlendColorxOES = NULL;
		glClearColorxOES = NULL;
		glClearDepthxOES = NULL;
		glClearAccumxOES = NULL;
		glAccumxOES = NULL;
		glMap1xOES = NULL;
		glMap2xOES = NULL;
		glMapGrid1xOES = NULL;
		glMapGrid2xOES = NULL;
		glGetMapxvOES = NULL;
		glEvalCoord1xOES = NULL;
		glEvalCoord1xvOES = NULL;
		glEvalCoord2xOES = NULL;
		glEvalCoord2xvOES = NULL;
		glFeedbackBufferxOES = NULL;
		glPassThroughxOES = NULL;
		glGetFixedvOES = NULL;
	}

	_GLES_OES_fixed_point = 0;
	if (glloader_is_supported("GLES_OES_fixed_point"))
	{
		_GLES_OES_fixed_point = 1;

		LOAD_FUNC1(glVertex2xOES);
		LOAD_FUNC1(glVertex3xOES);
		LOAD_FUNC1(glVertex4xOES);
		LOAD_FUNC1(glVertex2xvOES);
		LOAD_FUNC1(glVertex3xvOES);
		LOAD_FUNC1(glVertex4xvOES);
		LOAD_FUNC1(glNormal3xOES);
		LOAD_FUNC1(glNormal3xvOES);
		LOAD_FUNC1(glTexCoord1xOES);
		LOAD_FUNC1(glTexCoord2xOES);
		LOAD_FUNC1(glTexCoord3xOES);
		LOAD_FUNC1(glTexCoord4xOES);
		LOAD_FUNC1(glMultiTexCoord1xOES);
		LOAD_FUNC1(glMultiTexCoord2xOES);
		LOAD_FUNC1(glMultiTexCoord3xOES);
		LOAD_FUNC1(glMultiTexCoord4xOES);
		LOAD_FUNC1(glMultiTexCoord1xvOES);
		LOAD_FUNC1(glMultiTexCoord2xvOES);
		LOAD_FUNC1(glMultiTexCoord3xvOES);
		LOAD_FUNC1(glMultiTexCoord4xvOES);
		LOAD_FUNC1(glColor3xOES);
		LOAD_FUNC1(glColor4xOES);
		LOAD_FUNC1(glColor3xvOES);
		LOAD_FUNC1(glColor4xvOES);
		LOAD_FUNC1(glIndexxOES);
		LOAD_FUNC1(glIndexxvOES);
		LOAD_FUNC1(glRectxOES);
		LOAD_FUNC1(glRectxvOES);
		LOAD_FUNC1(glDepthRangexOES);
		LOAD_FUNC1(glLoadMatrixxOES);
		LOAD_FUNC1(glMultMatrixxOES);
		LOAD_FUNC1(glLoadTransposeMatrixxOES);
		LOAD_FUNC1(glMultTransposeMatrixxOES);
		LOAD_FUNC1(glRotatexOES);
		LOAD_FUNC1(glScalexOES);
		LOAD_FUNC1(glTranslatexOES);
		LOAD_FUNC1(glFrustumxOES);
		LOAD_FUNC1(glOrthoxOES);
		LOAD_FUNC1(glTexGenxOES);
		LOAD_FUNC1(glTexGenxvOES);
		LOAD_FUNC1(glGetTexGenxvOES);
		LOAD_FUNC1(glClipPlanexOES);
		LOAD_FUNC1(glGetClipPlanexOES);
		LOAD_FUNC1(glRasterPos2xOES);
		LOAD_FUNC1(glRasterPos3xOES);
		LOAD_FUNC1(glRasterPos4xOES);
		LOAD_FUNC1(glRasterPos2xvOES);
		LOAD_FUNC1(glRasterPos3xvOES);
		LOAD_FUNC1(glRasterPos4xvOES);
		LOAD_FUNC1(glMaterialxOES);
		LOAD_FUNC1(glMaterialxvOES);
		LOAD_FUNC1(glGetMaterialxOES);
		LOAD_FUNC1(glLightxOES);
		LOAD_FUNC1(glLightxvOES);
		LOAD_FUNC1(glGetLightxvOES);
		LOAD_FUNC1(glLightModelxOES);
		LOAD_FUNC1(glLightModelxvOES);
		LOAD_FUNC1(glPointSizexOES);
		LOAD_FUNC1(glPointParameterxvOES);
		LOAD_FUNC1(glLineWidthxOES);
		LOAD_FUNC1(glPolygonOffsetxOES);
		LOAD_FUNC1(glPixelStorexOES);
		LOAD_FUNC1(glPixelTransferxOES);
		LOAD_FUNC1(glPixelMapxOES);
		LOAD_FUNC1(glGetPixelMapxvOES);
		LOAD_FUNC1(glConvolutionParameterxOES);
		LOAD_FUNC1(glConvolutionParameterxvOES);
		LOAD_FUNC1(glGetConvolutionParameterxvOES);
		LOAD_FUNC1(glGetHistogramParameterxvOES);
		LOAD_FUNC1(glPixelZoomxOES);
		LOAD_FUNC1(glBitmapxOES);
		LOAD_FUNC1(glTexParameterxOES);
		LOAD_FUNC1(glGetTexParameterxvOES);
		LOAD_FUNC1(glGetTexLevelParameterxvOES);
		LOAD_FUNC1(glPrioritizeTexturesxOES);
		LOAD_FUNC1(glTexEnvxOES);
		LOAD_FUNC1(glGetTexEnvxvOES);
		LOAD_FUNC1(glFogxOES);
		LOAD_FUNC1(glFogxvOES);
		LOAD_FUNC1(glSampleCoveragexOES);
		LOAD_FUNC1(glAlphaFuncxOES);
		LOAD_FUNC1(glBlendColorxOES);
		LOAD_FUNC1(glClearColorxOES);
		LOAD_FUNC1(glClearDepthxOES);
		LOAD_FUNC1(glClearAccumxOES);
		LOAD_FUNC1(glAccumxOES);
		LOAD_FUNC1(glMap1xOES);
		LOAD_FUNC1(glMap2xOES);
		LOAD_FUNC1(glMapGrid1xOES);
		LOAD_FUNC1(glMapGrid2xOES);
		LOAD_FUNC1(glGetMapxvOES);
		LOAD_FUNC1(glEvalCoord1xOES);
		LOAD_FUNC1(glEvalCoord1xvOES);
		LOAD_FUNC1(glEvalCoord2xOES);
		LOAD_FUNC1(glEvalCoord2xvOES);
		LOAD_FUNC1(glFeedbackBufferxOES);
		LOAD_FUNC1(glPassThroughxOES);
		LOAD_FUNC1(glGetFixedvOES);
	}
}

void init_GLES_OES_fragment_precision_high()
{
	glloader_GLES_OES_fragment_precision_high = _glloader_GLES_OES_fragment_precision_high;

	_GLES_OES_fragment_precision_high = 0;
	if (glloader_is_supported("GLES_OES_fragment_precision_high"))
	{
		_GLES_OES_fragment_precision_high = 1;
	}
}

void init_GLES_OES_framebuffer_object()
{
	glloader_GLES_OES_framebuffer_object = _glloader_GLES_OES_framebuffer_object;

	{
		glIsRenderbufferOES = NULL;
		glBindRenderbufferOES = NULL;
		glDeleteRenderbuffersOES = NULL;
		glGenRenderbuffersOES = NULL;
		glRenderbufferStorageOES = NULL;
		glGetRenderbufferParameterivOES = NULL;
		glIsFramebufferOES = NULL;
		glBindFramebufferOES = NULL;
		glDeleteFramebuffersOES = NULL;
		glGenFramebuffersOES = NULL;
		glCheckFramebufferStatusOES = NULL;
		glFramebufferTexture2DOES = NULL;
		glFramebufferRenderbufferOES = NULL;
		glGetFramebufferAttachmentParameterivOES = NULL;
		glGenerateMipmapOES = NULL;
	}

	_GLES_OES_framebuffer_object = 0;
	if (glloader_is_supported("GLES_OES_framebuffer_object"))
	{
		_GLES_OES_framebuffer_object = 1;

		LOAD_FUNC1(glIsRenderbufferOES);
		LOAD_FUNC1(glBindRenderbufferOES);
		LOAD_FUNC1(glDeleteRenderbuffersOES);
		LOAD_FUNC1(glGenRenderbuffersOES);
		LOAD_FUNC1(glRenderbufferStorageOES);
		LOAD_FUNC1(glGetRenderbufferParameterivOES);
		LOAD_FUNC1(glIsFramebufferOES);
		LOAD_FUNC1(glBindFramebufferOES);
		LOAD_FUNC1(glDeleteFramebuffersOES);
		LOAD_FUNC1(glGenFramebuffersOES);
		LOAD_FUNC1(glCheckFramebufferStatusOES);
		LOAD_FUNC1(glFramebufferTexture2DOES);
		LOAD_FUNC1(glFramebufferRenderbufferOES);
		LOAD_FUNC1(glGetFramebufferAttachmentParameterivOES);
		LOAD_FUNC1(glGenerateMipmapOES);
	}
}

void init_GLES_OES_get_program_binary()
{
	glloader_GLES_OES_get_program_binary = _glloader_GLES_OES_get_program_binary;

	{
		glGetProgramBinaryOES = NULL;
		glProgramBinaryOES = NULL;
	}

	_GLES_OES_get_program_binary = 0;
	if (glloader_is_supported("GLES_OES_get_program_binary"))
	{
		_GLES_OES_get_program_binary = 1;

		LOAD_FUNC1(glGetProgramBinaryOES);
		LOAD_FUNC1(glProgramBinaryOES);
	}
}

void init_GLES_OES_mapbuffer()
{
	glloader_GLES_OES_mapbuffer = _glloader_GLES_OES_mapbuffer;

	{
		glGetBufferPointervOES = NULL;
		glMapBufferOES = NULL;
		glUnmapBufferOES = NULL;
	}

	_GLES_OES_mapbuffer = 0;
	if (glloader_is_supported("GLES_OES_mapbuffer"))
	{
		_GLES_OES_mapbuffer = 1;

		LOAD_FUNC1(glGetBufferPointervOES);
		LOAD_FUNC1(glMapBufferOES);
		LOAD_FUNC1(glUnmapBufferOES);
	}
}

void init_GLES_OES_matrix_get()
{
	glloader_GLES_OES_matrix_get = _glloader_GLES_OES_matrix_get;

	_GLES_OES_matrix_get = 0;
	if (glloader_is_supported("GLES_OES_matrix_get"))
	{
		_GLES_OES_matrix_get = 1;
	}
}

void init_GLES_OES_matrix_palette()
{
	glloader_GLES_OES_matrix_palette = _glloader_GLES_OES_matrix_palette;

	{
		glCurrentPaletteMatrixOES = NULL;
		glLoadPaletteFromModelViewMatrixOES = NULL;
		glMatrixIndexPointerOES = NULL;
		glWeightPointerOES = NULL;
	}

	_GLES_OES_matrix_palette = 0;
	if (glloader_is_supported("GLES_OES_matrix_palette"))
	{
		_GLES_OES_matrix_palette = 1;

		LOAD_FUNC1(glCurrentPaletteMatrixOES);
		LOAD_FUNC1(glLoadPaletteFromModelViewMatrixOES);
		LOAD_FUNC1(glMatrixIndexPointerOES);
		LOAD_FUNC1(glWeightPointerOES);
	}
}

void init_GLES_OES_packed_depth_stencil()
{
	glloader_GLES_OES_packed_depth_stencil = _glloader_GLES_OES_packed_depth_stencil;

	_GLES_OES_packed_depth_stencil = 0;
	if (glloader_is_supported("GLES_OES_packed_depth_stencil"))
	{
		_GLES_OES_packed_depth_stencil = 1;
	}
}

void init_GLES_OES_point_size_array()
{
	glloader_GLES_OES_point_size_array = _glloader_GLES_OES_point_size_array;

	{
		glPointSizePointerOES = NULL;
	}

	_GLES_OES_point_size_array = 0;
	if (glloader_is_supported("GLES_OES_point_size_array"))
	{
		_GLES_OES_point_size_array = 1;

		LOAD_FUNC1(glPointSizePointerOES);
	}
}

void init_GLES_OES_point_sprite()
{
	glloader_GLES_OES_point_sprite = _glloader_GLES_OES_point_sprite;

	_GLES_OES_point_sprite = 0;
	if (glloader_is_supported("GLES_OES_point_sprite"))
	{
		_GLES_OES_point_sprite = 1;
	}
}

void init_GLES_OES_query_matrix()
{
	glloader_GLES_OES_query_matrix = _glloader_GLES_OES_query_matrix;

	{
		glQueryMatrixxOES = NULL;
	}

	_GLES_OES_query_matrix = 0;
	if (glloader_is_supported("GLES_OES_query_matrix"))
	{
		_GLES_OES_query_matrix = 1;

		LOAD_FUNC1(glQueryMatrixxOES);
	}
}

void init_GLES_OES_read_format()
{
	glloader_GLES_OES_read_format = _glloader_GLES_OES_read_format;

	_GLES_OES_read_format = 0;
	if (glloader_is_supported("GLES_OES_read_format"))
	{
		_GLES_OES_read_format = 1;
	}
}

void init_GLES_OES_required_internalformat()
{
	glloader_GLES_OES_required_internalformat = _glloader_GLES_OES_required_internalformat;

	_GLES_OES_required_internalformat = 0;
	if (glloader_is_supported("GLES_OES_required_internalformat"))
	{
		_GLES_OES_required_internalformat = 1;
	}
}

void init_GLES_OES_rgb8_rgba8()
{
	glloader_GLES_OES_rgb8_rgba8 = _glloader_GLES_OES_rgb8_rgba8;

	_GLES_OES_rgb8_rgba8 = 0;
	if (glloader_is_supported("GLES_OES_rgb8_rgba8"))
	{
		_GLES_OES_rgb8_rgba8 = 1;
	}
}

void init_GLES_OES_shader_binary()
{
	glloader_GLES_OES_shader_binary = _glloader_GLES_OES_shader_binary;

	{
		glShaderBinaryOES = NULL;
	}

	_GLES_OES_shader_binary = 0;
	if (glloader_is_supported("GLES_OES_shader_binary"))
	{
		_GLES_OES_shader_binary = 1;

		LOAD_FUNC1(glShaderBinaryOES);
	}
}

void init_GLES_OES_shader_source()
{
	glloader_GLES_OES_shader_source = _glloader_GLES_OES_shader_source;

	{
		glReleaseShaderCompilerOES = NULL;
		glGetShaderPrecisionFormatOES = NULL;
	}

	_GLES_OES_shader_source = 0;
	if (glloader_is_supported("GLES_OES_shader_source"))
	{
		_GLES_OES_shader_source = 1;

		LOAD_FUNC1(glReleaseShaderCompilerOES);
		LOAD_FUNC1(glGetShaderPrecisionFormatOES);
	}
}

void init_GLES_OES_single_precision()
{
	glloader_GLES_OES_single_precision = _glloader_GLES_OES_single_precision;

	{
		glDepthRangefOES = NULL;
		glFrustumfOES = NULL;
		glOrthofOES = NULL;
		glClipPlanefOES = NULL;
		glGetClipPlanefOES = NULL;
		glClearDepthfOES = NULL;
	}

	_GLES_OES_single_precision = 0;
	if (glloader_is_supported("GLES_OES_single_precision"))
	{
		_GLES_OES_single_precision = 1;

		LOAD_FUNC1(glDepthRangefOES);
		LOAD_FUNC1(glFrustumfOES);
		LOAD_FUNC1(glOrthofOES);
		LOAD_FUNC1(glClipPlanefOES);
		LOAD_FUNC1(glGetClipPlanefOES);
		LOAD_FUNC1(glClearDepthfOES);
	}
}

void init_GLES_OES_standard_derivatives()
{
	glloader_GLES_OES_standard_derivatives = _glloader_GLES_OES_standard_derivatives;

	_GLES_OES_standard_derivatives = 0;
	if (glloader_is_supported("GLES_OES_standard_derivatives"))
	{
		_GLES_OES_standard_derivatives = 1;
	}
}

void init_GLES_OES_stencil1()
{
	glloader_GLES_OES_stencil1 = _glloader_GLES_OES_stencil1;

	_GLES_OES_stencil1 = 0;
	if (glloader_is_supported("GLES_OES_stencil1"))
	{
		_GLES_OES_stencil1 = 1;
	}
}

void init_GLES_OES_stencil4()
{
	glloader_GLES_OES_stencil4 = _glloader_GLES_OES_stencil4;

	_GLES_OES_stencil4 = 0;
	if (glloader_is_supported("GLES_OES_stencil4"))
	{
		_GLES_OES_stencil4 = 1;
	}
}

void init_GLES_OES_stencil8()
{
	glloader_GLES_OES_stencil8 = _glloader_GLES_OES_stencil8;

	_GLES_OES_stencil8 = 0;
	if (glloader_is_supported("GLES_OES_stencil8"))
	{
		_GLES_OES_stencil8 = 1;
	}
}

void init_GLES_OES_stencil_wrap()
{
	glloader_GLES_OES_stencil_wrap = _glloader_GLES_OES_stencil_wrap;

	_GLES_OES_stencil_wrap = 0;
	if (glloader_is_supported("GLES_OES_stencil_wrap"))
	{
		_GLES_OES_stencil_wrap = 1;
	}
}

void init_GLES_OES_texture_3D()
{
	glloader_GLES_OES_texture_3D = _glloader_GLES_OES_texture_3D;

	{
		glTexImage3DOES = NULL;
		glTexSubImage3DOES = NULL;
		glCopyTexSubImage3DOES = NULL;
		glCompressedTexImage3DOES = NULL;
		glCompressedTexSubImage3DOES = NULL;
		glFramebufferTexture3DOES = NULL;
	}

	_GLES_OES_texture_3D = 0;
	if (glloader_is_supported("GLES_OES_texture_3D"))
	{
		_GLES_OES_texture_3D = 1;

		LOAD_FUNC1(glTexImage3DOES);
		LOAD_FUNC1(glTexSubImage3DOES);
		LOAD_FUNC1(glCopyTexSubImage3DOES);
		LOAD_FUNC1(glCompressedTexImage3DOES);
		LOAD_FUNC1(glCompressedTexSubImage3DOES);
		LOAD_FUNC1(glFramebufferTexture3DOES);
	}
}

void init_GLES_OES_texture_cube_map()
{
	glloader_GLES_OES_texture_cube_map = _glloader_GLES_OES_texture_cube_map;

	{
		glTexGenfOES = NULL;
		glTexGenfvOES = NULL;
		glTexGeniOES = NULL;
		glTexGenivOES = NULL;
		glGetTexGenfvOES = NULL;
		glGetTexGenivOES = NULL;
	}

	_GLES_OES_texture_cube_map = 0;
	if (glloader_is_supported("GLES_OES_texture_cube_map"))
	{
		_GLES_OES_texture_cube_map = 1;

		LOAD_FUNC1(glTexGenfOES);
		LOAD_FUNC1(glTexGenfvOES);
		LOAD_FUNC1(glTexGeniOES);
		LOAD_FUNC1(glTexGenivOES);
		LOAD_FUNC1(glGetTexGenfvOES);
		LOAD_FUNC1(glGetTexGenivOES);
	}
}

void init_GLES_OES_texture_env_crossbar()
{
	glloader_GLES_OES_texture_env_crossbar = _glloader_GLES_OES_texture_env_crossbar;

	_GLES_OES_texture_env_crossbar = 0;
	if (glloader_is_supported("GLES_OES_texture_env_crossbar"))
	{
		_GLES_OES_texture_env_crossbar = 1;
	}
}

void init_GLES_OES_texture_float()
{
	glloader_GLES_OES_texture_float = _glloader_GLES_OES_texture_float;

	_GLES_OES_texture_float = 0;
	if (glloader_is_supported("GLES_OES_texture_float"))
	{
		_GLES_OES_texture_float = 1;
	}
}

void init_GLES_OES_texture_float_linear()
{
	glloader_GLES_OES_texture_float_linear = _glloader_GLES_OES_texture_float_linear;

	_GLES_OES_texture_float_linear = 0;
	if (glloader_is_supported("GLES_OES_texture_float_linear"))
	{
		_GLES_OES_texture_float_linear = 1;
	}
}

void init_GLES_OES_texture_half_float()
{
	glloader_GLES_OES_texture_half_float = _glloader_GLES_OES_texture_half_float;

	_GLES_OES_texture_half_float = 0;
	if (glloader_is_supported("GLES_OES_texture_half_float"))
	{
		_GLES_OES_texture_half_float = 1;
	}
}

void init_GLES_OES_texture_half_float_linear()
{
	glloader_GLES_OES_texture_half_float_linear = _glloader_GLES_OES_texture_half_float_linear;

	_GLES_OES_texture_half_float_linear = 0;
	if (glloader_is_supported("GLES_OES_texture_half_float_linear"))
	{
		_GLES_OES_texture_half_float_linear = 1;
	}
}

void init_GLES_OES_texture_mirrored_repeat()
{
	glloader_GLES_OES_texture_mirrored_repeat = _glloader_GLES_OES_texture_mirrored_repeat;

	_GLES_OES_texture_mirrored_repeat = 0;
	if (glloader_is_supported("GLES_OES_texture_mirrored_repeat"))
	{
		_GLES_OES_texture_mirrored_repeat = 1;
	}
}

void init_GLES_OES_texture_npot()
{
	glloader_GLES_OES_texture_npot = _glloader_GLES_OES_texture_npot;

	_GLES_OES_texture_npot = 0;
	if (glloader_is_supported("GLES_OES_texture_npot"))
	{
		_GLES_OES_texture_npot = 1;
	}
}

void init_GLES_OES_vertex_array_object()
{
	glloader_GLES_OES_vertex_array_object = _glloader_GLES_OES_vertex_array_object;

	{
		glBindVertexArrayOES = NULL;
		glDeleteVertexArraysOES = NULL;
		glGenVertexArraysOES = NULL;
		glIsVertexArrayOES = NULL;
	}

	_GLES_OES_vertex_array_object = 0;
	if (glloader_is_supported("GLES_OES_vertex_array_object"))
	{
		_GLES_OES_vertex_array_object = 1;

		LOAD_FUNC1(glBindVertexArrayOES);
		LOAD_FUNC1(glDeleteVertexArraysOES);
		LOAD_FUNC1(glGenVertexArraysOES);
		LOAD_FUNC1(glIsVertexArrayOES);
	}
}

void init_GLES_OES_vertex_half_float()
{
	glloader_GLES_OES_vertex_half_float = _glloader_GLES_OES_vertex_half_float;

	_GLES_OES_vertex_half_float = 0;
	if (glloader_is_supported("GLES_OES_vertex_half_float"))
	{
		_GLES_OES_vertex_half_float = 1;
	}
}

void init_GLES_OES_vertex_type_10_10_10_2()
{
	glloader_GLES_OES_vertex_type_10_10_10_2 = _glloader_GLES_OES_vertex_type_10_10_10_2;

	_GLES_OES_vertex_type_10_10_10_2 = 0;
	if (glloader_is_supported("GLES_OES_vertex_type_10_10_10_2"))
	{
		_GLES_OES_vertex_type_10_10_10_2 = 1;
	}
}

void init_GLES_QCOM_driver_control()
{
	glloader_GLES_QCOM_driver_control = _glloader_GLES_QCOM_driver_control;

	{
		glGetDriverControlsQCOM = NULL;
		glGetDriverControlStringQCOM = NULL;
		glEnableDriverControlQCOM = NULL;
		glDisableDriverControlQCOM = NULL;
	}

	_GLES_QCOM_driver_control = 0;
	if (glloader_is_supported("GLES_QCOM_driver_control"))
	{
		_GLES_QCOM_driver_control = 1;

		LOAD_FUNC1(glGetDriverControlsQCOM);
		LOAD_FUNC1(glGetDriverControlStringQCOM);
		LOAD_FUNC1(glEnableDriverControlQCOM);
		LOAD_FUNC1(glDisableDriverControlQCOM);
	}
}

void init_GLES_QCOM_extended_get()
{
	glloader_GLES_QCOM_extended_get = _glloader_GLES_QCOM_extended_get;

	{
		glExtGetTexturesQCOM = NULL;
		glExtGetBuffersQCOM = NULL;
		glExtGetRenderbuffersQCOM = NULL;
		glExtGetFramebuffersQCOM = NULL;
		glExtGetTexLevelParameterivQCOM = NULL;
		glExtTexObjectStateOverrideiQCOM = NULL;
		glExtGetTexSubImageQCOM = NULL;
		glExtGetBufferPointervQCOM = NULL;
	}

	_GLES_QCOM_extended_get = 0;
	if (glloader_is_supported("GLES_QCOM_extended_get"))
	{
		_GLES_QCOM_extended_get = 1;

		LOAD_FUNC1(glExtGetTexturesQCOM);
		LOAD_FUNC1(glExtGetBuffersQCOM);
		LOAD_FUNC1(glExtGetRenderbuffersQCOM);
		LOAD_FUNC1(glExtGetFramebuffersQCOM);
		LOAD_FUNC1(glExtGetTexLevelParameterivQCOM);
		LOAD_FUNC1(glExtTexObjectStateOverrideiQCOM);
		LOAD_FUNC1(glExtGetTexSubImageQCOM);
		LOAD_FUNC1(glExtGetBufferPointervQCOM);
	}
}

void init_GLES_QCOM_extended_get2()
{
	glloader_GLES_QCOM_extended_get2 = _glloader_GLES_QCOM_extended_get2;

	{
		glExtGetShadersQCOM = NULL;
		glExtGetProgramsQCOM = NULL;
		glExtIsProgramBinaryQCOM = NULL;
		glExtGetProgramBinarySourceQCOM = NULL;
	}

	_GLES_QCOM_extended_get2 = 0;
	if (glloader_is_supported("GLES_QCOM_extended_get2"))
	{
		_GLES_QCOM_extended_get2 = 1;

		LOAD_FUNC1(glExtGetShadersQCOM);
		LOAD_FUNC1(glExtGetProgramsQCOM);
		LOAD_FUNC1(glExtIsProgramBinaryQCOM);
		LOAD_FUNC1(glExtGetProgramBinarySourceQCOM);
	}
}

void init_GLES_QCOM_performance_monitor_global_mode()
{
	glloader_GLES_QCOM_performance_monitor_global_mode = _glloader_GLES_QCOM_performance_monitor_global_mode;

	_GLES_QCOM_performance_monitor_global_mode = 0;
	if (glloader_is_supported("GLES_QCOM_performance_monitor_global_mode"))
	{
		_GLES_QCOM_performance_monitor_global_mode = 1;
	}
}

void init_GLES_QCOM_tiled_rendering()
{
	glloader_GLES_QCOM_tiled_rendering = _glloader_GLES_QCOM_tiled_rendering;

	{
		glStartTilingQCOM = NULL;
		glEndTilingQCOM = NULL;
	}

	_GLES_QCOM_tiled_rendering = 0;
	if (glloader_is_supported("GLES_QCOM_tiled_rendering"))
	{
		_GLES_QCOM_tiled_rendering = 1;

		LOAD_FUNC1(glStartTilingQCOM);
		LOAD_FUNC1(glEndTilingQCOM);
	}
}

void init_GLES_QCOM_writeonly_rendering()
{
	glloader_GLES_QCOM_writeonly_rendering = _glloader_GLES_QCOM_writeonly_rendering;

	_GLES_QCOM_writeonly_rendering = 0;
	if (glloader_is_supported("GLES_QCOM_writeonly_rendering"))
	{
		_GLES_QCOM_writeonly_rendering = 1;
	}
}

void init_GLES_VERSION_1_0()
{
	glloader_GLES_VERSION_1_0 = _glloader_GLES_VERSION_1_0;

	_GLES_VERSION_1_0 = 0;
	if (glloader_is_supported("GLES_VERSION_1_0"))
	{
		_GLES_VERSION_1_0 = 1;

		LOAD_FUNC1(glActiveTexture);
		LOAD_FUNC1(glAlphaFunc);
		LOAD_FUNC1(glAlphaFuncx);
		LOAD_FUNC1(glBindTexture);
		LOAD_FUNC1(glBlendFunc);
		LOAD_FUNC1(glClear);
		LOAD_FUNC1(glClearColor);
		LOAD_FUNC1(glClearColorx);
		LOAD_FUNC1(glClearDepthf);
		LOAD_FUNC1(glClearDepthx);
		LOAD_FUNC1(glClearStencil);
		LOAD_FUNC1(glClientActiveTexture);
		LOAD_FUNC1(glColor4f);
		LOAD_FUNC1(glColor4x);
		LOAD_FUNC1(glColorMask);
		LOAD_FUNC1(glColorPointer);
		LOAD_FUNC1(glCompressedTexImage2D);
		LOAD_FUNC1(glCompressedTexSubImage2D);
		LOAD_FUNC1(glCopyTexImage2D);
		LOAD_FUNC1(glCopyTexSubImage2D);
		LOAD_FUNC1(glCullFace);
		LOAD_FUNC1(glDeleteTextures);
		LOAD_FUNC1(glDepthFunc);
		LOAD_FUNC1(glDepthMask);
		LOAD_FUNC1(glDepthRangef);
		LOAD_FUNC1(glDepthRangex);
		LOAD_FUNC1(glDisable);
		LOAD_FUNC1(glDisableClientState);
		LOAD_FUNC1(glDrawArrays);
		LOAD_FUNC1(glDrawElements);
		LOAD_FUNC1(glEnable);
		LOAD_FUNC1(glEnableClientState);
		LOAD_FUNC1(glFinish);
		LOAD_FUNC1(glFlush);
		LOAD_FUNC1(glFogf);
		LOAD_FUNC1(glFogfv);
		LOAD_FUNC1(glFogx);
		LOAD_FUNC1(glFogxv);
		LOAD_FUNC1(glFrontFace);
		LOAD_FUNC1(glFrustumf);
		LOAD_FUNC1(glFrustumx);
		LOAD_FUNC1(glGenTextures);
		LOAD_FUNC1(glGetError);
		LOAD_FUNC1(glGetIntegerv);
		LOAD_FUNC1(glGetString);
		LOAD_FUNC1(glHint);
		LOAD_FUNC1(glLightModelf);
		LOAD_FUNC1(glLightModelfv);
		LOAD_FUNC1(glLightModelx);
		LOAD_FUNC1(glLightModelxv);
		LOAD_FUNC1(glLightf);
		LOAD_FUNC1(glLightfv);
		LOAD_FUNC1(glLightx);
		LOAD_FUNC1(glLightxv);
		LOAD_FUNC1(glLineWidth);
		LOAD_FUNC1(glLineWidthx);
		LOAD_FUNC1(glLoadIdentity);
		LOAD_FUNC1(glLoadMatrixf);
		LOAD_FUNC1(glLoadMatrixx);
		LOAD_FUNC1(glLogicOp);
		LOAD_FUNC1(glMaterialf);
		LOAD_FUNC1(glMaterialfv);
		LOAD_FUNC1(glMaterialx);
		LOAD_FUNC1(glMaterialxv);
		LOAD_FUNC1(glMatrixMode);
		LOAD_FUNC1(glMultMatrixf);
		LOAD_FUNC1(glMultMatrixx);
		LOAD_FUNC1(glMultiTexCoord4f);
		LOAD_FUNC1(glMultiTexCoord4x);
		LOAD_FUNC1(glNormal3f);
		LOAD_FUNC1(glNormal3x);
		LOAD_FUNC1(glNormalPointer);
		LOAD_FUNC1(glOrthof);
		LOAD_FUNC1(glOrthox);
		LOAD_FUNC1(glPixelStorei);
		LOAD_FUNC1(glPointSize);
		LOAD_FUNC1(glPointSizex);
		LOAD_FUNC1(glPolygonOffset);
		LOAD_FUNC1(glPolygonOffsetx);
		LOAD_FUNC1(glPopMatrix);
		LOAD_FUNC1(glPushMatrix);
		LOAD_FUNC1(glReadPixels);
		LOAD_FUNC1(glRotatef);
		LOAD_FUNC1(glRotatex);
		LOAD_FUNC1(glSampleCoverage);
		LOAD_FUNC1(glSampleCoveragex);
		LOAD_FUNC1(glScalef);
		LOAD_FUNC1(glScalex);
		LOAD_FUNC1(glScissor);
		LOAD_FUNC1(glShadeModel);
		LOAD_FUNC1(glStencilFunc);
		LOAD_FUNC1(glStencilMask);
		LOAD_FUNC1(glStencilOp);
		LOAD_FUNC1(glTexCoordPointer);
		LOAD_FUNC1(glTexEnvf);
		LOAD_FUNC1(glTexEnvfv);
		LOAD_FUNC1(glTexEnvx);
		LOAD_FUNC1(glTexEnvxv);
		LOAD_FUNC1(glTexImage2D);
		LOAD_FUNC1(glTexParameterf);
		LOAD_FUNC1(glTexParameterx);
		LOAD_FUNC1(glTexSubImage2D);
		LOAD_FUNC1(glTranslatef);
		LOAD_FUNC1(glVertexPointer);
		LOAD_FUNC1(glTranslatex);
		LOAD_FUNC1(glViewport);
	}
}

void init_GLES_VERSION_1_1()
{
	glloader_GLES_VERSION_1_1 = _glloader_GLES_VERSION_1_1;

	_GLES_VERSION_1_1 = 0;
	if (glloader_is_supported("GLES_VERSION_1_1"))
	{
		_GLES_VERSION_1_1 = 1;

		LOAD_FUNC1(glClipPlanef);
		LOAD_FUNC1(glGetClipPlanef);
		LOAD_FUNC1(glGetFloatv);
		LOAD_FUNC1(glGetLightfv);
		LOAD_FUNC1(glGetMaterialfv);
		LOAD_FUNC1(glGetTexEnvfv);
		LOAD_FUNC1(glGetTexParameterfv);
		LOAD_FUNC1(glPointParameterf);
		LOAD_FUNC1(glPointParameterfv);
		LOAD_FUNC1(glTexParameterfv);
		LOAD_FUNC1(glBindBuffer);
		LOAD_FUNC1(glBufferData);
		LOAD_FUNC1(glBufferSubData);
		LOAD_FUNC1(glClipPlanex);
		LOAD_FUNC1(glColor4ub);
		LOAD_FUNC1(glDeleteBuffers);
		LOAD_FUNC1(glGetBooleanv);
		LOAD_FUNC1(glGetBufferParameteriv);
		LOAD_FUNC1(glGetClipPlanex);
		LOAD_FUNC1(glGenBuffers);
		LOAD_FUNC1(glGetFixedv);
		LOAD_FUNC1(glGetLightxv);
		LOAD_FUNC1(glGetMaterialxv);
		LOAD_FUNC1(glGetPointerv);
		LOAD_FUNC1(glGetString);
		LOAD_FUNC1(glGetTexEnviv);
		LOAD_FUNC1(glGetTexEnvxv);
		LOAD_FUNC1(glGetTexParameteriv);
		LOAD_FUNC1(glGetTexParameterxv);
		LOAD_FUNC1(glIsBuffer);
		LOAD_FUNC1(glIsEnabled);
		LOAD_FUNC1(glIsTexture);
		LOAD_FUNC1(glTexEnvi);
		LOAD_FUNC1(glTexEnviv);
		LOAD_FUNC1(glTexParameteri);
		LOAD_FUNC1(glTexParameteriv);
		LOAD_FUNC1(glTexParameterxv);
	}
}

void init_GLES_VERSION_2_0()
{
	glloader_GLES_VERSION_2_0 = _glloader_GLES_VERSION_2_0;

	_GLES_VERSION_2_0 = 0;
	if (glloader_is_supported("GLES_VERSION_2_0"))
	{
		_GLES_VERSION_2_0 = 1;

		LOAD_FUNC1(glAttachShader);
		LOAD_FUNC1(glBindAttribLocation);
		LOAD_FUNC1(glBindFramebuffer);
		LOAD_FUNC1(glBindRenderbuffer);
		LOAD_FUNC1(glBlendColor);
		LOAD_FUNC1(glBlendEquation);
		LOAD_FUNC1(glBlendEquationSeparate);
		LOAD_FUNC1(glBlendFuncSeparate);
		LOAD_FUNC1(glCheckFramebufferStatus);
		LOAD_FUNC1(glCompileShader);
		LOAD_FUNC1(glCreateProgram);
		LOAD_FUNC1(glCreateShader);
		LOAD_FUNC1(glDeleteFramebuffers);
		LOAD_FUNC1(glDeleteProgram);
		LOAD_FUNC1(glDeleteRenderbuffers);
		LOAD_FUNC1(glDeleteShader);
		LOAD_FUNC1(glDetachShader);
		LOAD_FUNC1(glDisableVertexAttribArray);
		LOAD_FUNC1(glEnableVertexAttribArray);
		LOAD_FUNC1(glFramebufferRenderbuffer);
		LOAD_FUNC1(glFramebufferTexture2D);
		LOAD_FUNC1(glGenerateMipmap);
		LOAD_FUNC1(glGenFramebuffers);
		LOAD_FUNC1(glGenRenderbuffers);
		LOAD_FUNC1(glGetActiveAttrib);
		LOAD_FUNC1(glGetActiveUniform);
		LOAD_FUNC1(glGetAttachedShaders);
		LOAD_FUNC1(glGetAttribLocation);
		LOAD_FUNC1(glGetFramebufferAttachmentParameteriv);
		LOAD_FUNC1(glGetProgramiv);
		LOAD_FUNC1(glGetProgramInfoLog);
		LOAD_FUNC1(glGetRenderbufferParameteriv);
		LOAD_FUNC1(glGetShaderiv);
		LOAD_FUNC1(glGetShaderInfoLog);
		LOAD_FUNC1(glGetShaderPrecisionFormat);
		LOAD_FUNC1(glGetShaderSource);
		LOAD_FUNC1(glGetUniformfv);
		LOAD_FUNC1(glGetUniformiv);
		LOAD_FUNC1(glGetUniformLocation);
		LOAD_FUNC1(glGetVertexAttribfv);
		LOAD_FUNC1(glGetVertexAttribiv);
		LOAD_FUNC1(glGetVertexAttribPointerv);
		LOAD_FUNC1(glIsFramebuffer);
		LOAD_FUNC1(glIsProgram);
		LOAD_FUNC1(glIsRenderbuffer);
		LOAD_FUNC1(glIsShader);
		LOAD_FUNC1(glLinkProgram);
		LOAD_FUNC1(glReleaseShaderCompiler);
		LOAD_FUNC1(glRenderbufferStorage);
		LOAD_FUNC1(glShaderBinary);
		LOAD_FUNC1(glShaderSource);
		LOAD_FUNC1(glStencilFuncSeparate);
		LOAD_FUNC1(glStencilMaskSeparate);
		LOAD_FUNC1(glStencilOpSeparate);
		LOAD_FUNC1(glUniform1f);
		LOAD_FUNC1(glUniform2f);
		LOAD_FUNC1(glUniform3f);
		LOAD_FUNC1(glUniform4f);
		LOAD_FUNC1(glUniform1i);
		LOAD_FUNC1(glUniform2i);
		LOAD_FUNC1(glUniform3i);
		LOAD_FUNC1(glUniform4i);
		LOAD_FUNC1(glUniform1fv);
		LOAD_FUNC1(glUniform2fv);
		LOAD_FUNC1(glUniform3fv);
		LOAD_FUNC1(glUniform4fv);
		LOAD_FUNC1(glUniform1iv);
		LOAD_FUNC1(glUniform2iv);
		LOAD_FUNC1(glUniform3iv);
		LOAD_FUNC1(glUniform4iv);
		LOAD_FUNC1(glUniformMatrix2fv);
		LOAD_FUNC1(glUniformMatrix3fv);
		LOAD_FUNC1(glUniformMatrix4fv);
		LOAD_FUNC1(glUseProgram);
		LOAD_FUNC1(glValidateProgram);
		LOAD_FUNC1(glVertexAttrib1f);
		LOAD_FUNC1(glVertexAttrib1fv);
		LOAD_FUNC1(glVertexAttrib2f);
		LOAD_FUNC1(glVertexAttrib2fv);
		LOAD_FUNC1(glVertexAttrib3f);
		LOAD_FUNC1(glVertexAttrib3fv);
		LOAD_FUNC1(glVertexAttrib4f);
		LOAD_FUNC1(glVertexAttrib4fv);
		LOAD_FUNC1(glVertexAttribPointer);
	}
}

void init_GLES_VIV_shader_binary()
{
	glloader_GLES_VIV_shader_binary = _glloader_GLES_VIV_shader_binary;

	_GLES_VIV_shader_binary = 0;
	if (glloader_is_supported("GLES_VIV_shader_binary"))
	{
		_GLES_VIV_shader_binary = 1;
	}
}


void gles_init()
{
	init_GLES_AMD_compressed_3DC_texture();
	init_GLES_AMD_compressed_ATC_texture();
	init_GLES_AMD_performance_monitor();
	init_GLES_AMD_program_binary_Z400();
	init_GLES_ANGLE_framebuffer_blit();
	init_GLES_ANGLE_framebuffer_multisample();
	init_GLES_ANGLE_instanced_arrays();
	init_GLES_ANGLE_pack_reverse_row_order();
	init_GLES_ANGLE_texture_compression_dxt3();
	init_GLES_ANGLE_texture_compression_dxt5();
	init_GLES_ANGLE_texture_usage();
	init_GLES_ANGLE_translated_shader_source();
	init_GLES_APPLE_copy_texture_levels();
	init_GLES_APPLE_framebuffer_multisample();
	init_GLES_APPLE_rgb_422();
	init_GLES_APPLE_sync();
	init_GLES_APPLE_texture_2D_limited_npot();
	init_GLES_APPLE_texture_format_BGRA8888();
	init_GLES_APPLE_texture_max_level();
	init_GLES_ARM_mali_shader_binary();
	init_GLES_ARM_rgba8();
	init_GLES_EXT_blend_minmax();
	init_GLES_EXT_color_buffer_half_float();
	init_GLES_EXT_debug_label();
	init_GLES_EXT_debug_marker();
	init_GLES_EXT_discard_framebuffer();
	init_GLES_EXT_frag_depth();
	init_GLES_EXT_map_buffer_range();
	init_GLES_EXT_multisampled_render_to_texture();
	init_GLES_EXT_multiview_draw_buffers();
	init_GLES_EXT_multi_draw_arrays();
	init_GLES_EXT_occlusion_query_boolean();
	init_GLES_read_format_bgra();
	init_GL_EXT_robustness();
	init_GLES_EXT_separate_shader_objects();
	init_GLES_EXT_shader_framebuffer_fetch();
	init_GLES_EXT_shader_texture_lod();
	init_GLES_EXT_shadow_samplers();
	init_GLES_EXT_sRGB();
	init_GLES_EXT_texture_compression_dxt1();
	init_GLES_EXT_texture_compression_latc();
	init_GLES_EXT_texture_compression_s3tc();
	init_GLES_EXT_texture_filter_anisotropic();
	init_GLES_EXT_texture_format_BGRA8888();
	init_GLES_EXT_texture_lod_bias();
	init_GLES_EXT_texture_rg();
	init_GLES_EXT_texture_storage();
	init_GLES_EXT_texture_type_2_10_10_10_REV();
	init_GLES_EXT_unpack_subimage();
	init_GLES_FJ_shader_binary_GCCSO();
	init_GLES_IMG_multisampled_render_to_texture();
	init_GLES_IMG_program_binary();
	init_GLES_IMG_read_format();
	init_GLES_shader_binary();
	init_GLES_IMG_texture_compression_pvrtc();
	init_GLES_IMG_texture_env_enhanced_fixed_function();
	init_GLES_IMG_user_clip_plane();
	init_GLES_NV_coverage_sample();
	init_GLES_NV_depth_nonlinear();
	init_GLES_NV_draw_buffers();
	init_GLES_NV_draw_texture();
	init_GLES_NV_EGL_stream_consumer_external();
	init_GLES_NV_fbo_color_attachments();
	init_GLES_NV_fence();
	init_GLES_NV_read_buffer();
	init_GLES_NV_read_depth();
	init_GLES_NV_read_depth_stencil();
	init_GLES_NV_read_stencil();
	init_GLES_NV_texture_compression_s3tc_update();
	init_GLES_NV_texture_npot_2D_mipmap();
	init_GLES_OES_blend_equation_separate();
	init_GLES_OES_blend_func_separate();
	init_GLES_OES_blend_subtract();
	init_GLES_OES_byte_coordinates();
	init_GLES_OES_compressed_ETC1_RGB8_texture();
	init_GLES_OES_compressed_paletted_texture();
	init_GLES_OES_depth24();
	init_GLES_OES_depth32();
	init_GLES_OES_depth_texture();
	init_GLES_OES_draw_texture();
	init_GLES_OES_EGL_image();
	init_GLES_OES_EGL_image_external();
	init_GLES_OES_element_index_uint();
	init_GLES_OES_extended_matrix_palette();
	init_GLES_OES_fbo_render_mipmap();
	init_GLES_OES_fixed_point();
	init_GLES_OES_fragment_precision_high();
	init_GLES_OES_framebuffer_object();
	init_GLES_OES_get_program_binary();
	init_GLES_OES_mapbuffer();
	init_GLES_OES_matrix_get();
	init_GLES_OES_matrix_palette();
	init_GLES_OES_packed_depth_stencil();
	init_GLES_OES_point_size_array();
	init_GLES_OES_point_sprite();
	init_GLES_OES_query_matrix();
	init_GLES_OES_read_format();
	init_GLES_OES_required_internalformat();
	init_GLES_OES_rgb8_rgba8();
	init_GLES_OES_shader_binary();
	init_GLES_OES_shader_source();
	init_GLES_OES_single_precision();
	init_GLES_OES_standard_derivatives();
	init_GLES_OES_stencil1();
	init_GLES_OES_stencil4();
	init_GLES_OES_stencil8();
	init_GLES_OES_stencil_wrap();
	init_GLES_OES_texture_3D();
	init_GLES_OES_texture_cube_map();
	init_GLES_OES_texture_env_crossbar();
	init_GLES_OES_texture_float();
	init_GLES_OES_texture_float_linear();
	init_GLES_OES_texture_half_float();
	init_GLES_OES_texture_half_float_linear();
	init_GLES_OES_texture_mirrored_repeat();
	init_GLES_OES_texture_npot();
	init_GLES_OES_vertex_array_object();
	init_GLES_OES_vertex_half_float();
	init_GLES_OES_vertex_type_10_10_10_2();
	init_GLES_QCOM_driver_control();
	init_GLES_QCOM_extended_get();
	init_GLES_QCOM_extended_get2();
	init_GLES_QCOM_performance_monitor_global_mode();
	init_GLES_QCOM_tiled_rendering();
	init_GLES_QCOM_writeonly_rendering();
	init_GLES_VERSION_1_0();
	init_GLES_VERSION_1_1();
	init_GLES_VERSION_2_0();
	init_GLES_VIV_shader_binary();
}

#ifdef __cplusplus
}
#endif

#endif		/* GLLOADER_GLES */
