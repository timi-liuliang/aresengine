#pragma once

#include <string>
#include <vector>
#include "AresPlatform.h"

using namespace std;

namespace Ares
{
	//----------------------------------
	// 文件工具 2013-7-7 帝林
	//----------------------------------
	class FileUtils
	{
	public:
		FileUtils();
		~FileUtils();

		// 新建实例
		static FileUtils* Create( const char* rootPath="");
		
		// 获取资源数据
		virtual void ExtractFileData( vector<BYTE>& oData, const char* fileName);
		
		// 获取完整路径
		virtual string FullPath( const char* fileName);
		
		// 添加搜寻路径
		void AddSearchPath( const char* path);

		// 递归添加搜寻路径
		void AddSearchPathRecursive( const char* path);

		// 查找指定目录下文件
		void SearchFilesByExt( vector<string>& oFiles, const char* path, const char* ext);
		
		// 获取搜寻路径
		vector<string>& GetSearchPath() { return m_searchPaths; }
		
		// 检测文件是否存在
		static bool IsFileExist( const string& strFilePath);
		
		// 检测是否为绝对路径
		static bool IsAbsolutePath( const string& strPath);
        
        // 获取主路径
        const string& GetRootPath() { return m_rootPath; }

	public:
		// 替换后缀名
		static void ReplaceExt( string& out, const string& source, const string& ext);

		// 替换文件名
		static void ReplaceNameExt( string& out, const string& source, const string& nameExt);

		// 获取后缀名
		static string GetExt( const string& source);

		// 获取名称
		static string GetNameExt( const string& source);
        
    protected:
        // 设置主路径
        void SetRootPath( const char* rootPath);
		
	private:
		string			m_rootPath;			// 主目录
		vector<string>	m_searchPaths;		// 搜寻路径
	};
}