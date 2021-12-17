#include "Time.h"
#include <string>

using namespace std;
using namespace std::chrono;

INT Time::init()
{
	m_lastStamp = high_resolution_clock::now();

	return 0;
}

void Time::update()
{
	high_resolution_clock::time_point now = high_resolution_clock::now();
	duration<FLOAT> diff = now - m_lastStamp;
	m_deltaTime = diff.count();
	m_totalTime += m_deltaTime;
	m_lastStamp = now;

	m_fpsCount++;
	m_fpsTime += m_deltaTime;

#if _DEBUG
	static INT actualFPS = 0;

	if (m_fpsTime >= 1.0f)
	{
		m_fpsTime -= 1.0f;
		actualFPS = m_fpsCount;
		m_fpsCount = 0;
	}

#if UNICODE
	wstring output = TEXT("deltaTime: ") + to_wstring(m_deltaTime) + TEXT(" s\n");
	output += TEXT("fps: ") + to_wstring(actualFPS) + TEXT("\n");
#else // UNICODE
	string output = TEXT("deltaTime: ") + to_string(m_deltaTime) + TEXT(" s\n");
	output += TEXT("fps: ") + to_string(actualFPS) + TEXT("\n");
#endif // UNICODE
	OutputDebugString(output.c_str());


#endif // _DEBUG

}

void Time::deInit()
{
}
