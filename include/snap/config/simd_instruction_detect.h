//---- snap/config/simd_instruction_detect.h --------------- -*- C++ -*- ----//
//
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

namespace snap {

/// Defines alignment for MSVC compiler.
#if defined(_MSC_VER) && defined(_MSC_FULL_VER)
 #define SNAP_ALIGN(x) __declspec(align(x))
#elif defined(__GNUC__)
 #define SNAP_ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(__clang__)
 #define SNAP_ALIGN(x) __attribute__ ((align_value(x)))
#else
 #error "Unsupported Compiler!"
#endif

/// Defines all possible simd instruction 
/// types supported by snap.
enum simd_types : uint8_t {
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
enum alignments : uint8_t {
  AL_8  = 8 , 
  AL_16 = 16,
  AL_32 = 32
};

/// Defines the highest level of SIMD 
/// instructions detected by the compiler.
#if defined(__arm__)
#include <arm_neon.h>

#if defined(__aarch64__)
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_8;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_NEON64;
#else 
  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_4;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_NEON; 
#endif

#elif defined(__AVX2__)
#include <immintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_32;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_AVX2;

#elif defined(__AVX__)
#include <immintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_32;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_AVX;

#elif defined(__SSE4_2__)
#include <nmmintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE42;
#elif defined(__SSE4_1__)
#include <smmintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level of SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE41;

#elif defined(__SSSE3__)
#include <tmmintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSSE3;

#elif defined(__SSE3__)
#include <pmmintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE3; 

#elif defined(__SSE2__) || defined(__x86_64__)
#include <emmintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIND_TYPE = ST_SSE2;

#elif defined(__SSE)
#include <xmmintrin.h>

  /// Defines memory alignment.
  static constexpr uint8_t ALIGNMENT = AL_16;

  /// Defines highest level if SIMD instructions.
  static constexpr uint8_t SIMD_TYPE = ST_SSE;

#else
 #error "No SIMD Instructions found! Snap++ can't function!"
#endif

} // namespace snap

#endif // SNAP_CONFIG_SIMD_INSTRUCTION_DETECT_H
