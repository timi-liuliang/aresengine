#include <boost/filesystem.hpp>
#include <Core/AresFileUtils.h>

namespace Ares
{
	// 替换后缀名
	void FileUtils::ReplaceExt( string& out, const string& source, const string& ext)
	{
		boost::filesystem::path filePath( source);

		out = filePath.replace_extension( ext).string();
	}

	// 更换文件名
	void FileUtils::ReplaceNameExt( string& out, const string& source, const string& nameExt)
	{
		boost::filesystem::path filePath(source);

		boost::filesystem::rename( filePath, nameExt);

		out = filePath.string();
	}

	// 获取后缀名
	string FileUtils::GetExt( const string& source)
	{
		return boost::filesystem::path( source).extension().string();
	}

	// 获取名称
	string FileUtils::GetNameExt( const string& source)
	{
		return boost::filesystem::path( source).filename().string();
	}

	// 构造函数
	FileUtils::FileUtils()
		: m_rootPath( "")
	{
	}

	// 析构函数
	FileUtils::~FileUtils()
	{

	}

	// 添加搜寻路径
	void FileUtils::AddSearchPath( const char* path_)
	{
		string strPrefix;
		string path( path_);
		if( !IsAbsolutePath( path))
			strPrefix = m_rootPath;

		path = strPrefix + path;
		if( path.length()>0 && path[path.length()-1]!='/')
			path += "/";

		m_searchPaths.push_back( path);
	}

	// 递归添加搜寻路径
	void FileUtils::AddSearchPathRecursive( const char* path)
	{
		using namespace boost::filesystem;

		AddSearchPath( path);

		for( directory_iterator pos(path); pos!=directory_iterator(); pos++)
		{
			if( is_directory( *pos))
			{
				AddSearchPathRecursive( pos->path().string().c_str());
			}
		}
	}

	// 查找指定目录下文件
	void FileUtils::SearchFilesByExt( vector<string>& oFiles, const char* path, const char* ext)
	{
		using namespace boost::filesystem;
		for( directory_iterator pos(path); pos!=directory_iterator(); pos++)
		{
			string posPath = pos->path().string();

			if( is_directory( *pos))
			{
				SearchFilesByExt( oFiles, posPath.c_str(), ext);
			}
			else if( GetExt( posPath) == ext)
			{
				oFiles.push_back( posPath);
			}
		}
	}

	// 获取完整路径
	string FileUtils::FullPath( const char* fileName)
	{
		string strFileName = fileName;
		if( IsFileExist( strFileName))
			return strFileName;
		else
		{
			for ( size_t i=0; i<m_searchPaths.size(); i++)
			{
				string resPath = m_searchPaths[i] + strFileName;
				if( IsFileExist( resPath))
					return resPath;
			}
		}

		return "";
	}
    
    // 设置主路径
    void FileUtils::SetRootPath( const char* rootPath)
    {
        m_rootPath = rootPath;
        
        m_searchPaths.push_back( m_rootPath);
    }
    
	// 获取资源数据
	void FileUtils::ExtractFileData( vector<BYTE>& oData, const char* fileName)
	{

	}
}