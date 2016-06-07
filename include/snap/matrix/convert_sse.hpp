//---- snap/matrix/convert_sse.hpp ------------------------- -*- C++ -*- ----//
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
/// \file  convert_sse.hpp
/// \brief Defines conversion functions for matrices which use SSE itrinsics.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_CONVERT_SSE_HPP
#define SNAP_MATRIX_CONVERT_SSE_HPP

#include "format_traits_sse.hpp"
#include "matrix_sse.hpp"

namespace snap {
namespace mat  {

/// Defines a class to convert matrices from one format to another.
/// \tparam Formats The formats to convert to/from.
template <uint8_t... Formats>
struct Convert;

// Specialization for snap greyscale matrices.
template <>
struct Convert<FM_GREY_8> {
  using ConvertType = Convert<FM_GREY_8>;

  // Define the untoll size.
  static constexpr size_t UNROLL_SIZE = 8;

  /// Converts an opencv matrix to a snap matrix. It is assumed that the snap
  /// matrix is compatible with the OpenCv matrix (the step size and number of
  /// rows are the same).
  /// \param[out] outMat  The output matrix.
  /// \param[in]  inMat   The data from the input matrix
  static void cvToSnap(Matrix<FM_GREY_8>& outMat, uint8_t* inMat) {
    // Need to add padding calculation.
    

    for (size_t i = 0; i < outMat.size(); i += ConvertType::UNROLL_SIZE) {
    
    }
  }

};


} // namespace mat
} // namespace snap

#endif // SNAP_MATRIX_CONVERT_SSE_HPP

