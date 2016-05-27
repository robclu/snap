//---- snap/svec/svec_general.hpp -------------------------- -*- C++ -*- ----//
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
/// \file  svec_general.hpp
/// \brief Defiition of the general svec class which can be specialized for the 
///        different platforms and width's which are supported.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SVEC_SVEC_GENERAL_HPP
#define SNAP_SVEC_SVEC_GENERAL_HPP

namespace snap {

/// Defines a general vector class which can be overloaded for specific
/// platforms, data types, and widths.                                       \n
///
/// The following data types are supported:                                  \n
///   -) uint8_t                                                             \n 
///   -) int8_t                                                              \n 
///   -) uint16_t                                                            \n
///   -) int16_t                                                             \n
///   -) uint32_t
///   -) int32_t 
///   -) uint64_t 
///   -) int64_t
///   -) float
///   -) double
///
/// The following widths are supported:
///   -) 2
///   -) 4
///   -) 8
///   -) 16
///   -) 32
///   -) 64
///
/// \note The width must be a power of 2. A vector may contain a non power of 2
///       number of elements but the width should be the next highest power of
///       2 number. For example, for an 11 element vector, specify the width to
///       be 16.
/// \tparam DType The internal data type being used. 
/// \tparam Width The number of Type elements in the vector.
template <typename Type, uint8_t Width>
class svec;

} // namespace snap

#endif // SNAP_SVEC_SVEC_GENERAL_HPP

