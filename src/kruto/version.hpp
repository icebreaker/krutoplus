#ifndef KRVERSION_HPP
#define KRVERSION_HPP

#define KR_STRINGIFY_HELPER(a) #a
#define KR_STRINGIFY(a) KR_STRINGIFY_HELPER(a)

#define KR_VERSION_MAJOR 1
#define KR_VERSION_MINOR 0
#define KR_VERSION_BUILD 1337
#define KR_VERSION ((KR_VERSION_MAJOR << 8) | KR_VERSION_MINOR)

#define KR_VERSION_NAME "Kruto+"

#define KR_VERSION_NUMBER \
		KR_STRINGIFY(KR_VERSION_MAJOR) "." \
		KR_STRINGIFY(KR_VERSION_MINOR) "." \
		KR_STRINGIFY(KR_VERSION_BUILD)

#define KR_VERSION_STRING \
		KR_VERSION_NAME "/" \
		KR_VERSION_NUMBER

#define KR_UNUSED(x) (void)(x)

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32)
	#define KR_WIN32
#endif

#ifdef KR_WIN32
	#include <windows.h>
#endif

#ifdef KR_WIN32
	#define KR_MAIN(klass) \
	int WINAPI WinMain( \
		HINSTANCE hInstance, \
		HINSTANCE hPrevInstance, \
		LPSTR lpCmdLine, \
		int nCmdShow) \
	{ \
		klass app(0, NULL); \
		return app.exec(); \
	}
#else
	#define KR_MAIN(klass) \
	int main(int argc, char *argv[]) \
	{ \
		klass app(argc, argv); \
	    return app.exec(); \
	}
#endif

#ifdef KR_DEBUG
	#define KR_ASSERT(exp) \
		if(!(exp)) \
			fprintf(stderr, "ASSERTION: %s in %s on line %f\n" #exp, __FILE__, __LINE__)
#else
	#define KR_ASSERT(exp)
#endif

#define KR_INDEX(x, y, w) (x + (y * w))

#endif
