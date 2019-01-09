//#pragma once
//
//#include <list>
//#include <string>
//#include <Core/AresSingleton.hpp>
//#include <Core/3rdParty/RapidXml/rapidxml.hpp>
//#include <boost/signals2/signal.hpp>
//
//using namespace std;
//using namespace rapidxml;
//using namespace boost::signals2;
//
//namespace Ares
//{
//	//-------------------------------------------
//	// Lightmass配置管理器 2012-4-11 帝林
//	//-------------------------------------------
//	class ConfigMgr : public Singleton<ConfigMgr>
//	{
//	public:
//		typedef list<string> FileList;
//
//	public:
//		// 构造函数
//		ConfigMgr();
//
//		// 析构函数
//		~ConfigMgr();
//
//		// 加载
//		void Load();
//
//		// 保存
//		void Save();
//
//		// 添加到文件列表
//		void AddToRecentlyFile( const char* fileName);
//
//		// 设置程序状态
//		void SetProgramState( const char* pbrt, const char* img, int tabIdx);
//
//	private:
//		// 加载最近打开文件列表
//		void LoadRencentlyFiles( xml_document<>& doc);
//
//		// 保存最近打开文件列表
//		void SaveRencentlyFiles( xml_document<>& doc, xml_node<>* rootNode);
//
//		// 加载上次关闭时状态
//		void LoadProgramState( xml_document<>& doc);
//
//		// 保存程序状态
//		void SaveProgramState( xml_document<>& doc, xml_node<>* rootNode);
//
//	public:
//		// Recent files changed signals
//		signal<void( const FileList&)> Signal_OnRencentFilesChanged;
//
//		// Restore program state
//		signal<void( const char*, const char*, int)> Signal_OnRestoreState;
//
//	private:
//		string			m_fileName;			// 配置文件名
//
//		string			m_lastFileOpen;		// 上次打开文件
//		int				m_tabIdx;			// 所在TabIdx
//		string			m_lastImgOpen;		// 上次打开图象
//
//		FileList		m_recentlyFiles;	// 最近打开文件列表(顺序存放)
//	};
//}