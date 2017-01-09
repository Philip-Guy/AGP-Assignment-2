#include "Model.h"

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection;	// 64 bytes
	// Total size = 64 bytes
};

MODEL_CONSTANT_BUFFER model_cb_values;
model_cb_values.WorldViewProjection = world*(*view)*(*projection);

VSSetConstantBuffer();

Model::Model(ID3D11Device* g_pD3DDevice, ID3D11DeviceContext* g_pImmediateContext)
{
	m_pD3DDevice = g_pD3DDevice;
	m_pImmediateContext = g_pImmediateContext;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_xangle = 0.0f;
	m_zangle = 0.0f;
	m_yangle = 0.0f;
	m_scale = 1.0f;
}

int Model::LoadObjModel(char* filename)
{
	ObjFileModel *pObject;

	pObject = new ObjFileModel(filename, m_pD3DDevice, m_pImmediateContext);

	if (pObject->filename == "FILE NOT LOADED") return S_FALSE;

	// Collision
	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();
}

void Model::Draw(XMMATRIX view, XMMATRIX projection)
{
	XMMATRIX world, translation, rotation, scale;

	rotation = XMMatrixRotationX(XMConvertToRadians(45.0));
	scale = XMMatrixScaling(m_x, m_y, m_z);
	translation = XMMatrixTranslation(0, 0, 5);

	world *= XMMatrixScaling(m_scale, m_scale m_scale);
}

void Model::CalculateModelCentrePoint()
{
	float min_x = 0;
	float max_x = 0;
	float min_y = 0;
	float max_y = 0;
	float min_z = 0;
	float max_z = 0;

	// Determine the minimum and maximum x, y, and z values
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		// x values
		if ((m_pObject->vertices[i].Pos.x) < min_x)
		{
			min_x = m_pObject->vertices[i].Pos.x;
		}
		if ((m_pObject->vertices[i].Pos.x) > max_x)
		{
			max_x = m_pObject->vertices[i].Pos.x;
		}

		// y values
		if ((m_pObject->vertices[i].Pos.y) < min_y)
		{
			min_y = m_pObject->vertices[i].Pos.y;
		}
		if ((m_pObject->vertices[i].Pos.y) > max_y)
		{
			max_y = m_pObject->vertices[i].Pos.x;
		}

		// z values
		if ((m_pObject->vertices[i].Pos.z) < min_z)
		{
			min_z = m_pObject->vertices[i].Pos.z;
		}
		if ((m_pObject->vertices[i].Pos.z) > max_z)
		{
			max_z = m_pObject->vertices[i].Pos.z;
		}
	}

	// Set centre points for sphere
	m_bounding_sphere_centre_x = max_x - min_x;
	m_bounding_sphere_centre_y = max_y - min_y;
	m_bounding_sphere_centre_z = max_z - min_z;
}

void Model::CalculateBoundingSphereRadius()
{
	float highest_distance_x = 0.0f;
	float highest_distance_y = 0.0f;
	float highest_distance_z = 0.0f;

	for (int i = 0; i < m_pObject->numverts; i++)
	{
		// x value
		if ((m_pObject->vertices[i].Pos.x - m_bounding_sphere_centre_x) > highest_distance_x)
		{
			highest_distance_x = m_pObject->vertices[i].Pos.x;
		}
		// y value
		if ((m_pObject->vertices[i].Pos.y - m_bounding_sphere_centre_y) > highest_distance_y)
		{
			highest_distance_y = m_pObject->vertices[i].Pos.y;
		}
		// z value
		if ((m_pObject->vertices[i].Pos.z - m_bounding_sphere_centre_z) > highest_distance_z)
		{
			highest_distance_z = m_pObject->vertices[i].Pos.z;
		}
	}

	// Compare each highest vertex to find the furthest vertex from the centre
	if (highest_distance_x >= highest_distance_y && highest_distance_z)
		m_bounding_sphere_radius = highest_distance_x;
	if (highest_distance_y >= highest_distance_x && highest_distance_z)
		m_bounding_sphere_radius = highest_distance_y;
	if (highest_distance_z >= highest_distance_y && highest_distance_x)
		m_bounding_sphere_radius = highest_distance_z;
}

XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world, translation, rotation, scale;

	scale = XMMatrixScaling(1, 1, 2);
	rotation = XMMatrixRotationX(XMConvertToRadians(45.0));
	translation = XMMatrixTranslation(0, 0, 5);

	world = scale * rotation * translation;

	XMVECTOR offset;

	offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0);
	offset = XMVector3Transform(offset, world);
	
	return offset;
}

float Model::GetBoundingSphereRadius()
{
	return m_bounding_sphere_radius;
}

bool Model::CheckCollision(Model* model)
{
	if (model == this->LoadObjModel) // returns false if the model passed in is the same as the current model
		return false;

	XMVECTOR current_model_position;

	GetBoundingSphereWorldSpacePosition();
	current_model_position = model->GetBoundingSphereWorldSpacePosition();

	float x1 = XMVectorGetX(current_model_position);
	float x2 = XMVectorGetX(GetBoundingSphereWorldSpacePosition());
	float y1 = XMVectorGetY(current_model_position);
	float y2 = XMVectorGetY(GetBoundingSphereWorldSpacePosition());
	float z1 = XMVectorGetZ(current_model_position);
	float z2 = XMVectorGetZ(GetBoundingSphereWorldSpacePosition());

	float distance_squared = pow((x1 - x2), 2) + pow((y1 - y2), 2) + pow((z1 - z2), 2);

	if (distance_squared < pow((GetBoundingSphereRadius() + model->GetBoundingSphereRadius()), 2)) // Collision!
		return true;

	return false; // else collision is false
}