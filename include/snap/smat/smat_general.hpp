//---- snap/smat/smat_general.hpp -------------------------- -*- C++ -*- ----//
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
/// \file  smat_general.hpp
/// \brief Defiition of the general smat class which can be specialized for the 
///        different platforms and matrix formats.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SMAT_SMAT_GENERAL_HPP
#define SNAP_SMAT_SMAT_GENERAL_HPP

namespace snap {
namespace mat  {

/// Defines the possible formats for SIMD matrix types.
enum format : uint8_t {
  FMT_GREY_8  = 0,
  FMT_BGR_24  = 1, 
  FMT_BGRA_32 = 2
};

} // namespace mat

/// Defines a metaclass to get a typename for a specific format.
/// \tparam Format The format to get the relevant typename of.
template <uint8_t Format>
struct get_format_type;

} // namespace snap

#endif // SNAP_SMAT_SMAT_GENERAL_HPP

