#pragma once

#define CheckFailed(hr, error) if (FAILED(hr)) return error
#define CheckError(code) if (code != 0) return code

template<typename T>

void safeRelease(T* &obj)
{
	if (obj != nullptr)
	{
		obj->Release();
		obj = nullptr;
	}
}

template <typename T, size_t N>
size_t getLength(T(&arr)[N])
{
	return N;
}
