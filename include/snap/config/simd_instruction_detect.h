//                                 Snap
//                          
//                      Copyright (c) 2016 Rob Clucas        
//                    Distributed under the MIT License
//                (See accompanying file LICENSE or copy at
//                   https://opensource.org/licenses/MIT)
//
//  This is based off the avx detection in Agner Fog's C++ vector library at:
//               http://www.agner.org/optimize/vectorclass.zip
//
// ========================================================================= //
//
/// \file  simd_instruction_detect.h
/// \brief Detects and sets the simd instruction available.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_CONFIG_SIMD_INSTRUCTION_DETECT_H
#define SNAP_CONFIG_SIMD_INSTRUCTION_DETECT_H

#include <stdint.h>

// Specifies the vector instruction set enabled.
#define AVX2_ENABLED 0
#define AVX_ENABLED  0

/// Defines alignment for MSVC compiler.
#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
 #define SNAP_ALIGN(x) __declspec(align(x))
#elif defined(__GNUC__)
 #define SNAP_ALIGN(x) __attribute__((aligned(x)))
 #define SNAP_INLINE   inline __attribute__((always_inline))
#elif defined(__clang__)
 #define SNAP_ALIGN(x) __attribute__ ((align_value(x)))
 #define SNAP_INLINE   inline __attribute__((always_inline))
#else
 #error "Unsupported Compiler!"
#endif

namespace snap {

/// Defines all possible simd instruction types supported by snap.
enum SimdType : uint8_t {
  ST_SSE    = 0 ,
  ST_SSE2   = 1 , 
  ST_SSE3   = 2 , 
  ST_SSSE3  = 4 ,
  ST_SSE41  = 5 ,
  ST_SSE42  = 6 , 
  ST_AVX    = 7 ,
  ST_AVX2   = 8 ,
  ST_NEON   = 9 ,
  ST_NEON64 = 10
};

/// Defines all possible alignments.
enum Alignments : uint8_t {
  AL_8  = 8 , 
  AL_16 = 16,
  AL_32 = 32
};

} // namespace snap

/// Defines the highest level of SIMD 
/// instructions detected by the compiler.
#if defined(__arm__)
#include <arm_neon.h>

namespace snap {
#if defined(__aarch64__)
#define NEON64_ENABLED 1

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_8;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_NEON64;
#else 
#define NEON_ENABLED 1
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_4;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_NEON; 
#endif
} // namespace snap

#elif defined(__AVX2__) && AVX2_ENABLED
#include <immintrin.h>
#define AVX2_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_32;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_AVX2;
} // namespace snap

#elif defined(__AVX__) && AVX_ENABLED
#include <immintrin.h>
#define AVX_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_32;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_AVX;
} // namespace snap

#elif defined(__SSE4_2__)
#include <nmmintrin.h>
#define SSE_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE42;
} // namespace snap

#elif defined(__SSE4_1__)
#include <smmintrin.h>
#define SSE_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE41;
} // namespace snap

#elif defined(__SSSE3__)
#include <tmmintrin.h>
#define SSE_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSSE3;
} // namespace snap

#elif defined(__SSE3__)
#include <pmmintrin.h>
#define SSE_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE3; 
} // namespace snap

#elif defined(__SSE2__) || defined(__x86_64__)
#include <emmintrin.h>
#define SSE_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIND_TYPE = ST_SSE2;
} // namespace snap

#elif defined(__SSE)
#include <xmmintrin.h>
#define SSE_ENABLED 1

namespace snap {
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE;
} // namespace snap

#else
 #error "No SIMD Instructions found! Snap can't function!"
#endif

#define SNAP_ALIGNED SNAP_ALIGN(snap::ALIGNMENT)

#endif // SNAP_CONFIG_SIMD_INSTRUCTION_DETECT_H
