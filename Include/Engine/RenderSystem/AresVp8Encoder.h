#pragma once

#include <stdio.h>
#include <Core/3rdParty/Vpx/vpx_encoder.h>
#include <Core/3rdParty/Vpx/vp8cx.h>

namespace Ares
{
	//-------------------------------
	// Webm编码器 2013-1-19 帝林
	//-------------------------------
	class Vp8Encoder
	{
	public:
		Vp8Encoder();
		~Vp8Encoder();

		// 设置宽
		bool SetWidth( long width);

		// 设置高
		bool SetHeight( long height);

		// 开始
		bool Begin( const char* filePath);

		// 附加帧数据
		bool AppendFrame();

		// 结束
		bool End();

	private:
		// 写文件头
		void write_file_header();

		// 写帧头
		void write_frame_header( const vpx_codec_cx_pkt_t* pkt);

		// 写纹理数据
		void write_image();

	private:
		long				m_width;	// 视频宽
		long				m_height;	// 视频高

		FILE*				m_file;	// 输出
		vpx_codec_ctx_t		m_codec;
		vpx_image_t			m_image;
		vpx_codec_enc_cfg_t	m_config;	// 配置
		int					m_frameNum;	// 帧数量
		int					m_flags;	// 标记
		vpx_fixed_buf_t		m_stats;
	};
}