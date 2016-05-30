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

namespace snap {

/// Defines a matrix class for which SIMD operations can be used to improve
/// processing performance.
class smat {
 private:
  uint8_t* data;  //!< Pointer to the raw matrix data. 

 public:
  /// Constructor: Creates an empty matrix.
  smat();
};

} // namespace snap

#endif // SNAP_SMAT_SMAT_SSE_HPP


