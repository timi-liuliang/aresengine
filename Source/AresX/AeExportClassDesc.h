#pragma once

#include <max.h>

namespace M1_IGame
{
	//----------------------------------------
	// 插件类描述 2010-08-26   帝林
	//----------------------------------------
	class CAeExportClassDesc : public ClassDesc
	{
	public:
		// show this is create branch
		virtual int	IsPublic();

		// 返回导出类实例
		virtual void* Create(BOOL loading=FALSE);

		// 返回导出类名称
		virtual const char* ClassName();

		// 决定插件类型 dle, dlu 等等
		virtual SClass_ID SuperClassID();

		// 插件的唯一标识符,max用来区分插件
		virtual Class_ID ClassID();

		// 种类 类别
		virtual const char* Category();  

		//// 导出类内部名
		//virtual const char* InternalName();

		// dll 实例句柄
		HINSTANCE HInstance();

	public:
		// 单件模式
		static CAeExportClassDesc* GetInstance();

	private:
		// 私有构造函数
		CAeExportClassDesc(){}

	private:
		static CAeExportClassDesc*  instance;
	};


	// 创建管理器实例 (单件模式)
	inline CAeExportClassDesc* CAeExportClassDesc::GetInstance()
	{
		if ( instance == NULL)
		{
			instance = new CAeExportClassDesc();
		}

		return instance;
	}
}