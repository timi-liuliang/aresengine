#include <AresErrorCallback.h>

using namespace Ares;


class CRenderModuleIOInstance : public IErrorCallback
{
public:
	// ReportError
	virtual void ReportError( int code, const char* message, const char* file, int line);
};

extern CRenderModuleIOInstance g_renderIO;

