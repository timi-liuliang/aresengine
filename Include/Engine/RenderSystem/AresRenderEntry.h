#pragma  once

#include "../ResSystem/AresResource.h"

namespace Ares
{
	//----------------------------------------------------------------
	// SRenderEntry 2010-04-02 17:59 -- 未知     帝林
	// A Render Entry is a 20 byte piece of data used to represent
	// a desired render operation in the queue. the top 12 bytes 
	// represent a numerical value which allows us to sort these 
	// objects into a optimal render order.
	// 
	//----------------------------------------------------------------

	// these flags are passed to the render callbacks to let the object
	// konw which of it's ender components need to be activated
	enum ActivationFlagByts
	{
		EM_ACTIVATE_RENDER_METHOD = 0,
		EM_ACTIVATE_RENDER_METHOD_PASS,
		//EM_ACTIVATE_RENDER_METHOD_PARAM,
		//EM_ACTIVATE_RENDER_METHOD_LOD,
		EM_ACTIVATE_MODEL,
		EM_ACTIVATE_MODEL_PARAMA,
		EM_ACTIVATE_MODEL_PARAMB,
		EM_ACTIVATE_SURFACE_MATERIAL,

		EM_TOTAL_ACTIVATION_FLAGS
	};

	class IRenderEntity;
	struct SRenderEntry
	{
		// we turn on byte packing to ensure a tight fit
#pragma pack(1)

		union
		{
			// this union allows us to sort our 
			// render parameters as 3 DWORD value
			struct  
			{
				UINT32  sortValueA;
				UINT32  sortValueB;
				UINT32  sortValueC;
				UINT64  sortValueD;
			};

			// The following members map to sortValueA
			struct  
			{
				//UINT8 detailLevel: 4;
				BYTE modelType  : 2;
				//UINT8 boneCount  : 2;

				// which render pass to use(使用哪个渲染pass)
				BYTE renderPass;

				// which effect file to use;
				Handle  hEffectFile;

				// 顶点格式声明句柄
				Handle	hDeclaration;

				// primary vertex buffer or model index
				Handle hModel;

				// The following member map to sort ValueB
				WORD   modelParamA;

				// index buffer or model subset
				WORD  modelParamB;

				// the surface material used;
				Handle hSurfaceMaterial;

				WORD       renderQueue;		// 渲染队列
				WORD       userData;
			};
		};

		// Additional unsorted fields
		IRenderEntity*	 object;

		// we can go back to default packing now
#pragma pack()

		// these enum values are used to set the modelType value above.This
		// tells the queue if the model data represents a model resource or
		// a set of vertex and index buffers
		enum ETypeFlags
		{
			EM_BUFFER_ENTRY = 0,
			EM_MODEL_ENTRY,
		};

		SRenderEntry(){}
		~SRenderEntry() {}

		// Clear the Entry to default values
		void Clear()
		{
			sortValueA = 0;
			sortValueB = 0;
			sortValueC = 0;
			sortValueD = 0;
			object     = 0;
		}

		// 两entry是否相同
		static BOOL IsRenderEntryEqual( SRenderEntry& e1, SRenderEntry& e2)
		{
			return	e1.sortValueA == e2.sortValueA	&&	e1.sortValueB == e2.sortValueB &&
					e1.sortValueC == e2.sortValueC	&&	e1.sortValueD == e2.sortValueD;
		}
	};
}