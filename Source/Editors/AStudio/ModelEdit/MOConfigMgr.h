#pragma once

#include <list>
#include <string>
#include <Core/AresSingleton.hpp>
#include <Core/3rdParty/RapidXml/rapidxml.hpp>
#include <boost/signals2/signal.hpp>

using namespace std;
using namespace rapidxml;
using namespace boost::signals2;

namespace Ares
{
	//-------------------------------------------
	// Lightmass配置管理器 2012-4-11 帝林
	//-------------------------------------------
	class MOConfigMgr : public Singleton<MOConfigMgr>
	{
	public:
		typedef list<string> FileList;

		MOConfigMgr();
		~MOConfigMgr();

	public:
		// 加载
		void Load();

		// 保存
		void Save();

		// 添加到文件列表
		void AddToRecentlyFile( const char* fileName);

		// 设置摄像机参数
		void AddToResDirectorys( const char* resDir);

	private:
		// 加载最近打开文件列表
		void LoadRencentlyFiles( xml_document<>& doc);

		// 保存最近打开文件列表
		void SaveRencentlyFiles( xml_document<>& doc, xml_node<>* rootNode);

		// 加载摄像机设置
		void LoadResDirectory(  xml_document<>& doc);

	public:
		// Recent files changed signals
		signal<void( const FileList&)> Signal_OnRencentFilesChanged;

		// CameraSettings changed signals
		signal<void( const FileList&)> Signal_ResDirectorysChanged;

	private:
		string			m_fileName;			// 配置文件名

		FileList		m_recentlyFiles;	// 最近打开文件列表(顺序存放)
		FileList		m_resDirectorys;	// 资源目录
	};
}