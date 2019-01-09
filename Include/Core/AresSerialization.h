#pragma once

#include <string>
#include <ostream>
#include <fstream>
#include <istream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>

using namespace std;

namespace Ares
{
	//--------------------------------------------
	// 序列化(暂理解有误) 2011-10-27 帝林
	//--------------------------------------------
	namespace Serialization
	{
		// 序列化到二进制文件
		template <typename T> void SerializeToBinaryFile( const char* filename, const T& object)
		{
			ofstream ofs( filename, ios::binary | ios::out);
			if( ofs.is_open())
			{
				boost::archive::binary_oarchive boarchive( ofs);
				boarchive << object;
			}
		}

		// 二进制文件反序列化
		template <typename T> void UnSerializeFromBinaryFile( const char* filename, T& object)
		{
			ifstream ifs( filename, ios::binary | ios::in);
			if( ifs.is_open())
			{
				boost::archive::binary_iarchive biarchive( ifs);
				biarchive >> object;
			}
		}

		// 序列化到文本文件
		template <typename T> void SerializeToTextFile( const char* filename, const T& object)
		{
			ofstream ofs( filename, ios::out);
			if( ofs.is_open())
			{
				boost::archive::text_oarchive toarchive( ofs);
				toarchive << object;
			}
		}

		// 文本文件反序列化
		template <typename T> void UnSerializeFromTextFile( const char* filename, T& object)
		{
			ifstream ifs( filename, ios::in);
			if( ifs.is_open())
			{
				boost::archive::text_iarchive tiarchive( ifs);
				tiarchive >> object;
			}
		}

		// 打包字符串
		void PackString( string& out, const vector<float>& source, char separator);

		// 打包字符串
		void PackString( string& out, const vector<int>& source, char separator);

		// 解包字符串(float)
		void UnPackString(vector<float>& out, const string& source, char separator);

		// 解包字符串(int)
		void UnPackString(vector<int>& out, const string& source, char separator);
	};
}