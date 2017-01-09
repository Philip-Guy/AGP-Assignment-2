// Header file - camera.h
#pragma once

class camera
{
private:
	// Member variables
	float m_x;
	float m_y;
	float m_z;
	float m_dx;
	float m_dz;

	float m_camera_rotation;
	XMVECTOR m_position;
	XMVECTOR m_lookat;
	XMVECTOR m_up;

public:
	// Constructor
	camera(float x, float y, float z, float rotation);

	void Rotate(float degrees);
	void Forward(float distance);
	void Up(float distance);
	XMMATRIX GetViewMatrix();
};