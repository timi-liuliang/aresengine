#pragma once

#include <Core/3rdParty/Vpx/vpx_decoder.h>
#include <Core/3rdParty/Vpx/vp8dx.h>

namespace Ares
{
	//----------------------------------
	// Vp8解码器 2013-1-18 帝林
	//----------------------------------
	class Vp8Decoder
	{
	public:
		Vp8Decoder();
		~Vp8Decoder();

		// 简单解码(测试用)
		void SimpleDecode();

	private:
		// 错误提示
		void ReportError( vpx_codec_ctx_t& codec);
	};
}