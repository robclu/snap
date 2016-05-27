//---- snap/svec/svec.hpp ---------------------------------- -*- C++ -*- ----//
//
//                                 Snap
//                          
//                      Copyright (c) 2016 Rob Clucas        
//                    Distributed under the MIT License
//                (See accompanying file LICENSE or copy at
//                   https://opensource.org/licenses/MIT)
//
// ========================================================================= //
//
/// \file  svec.hpp
/// \brief Main header file for svec class to include the relevant headers
///        based on the vector instruction set enabled. It also provides 
///        the aliases for the available types for each set of vector
///        instructions.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SVEC_SVEC_HPP
#define SNAP_SVEC_SVEC_HPP

#include "snap/config/simd_instruction_detect.h"

#ifdef NEON_ENABLED

#ifdef NEON64_ENABLED

#endif // NEON64_ENABLED

#elif SSE_ENABLED 
#include "svec_sse.hpp"

namespace snap {

using svec16x8u = svec<uint8_t, 16>; //!< A 16 element vec of 8-bit uints.
using svec16x8s = svec<int8_t , 16>; //!< A 16 element vec of 8-bit sints.

} // namespace snap

#elif AVX_ENABLED 

#elif AVX2_ENABLED 

#else 
 #error "No vector instructions are enabled"
#endif

#endif // SNAP_SVEC_SVEC_HPP
