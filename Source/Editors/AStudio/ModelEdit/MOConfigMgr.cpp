#include <algorithm>
#include "MOConfigMgr.h"
#include <boost/lexical_cast.hpp>
#include <Core/3rdParty/RapidXml/rapidxml_utils.hpp>
#include <Core/3rdParty/RapidXml/rapidxml_print.hpp>

using namespace boost;

namespace Ares
{
	template<> MOConfigMgr* Singleton<MOConfigMgr>::m_singleton = new MOConfigMgr;

	// 构造函数
	MOConfigMgr::MOConfigMgr()
	{
		m_fileName = "config\\model_edit.xml";
	}

	// 析构函数
	MOConfigMgr::~MOConfigMgr()
	{

	}

	// 加载
	void MOConfigMgr::Load()
	{
		try
		{
			file<> fdoc( m_fileName.c_str());
			xml_document<>  doc;
			doc.parse<0>( fdoc.data());

			LoadRencentlyFiles( doc);

			LoadResDirectory( doc);
		}
		catch ( ...)
		{

		}
	}

	// 保存
	void MOConfigMgr::Save()
	{
		list<string>  saveHelper;

		xml_document<>  doc;
		xml_node<>* xmlNode  = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		xml_node<>* rootNode =  doc.allocate_node(node_element,"config");  

		doc.append_node( xmlNode);
		doc.append_node( rootNode);

		SaveRencentlyFiles( doc, rootNode);

		// 写文件
		std::ofstream out( m_fileName.c_str());
		out << doc;
	}

	// 添加到文件列表
	void MOConfigMgr::AddToRecentlyFile( const char* fileName)
	{
		if( fileName)
		{
			// 1.若已存在于列表 交换
			FileList::iterator it = std::find( m_recentlyFiles.begin(), m_recentlyFiles.end(), fileName);
			if( it != m_recentlyFiles.end())
			{
				m_recentlyFiles.erase( it);

				// 保证最近文件数目不超过10
				while( m_recentlyFiles.size()>=10)
					m_recentlyFiles.pop_back();
			}

			m_recentlyFiles.push_front( fileName);
		}

		// 触发信号
		Signal_OnRencentFilesChanged( m_recentlyFiles);

		Save();
	}

	// 设置摄像机参数
	void MOConfigMgr::AddToResDirectorys( const char* resDir)
	{ 
	}

	// 加载最近打开文件列表
	void MOConfigMgr::LoadRencentlyFiles( xml_document<>& doc)
	{
		m_recentlyFiles.clear();

		// 主节点, 势力结点
		xml_node<>* pRootNode = doc.first_node("config");
		if( !pRootNode) return;

		xml_node<>* pRencentFilesNode = pRootNode->first_node( "rencent_files");
		if( !pRencentFilesNode) return;

		xml_node<>* pRencentFileNode  = pRencentFilesNode->first_node( "file");
		for ( ; pRencentFileNode ;pRencentFileNode=pRencentFileNode->next_sibling( "file"))
		{
			m_recentlyFiles.push_back( pRencentFileNode->first_attribute("path")->value());
		}

		// 触发信号
		Signal_OnRencentFilesChanged( m_recentlyFiles);
	}

	// 保存最近打开文件列表
	void MOConfigMgr::SaveRencentlyFiles( xml_document<>& doc, xml_node<>* rootNode)
	{
		if( rootNode)
		{
			xml_node<>* rencentFilesNode = doc.allocate_node(node_element,"rencent_files");  
			rootNode->append_node( rencentFilesNode);

			for( FileList::iterator it=m_recentlyFiles.begin(); it!=m_recentlyFiles.end(); it++)
			{
				xml_node<>* tNode = doc.allocate_node(node_element, "file");
				xml_attribute<>* pAttribute = doc.allocate_attribute( "path", it->c_str());

				tNode->append_attribute(pAttribute);
				rencentFilesNode->append_node( tNode);
			}
		}
	}

	// 加载摄像机设置
	void MOConfigMgr::LoadResDirectory( xml_document<>& doc)
	{
		m_resDirectorys.clear();

		// 主节点, 势力结点
		xml_node<>* pRootNode = doc.first_node("config");
		if( !pRootNode)
			return;

		xml_node<>* pResDirectorysNode = pRootNode->first_node( "directorys");
		if( !pResDirectorysNode) return;

		xml_node<>* pDirectoryNode  = pResDirectorysNode->first_node( "directory");
		for ( ; pDirectoryNode ;pDirectoryNode=pDirectoryNode->next_sibling( "directory"))
		{
			m_resDirectorys.push_back( pDirectoryNode->first_attribute("path")->value());
		}

		Signal_ResDirectorysChanged( m_resDirectorys);
	}
}