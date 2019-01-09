#include <stdio.h>
#include <Engine/RenderSystem/AresVp8Decoder.h>
#include <string>

using namespace std;

#define IVF_FILE_HDR_SZ  (32)
#define IVF_FRAME_HDR_SZ (12)

namespace Ares
{
	static unsigned int mem_get_le32(const unsigned char *mem) 
	{
		return (mem[3] << 24)|(mem[2] << 16)|(mem[1] << 8)|(mem[0]);
	}

	// 构造函数
	Vp8Decoder::Vp8Decoder()
	{

	}

	// 析构函数
	Vp8Decoder::~Vp8Decoder()
	{

	}

	// 简单解码(测试用)
	void Vp8Decoder::SimpleDecode()
	{
		FILE*			 infile;
		vpx_codec_ctx_t  codec;
		int              flags = 0, frame_cnt = 0;
		unsigned char    file_hdr[IVF_FILE_HDR_SZ];
		unsigned char    frame_hdr[IVF_FRAME_HDR_SZ];
		unsigned char    frame[256*1024];

		infile = fopen( "data\\movies\\fx.webm", "rb");

		if( fread(file_hdr, 1, IVF_FILE_HDR_SZ, infile) != IVF_FILE_HDR_SZ)
			return;

		if( !( file_hdr[0]=='D' && file_hdr[1]=='K' && file_hdr[2]=='I'&& file_hdr[3]=='F'))
		{
			// is not an ivf file
			//return;
		}

		string ifaceName = vpx_codec_iface_name( vpx_codec_vp8_dx());

		// Initialize codec                                                  
		if( vpx_codec_dec_init(&codec, vpx_codec_vp8_dx(), NULL, flags))
		{
			// Failed to initialize decoder
			ReportError( codec);
		}

		/* Read each frame */
		while(fread(frame_hdr, 1, IVF_FRAME_HDR_SZ, infile) == IVF_FRAME_HDR_SZ) 
		{
			int frame_sz = mem_get_le32(frame_hdr);

			frame_cnt++;
			if( frame_sz>sizeof(frame))
			{
				//("Frame %d data too big for example code buffer", frame_sz);
				frame_sz = sizeof( frame);
			}

			if(fread(frame, 1, frame_sz, infile) != frame_sz)
			{
				// die("Frame %d failed to read complete frame", frame_cnt);
				ReportError( codec);

				return;
			}


			// Decode the frame 
			if( vpx_codec_decode(&codec, frame, frame_sz, NULL, 0))
			{
				//die_codec(&codec, "Failed to decode frame");
				ReportError( codec);

				return;
			}

			vpx_codec_iter_t  iter = NULL;
			vpx_image_t      *img;

			// Write decoded data to image
			while((img = vpx_codec_get_frame(&codec, &iter))) 
			{ 
				unsigned int plane, y;

				for(plane=0; plane < 3; plane++) 
				{  
					unsigned char *buf =img->planes[plane];
					for(y=0; y < (plane ? (img->d_h + 1) >> 1 : img->d_h); y++) 
					{ 
						// 复制到图片中
						//if(fwrite(buf, 1, (plane ? (img->d_w + 1) >> 1 : img->d_w), outfile)); 

						buf += img->stride[plane];                        
					}
				}
			}
		}

		printf("Processed %d frames.\n",frame_cnt);
		if(vpx_codec_destroy(&codec))
		{
			//die_codec(&codec, "Failed to destroy codec"); 

			return;
		}

		fclose(infile);
	}

	// 错误提示
	void Vp8Decoder::ReportError( vpx_codec_ctx_t& codec)
	{
		string error = vpx_codec_error( &codec);
		const char* detail= vpx_codec_error_detail( &codec) ;

		int a = 10;
	}
}