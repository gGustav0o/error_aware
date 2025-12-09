#pragma once

// --- Feature-test helpers ----------------------------------------------------
#ifndef EA_HAS_CPP_ATTR
#  if defined(__has_cpp_attribute)
#    define EA_HAS_CPP_ATTR(x) __has_cpp_attribute(x)
#  else
#    define EA_HAS_CPP_ATTR(x) 0
#  endif
#endif

#ifndef EA_HAS_ATTR
#  if defined(__has_attribute)
#    define EA_HAS_ATTR(x) __has_attribute(x)
#  else
#    define EA_HAS_ATTR(x) 0
#  endif
#endif

#ifndef EA_HAS_DECLSPEC_ATTR
#  if defined(__has_declspec_attribute)
#    define EA_HAS_DECLSPEC_ATTR(x) __has_declspec_attribute(x)
#  else
#    define EA_HAS_DECLSPEC_ATTR(x) 0
#  endif
#endif

#ifndef EA_HAS_BUILTIN
#  if defined(__has_builtin)
#    define EA_HAS_BUILTIN(x) __has_builtin(x)
#  else
#    define EA_HAS_BUILTIN(x) 0
#  endif
#endif

#ifndef EA_STRINGIFY
#  define EA_STRINGIFY_IMPL(x) #x
#  define EA_STRINGIFY(x) EA_STRINGIFY_IMPL(x)
#endif

#ifndef EA_CPLUSPLUS
#  if defined(_MSVC_LANG)
#    define EA_CPLUSPLUS _MSVC_LANG
#  else
#    define EA_CPLUSPLUS __cplusplus
#  endif
#endif

// --- Compiler detection ------------------------------------------------------
#if defined(_MSC_VER)
#  define EA_COMPILER_MSVC 1
#else
#  define EA_COMPILER_MSVC 0
#endif

#if defined(__clang__)
#  define EA_COMPILER_CLANG 1
#else
#  define EA_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !EA_COMPILER_CLANG
#  define EA_COMPILER_GCC 1
#else
#  define EA_COMPILER_GCC 0
#endif

#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#  define EA_COMPILER_INTEL 1
#else
#  define EA_COMPILER_INTEL 0
#endif

#if defined(__CUDACC__) || defined(__CUDA_ARCH__)
#  define EA_COMPILER_NVCC 1
#else
#  define EA_COMPILER_NVCC 0
#endif

// --- Branch prediction hints -------------------------------------------------
#if (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_LIKELY(x)   __builtin_expect(!!(x), 1)
#  define EA_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#  define EA_LIKELY(x)   (x)
#  define EA_UNLIKELY(x) (x)
#endif

// --- Function hot/cold sections ---------------------------------------------
#if (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_HOT   __attribute__((hot))
#  define EA_COLD  __attribute__((cold))
#else
#  define EA_HOT
#  define EA_COLD
#endif

// --- Force-inline / noinline -------------------------------------------------
#if EA_COMPILER_MSVC
#  define EA_FORCEINLINE __forceinline
#  define EA_NOINLINE    __declspec(noinline)
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_FORCEINLINE inline __attribute__((always_inline))
#  define EA_NOINLINE    __attribute__((noinline))
#else
#  define EA_FORCEINLINE inline
#  define EA_NOINLINE
#endif

// --- Standard-ish attributes -------------------------------------------------
// [[nodiscard]] / warn_unused_result
#if (EA_HASCPPATTR_nodiscard) // helper to allow nice preprocessor use
#  define EA_NODISCARD [[nodiscard]]
#elif EA_HAS_CPP_ATTR(nodiscard) || (EA_CPLUSPLUS >= 201703L)
#  define EA_NODISCARD [[nodiscard]]
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_NODISCARD __attribute__((warn_unused_result))
#elif EA_COMPILER_MSVC
#  define EA_NODISCARD _Check_return_
#else
#  define EA_NODISCARD
#endif

// [[noreturn]]
#if EA_HAS_CPP_ATTR(noreturn) || (EA_CPLUSPLUS >= 201103L)
#  define EA_NORETURN [[noreturn]]
#elif EA_COMPILER_MSVC
#  define EA_NORETURN __declspec(noreturn)
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_NORETURN __attribute__((noreturn))
#else
#  define EA_NORETURN
#endif

// [[deprecated("msg")]]
#if EA_HAS_CPP_ATTR(deprecated) || (EA_CPLUSPLUS >= 201402L)
#  define EA_DEPRECATED(msg) [[deprecated(msg)]]
#  define EA_DEPRECATED_     [[deprecated]]
#elif EA_COMPILER_MSVC
#  define EA_DEPRECATED(msg) __declspec(deprecated(msg))
#  define EA_DEPRECATED_
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_DEPRECATED(msg) __attribute__((deprecated(msg)))
#  define EA_DEPRECATED_
#else
#  define EA_DEPRECATED(msg)
#  define EA_DEPRECATED_
#endif

// [[fallthrough]] (C++17) – use no-op when unavailable
#if EA_HAS_CPP_ATTR(fallthrough) || (EA_CPLUSPLUS >= 201703L)
#  define EA_FALLTHROUGH [[fallthrough]]
#elif EA_HAS_CPP_ATTR(gnu::fallthrough)
#  define EA_FALLTHROUGH [[gnu::fallthrough]]
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_FALLTHROUGH __attribute__((fallthrough))
#else
#  define EA_FALLTHROUGH ((void)0)
#endif

