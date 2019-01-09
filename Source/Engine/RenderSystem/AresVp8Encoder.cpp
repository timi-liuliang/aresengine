#include <Engine/RenderSystem/AresVp8Encoder.h>
#include <Engine/AresRoot.h>

#define FOURCC    0x30385056

namespace Ares
{
	static void mem_put_le16(char *mem, unsigned int val) 
	{
		mem[0] = val;
		mem[1] = val>>8;
	}

	static void mem_put_le32(char *mem, unsigned int val) 
	{
		mem[0] = val;
		mem[1] = val>>8;
		mem[2] = val>>16;
		mem[3] = val>>24;
	}

	static void RGBToYCc( char iR, char iG, char iB, unsigned char& oC, unsigned char& oB, unsigned char& oR)
	{
		oC = (unsigned char)( 0.2990 * iR + 0.5870 * iG + 0.1140 * iB);
		oB = (unsigned char)(-0.1686 * iR - 0.3311 * iG + 0.4997 * iB) + 128;
		oR = (unsigned char)( 0.4998 * iR - 0.4185 * iG - 0.0813 * iB) + 128;
	}

	// 构造函数
	Vp8Encoder::Vp8Encoder()
		: m_width( 640)
		, m_height( 480)
		, m_file( NULL)
		, m_flags( 0)
		, m_frameNum( 0)
	{
		m_stats.buf = NULL;
		m_stats.sz  = 0;
	}

	// 析构函数
	Vp8Encoder::~Vp8Encoder()
	{

	}

	// 设置宽
	bool Vp8Encoder::SetWidth( long width) 
	{ 
		if( !(width<16 || width%2))
		{
			m_width=width;

			return true;
		}
			
		ALogSystem.Warning( "VP8Encoder::Invalid width resolution");

		return false;
	}

	// 设置高
	bool Vp8Encoder::SetHeight( long height) 
	{ 
		if( !(height<16 || height%2)) 
		{
			m_height=height;

			return true;
		}

		ALogSystem.Warning( "VP8Encoder::Invalid width resolution");
		
		return false;
	}

	// 开始
	bool Vp8Encoder::Begin( const char* filePath)
	{
		m_frameNum = 0;

		// 1.分配内存
		if( !vpx_img_alloc( &m_image, VPX_IMG_FMT_I420, m_width, m_height, 1))
		{
			ALogSystem.Warning( "Vp8Encoder::Failed to allocate image %ld %ld", m_width, m_height);

			return false;
		}

		// 打开文件
		m_file = fopen( filePath, "wb");
		if( !m_file)
		{
			ALogSystem.Warning( "Failed to open %s for writing %s", filePath);

			return false;
		}

		// 3.
		vpx_codec_iface_name( vpx_codec_vp8_cx());

		// 4.
		vpx_codec_err_t res = vpx_codec_enc_config_default( vpx_codec_vp8_cx(), &m_config, 0);
		if( res)
		{
			ALogSystem.Warning( "Failed to get config");

			return false;
		}

		// 5.配置
		m_config.rc_target_bitrate = m_width * m_height * m_config.rc_target_bitrate / m_config.g_w / m_config.g_h;
		m_config.g_w			   = m_width;
		m_config.g_h			   = m_height;

		// 6.写头
		write_file_header();

		// 7.初始化环境
		if( vpx_codec_enc_init( &m_codec, vpx_codec_vp8_cx(), &m_config, 0))
		{
			ALogSystem.Warning( "Initialize codec failed!");

			return false;
		}

		return true;
	}

	// 附加帧数据
	bool Vp8Encoder::AppendFrame()
	{
		// 把Raw数据编码到m_codec
		write_image();


		vpx_codec_err_t error = vpx_codec_encode( &m_codec, &m_image, m_frameNum, 1, m_flags, VPX_DL_BEST_QUALITY);
		if( error==VPX_CODEC_OK)
		{
			vpx_codec_iter_t		  iter=NULL;
			const vpx_codec_cx_pkt_t* pkt;
			while( pkt=vpx_codec_get_cx_data( &m_codec, &iter))
			{
				switch( pkt->kind)
				{
				case VPX_CODEC_CX_FRAME_PKT:
					{
						write_frame_header( pkt);

						fwrite( pkt->data.frame.buf, 1, pkt->data.frame.sz, m_file);
					}
					break;
				
				case VPX_CODEC_STATS_PKT:
					{
						m_stats.buf = realloc( m_stats.buf, m_stats.sz+pkt->data.twopass_stats.sz);
						if( m_stats.buf)
						{
							std::memcpy( (char*)m_stats.buf + m_stats.sz, pkt->data.twopass_stats.buf, pkt->data.twopass_stats.sz);
							m_stats.sz += pkt->data.twopass_stats.sz;
						}
					}
					break;

				default:
					break;
				}
			}

			m_frameNum++;

			return true;
		}

		return false;
	}

	// 结束
	bool Vp8Encoder::End()
	{
		// try to rewrite the file header with the actual frame count
		if( !fseek( m_file, 0, SEEK_SET))
		{
			write_file_header();
			fflush( m_file);
			fclose( m_file);

			return true;
		}

		return false;
	}

	// 写文件头
	void Vp8Encoder::write_file_header()
	{
		char header[32];

		if( m_config.g_pass != VPX_RC_ONE_PASS && m_config.g_pass != VPX_RC_LAST_PASS)
			return;

		header[0] = 'D';
		header[1] = 'K';
		header[2] = 'I';
		header[3] = 'F';
		mem_put_le16(header+4,  0);							/* version */
		mem_put_le16(header+6,  32);						/* headersize */
		mem_put_le32(header+8,  FOURCC);					/* headersize */
		mem_put_le16(header+12, m_config.g_w);				/* width */
		mem_put_le16(header+14, m_config.g_h);				/* height */
		mem_put_le32(header+16, m_config.g_timebase.den);	/* rate */
		mem_put_le32(header+20, m_config.g_timebase.num);	/* scale */
		mem_put_le32(header+24, m_frameNum);				/* length */
		mem_put_le32(header+28, 0);							/* unused */

		fwrite(header, 1, 32, m_file);
	}

	// 写帧头
	void Vp8Encoder::write_frame_header( const vpx_codec_cx_pkt_t* pkt)
	{
		char             header[12];
		vpx_codec_pts_t  pts;

		if( pkt->kind != VPX_CODEC_CX_FRAME_PKT)
			return;

		pts = pkt->data.frame.pts;
		mem_put_le32(header,   pkt->data.frame.sz);
		mem_put_le32(header+4, pts&0xFFFFFFFF);
		mem_put_le32(header+8, pts >> 32);

		fwrite(header, 1, 12, m_file);
	}

	// 写纹理数据
	void Vp8Encoder::write_image()
	{
		size_t to_read = m_image.w * m_image.h * 3/ 2;

		memset( m_image.planes[0], 1, to_read);

		//for( UINT i=0; i<m_image.w; i++)
		//{
		//	for( UINT j=0; j<m_image.h; j++)
		//	{
		//		int idx = j*m_image.w+i;

		//		RGBToYCc( 244, 244, 244, m_image.planes[0][idx], m_image.planes[1][idx], m_image.planes[2][idx]);
		//	}
		//}
	}
}