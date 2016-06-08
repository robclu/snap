//---- snap/matrix/format.hpp ------------------------------ -*- C++ -*- ----//
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
/// \file  format.hpp
/// \brief Definition of the various matrix formats and the format related
///        traits functions.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_FORMAT_HPP
#define SNAP_MATRIX_FORMAT_HPP

#include "snap/vector/vector.hpp"

namespace snap {
namespace mat  {

/// Defines the possible formats for SIMD matrix types.
enum Format : uint8_t {
  FM_GREY_8  = 0,
  FM_BGR_24  = 1, 
  FM_BGRA_32 = 2
};

} // namespace mat

/// Defines a metaclass to get traits for a specific format.
/// \tparam Format The format to get the traits of.
template <uint8_t Format>
struct format_traits;

// Specialization for when the format is greyscale.
template <>
struct format_traits<mat::FM_GREY_8> {
  /// Defines the data type used for 8-bit greyscale values.
  using type = Vec16x8u;
};

} // namespace snap

#endif // SNAP_MATRIX_FORMAT_HPP

