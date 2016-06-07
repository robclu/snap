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

#include "matrix_general.hpp"
#include "snap/allocate/allocator.hpp"
#include "snap/config/simd_instruction_detect.h"
#include "snap/vector/vector.hpp"

namespace snap {

// Specialization for when the format is greyscale.
template <>
struct format_traits<mat::FM_GREY_8> {
  /// Defines the data type used for 8-bit greyscale values.
  using type = Vec16x8u;
};

/// Defines a matrix class for which SIMD operations can be used to improve
/// processing performance. The constructor always allocates data for the
/// elements, and the data is stored at arrays of svec types to ensure
/// alignment and faster processing. The initial overhead of the memory
/// allocation is insignificant relative to the additional performance gained
/// by the vectorized and correctly aligned data.
///
/// The alignment requirement of the data means that extra data might be used
/// if the number of matrix elements is not a multiple of the alignment
/// requirement for the vectorized data types. This is typically a very small
/// overhead, as the max number of additional elements will be less than 32.
///
/// The matrix should be allocated initiallly, and the data can then be
/// overwritten using the load operations.
///
/// \tparam Format    The format of the matrix.
/// \tparam Allocator The allocator for the data.
template <
  uint8_t  Format, 
  typename Allocator = AlignedAllocator<typename format_traits<Format>::type>
  >
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
  Matrix(size_t rows, size_t cols);

  /// Destructor: Cleans up matrix memory.
  ~Matrix();

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


