#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();

	void setProjectionValues(FLOAT t_fovDegrees, INT t_screenWidth, INT t_screenHeight, FLOAT t_nearZ, FLOAT t_farZ);
	void update(FLOAT t_deltaTime);
	void deInit();

	const XMMATRIX& getViewMatrix() const;
	const XMFLOAT4X4 getViewMatrix4x4();
	const XMMATRIX& getProjectionMatrix() const;
	const XMFLOAT4X4 getProjectionMatrix4x4();

	const XMVECTOR& getPositionVector() const;
	const XMFLOAT3& getPositionFloat3() const;
	const XMVECTOR& getRotationVector() const;
	const XMFLOAT3& getRotationFloat3() const;

	void setPosition(const XMVECTOR& pos);
	void setPosition(FLOAT x, FLOAT y, FLOAT z);
	void adjustPosition(const XMVECTOR& pos);
	void adjustPosition(FLOAT x, FLOAT y, FLOAT z);
	void setRotation(const XMVECTOR& rot);
	void setRotation(FLOAT x, FLOAT y, FLOAT z);
	void adjustRotation(const XMVECTOR& rot);
	void adjustRotation(FLOAT x, FLOAT y, FLOAT z);
	void setLookAtPos(XMFLOAT3 t_lookAtPos);

	const XMVECTOR& getForwardVector();
	const XMVECTOR& getBackwardVector();
	const XMVECTOR& getleftVector();
	const XMVECTOR& getRightVector();

private:
	void updateViewMatrix();

	XMVECTOR m_positionVector;
	XMVECTOR m_rotationVector;

	XMFLOAT3 m_positionFloat3;
	XMFLOAT3 m_rotationFloat3;

	XMMATRIX m_viewMatrix;
	XMFLOAT4X4 m_viewMatrix4x4;
	XMMATRIX m_projectionMatrix;
	XMFLOAT4X4 m_projectionMatrix4x4;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR m_forwardVector;
	XMVECTOR m_backwardVector;
	XMVECTOR m_leftVector;
	XMVECTOR m_rightVector;
};

