#include "IOInstance.h"
#include "MainWindow.h"
#include "OutPut.h"

using namespace Ares;

CRenderModuleIOInstance g_renderIO;

// ReportError
void CRenderModuleIOInstance::ReportError( int code, const char* message, const char* file, int line)
{
	char text[1024];

	// ÎÄ±¾
	sprintf( text, "%s,%s,%d", message, file, line);

	QT_UI::CMainWindow::m_outPut->AppendText( text);
}