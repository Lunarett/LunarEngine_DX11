#pragma once
#include <chrono>
#include <Windows.h>

class Time
{
public:
	INT init();
	void update();
	void deInit();

	FLOAT getDeltaTime() { return m_deltaTime; }
	FLOAT getTotalTime() { return m_totalTime; }

private:
	std::chrono::high_resolution_clock::time_point m_lastStamp = {};
	FLOAT m_deltaTime = 0;
	FLOAT m_totalTime = 0;
	INT m_fpsCount = 0;
	FLOAT m_fpsTime = 0;
};

