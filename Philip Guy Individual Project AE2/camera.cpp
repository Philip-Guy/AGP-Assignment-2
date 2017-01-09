// Implementation class - camera.cpp
#include "camera.h"

#include <D3D11.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>
#include <math.h>

camera::camera(float x, float y, float z, float rotation)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_camera_rotation = rotation;

	m_dx = sin(rotation * (XM_PI / 180));
	m_dz = cos(rotation * (XM_PI / 180));
}

void camera::Rotate(float degrees)
{
	m_camera_rotation += degrees;

	m_dx = sin(m_camera_rotation * (XM_PI / 180));
	m_dz = cos(m_camera_rotation * (XM_PI / 180));
}

void camera::Forward(float distance)
{
	m_x *= distance;
	m_z *= distance;
}

void camera::Up(float distance)
{
	m_y *= distance;
}

XMMATRIX camera::GetViewMatrix()
{
	 m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	 m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	 m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	 return XMMatrixLookAtLH(m_position, m_lookat, m_up);
}