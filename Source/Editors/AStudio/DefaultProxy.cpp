#include "DefaultProxy.h"
#include "SCLogic.h"

namespace Ares
{
	DefaultProxy::DefaultProxy()
		: m_vPos( Vector3( 0.f, 20.f, 15.f))
		, m_fSpeed( 2.f)
	{
		m_camera = new_ LiberityViewCamera;
		if( m_camera)
		{
			m_camera->SetPos( &m_vPos);
			m_camera->SetLookAt( &Vector3( 0.f, 0.f, 0.f));
		}
	}

	DefaultProxy::~DefaultProxy()
	{

	}

	// ¼¤»î
	void DefaultProxy::Active()
	{
		SCLogic_Camera->SetCamera( m_camera);
	}

	// ¸üÐÂ
	void DefaultProxy::FrameMove( float elapsedTime)
	{
		if ( m_camera)
		{
			if( SCLogic_Input->IsKeyDown(OIS::KC_W)) m_vPos += m_camera->GetDirection() * 2.f * m_fSpeed * elapsedTime;
			if( SCLogic_Input->IsKeyDown(OIS::KC_S)) m_vPos -= m_camera->GetDirection() * 2.f * m_fSpeed * elapsedTime;
			if( SCLogic_Input->IsKeyDown(OIS::KC_D)) m_vPos += m_camera->GetRight()     * 2.f * m_fSpeed * elapsedTime;
			if( SCLogic_Input->IsKeyDown(OIS::KC_A)) m_vPos -= m_camera->GetRight()     * 2.f * m_fSpeed * elapsedTime;
			if( SCLogic_Input->IsKeyDown(OIS::KC_SPACE))    m_vPos.z += 1.0f * elapsedTime;
			if( SCLogic_Input->IsKeyDown(OIS::KC_X))		  m_vPos.z -= 1.0f * elapsedTime;

			m_camera->SetPos( &m_vPos);

			m_camera->FrameMove( elapsedTime);
		}
	}
}