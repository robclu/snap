//---- snap/matrix/matrix_base.hpp ------------------------- -*- C++ -*- ----//
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
/// \file  matrix_base.hpp
/// \brief Defines a base CRTP class for a matrix class.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_MATRIX_BASE_HPP
#define SNAP_MATRIX_MATRIX_BASE_HPP

namespace snap {

/// Defines a matrix class which uses SIMD operations to improve processing
/// performance. The matrix uses CRTP to specilize the specific types of
/// matrices that are available. The two main matrix types allow for either 
/// fixed dimensions for the matrix, or no fixed dimensions for the matrix, to
/// be specified.
///
/// When the dimension sizes are compile-time constants (fixed) then the matrix 
/// is allocated on the stack, and many of the operations have better 
/// performance because the additional knowledge gained from knowing the size 
/// of the matrix at compile-time can be used. When the dimension sizes are 
/// specified at run-time then the matrix data is allocated on the heap.
///
/// The alignment requirement of the internal vectorized data types means that 
/// extra data might be used if the number of matrix elements in each row is 
/// not a multiple of the alignment requirement for the internal vectorized 
/// data types. Padding is added at the end of each row to ensure than the 
/// matrix step size is a multiple of the alignment requirement. The nature of 
/// the padding data depends on how the matrix was created.
///
/// \tparam SpecificType The specific type of matrix.
template <typename SpecificType>
class Matrix {
 public:
  /// The data type of each vectorized element in the matrix.
  using VecDataType  = typename SpecificType::VecDataType;

  /// Defines the format of the internal data.
  static constexpr uint8_t formatType = SpecificType::formatType;
  /// Defines the type of the matrix.
  static constexpr uint8_t type = SpecificType::type;

  /// SpecificType getter: Gets a pointer to the specific type of matrix which
  /// provides the implementation.
  /// \return A pointer to the specific type of matrix.
  SpecificType* specificType() { 
    return static_cast<SpecificType*>(this); 
  }

  /// SpecificType getter: Gets a constant pointer to the specific type of 
  /// matrix which provides the implementation.
  /// \return A constant pointer to the specific type of matrix.
  const SpecificType* specificType() const { 
    return static_cast<const SpecificType*>(this); 
  }
};

} // namespace snap

#endif // SNAP_MATRIX_MATRIX_BASE_HPP

