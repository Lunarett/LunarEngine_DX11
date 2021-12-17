#include "Camera.h";

Camera::Camera()
{
	// Set Position
	m_positionFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_positionVector = XMLoadFloat3(&m_positionFloat3);
	
	// Set Rotation
	m_rotationFloat3 = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotationVector = XMLoadFloat3(&m_rotationFloat3);

	updateViewMatrix();
}

void Camera::setProjectionValues(FLOAT t_fovDegrees, INT t_screenWidth, INT t_screenHeight, FLOAT t_nearZ, FLOAT t_farZ)
{
	// Convert FOV from degrees to Radians
	FLOAT fovRadians = (t_fovDegrees / 360.0f) * XM_2PI;

	// Calculate Aspect Ratio
	FLOAT aspectRatio = static_cast<FLOAT>(t_screenWidth) / static_cast<FLOAT>(t_screenHeight);

	// Create Projection Matrix
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, t_nearZ, t_farZ);
	updateViewMatrix();
}

void Camera::update(FLOAT t_deltaTime)
{
	float move = 5.0f * t_deltaTime;			// Move Speed
	float camSpeed = 2.0f * t_deltaTime;		// Rotation Sensitivity

	// Move Camera using WASD
	if (GetAsyncKeyState('W') & 0x8000) adjustPosition(getForwardVector() * move);		// Move Forward
	if (GetAsyncKeyState('A') & 0x8000) adjustPosition(getleftVector() * move);			// Move Left
	if (GetAsyncKeyState('S') & 0x8000) adjustPosition(getBackwardVector() * move);		// Move Backwards
	if (GetAsyncKeyState('D') & 0x8000) adjustPosition(getRightVector() * move);		// Move Right

	// Move Camera Up and Down using Q and E
	if (GetAsyncKeyState('E') & 0x8000) adjustPosition(0.0f, move, 0.0f);				// Move Up
	if (GetAsyncKeyState('Q') & 0x8000) adjustPosition(0.0f, -move, 0.0f);				// Move Down

	// Rotate Camera using Arrow keys
	if (GetAsyncKeyState(VK_UP) & 0x8000) adjustRotation(-camSpeed, 0.0f, 0.0f);		// Look Up
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) adjustRotation(camSpeed, 0.0f, 0.0f);		// Look Down
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) adjustRotation(0.0f, -camSpeed, 0.0f);		// Turn Left
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) adjustRotation(0.0f, camSpeed, 0.0f);		// Turn Right
}

void Camera::deInit()
{
}

const DirectX::XMMATRIX& Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

const DirectX::XMFLOAT4X4 Camera::getViewMatrix4x4()
{
	XMStoreFloat4x4(&m_viewMatrix4x4, m_viewMatrix);
	return m_viewMatrix4x4;
}

const DirectX::XMMATRIX& Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

const DirectX::XMFLOAT4X4 Camera::getProjectionMatrix4x4()
{
	XMStoreFloat4x4(&m_projectionMatrix4x4, m_projectionMatrix);
	return m_projectionMatrix4x4;
}

const DirectX::XMVECTOR& Camera::getPositionVector() const
{
	return m_positionVector;
}

const DirectX::XMFLOAT3& Camera::getPositionFloat3() const
{
	return m_positionFloat3;
}

const DirectX::XMVECTOR& Camera::getRotationVector() const
{
	return m_rotationVector;
}

const DirectX::XMFLOAT3& Camera::getRotationFloat3() const
{
	return m_rotationFloat3;
}

void Camera::setPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&m_positionFloat3, pos);
	m_positionVector = pos;
	updateViewMatrix();
}

void Camera::setPosition(FLOAT x, FLOAT y, FLOAT z)
{
	m_positionFloat3 = XMFLOAT3(x, y, z);
	m_positionVector = XMLoadFloat3(&m_positionFloat3);
	updateViewMatrix();
}

