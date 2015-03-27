//*****************************************************************************
//
// ���C�g���� [light.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "light.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight(void)
{
	// �����o������
	m_index	 = 0;
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// ����
//=============================================================================
CLight* CLight::Create(LPDIRECT3DDEVICE9 device, int index, D3DLIGHTTYPE type, D3DXVECTOR3 vecDir, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular, D3DCOLORVALUE ambient)
{
	CLight* light = new CLight;

	light->Init(device, index, type, vecDir, diffuse, ambient, specular);

	return light;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLight::Init(LPDIRECT3DDEVICE9 device, int index, D3DLIGHTTYPE type, D3DXVECTOR3 vecDir, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular, D3DCOLORVALUE ambient)
{
	//----------------------------
	// �����o�[�ݒ�
	//----------------------------
	m_index	 = index;
	m_vecDir = vecDir;

	//----------------------------
	// ���C�����C�g�̏�����
	//----------------------------
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));					// ������(�N���A)

	m_light.Type	 = type;
	m_light.Diffuse	 = diffuse;
	m_light.Ambient	 = ambient;
	m_light.Specular = specular;

	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &m_vecDir);	// ���K��

	device->SetLight(m_index, &m_light);	// �l��ݒ�
	device->LightEnable(m_index, TRUE);		// �L���ɂ���

	//----------------------------
	// ����������
	//----------------------------
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CLight::Update(void)
{
}
