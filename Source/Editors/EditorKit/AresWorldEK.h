#pragma once

namespace Ares
{
	//-----------------------------------------
	// 地形编辑结构体 2010-07-24  帝林
	//-----------------------------------------
	struct STerrainMeshEditorData
	{
		enum EditorType
		{
			EM_UP_HEAVAL,		// 隆起地表
			EM_DEBASE,			// 降低地表
			EM_CALM1,           // 光滑整平
			EM_CALM2,           // 强制整平
			EM_CRAGGEDNESS,		// 陡峭地表
		};

		enum TextureNum
		{
			EM_TEXTURE0 = 0,	// 纹理0	
			EM_TEXTURE1,		// 纹理1	
			EM_TEXTURE2,		// 纹理2	
			EM_TEXTURE3,		// 纹理3	
			EM_TEXTURE4,		// 纹理4	
			EM_TEXTURE5,		// 纹理5	
			EM_TEXTURE6,		// 纹理6	
			EM_TEXTURE7,		// 纹理7	
			EM_TEXTURETOTAL,	// 最大纹理数
		};

		enum  EditorShape
		{
			EM_TYPE_CIRCULAR,	// 圆形	
			EM_TYPE_SQUARE,		// 正方形
		};

		EditorType  type;
		TextureNum  activeTexture;
		int         size;
		EditorShape shape;
		float       height;			// 推平地表时使用	

		STerrainMeshEditorData()
		{
			type = EM_UP_HEAVAL;
			activeTexture = EM_TEXTURE0;
			size = 1;
			shape= EM_TYPE_CIRCULAR;
			height = 0.0f;
		}
	};
}