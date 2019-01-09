#include "Core/Graphics/CgTokenizer.h"

namespace Ares
{
	// 错误消息提醒
	static void DebugPrintf( vector<string>& errors, const char* format, ...)
	{
		char    buf[2048];
		va_list args;

		va_start(args, format);
		_vsnprintf( buf, 2048, format, args); 
		va_end(args);

		errors.push_back( buf);
	}

	#define DPF DebugPrintf

	// Tokenize the contents of a file
	bool CgTokenizer::TokenizeFile( const char* fileName, DWORD flags, const char* delims)
	{
		FILE* fileHandle = fopen( fileName, "rt");
		if( fileHandle)
		{
			// 检测文件大小,一次性读进内存
			fseek( fileHandle, 0, SEEK_END);
			size_t iEnd = ftell( fileHandle);
			fseek( fileHandle, 0, SEEK_SET);
			size_t iStart = ftell( fileHandle);
			size_t fileSize = iEnd - iStart;

			// Allocate memory for whole file
			BYTE* effectBuffer = new_ BYTE[ fileSize+1];
			if ( !effectBuffer) 
				return false;

			// load file into buffer
			size_t size = fread( effectBuffer, 1, fileSize, fileHandle);
			if ( size != fileSize) 
				DPF( m_errors, "Warning : size %d != fileSize %d", size, fileSize);

			// close the file , we don't need it any more
			fflush( fileHandle);
			fclose( fileHandle);

			effectBuffer[size] = '\0';

			bool succeed = TokenizeString( effectBuffer, size, flags, delims);

			SAFE_DELETE_ARRAY( effectBuffer);

			return succeed;
		}

		DPF( m_errors, "Error : Open %s failed!", fileName);

		return m_errors.size() ? false : true;
	}

	// Tokenize string
	bool CgTokenizer::TokenizeString( BYTE* code, int size, DWORD flags, const char* delims)
	{
		int numDelims = static_cast<int>(strlen(delims));

		// destroy any tokens that may have already existed
		DestroyTokens();

		CgToken*  pToken	  = NULL;
		m_curTokenIdx	      = 0;
		TokState curState	  = TKS_InWhite;		// 默认指向空白处
		int		 curPos		  = 0;
		char	 tBuffer[512] = {0};
		int		 tBufferSize  = 0;
		char	 byCurChar;
		char	 byNextChar;
		BOOL	 isSpecialChar= FALSE;

		while( curPos < size)
		{
			byCurChar = code[curPos++];
			byNextChar= code[curPos];

			switch( curState)
			{
				// In whitespace
			case TKS_InWhite:
				{
					// 无视空白区域 与分隔符
					if( IsDelimiter( byCurChar, delims, numDelims))
					{
						AddLineNum( byCurChar);

						continue;
					}

					switch( byCurChar)
					{
						// Is this the beginning of annotation
					case '/':
						{
							if( byNextChar=='/')
								curState = TKS_InSingleAnnotation;
							else if( byNextChar == '*')
								curState = TKS_InMultiAnnotation;
						}
						continue;

						// if we're quotes
					case '"':
						{
							curState = TKS_InQuots;
						}
						continue;
					}

					// if we got here then this is not a delimiter, this is text...
					curState = TKS_InText;
					curPos--;
				}
				break;

			case TKS_InText:
				{
					// check for delimiters to stop getting text
					isSpecialChar = IsSpecialCharacter( byCurChar);
					if( IsDelimiter( byCurChar, delims, numDelims) || isSpecialChar)
					{
						if( isSpecialChar)
						{
							if( strlen( tBuffer) > 0)
								curPos--;
							else
								tBuffer[0] = byCurChar;
						}

						FinalizeToken( tBuffer, flags);
						tBufferSize = 0;
						memset( tBuffer, 0, sizeof(tBuffer));
						curState = TKS_InWhite;

						AddLineNum( byCurChar);

						continue;
					}

					if( tBufferSize < 512)
						tBuffer[tBufferSize++] = byCurChar;
				}
				break;

				// In Quotations.
			case TKS_InQuots:
				{
					// If we found the end of the quote...
					if ( byCurChar == '"'  )
					{
						// Make the token and throw it into the list.
						FinalizeToken( tBuffer, flags);
						tBufferSize = 0;
						memset( tBuffer, 0, 512 );
						curState = TKS_InWhite;

						continue;
					}
					if( tBufferSize < 512)
						tBuffer[ tBufferSize++ ] = byCurChar;
				}		
				break;

			// In single-line comment.
			case TKS_InSingleAnnotation:
				{
					// Check to see if the comment has ended (we've reached the end of line).
					if ( byCurChar == '\n' )
					{
						curState = TKS_InWhite;
						m_curLine++;
					}
				}
				break;

			// In multi-line comment.
			case TKS_InMultiAnnotation:
				{
					// Check to see if the comment is ending.
					if ( byCurChar == '/' )
					{
						// Check previous character.
						if ( code[ curPos - 2 ] == '*' )
						{
							curState = TKS_InWhite;
						}
					}
				}
				break;

			}
		}

		// we're done with the string but we're left over with a token
		if( tBufferSize)
			FinalizeToken( tBuffer, flags);

		return true;
	}

