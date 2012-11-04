#ifndef FFTW_NEW
#define FFTW_NEW

#include "fftw3.h"
#include <memory>

void* operator new[](std::size_t _Count)
{
	return fftw_malloc(_Count);
}

void operator delete[](void* _Ptr)
{
	fftw_free(_Ptr);
}

#endif