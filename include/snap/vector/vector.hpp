//---- snap/vector/vector.hpp ------------------------------ -*- C++ -*- ----//
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
/// \file  vector.hpp
/// \brief Main header file for Vector class to include the relevant headers
///        based on the vector instruction set enabled. It also provides 
///        the aliases for the available types for each set of vector
///        instructions.
///
///        Aliases define vectors using the following format:
///
///         <Vec><Width>x<Data Type>, where:
///
///           Width     : Number of elements in the vector.
///           Data Type : Type of each of the Width elements in the vector.
///
/// \note  The aliases use the prefix Vec rather than Vector to make it more
///        clean in code which uses the aliases, that the alias is being used
///        rather than the actual vector type. 
//
//---------------------------------------------------------------------------//

#ifndef SNAP_VECTOR_VECTOR_HPP
#define SNAP_VECTOR_VECTOR_HPP

#include "snap/config/simd_instruction_detect.h"

#ifdef NEON_ENABLED

#ifdef NEON64_ENABLED

#endif // NEON64_ENABLED

#elif SSE_ENABLED 
#include "vector_sse.hpp"

namespace snap {

using Vec16x8u = Vector<uint8_t, 16>; //!< A 16 element vec of 8-bit uints.
using Vec16x8s = Vector<int8_t , 16>; //!< A 16 element vec of 8-bit sints.

} // namespace snap

#elif AVX_ENABLED 

#elif AVX2_ENABLED 

#else 
 #error "No vector instructions are enabled"
#endif

#endif // SNAP_SVEC_SVEC_HPP
