#pragma once

#define EA_VERSION_MAJOR 0
#define EA_VERSION_MINOR 1
#define EA_VERSION_PATCH 0

#ifndef EA_HAS_EIGEN
#define EA_HAS_EIGEN 0
#endif

#define EA_STRINGIFY2(x) #x
#define EA_STRINGIFY(x) EA_STRINGIFY2(x)

#ifndef EA_LOG_EXPAND_COND
#  define EA_LOG_EXPAND_COND 0
#endif

#if EA_LOG_EXPAND_COND
#  define EA_COND_STR(cond) EA_STRINGIFY(cond)
#else
#  define EA_COND_STR(cond) EA_STRINGIFY2(cond)
#endif

#define EA_PRAGMA(x) _Pragma(EA_STRINGIFY(x))

#ifdef _MSC_VER
  #undef  EA_PRAGMA
  #define EA_PRAGMA(x) __pragma(x)
#endif


#if defined(EA_NO_PARALLEL) || !defined(_OPENMP)
    #define EA_OMP(x)
#else
    #define EA_OMP(x) EA_PRAGMA(x)
#endif

#define EA_OMP_PARALLEL_FOR_PRIVATE(...)         EA_OMP(omp parallel for private(__VA_ARGS__))
#define EA_OMP_PARALLEL_FOR_FIRSTPRIVATE(...)    EA_OMP(omp parallel for firstprivate(__VA_ARGS__))
#define EA_OMP_FOR_SCHEDULE(kind, chunk)         EA_OMP(omp for schedule(kind, chunk))
#define EA_OMP_PARALLEL_FOR_SCHED(kind, ch)      EA_OMP(omp parallel for schedule(kind, ch))
#define EA_OMP_PARALLEL_FOR                      EA_OMP(omp parallel for)
#define EA_OMP_PARALLEL                          EA_OMP(omp parallel)
#define EA_OMP_FOR                               EA_OMP(omp for)

#ifndef EA_DEBUG
    #if defined(EA_ENABLE_DEBUG) && (EA_ENABLE_DEBUG + 0)
        #define EA_DEBUG 1
    #elif defined(EA_DISABLE_DEBUG) && (EA_DISABLE_DEBUG + 0)
        #define EA_DEBUG 0
    #elif defined(_MSC_VER)
        #if defined(_DEBUG) && !defined(NDEBUG)
            #define EA_DEBUG 1
        #else
            #define EA_DEBUG 0
        #endif
    #else
        #if !defined(NDEBUG)
            #define EA_DEBUG 1
        #else
            #define EA_DEBUG 0
        #endif
    #endif
#endif


#ifndef EA_RELEASE
    #if defined(EA_ENABLE_DEBUG) && (EA_ENABLE_DEBUG + 0)
        #define EA_RELEASE 0
    #elif defined(EA_DISABLE_DEBUG) && (EA_DISABLE_DEBUG + 0)
        #define EA_RELEASE 1
    #elif defined(_MSC_VER)
        #if defined(_DEBUG) && !defined(NDEBUG)
            #define EA_RELEASE 0
        #else
            #define EA_RELEASE 1
        #endif
    #else
        #if !defined(NDEBUG)
            #define EA_RELEASE 0
        #else
            #define EA_RELEASE 1
        #endif
    #endif
#endif

#if EA_DEBUG
    #undef  EA_DEBUG
    #define EA_DEBUG 1
#else
    #undef  EA_DEBUG
    #define EA_DEBUG 0
#endif

#if EA_RELEASE
    #undef  EA_RELEASE
    #define EA_RELEASE 1
#else
    #undef  EA_RELEASE
    #define EA_RELEASE 0
#endif

#ifdef _MSC_VER
    #define EA_ASSUME(cond) __assume(cond)
#elif defined(__clang__)
    #define EA_ASSUME(cond) __builtin_assume(cond)
#elif defined(__GNUC__)
    #define EA_ASSUME(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)
#else
    #define EA_ASSUME(cond)
#endif

#if EA_DEBUG
    #include <cassert>
    #define ea_assert(x) assert(x)
#else
    #define ea_assert(x) (void(0))
#endif

#if !defined(EA_DISABLE_SIMD) && defined(__has_include)
    #if __has_include(<simd>)
        #define EA_HAS_SIMD 1
        #include <simd>
    #elif __has_include(<experimental/simd>)
        #define EA_HAS_SIMD 1
        #include <experimental/simd>
    #else
        #define EA_HAS_SIMD 0
    #endif
#else
    #define EA_HAS_SIMD 0
#endif