void Camera::adjustPosition(const XMVECTOR& pos)
{
	m_positionVector += pos;
	XMStoreFloat3(&m_positionFloat3, m_positionVector);
	updateViewMatrix();
}

void Camera::adjustPosition(FLOAT x, FLOAT y, FLOAT z)
{
	m_positionFloat3.x += x;
	m_positionFloat3.y += y;
	m_positionFloat3.z += z;

	m_positionVector = XMLoadFloat3(&m_positionFloat3);
	updateViewMatrix();
}

void Camera::setRotation(const XMVECTOR& rot)
{
	m_rotationVector = rot;
	XMStoreFloat3(&m_rotationFloat3, m_rotationVector);
	updateViewMatrix();
}

void Camera::setRotation(FLOAT x, FLOAT y, FLOAT z)
{
	m_rotationFloat3 = XMFLOAT3(x, y, z);
	m_rotationVector = XMLoadFloat3(&m_rotationFloat3);
	updateViewMatrix();
}

void Camera::adjustRotation(const XMVECTOR& rot)
{
	m_rotationVector += rot;
	XMStoreFloat3(&m_rotationFloat3, m_rotationVector);
	updateViewMatrix();
}

void Camera::adjustRotation(FLOAT x, FLOAT y, FLOAT z)
{
	m_rotationFloat3.x += x;
	m_rotationFloat3.y += y;
	m_rotationFloat3.z += z;

	m_rotationVector = XMLoadFloat3(&m_rotationFloat3);
	updateViewMatrix();
}

void Camera::setLookAtPos(XMFLOAT3 t_lookAtPos)
{
	if (t_lookAtPos.x == m_positionFloat3.x && t_lookAtPos.y == m_positionFloat3.y && t_lookAtPos.z == m_positionFloat3.z)
		return;

	t_lookAtPos.x = m_positionFloat3.x - t_lookAtPos.x;
	t_lookAtPos.y = m_positionFloat3.y - t_lookAtPos.y;
	t_lookAtPos.z = m_positionFloat3.z - t_lookAtPos.z;

	// Calculate Pitch
	float pitch = 0.0f;
	if (t_lookAtPos.y != 0.0f)
	{
		const float dist = sqrt(t_lookAtPos.x * t_lookAtPos.x + t_lookAtPos.z * t_lookAtPos.z);
		pitch = atan(t_lookAtPos.y / dist);
	}

	// Calculate YAW
	float yaw = 0.0f;
	if (t_lookAtPos.x != 0.0f)
		yaw = atan(t_lookAtPos.x / t_lookAtPos.z);

	if (t_lookAtPos.z > 0)
		yaw += XM_PI;

	setRotation(pitch, yaw, 0.0f);
}

const DirectX::XMVECTOR& Camera::getForwardVector()
{
	return m_forwardVector;
}

const DirectX::XMVECTOR& Camera::getBackwardVector()
{
	return m_backwardVector;
}

const DirectX::XMVECTOR& Camera::getleftVector()
{
	return m_leftVector;
}

const DirectX::XMVECTOR& Camera::getRightVector()
{
	return m_rightVector;
}

void Camera::updateViewMatrix()
{
	// Calculate Rotation Matrix
	XMMATRIX camRotMatrix = XMMatrixRotationRollPitchYaw(m_rotationFloat3.x, m_rotationFloat3.y, m_rotationFloat3.z);

	// Calculate Camera Target
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotMatrix);

	// Offset camera to camera's current position
	camTarget += m_positionVector;

	// Calculate Up Direction
	XMVECTOR upDirection = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotMatrix);

	// Rebuild
	m_viewMatrix = XMMatrixLookAtLH(m_positionVector, camTarget, upDirection);


	XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(m_rotationFloat3.x, m_rotationFloat3.y, 0.0f);

	m_forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotMatrix);
	m_backwardVector = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, rotMatrix);
	m_leftVector = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, rotMatrix);
	m_rightVector = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, rotMatrix);
}

