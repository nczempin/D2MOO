#pragma once

#if !defined(D2_VERSION_MAJOR)
#define D2_VERSION_110F
#define D2_VERSION_MAJOR 1
#define D2_VERSION_MINOR 10
#define D2_VERSION_PATCH 'F'
#endif

#if !defined(D2_IS_MONOLITHIC)
# if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 14
#   define D2_IS_MONOLITHIC TRUE  // Everything is linked statically since 1.14a
# elif D2_VERSION_SHAREWARE
#   define D2_IS_MONOLITHIC TRUE  // But this was also the case for the sharewares !
# else
#   define D2_IS_MONOLITHIC FALSE // By default, we had .DLLs
# endif
#endif

#define D2_VERSION_EXPANSION 1

#define D2_HAS_OPENGL 0
#define D2_HAS_RAVE 0

// TODO: figure out when this was removed
#ifdef D2_VERSION_100
#define D2_HAS_MULTILAN 1
#else
#define D2_HAS_MULTILAN 0
#endif

#if D2_VERSION_MAJOR >= 1 && D2_VERSION_MINOR >= 11
#define D2_VERSION_HAS_UBERS
#endif

#if defined(D2_VERSION_110F)
#define D2_BUILD_NUMBER 39
#elif defined(D2_VERSION_111)
#define D2_BUILD_NUMBER 45
#elif defined(D2_VERSION_111B)
#define D2_BUILD_NUMBER 46
#elif defined(D2_VERSION_112A)
#define D2_BUILD_NUMBER 49
#elif defined(D2_VERSION_113C)
#define D2_BUILD_NUMBER 60
#elif defined(D2_VERSION_113D) || defined(D2_VERSION_114A)
#define D2_BUILD_NUMBER 64
#elif defined(D2_VERSION_114B)
#define D2_BUILD_NUMBER 68
#elif defined(D2_VERSION_114C)
#define D2_BUILD_NUMBER 70
#elif defined(D2_VERSION_114D)
#define D2_BUILD_NUMBER 71
#else
#define D2_BUILD_NUMBER 0
#endif

#define D2_VERSION_PACKED \
	((D2_VERSION_MAJOR * 1000000) + (D2_VERSION_MINOR * 10000) + ((D2_VERSION_PATCH - 'A' + 1) * 100) + D2_BUILD_NUMBER)

