#pragma once

#include <string>
#include "Engine/SceneSystem/SceneNode.h"

using namespace std;

namespace Ares
{
	//--------------------------------
	// Attachment 2011-08-08 帝林
	// 说明：
	//     角色附属,武器等
	//--------------------------------
	struct SAttachment
	{
		string			 m_boneName;	// 骨骼名
		int				 m_boneIdx;		// 骨骼索引
		//s_ptr<SceneNode>  m_sceneNode;	// 结点(因绑定物体可能为声音等,所以尽量采用SceneNode)	
	};
}