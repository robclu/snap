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

#include "matrix_sse.hpp"
#include "snap/config/opencv.h"
#include "iostream"

namespace snap {
namespace mat  {

/// Defines a class to convert matrices with different formats and with
/// different representations. This function just provides the interface, the
/// conversion dispatcher calls the appropriate conversion function based on
/// the matrix type and the format. Where possible, the dispatching is done
/// statically, and where not possible, runtime dispatching is required.
/// \tparam MatFromType Type of matrix being converted from.
/// \tparam MatToType   Type of matrix being converted to.
template <typename MatFromType, typename MatToType>
static inline void convert(const MatFromType& matFrom, MatToType& matTo);

namespace detail {

/// Defines a class to dispatch the appropriate conversion function.
/// /tparam MatTypes The types of the matrices being converted to and from.
template <typename... MatType>
struct conversion_dispatcher;


// Specialization for a snap SSE matrix to a snap SSE matrix.
template <uint8_t FormatA, uint8_t FormatB>
struct conversion_dispatcher<Matrix<FormatA>, Matrix<FormatB>> {
};


#ifdef OPENCV

// Specialization for opencv matrix to snap greyscale matrix.
template <>
struct conversion_dispatcher<cv::Mat, Matrix<FM_GREY_8>> {
  // Conversion implementation function.
  static inline void convertImpl(const cv::Mat&     matFrom, 
                                 Matrix<FM_GREY_8>& matTo  ) {
    std::cout << "cv::mat to snap matrix\n";
  }
};

// Specialization for snap grepscale matrix to opencv matrix.
template <>
struct conversion_dispatcher<Matrix<FM_GREY_8>, cv::Mat> {
  // Conversion implementation.
  static inline void convertImpl(const Matrix<FM_GREY_8>& matFrom, 
                                 cv::Mat&                 matTo  ) {
  }
};

#endif // OPENCV

} // namespace detail

template <typename MatFromType, typename MatToType>
static inline void convert(const MatFromType& matFrom, MatToType& matTo) {
  detail::conversion_dispatcher<MatFromType, MatToType>::convertImpl(
    matFrom, matTo
  );
}

} // namespace mat
} // namespace snap

#endif // SNAP_MATRIX_CONVERT_SSE_HPP

