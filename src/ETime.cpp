#include <stdafx.h>
#include <ETime.h>
#ifdef PLATFORM_UNIX
    #include <sys/time.h>
#endif
//cpu Ticks
extern Sphere::bit64 Sphere::GetTimeTicks(){
	Sphere::bit64 val;
	#if defined(_MSC_VER)
		QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	#else
		timeval timeVal;

		gettimeofday( &timeVal, NULL );

		val = (Sphere::bit64)timeVal.tv_sec * (1000*1000) + (Sphere::bit64)timeVal.tv_usec;
	#endif
	return val;
}
//time
extern double Sphere::GetTime(){
	static double	coe;

#if defined(_MSC_VER)
	static Sphere::bit64 freq;

	if ( freq == 0 )
	{
		QueryPerformanceFrequency( (LARGE_INTEGER *)&freq );
		coe = 1000.0 / freq;
	}

#else
	coe = 1.0 / 1000.0;

#endif

	return GetTimeTicks() * coe;
}
