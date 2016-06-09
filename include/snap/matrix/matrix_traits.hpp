//---- snap/matrix/matrix_traits.hpp ----------------------- -*- C++ -*- ----//
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
/// \file  matrix_traits.hpp
/// \brief Defines traits for the matrix classes.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_MATRIX_TRAITS_HPP
#define SNAP_MATRIX_MATRIX_TRAITS_HPP

namespace snap {
namespace mat {

/// Defines the types of matrices.
enum MatrixType : uint8_t {
  FIXED   = 0, 
  DYNAMIC = 1
};

} // namespace mat
} // namespace snap

#endif // SNAP_MATRIX_MATRIX_TRAITS_HPP


