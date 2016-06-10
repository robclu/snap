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

#include "convert_sse.hpp"
#include "matrix_traits.hpp"
#include "snap/allocate/allocator.hpp"
#include "snap/config/config.hpp"
#include "snap/vector/vector.hpp"
#include <array>

namespace snap {

// Only works if alignment is %2
template <bool IsAligned, size_t Cols, size_t Alignement> 
struct get_step;

template <size_t Cols, size_t Alignment>
struct get_step<true, Cols, Alignment> {
  static constexpr size_t value = Cols;
};

template <size_t Cols, size_t Alignment> 
struct get_step<false, Cols, Alignment> {
  static constexpr size_t value = 
    Cols + (Alignment - (Cols & (Alignment - 1)));
};

// ---- Definition : Matrix Fixed ------------------------------------------ //

/// Defines a matrix class with a fixed size.
/// \tparam Format The format of the matrix which defines the type of the
///         elements.
/// \tparam Rows   The number of rows in the matrix.
/// \tparam Cols   The number of columns in the matrix.
template <uint8_t Format, size_t Rows, size_t Cols>
class MatrixFixed {
 public:
  /// The data type of each vectorized element in the matrix.
  using VecDataType  = typename format_traits<Format>::type;
  /// The data type of each element in the vectorized elements.
  using BaseDataType = typename VecDataType::BaseType;

  /// Defines the format of the internal data.
  static constexpr uint8_t formatType = Format;
  /// Defines the type of the matrix.
  static constexpr uint8_t type       = mat::FIXED;
  /// Returns the number of rows at compile time.
  static constexpr size_t  rowsCx     = Rows;
  /// Returns the number of columns at compile time.
  static constexpr size_t  colsCx     = Cols;
  /// Returns the step size of the matrix.
  static constexpr size_t  stepCx = 
    get_step<!(Cols & (ALIGNMENT - 1)), Cols, ALIGNMENT>::value;

  /// Constructor: Creates an empty matrix with the fixed size.
  MatrixFixed();

#ifdef OPENCV
  /// Constructor: Creates a matrix from an OpenCV matrix. This copies the data
  /// so that the data is aligned correctly for the internal vectorized data.
  /// \param[in] cvMat The opencv matrix to use to initialize this matrix.
  MatrixFixed(cv::Mat& cvMat);
#endif 

  /// Data getter: Gets a pointer to the vectorixzed data.
  VecDataType* data() { return Data.data(); }

  /// Data getter: Gets a const pointer to the vectorized data.
  const VecDataType* data() const { return Data.data(); }

  /// Row number getter: Gets the number of rows in the matrix.
  size_t rows() const { return Rows; }

  /// Col number getter: Gets the number of columns in the matrix.
  size_t cols() const { return Cols; }

  /// Step size getter: Gets the step size of the matrix.
  size_t step() const { return step; }

 private:
  /// The array type of the vector elements.
  using ArrayType = std::array<VecDataType, 
                      rowsCx * stepCx / VecDataType::width>;
  
  ArrayType Data; //!< Raw array of vectorized elements. 
};

// ---- Implementation : Matrix Fixed -------------------------------------- //

template <uint8_t F, size_t R, size_t C>
MatrixFixed<F, R, C>::MatrixFixed() {}

#ifdef OPENCV

template <uint8_t F, size_t R, size_t C>
MatrixFixed<F, R, C>::MatrixFixed(cv::Mat& cvMat) {
  // Convert to the correct vectorized format for this matrix.
  mat::convert(cvMat, *this);
}

#endif // OPENCV

// ---- Definition: Matrix Dynamic ----------------------------------------- //

/// Defines a matrix class with a dynamic size.
/// \tparam Format    The format of the matrix which defines the type of the
///         elements.
/// \tparam Allocator The type of allocator used to allocate the data.
template <uint8_t Format, typename Allocator>
class MatrixDynamic {
 public:
  /// The data type of each vectorized element in the matrix.
  using VecDataType  = typename format_traits<Format>::type;
  /// The data type of each element in the vectorized elements.
  using BaseDataType = typename VecDataType::BaseType;

  /// Defines the format of the internal data.
  static constexpr uint8_t formatType = Format;
  /// Defines the type of the matrix.
  static constexpr uint8_t type = mat::DYNAMIC;

  /// Constructor: Creates an empty matrix.
  MatrixDynamic();

  /// Constructor: Creates a matrix with a specific size. If the default
  /// allocator is used, and cols is not a multiple of the alignment 
  /// (snap::ALIGNMENT) each row is padded so that the number of column
  /// elements is a multiple of the alignment requirement.
  /// \param[in] rows The number of rows for the matrix.
  /// \param[in] cols The number of colums for the matrix.
  MatrixDynamic(size_t rows, size_t cols);

#ifdef OPENCV
  /// Constructor: Creates a matrix from an OpenCV matrix. This copies the data
  /// so that the data is aligned correctly for the internal vectorized data.
  /// \param[in] cvMat The opencv matrix to use to initialize this matrix.
  MatrixDynamic(cv::Mat& cvMat);
#endif

  /// Destructor: Cleans up matrix memory.
  ~MatrixDynamic();

  /// Data getter: Gets a pointer to the vectorixzed data.
  VecDataType* data() { return Data; }

  /// Data getter: Gets a const pointer to the vectorized data.
  const VecDataType* data() const { return Data; }

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
  VecDataType*  Data;    //!< Pointer to the raw matrix vectorized data. 
  size_t        Rows;    //!< Number of rows in the matrix.
  size_t        Cols;    //!< Number of columns in the matrix.
  size_t        Step;    //!< Number of elements per row.

  /// Allocates the correct amount of data for the matrix.
  void allocateData();
};

// ---- Implementation : MatrixDynamic ------------------------------------- //

template <uint8_t F, typename A>
MatrixDynamic<F, A>::MatrixDynamic() 
: Data(nullptr), Rows(0), Cols(0), Step(0) {
}

template <uint8_t F, typename A>
MatrixDynamic<F, A>::MatrixDynamic(size_t rows, size_t cols)
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
MatrixDynamic<F, A>::MatrixDynamic(cv::Mat& cvMat)
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
MatrixDynamic<F, A>::~MatrixDynamic() {
  if (Data != nullptr) {
    A::free(Data);
  }
}

// ---- Private ------------------------------------------------------------ //

template <uint8_t F, typename A>
void MatrixDynamic<F, A>::allocateData() {
  Data = A::alloc(Rows * Step, ALIGNMENT); 
}

} // namespace snap

#endif // SNAP_MATRIX_MATRIX_SSE_HPP


