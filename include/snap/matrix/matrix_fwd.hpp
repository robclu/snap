//---- snap/matrix/matrix_fsd.hpp -------------------------- -*- C++ -*- ----//
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
/// \file  matrix_fwd.hpp
/// \brief Forward declaration of marix classes.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_MATRIX_FWD_HPP
#define SNAP_MATRIX_MATRIX_FWD_HPP


namespace snap {

/// Defines a matrix class with a fixed size.
/// \tparam Format The format of the matrix which defines the type of the
///         elements.
/// \tparam Rows   The number of rows in the matrix.
/// \tparam Cols   The number of columns in the matrix.
template <uint8_t Format, size_t Rows, size_t Cols>
class MatrixFixed;

/// Defines a matrix class with a dynamic size.
/// \tparam Format    The format of the matrix which defines the type of the
///         elements.
/// \tparam Allocator The type of allocator used to allocate the data.
template <uint8_t Format, typename Allocator>
class MatrixDynamic;

} // namespace snap

#endif // SNAP_MATRIX_FWD_HPP

