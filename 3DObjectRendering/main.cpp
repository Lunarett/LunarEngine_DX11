#include <Windows.h>
#include <DirectXMath.h>
#include "Engine/Window.h"
#include "Graphics/Direct3D.h"
#include "Helpers/Utilities.h"
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Engine/Time.h"
#include "Graphics/Material.h"
#include "Graphics/Buffers/LightBuffer.h"

FLOAT oscillate(FLOAT initY, FLOAT amplitude, FLOAT frequency, FLOAT totalTime, FLOAT phase) { return initY + (amplitude * sin(2 * XM_PI * frequency * totalTime + phase)); }

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	UINT width = 1920;
	UINT height = 1080;
	BOOL isFullscreen = true;

	// Create Window
	Window window = {};
	int error = window.init(hInstance, width, height);
	CheckError(error);

	// Create Direct3D
	Direct3D d3d = {};
	error = d3d.init(window.getWindowHandle(), width, height, isFullscreen);
	CheckError(error);

	
	// Create Cube
	Mesh cube = {};
	XMFLOAT3 cubePos = { 0.0f, 0, 0 };
	XMFLOAT3 cubeRot = { 0, 0, 0 };
	XMFLOAT3 cubeScale = { 1, 1, 1 };
	error = cube.loadMesh(d3d.getDevice(), "Mesh/Cube.obj", cubePos, cubeRot, cubeScale);
	CheckError(error);

	// Create Cone
	Mesh cone = {};
	XMFLOAT3 conePos = { 3, 0, 0 };
	XMFLOAT3 coneRot = { 0, 0, 0 };
	XMFLOAT3 coneScale = { 1, 1, 1 };
	error = cone.loadMesh(d3d.getDevice(), "Mesh/Cone.obj", conePos, coneRot, coneScale);
	CheckError(error);

	// Create Sphere
	Mesh sphere = {};
	XMFLOAT3 spherePos = { 6.0f, 0, 0 };
	XMFLOAT3 sphereRot = { 0, 0, 0 };
	XMFLOAT3 sphereScale = { 1, 1, 1 };
	error = sphere.loadMesh(d3d.getDevice(), "Mesh/Sphere.obj", spherePos, sphereRot, sphereScale);
	CheckError(error);

	// Create Torus
	Mesh torus = {};
	XMFLOAT3 torusPos = { 9.0f, 0, 0 };
	XMFLOAT3 torusRot = { 0, 0, 0 };
	XMFLOAT3 torusScale = { 1, 1, 1 };
	error = torus.loadMesh(d3d.getDevice(), "Mesh/Torus.obj", torusPos, torusRot, torusScale);
	CheckError(error);

	// Create Monkey
	Mesh monkey = {};
	XMFLOAT3 monkeyPos = { 12.0f, 0, 0 };
	XMFLOAT3 monkeyRot = { 0, 9.5f, 0 };
	XMFLOAT3 monkeyScale = { 1, 1, 1 };
	error = monkey.loadMesh(d3d.getDevice(), "Mesh/Monkey.obj", monkeyPos, monkeyRot, monkeyScale);
	CheckError(error);



	// Create Camera
	Camera camera = {};
	camera.setPosition(0.0f, 0.0f, -4.0f);
	camera.setProjectionValues(60.0f, width, height, 0.1f, 1000.0f);

	// Create Time
	Time time = {};
	error = time.init();
	CheckError(error);


	// Create Cube Material
	Material cubeMat = {};
	error = cubeMat.init(d3d.getDevice(), TEXT("Textures/Texture.jpg"), TEXT("Graphics/Shaders/Compiled/MattVertexShader.cso"), TEXT("Graphics/Shaders/Compiled/MattPixelShader.cso"));
	CheckError(error);

	// Create Cone Material
	Material coneMat = {};
	error = coneMat.init(d3d.getDevice(), TEXT("Textures/Texture2.jpg"), TEXT("Graphics/Shaders/Compiled/MattVertexShader.cso"), TEXT("Graphics/Shaders/Compiled/MattPixelShader.cso"));
	CheckError(error);

	// Create Sphere Material
	Material sphereMat = {};
	error = sphereMat.init(d3d.getDevice(), TEXT("Textures/Texture4.jpg"), TEXT("Graphics/Shaders/Compiled/GlossyVertexShader.cso"), TEXT("Graphics/Shaders/Compiled/GlossyPixelShader.cso"));
	CheckError(error);

	// Create Torus Material
	Material torusMat = {};
	error = torusMat.init(d3d.getDevice(), TEXT("Textures/Texture3.jpg"), TEXT("Graphics/Shaders/Compiled/GlossyVertexShader.cso"), TEXT("Graphics/Shaders/Compiled/GlossyPixelShader.cso"));
	CheckError(error);
	
	// Create Monkey Material
	Material monkeyMat = {};
	error = monkeyMat.init(d3d.getDevice(), TEXT("Textures/BlankR.jpg"), TEXT("Graphics/Shaders/Compiled/GlossyVertexShader.cso"), TEXT("Graphics/Shaders/Compiled/GlossyPixelShader.cso"));
	CheckError(error);



	// Create Light
	LightBuffer light = {};
	light.ambientColor = { 0.2f, 0.15f, 0.1f, 1.0f };
	light.lightDirection = { -0.5f, 0.5f, -1.0f };
	light.diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.specularPower = 32;
	light.specularColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	cubeMat.setLight(d3d.getDeviceContext(), light);
	coneMat.setLight(d3d.getDeviceContext(), light);
	sphereMat.setLight(d3d.getDeviceContext(), light);
	torusMat.setLight(d3d.getDeviceContext(), light);
	monkeyMat.setLight(d3d.getDeviceContext(), light);

	FLOAT coneInitY = cone.getPositionFloat3().y;
	FLOAT sphereInitY = sphere.getPositionFloat3().y;
	FLOAT monkeyInitY = monkey.getPositionFloat3().y;

	while (true)
	{
		if (!window.run())
			break;
		
		FLOAT rotSpeed = 1.0f * time.getDeltaTime();

		// Update
		time.update();
		camera.update(time.getDeltaTime());
		monkey.update(time.getDeltaTime());
		monkey.setRotation(monkey.getPositionFloat3().x, oscillate(monkeyInitY + 9.3f, 0.2f, 1, time.getTotalTime(), 0), monkey.getPositionFloat3().z);
		torus.update(time.getDeltaTime());
		torus.adjustRotation(rotSpeed, rotSpeed / 2, 0);
		sphere.update(time.getDeltaTime());
		sphere.setPosition(sphere.getPositionFloat3().x, -oscillate(sphereInitY, 1, 0.5f, time.getTotalTime(), 0), sphere.getPositionFloat3().z);
		sphere.adjustRotation(0, rotSpeed / 1.5f, 0);
		cone.update(time.getDeltaTime());
		cone.setPosition(cone.getPositionFloat3().x, oscillate(coneInitY, 1, 0.5f, time.getTotalTime(), 0), cone.getPositionFloat3().z);
		cube.update(time.getDeltaTime());
		cube.adjustRotation(0, rotSpeed, 0);

		
		// Draw
		d3d.beginScene(0.0f, 0.0f, 0.0f);

		// Render
		cubeMat.render(d3d.getDeviceContext());
		cubeMat.setMatrices(d3d.getDeviceContext(), cube.getWorldMatrix4x4(), camera.getViewMatrix4x4(), camera.getProjectionMatrix4x4(), camera.getPositionFloat3());
		cube.render(d3d.getDeviceContext());

		coneMat.render(d3d.getDeviceContext());
		coneMat.setMatrices(d3d.getDeviceContext(), cone.getWorldMatrix4x4(), camera.getViewMatrix4x4(), camera.getProjectionMatrix4x4(), camera.getPositionFloat3());
		cone.render(d3d.getDeviceContext());

		sphereMat.render(d3d.getDeviceContext());
		sphereMat.setMatrices(d3d.getDeviceContext(), sphere.getWorldMatrix4x4(), camera.getViewMatrix4x4(), camera.getProjectionMatrix4x4(), camera.getPositionFloat3());
		sphere.render(d3d.getDeviceContext());

		torusMat.render(d3d.getDeviceContext());
		torusMat.setMatrices(d3d.getDeviceContext(), torus.getWorldMatrix4x4(), camera.getViewMatrix4x4(), camera.getProjectionMatrix4x4(), camera.getPositionFloat3());
		torus.render(d3d.getDeviceContext());

		monkeyMat.render(d3d.getDeviceContext());
		monkeyMat.setMatrices(d3d.getDeviceContext(), monkey.getWorldMatrix4x4(), camera.getViewMatrix4x4(), camera.getProjectionMatrix4x4(), camera.getPositionFloat3());
		monkey.render(d3d.getDeviceContext());

		d3d.endScene();
	}

	monkeyMat.deInit();
	torusMat.deInit();
	sphereMat.deInit();
	coneMat.deInit();
	cubeMat.deInit();
	time.deInit();
	camera.deInit();
	monkey.deInit();
	sphere.deInit();
	torus.deInit();
	cone.deInit();
	cube.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}