	// Destroy the current tokens
	void CgTokenizer::DestroyTokens()
	{
		for ( size_t i=0; i<m_tokens.size(); i++)
			SAFE_DELETE( m_tokens[i]);

		m_tokens.clear();

		m_curLine = 1;
		m_curTokenIdx = 0;
	}

	// Try to decide what kind of token this may be
	// 是否为定界符
	TokenType CgTokenizer::IsDelimiter( char chTok, const char* delims, int numDelims)
	{
		// Figure out what kind of token this character might be
		for ( int i=0; i<numDelims; i++)
		{
			if( chTok == delims[i])
				return TKT_Delimiter;
		}

		// Last check, is this a null terminator?
		if( chTok == '\0')
			return TKT_Delimiter;

		return TKT_UnKnown;
	}

	// Add line num
	void CgTokenizer::AddLineNum( char chTok)
	{
		// 换行符
		if( chTok == '\n')
			m_curLine++;
	}

	// Is this a special character already existing as one of our tokens
	TokenType CgTokenizer::IsSpecialCharacter( char chTok)
	{
		switch( chTok)
		{
		case '{':
			return TKT_OpenBrace;

		case '}':
			return TKT_CloseBrace;

		case '[':
			return TKT_OpenBracket;

		case ']':
			return TKT_CloseBracket;

		case '(':
			return TKT_OpenParentheses;

		case ')':
			return TKT_CloseParentheses;

		case '=':
			return TKT_Equal;

		case '"':
			return TKT_Quote;
		}

		return TKT_UnKnown;
	}

	// Finalize a token( determine it's type and make it) and add it to the list
	TokenType CgTokenizer::FinalizeToken( const char* text, DWORD flags)
	{
		CgToken* pToken = AllocToken( ClassifyToken( text, flags), text);
		if( pToken)
		{
			m_tokens.push_back( pToken);

			return pToken->m_type;
		}

		return TKT_UnKnown;
	}

	// Allocate a new token
	CgToken* CgTokenizer::AllocToken( TokenType type, const char* text/*=NULL*/)
	{
		switch( type)
		{
		case TKT_Equal:				// =
		case TKT_OpenBrace:			// {
		case TKT_CloseBrace:		// }
		case TKT_OpenBracket:		// [
		case TKT_CloseBracket:		// ]
		case TKT_OpenParentheses:	// (
		case TKT_CloseParentheses:	// )
			return new_ CgToken( m_curLine, type);

		case TKT_String:
			return new_ CgTokenString( m_curLine, text);
		}
		
		return NULL;
	}

	// determine what type of token this is
	TokenType CgTokenizer::ClassifyToken( const char* text, DWORD flags)
	{
		// See if this is a number (first character is a number or first is '-' and second is num).
		if ( !( flags & TKF_StringsOnly ) && ( isdigit( text[0] ) ||
			( strlen(text) >= 2 && ( text[0] == '-' && isdigit( text[1])))))
		{
			return TKT_Float;
		}
		else if ( TokenType type = IsSpecialCharacter( text[0]))
			return type;
		else
			return TKT_String;
	}
}