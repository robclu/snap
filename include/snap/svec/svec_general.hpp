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

#include <limits>

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

/// Provides masks to use to extract elements from AoS to SoA based on the
/// datatype and the width.
namespace mask {

// Provides utility functions for creating the mask.
namespace detail {
  /// Creates a mask for every Step Elements starting at Offset.
  /// \param  step   The number of elements to miss before getting another.
  /// \param  offset The start offset of the first element.
  /// \tparam MT The type to use for the mask.
  /// \tparam ET The type of each element.
  template <typename MT, typename ET>
  static constexpr MaskType make_mask(uint8_t step, uint8_t offset) {
    ET             select   = std::numeric_limits<ElementType>::max();
    MT             mask     = select << (sizeof(MT) - (offset * sizeof(ET)));
    constexpr auto elements = sizeof(MT) / sizeof(ET);

    offset += step;
    while (offset < elements) {
      mask   ^= (mask >> step * sizeof(ET));
      offset += step;
    }
  }
} // namespace detail

/// Defines a meta class to get an extraction mask which can be used to convert
/// AoS to SoA data for faster processing. This can generate an array of
/// MaskType elements, each of which is a mask to get every Step elements,
/// where each element is of ElementType, and the element at index i in the
/// array mask gets it's first element starting at element i.              \n\n
///
/// Using the masks contiguously (as a single mask) will get eveery Step
/// elements for sizeof(MaskType) / sizeof(ElementType) * Step elements.   \n\n
///
/// For example, using:
///   
///   MaskType    = uint64_t
///   ElementType = uint8_t / int8_t
///   Step        = 3 (get every 3rd element)
///
/// Will generate the following array of masks:
///
///   m[0] = 0xff0000ff0000ff00
///   m[1] = 0x00ff0000ff0000ff
///   m[2] = 0x0000ff0000ff0000
template <typename MaskType, uint8_t ElementSize, uint8_t Step>
struct extractor_mask {
  static constexpr MaskType masks[Step] = {0};



} // namespace mask
} // namespace snap

#endif // SNAP_SVEC_SVEC_GENERAL_HPP

