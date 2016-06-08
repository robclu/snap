//---- snap/matrix/matrix_sse.hpp -------------------------- -*- C++ -*- ----//
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
/// \file  matrix_sse.hpp
/// \brief Defiition of Matrix class SSE implementation. The possible options 
///        for snap matrix types when using SSE instructions are the following:
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_MATRIX_SSE_HPP
#define SNAP_MATRIX_MATRIX_SSE_HPP

#include "matrix_fwd.hpp"
#include "convert_sse.hpp"
#include "snap/allocate/allocator.hpp"
#include "snap/config/config.hpp"
#include "snap/vector/vector.hpp"

namespace snap {

// Implementation of the matrix class with intel intrinsics.
template <uint8_t  Format, typename Allocator>
class Matrix {
 public:
  /// The data type of each vectorized element in the matrix.
  using DataType  = typename format_traits<Format>::type;

  /// Constructor: Creates an empty matrix.
  Matrix();

  /// Constructor: Creates a matrix with a specific size. If the default
  /// allocator is used, and cols is not a multiple of the alignment 
  /// (snap::ALIGNMENT) each row is padded with empty bytes so that the number
  /// of column elements are aligned.
  /// \param[in] rows The number of rows for the matrix.
  /// \param[in] cols The number of colums for the matrix.
  Matrix(size_t rows, size_t cols);

#ifdef OPENCV
  /// Constructor: Creates a matrix from an OpenCV matrix. This copies the data
  /// so that the data is aligned correctly and is all vectorized.
  /// \param cvMat The opencv matrix to use to initialize the snap matrix.
  Matrix(cv::Mat& cvMat);
#endif

  /// Destructor: Cleans up matrix memory.
  ~Matrix();

  /// Data getter: Gets a pointer to the data.
  DataType* data() { return Data; }

  /// Data getter: Gets a const pointer to the data.
  const DataType* data() const { return Data; }

  /// Row size getter: Gets the number of rows in the matrix.
  size_t rows() const { return Rows; }

  /// Col size getter: Gets the number of columns in the matrix.
  size_t cols() const { return Cols; }

  /// Step size getter: Gets the step size (number of elements per row).
  size_t step() const { return Step; }

  /// Size operation: Gets the total number of elements in the matrix.
  size_t size() const { 
    return Rows * Step; 
  }

 private:
  DataType*  Data;    //!< Pointer to the raw matrix vectorized data. 
  size_t     Rows;    //!< Number of rows in the matrix.
  size_t     Cols;    //!< Number of columns in the matrix.
  size_t     Step;    //!< Number of elements per row.

  /// Allocates the correct amount of data for the matrix.
  void allocateData();
};

// ---- Implementation ----------------------------------------------------- //

template <uint8_t F, typename A>
Matrix<F, A>::Matrix() : Data(nullptr), Rows(0), Cols(0), Step(0) {}

template <uint8_t F, typename A>
Matrix<F, A>::Matrix(size_t rows, size_t cols)
: Rows(rows), Cols(cols) {
  // Add padding if the number of columns is 
  // not a multiple of the alignment size.
  Step = (Cols % ALIGNMENT == 0) 
       ? Cols 
       : Cols + (ALIGNMENT - (Cols % ALIGNMENT));

  allocateData();
}

#ifdef OPENCV

template <uint8_t F, typename A>
Matrix<F, A>::Matrix(cv::Mat& cvMat)
: Rows(cvMat.rows), Cols(cvMat.cols) {
  // Add padding if the number of columns is 
  // not a multiple of the alignment size.
  Step = (Cols % ALIGNMENT == 0) 
       ? Cols 
       : Cols + (ALIGNMENT - (Cols % ALIGNMENT)); 
  
  allocateData();

  // Convert to the correct vectorized format for this matrix.
  mat::convert(cvMat, *this);
}

#endif // OPENCV

template <uint8_t F, typename A>
Matrix<F, A>::~Matrix() {
  if (Data != nullptr) {
    A::free(Data);
  }
}

// ---- Private ------------------------------------------------------------ //

template <uint8_t F, typename A>
void Matrix<F, A>::allocateData() {
  Data = A::alloc(Rows * Step, ALIGNMENT); 
}

} // namespace snap

#endif // SNAP_MATRIX_MATRIX_SSE_HPP


