#pragma once

namespace Ares
{
	//-----------------------------------------
	// 水片编辑结构体 2010-09-12  帝林
	//-----------------------------------------
	struct SWaterEditorData
	{
		enum EditorType
		{
			EM_ET_HEIGHT,		// 以高度为依据
			EM_ET_DEPTH,		// 以深度为依据
		};

		enum EditorSize
		{
			EM_SIZE1 = 1,
			EM_SIZE2 = 2,
			EM_SIZE3 = 3,
			EM_SIZE4 = 5,
			EM_SIZE5 = 8,
		};

		enum  EditorShape
		{
			EM_TYPE_CIRCULAR,	// 圆形	
			EM_TYPE_SQUARE,		// 正方形
		};

		EditorType  type;
		EditorSize  size;
		EditorShape shape;
		float       height;		// 水高			
		float       depth;		// 水深

		SWaterEditorData()
		{
			type = EM_ET_HEIGHT;
			size = EM_SIZE2;
			shape= EM_TYPE_CIRCULAR;
			height = 0.0f;
			depth  = 0.0f;
		}
	};
}