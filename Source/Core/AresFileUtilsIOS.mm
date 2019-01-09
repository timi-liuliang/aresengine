#import  <Foundation/Foundation.h>
#import  <UIKit/UIDevice.h>
#include <Core/AresFileUtils.h>

namespace Ares
{
	//-------------------------------
	// FileUtilsIOS 2013-7-28 帝林
	//-------------------------------
	class FileUtilsIOS : public FileUtils
	{
	public:
	};
    
    // 新建实例
    FileUtils* FileUtils::Create( const char* rootPath)
    {
        string _rootPath = rootPath;
        if( !IsAbsolutePath( _rootPath))
        {
            NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
            _rootPath = [bundlePath UTF8String] + string("/") + _rootPath;
        }
        
        FileUtilsIOS* fileUtils = new_ FileUtilsIOS();
        fileUtils->SetRootPath( _rootPath.c_str());
        
        return fileUtils;
    }
    
    // 默认文件管理器
    static NSFileManager* g_NSFileManager = [NSFileManager defaultManager];
	
	// 检测文件是否存
	bool FileUtils::IsFileExist( const string& strFilePath)
	{
		if ( !strFilePath.length())
			return false;

		bool ret = false;  
		if (strFilePath[0] != '/')
		{
			std::string path;
			std::string file;
			size_t pos = strFilePath.find_last_of("/");
			if ( pos!=std::string::npos)
			{
				file = strFilePath.substr( pos+1);
				path = strFilePath.substr( 0, pos+1);
			}
			else
			{
				file = strFilePath;
			}
        
			NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file.c_str()] ofType:@"glsl" inDirectory:[NSString stringWithUTF8String:path.c_str()]];
			if (fullpath != nil)
				ret = true;
		}
		else
		{
			// Search path is an absolute path.
			if ([g_NSFileManager fileExistsAtPath:[NSString stringWithUTF8String:strFilePath.c_str()]])
				ret = true;
        }
    
		return ret;
	}
    
    // 检测是否为绝对路径
    bool FileUtils::IsAbsolutePath( const string& strPath)
    {
        NSString* path = [NSString stringWithUTF8String:strPath.c_str()];
        
        return [path isAbsolutePath] ? true : false;
    }
}