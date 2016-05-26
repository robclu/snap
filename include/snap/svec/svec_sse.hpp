//---- snap/svec/svec_sse.hpp ------------------------------ -*- C++ -*- ----//
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
/// \file  svec_sse.hpp
/// \brief Defiition of svec class SSE implementation. The possible options 
///        for snap vector types when using SSE instructions are the following:
///
///        -) vec<uint8|sint8, 16> : Vec of 16 8 bit ints. 
///           Alias: 
///             : svec16x8u     : Vec of 16 8 bit unsigned integers.
///             : svec16x8s     : Vec of 16 8 bit signed integers.
///
///        -) vec<uint16|sint16, 8> : Vec of 8 16 bit integers.
///           Aliases: 
///             : svec8x16u     : Vec of 8 16 bit unsigned integers.
///             : svec8x16s     : Vec of 8 16 bit signed intefers.
///
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SVEC_SVEC_SSE_HPP
#define SNAP_SVEC_SVEC_SSE_HPP

#include "svec_general.hpp"
#include "snap/config/simd_instruction_detect.h"
#include "snap/types/types_sse.h"

namespace snap {

/// Implementation of svec class for integer cases and SSE instructions where
/// the width of the vector is 16 elements and the data type can be either an
/// 8-bit signed or unsigned integer.
/// \tparam DType The type of the data elements.
/// \tparam Width The number of elements in the vector.
template <typename DType>
SNAP_ALIGN class svec<DType, 16> {
 public:
  using data_type = __mm128i;           //!< Alias for the vector data type.
  using svec_type = svec<Dtype, Width>; //!< Alias for the type of vector.

  /// Width of the vector.
  static constexpr uint8_t width = 16;
 protected:
  data_type data;                       //!< Data for the vector.

 public:
  // ---- Constructors ----------------------------------------------------- //

  /// Default constructor: does nothing.
  svec() {}

  /// Constructor: Create vector from iternal intrinsic type.
  /// \param[in] x The intrinsic variable to use to initialize the internal 
  ///              vector.
  svec(const data_type& x);

  /// Constructor to broadcast a single 8 bit unsigned int. 
  /// \param[in] x The 8 bit unsigned int to broadcast.
  svec(uint8 x);

  /// Constructor to broadcast a single 8 bit signed int.
  /// \param[in] x The 8 bit signed int value to broadcast.
  svec(int8 x); 

  // ---- Operators -------------------------------------------------------- //
  
  /// Cast operator: Allow conversion to the intrinsic type.
  /// \return The internal intrinsic vector.
  operator __m128i() const;
  
  /// Assignment operator: Allows the conversion from intrinsic types.
  /// \param[in] x The intrinsic variable to use ti set the internal vector.
  /// \return      A reference to the vector.
  svec_type& operator=(const data_type& x);

  /// Access operator: Allows a specific unsigned element of the vector to be 
  /// fetched. This does not check bounds due to performance implications.
  /// \param[in] idx The index of the element to fetch.
  uint8 operator[](uint8 idx) const;

  /// Access operator: Allows a specific signed element of the vector to be 
  /// fetched. This does not check bounds due to performance reasons.
  /// \param[in] idx The index of the element to fetch.
  sint8 operator[](uint8 idx) const;
};

// ---- Implementation ----------------------------------------------------- //

template <typename DT> SNAP_INLINE
svec<DT, 16>::svec(const svec<DT, 16>::data_type& x) {
  data = x;
}

template <typename DT> SNAP_INLINE 
svec<DT, 16>::svec(int8 x) {
  data = _mm_set1_epi8(x);
}

template <typename DT> SNAP_INLINE
svec<DT, 16>::svec(uint8 x) {
  data = _mm_set1_epi8(x);
}

template <typename DT> SNAP_INLINE
operator svec<DT, 16>::__m128i() const {
  return data;
}

template <typename DT> SNAP_INLINE 
svec<DT, 16>& svec<DT, 16>::operator=(const svec<DT, 16>::data_type& x) {
  data = x;
  return *this;
}

template <typename DT> SNAP_INLINE
uint8 svec<DT, 16>::operator[](uint8 idx) const {
  return (data >> (idx << 0x03)) & 0xFF;
}

template <typename DT> SNAP_INLINE 
int8 svec<DT, 16>::operator[](uint8 idx) const {
  return (data >> (idx << 0x03)) & 0xFF;
}

} // namespace snap

#endif // SNAP_SVECTOR_SVECTOR_SSE_HPP