// [[likely]] / [[unlikely]] (C++20) – statement attributes; prefer EA_LIKELY/UNLIKELY for expressions
#if EA_HAS_CPP_ATTR(likely) && (EA_CPLUSPLUS >= 202002L)
#  define EA_ATTR_LIKELY [[likely]]
#else
#  define EA_ATTR_LIKELY
#endif
#if EA_HAS_CPP_ATTR(unlikely) && (EA_CPLUSPLUS >= 202002L)
#  define EA_ATTR_UNLIKELY [[unlikely]]
#else
#  define EA_ATTR_UNLIKELY
#endif

// --- Purity & const-ness (no observable state changes) ----------------------
#if (defined(IMAGEFX_NO_DEBUG) || defined(NDEBUG)) && (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_PURE      __attribute__((pure))      // depends only on args / global state, no side effects
#  define EA_CONST_FN  __attribute__((const))     // depends only on args, no global memory access
#else
#  define EA_PURE
#  define EA_CONST_FN
#endif

// --- malloc-like result (non-aliasing, fresh memory) ------------------------
#if (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_MALLOC_FN __attribute__((malloc))
#else
#  define EA_MALLOC_FN
#endif

// --- Alignment of declarations ----------------------------------------------
#if EA_COMPILER_MSVC
#  define EA_ALIGNED(n) __declspec(align(n))
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_ALIGNED(n) __attribute__((aligned(n)))
#else
#  define EA_ALIGNED(n)
#endif

// --- Restrict keyword (pointer aliasing hint) -------------------------------
#if EA_COMPILER_MSVC
#  define EA_RESTRICT __restrict
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_RESTRICT __restrict__
#else
#  define EA_RESTRICT
#endif

// --- Unreachable & assume ----------------------------------------------------
#if EA_COMPILER_MSVC
#  define EA_UNREACHABLE() __assume(0)
#  define EA_ASSUME(cond)  __assume(cond)
#elif EA_COMPILER_CLANG
#  define EA_UNREACHABLE() __builtin_unreachable()
#  if EA_HAS_BUILTIN(__builtin_assume)
#    define EA_ASSUME(cond) __builtin_assume(cond)
#  else
#    define EA_ASSUME(cond) do { if (!(cond)) __builtin_unreachable(); } while(0)
#  endif
#elif EA_COMPILER_GCC
#  define EA_UNREACHABLE() __builtin_unreachable()
#  define EA_ASSUME(cond)  do { if (!(cond)) __builtin_unreachable(); } while(0)
#else
#  define EA_UNREACHABLE() ((void)0)
#  define EA_ASSUME(cond)  ((void)0)
#endif

// --- Format checking (printf/scanf style) -----------------------------------
#if (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_FORMAT(archetype, fmt_index, vararg_index) __attribute__((format(archetype, fmt_index, vararg_index)))
#else
#  define EA_FORMAT(archetype, fmt_index, vararg_index)
#endif

// --- Visibility / DLL export/import -----------------------------------------
#if EA_COMPILER_MSVC
#  define EA_DLL_EXPORT __declspec(dllexport)
#  define EA_DLL_IMPORT __declspec(dllimport)
#  define EA_DLL_LOCAL
#elif (EA_COMPILER_GCC || EA_COMPILER_CLANG)
#  define EA_DLL_EXPORT __attribute__((visibility("default")))
#  define EA_DLL_IMPORT __attribute__((visibility("default")))
#  define EA_DLL_LOCAL  __attribute__((visibility("hidden")))
#else
#  define EA_DLL_EXPORT
#  define EA_DLL_IMPORT
#  define EA_DLL_LOCAL
#endif

// --- Warning control (push/pop & disable) -----------------------------------
#if EA_COMPILER_CLANG
#  define EA_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#  define EA_DIAGNOSTIC_POP  _Pragma("clang diagnostic pop")
#  define EA_DIAGNOSTIC_IGNORE(w) _Pragma(EA_STRINGIFY(clang diagnostic ignored w)) // pass string literal: "-W..."
#elif EA_COMPILER_GCC
#  define EA_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#  define EA_DIAGNOSTIC_POP  _Pragma("GCC diagnostic pop")
#  define EA_DIAGNOSTIC_IGNORE(w) _Pragma(EA_STRINGIFY(GCC diagnostic ignored w))
#elif EA_COMPILER_MSVC
#  define EA_DIAGNOSTIC_PUSH __pragma(warning(push))
#  define EA_DIAGNOSTIC_POP  __pragma(warning(pop))
#  define EA_DIAGNOSTIC_IGNORE(n) __pragma(warning(disable: n)) // pass number, e.g. 4996
#else
#  define EA_DIAGNOSTIC_PUSH
#  define EA_DIAGNOSTIC_POP
#  define EA_DIAGNOSTIC_IGNORE(x)
#endif

// --- Flatten / target specific (best-effort) --------------------------------
#if EA_COMPILER_GCC
#  define EA_FLATTEN __attribute__((flatten))
#else
#  define EA_FLATTEN
#endif

// Example for x86 vectorcall on MSVC & Clang-cl; no-op elsewhere.
#if EA_COMPILER_MSVC
#  define EA_VECTORCALL __vectorcall
#else
#  define EA_VECTORCALL
#endif


#if EA_COMPILER_MSVC
#  define EA_FUNCTION_SIGNATURE __FUNCSIG__
#else
#  define EA_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif