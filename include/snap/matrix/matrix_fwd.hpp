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

#include "format.hpp"
#include "snap/allocate/allocator.hpp"

namespace snap {

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
class Matrix;

} // namespace snap

#endif // SNAP_MATRIX_FWD_HPP

