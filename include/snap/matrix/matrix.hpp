//---- snap/matrix/matrix.hpp ------------------------------ -*- C++ -*- ----//
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
/// \file  matrix.hpp
/// \brief Includes all specific implementations of the Matrix class.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_MATRIX_HPP
#define SNAP_MATRix_MATRIX_HPP

#include "matrix_base.hpp"

#ifdef SSE_ENABLED 
#include "allocator_sse.hpp"
#include "matrix_see.hpp"

namespace snap {

// ---- Fixed size matrices ------------------------------------------------ //

/// Fixed matrix: Matrix with a specific format and size.
template <uint8_t Format, size_t Rows, size_t Cols>
using Mat = Matrix<MatrixFixed<Format, Rows, Cols>>;

/// Dynamic matrix: Matrix with a specific format and size.
template <uint8_t Format, typename Allocator = 
  AlignedAllocator<format_traits<Format>::type>>
using Mat = Matrix<MatrixDynamic<Format, Allocator>>;

} // namespace snap

#endif // SSE_ENABLED

#endif // SNAP_MATRIX_MATRIX_HPP

