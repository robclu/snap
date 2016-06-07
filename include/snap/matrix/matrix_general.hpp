//---- snap/matrix/matrix_general.hpp ---------------------- -*- C++ -*- ----//
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
/// \file  matrix_general.hpp
/// \brief Defiition of the general Matrix class which can be specialized for 
///        the different platforms and matrix formats.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_MATRIX_MATRIX_GENERAL_HPP
#define SNAP_MATRIX_MATRIX_GENERAL_HPP

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

} // namespace snap

#endif // SNAP_MATRIX_MATRIX_GENERAL_HPP

