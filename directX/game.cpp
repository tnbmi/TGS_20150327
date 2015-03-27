//*****************************************************************************
//
// CGame�N���X [game.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "game.h"
#include "manager.h"
#include "fade.h"

#include "result.h"

#include "inputKeyboard.h"

#include "camera.h"
#include "light.h"

#include "sceneBillboard.h"
#include "meshField.h"

#include "player.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// �J����
#define CAMERA_POS			D3DXVECTOR3(0.0f, 30.0f, -150.0f)
#define CAMERA_DEBUG_POS	D3DXVECTOR3(0.0f, 500.0f, -1000.0f)

// ���C�g
#define LIGHT_MAX	(3)
const D3DXVECTOR3 LIGHT_VEC[LIGHT_MAX] =
{
	D3DXVECTOR3(1.2f, -2.0f, 3.0f),
	D3DXVECTOR3(1.0f, -1.5f, -1.5f),
	D3DXVECTOR3(-4.0f, 1.0f, -1.0f)
};
const D3DCOLORVALUE LIGHT_DIFFUSE[LIGHT_MAX] =
{
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f),
	D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CCamera*	CGame::m_camera	= NULL;

//=============================================================================
// ������
//=============================================================================
HRESULT CGame::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �f�o�C�X�擾
	//----------------------------
	m_device = device;

	//----------------------------
	// �J����
	//----------------------------
	m_camera = CCamera::Create(D3DXVECTOR3(0.0f, 20.0f, 0.0f), CAMERA_POS);

	m_dcFlg = false;
	m_debugCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERA_DEBUG_POS);

	//----------------------------
	// ���C�g
	//----------------------------
	// ����
	for(int cnt = 0; cnt < LIGHT_MAX; ++cnt)
	{
		m_light[cnt] = CLight::Create(device,
									  cnt,
									  D3DLIGHT_DIRECTIONAL,
									  LIGHT_VEC[cnt],
									  LIGHT_DIFFUSE[cnt],
									  D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f),
									  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// ��d���I��
	device->SetRenderState(D3DRS_LIGHTING, TRUE);

	//----------------------------
	// �I�u�W�F�N�g
	//----------------------------
	InitObject(device);

	//----------------------------
	// �t�F�[�h
	//----------------------------
	m_fade = CFade::Create(device);
	m_fade->Start(CFade::FADESTATE_IN, 1, 1.0f, 1.0f, 1.0f, 1.0f);

	//----------------------------
	// ����������
	//----------------------------
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CGame::Uninit(void)
{
	//----------------------------
	// �I�u�W�F�N�g
	//----------------------------
	// �V�[��
	CScene::ReleaseAll();

	//----------------------------
	// �r���[
	//----------------------------
	// �J����
	SAFE_END(m_camera);
	SAFE_END(m_debugCamera);

	// ���C�g
	for(int cnt = 0; cnt < LIGHT_MAX; ++cnt)
	{
		SAFE_END(m_light[cnt]);
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CGame::Update(void)
{
	if(m_fade->GetState() == CFade::FADESTATE_NONE)
	{
		//----------------------------
		// �f�o�b�O�R�}���h
		//----------------------------
		Debug();

		//----------------------------
		// ���C�g
		//----------------------------
		for(int cnt = 0; cnt < LIGHT_MAX; ++cnt)
		{
			m_light[cnt]->Update();
		}

		//----------------------------
		// �X�V���e
		//----------------------------
		// �J����
		UpdateCamera();
	}

	//----------------------------
	// ��ʑJ��
	//----------------------------
	if(m_fade->GetState() == CFade::FADESTATE_OUTEND)
	{
		CManager::SetNextPhase((CPhase*)new CResult);
	}
}

//=============================================================================
// �`��
//=============================================================================
void CGame::Draw(void)
{
	// �J�����ݒ�
	if(m_dcFlg)
	{
		m_debugCamera->SetCamera(m_device);
		CSceneBillboard::SetMtxView(m_debugCamera->GetMtxView());
	}
	else
	{
		m_camera->SetCamera(m_device);
		CSceneBillboard::SetMtxView(m_camera->GetMtxView());
	}
}

//=============================================================================
// �f�o�b�N���e
//=============================================================================
void CGame::Debug(void)
{
	//----------------------------
	// ��ʑJ��
	//----------------------------
	if(m_keyboard->GetTrigger(DIK_RETURN))
	{
		m_fade->Start(CFade::FADESTATE_OUT, 1, 1.0f, 1.0f, 1.0f, 0.0f);
	}

	//----------------------------
	// �J�����؂�ւ�
	//----------------------------
	if(m_keyboard->GetTrigger(DIK_F2))
	{
		if(m_dcFlg)
		{
			m_dcFlg = false;
		}
		else
		{
			m_dcFlg = true;
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g������
//=============================================================================
void CGame::InitObject(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �t�B�[���h
	//----------------------------
	D3DXVECTOR3 size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	D3DXVECTOR3 num  = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	float*		heightVtx = nullptr;

	m_field = CMeshField::Create(device,
								 CImport::TEX_FIELD,
								 size,
								 num,
								 heightVtx,
								 CScene::MESHTEX_PATCH);

	//----------------------------
	// �L�����N�^�[
	//----------------------------
	m_player = CPlayer::Create(device);
	m_player->SetKeyboard(m_keyboard);
}

//=============================================================================
// �J�����X�V
//=============================================================================
void CGame::UpdateCamera(void)
{
	D3DXVECTOR3 targetPos	= m_player->GetPos();
	D3DXVECTOR3 targetVecF	= m_player->GetVecF();
	D3DXVECTOR3 targetSpeed	= m_player->GetSpeed();
	D3DXVECTOR3 targetSize	= m_player->GetSize();

	D3DXVECTOR3 at			= m_camera->GetAt();
	D3DXVECTOR3 pos			= m_camera->GetPos();
	D3DXVECTOR3 rot			= m_camera->GetRot();
	float		distance	= m_camera->GetDistance();

	D3DXVECTOR3 atDest		= m_camera->GetAtDest();
	D3DXVECTOR3 posDest		= m_camera->GetPosDest();

	//----------------------------
	// ���͏���
	//----------------------------
	// ������
	if(m_keyboard->GetPress(DIK_L) == true)
	{
		rot.y += 0.01f;
	}
	// �E����
	if(m_keyboard->GetPress(DIK_J) == true)
	{
		rot.y += -0.01f;
	}

	// �����
	if(m_keyboard->GetPress(DIK_I) == true
	&& rot.x < D3DX_PI * 0.2f)
	{
		rot.x += 0.01f;
	}
	// ������
	if(m_keyboard->GetPress(DIK_K) == true
	&& rot.x > -D3DX_PI * 0.05f)
	{
		rot.x += -0.01f;
	}

	// �Y�[���C��
	if(m_keyboard->GetPress(DIK_U) == true
	&& distance > 25.0f)
	{
		distance -= 1.0f;
		m_camera->SetDistance(distance);
	}
	// �Y�[���A�E�g
	if(m_keyboard->GetPress(DIK_O) == true
	&& distance < 80.0f)
	{
		distance += 1.0f;
		m_camera->SetDistance(distance);
	}

	//----------------------------
	// ��]
	//----------------------------
	// �����ւ�
	if(rot.y < -D3DX_PI || rot.y > D3DX_PI)
	{
		rot.y = rot.y + (-SIGN_F(rot.y) * D3DX_PI * 2.0f);
	}

	if(rot.x < -D3DX_PI || rot.x > D3DX_PI)
	{
		rot.x = rot.x + (-SIGN_F(rot.x) * D3DX_PI * 2.0f);
	}

	//----------------------------
	// �����_
	//----------------------------
	atDest.x = targetPos.x + targetVecF.x * 10.0f;
	atDest.z = targetPos.z + targetVecF.z * 10.0f;
	atDest.y = targetPos.y + targetSize.y;
	m_camera->SetAtDest(atDest);

	at += (atDest - at) * 0.5f;
	m_camera->SetAt(at);

	//----------------------------
	// ���_
	//----------------------------
	pos.x = cosf(rot.y - D3DX_PI * 0.5f) * cosf(rot.x) * distance;
	pos.z = sinf(rot.y - D3DX_PI * 0.5f) * cosf(rot.x) * distance;
	pos.y = sinf(rot.x) * distance;

	// �v���C���[���ړ���
	//if(abs(targetSpeed.x) >= 0.1f || abs(targetSpeed.z) >= 0.1f)
	{
		D3DXVECTOR2 vec[2];

		// �J�����x�N�g��
		vec[0].x = cosf(rot.y - D3DX_PI * 0.5f);
		vec[0].y = sinf(rot.y - D3DX_PI * 0.5f);

		// �v���C���[���x�N�g��
		vec[1].x = -targetVecF.x;
		vec[1].y = -targetVecF.z;

		// ����
		float dot = D3DXVec2Dot(&vec[0], &vec[1]);

		// �s�p�Ȃ�Ή�肱��
		//if(dot > 0.0f)
		{
			posDest.x = -targetVecF.x * cosf(rot.x) * distance;
			posDest.z = -targetVecF.z * cosf(rot.x) * distance;
			m_camera->SetPosDest(posDest);
		}

		pos += (posDest - pos) * 0.03f;
	}

	m_camera->SetPos(pos);
}
