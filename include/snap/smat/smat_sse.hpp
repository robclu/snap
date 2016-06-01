//---- snap/smat/smat_sse.hpp ------------------------------ -*- C++ -*- ----//
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
/// \file  smat_sse.hpp
/// \brief Defiition of smat class SSE implementation. The possible options 
///        for snap matrix types when using SSE instructions are the following:
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SMAT_SMAT_SSE_HPP
#define SNAP_SMAT_SMAT_SSE_HPP

#include "snap/config/simd_instruction_detect.h"

namespace snap {

/// Defines a matrix class for which SIMD operations can be used to improve
/// processing performance. At the moment, this copies the mat 16 bytes at a
/// time into a vector so the the memory is correctly aligned.
class smat {
 public:
  using vec_type = std::vector<vec16x8u>;
 private:
  uint8_t* data;  //!< Pointer to the raw matrix data. 

 public:
  /// Constructor: Creates an empty matrix.
  smat();
};

} // namespace snap

#endif // SNAP_SMAT_SMAT_SSE_HPP


