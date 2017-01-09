#pragma once

#include "objfilemodel.h"

ID3D11Device*			m_pD3DDevice;
ID3D11DeviceContext*	m_pImmediateContext;

ObjFileModel*			m_pObject;
ID3D11VertexShader*		m_pVShader;
ID3D11PixelShader*		m_pPShader;
ID3D11InputLayout*		m_pInputLayout;
ID3D11Buffer*			m_pConstantBuffer;

float m_x, m_y, m_z;
float m_xangle, m_zangle, m_yangle;
float m_scale;

class Model
{
private:
	float m_bounding_sphere_centre_x;
	float m_bounding_sphere_centre_y;
	float m_bounding_sphere_centre_z;
	float m_bounding_sphere_radius;

	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();

public:
	Model(ID3D11Device* g_pD3DDevice, ID3D11DeviceContext* g_pImmediateContext);
	~Model();

	int LoadObjModel(char* filename);
	void Draw(XMMATRIX view, XMMATRIX projection);

	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius();
	bool CheckCollision(Model* model);
};