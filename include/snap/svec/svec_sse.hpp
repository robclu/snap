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

namespace snap {

/// Implementation of svec class for integer cases and SSE instructions where
/// the width of the vector is 16 elements and the data type can be either an
/// 8-bit signed or unsigned integer.
/// \tparam DType The type of the data elements.
/// \tparam Width The number of elements in the vector.
template <typename DType>
class svec<DType, 16> {
 public:
  using intern_dtype = DType;              //!< Alias for internal data type.
  using data_type    = __m128i;            //!< Alias for the vector data type.
  using svec_type    = svec<DType, 16>;    //!< Alias for the type of vector.

  /// Width of the vector.
  static constexpr uint8_t width = 16;
 private:
  data_type data;                       //!< Data for the vector.

 public:
  // ---- Constructors ----------------------------------------------------- //

  /// Default constructor: does nothing.
  svec() {}

  /// Constructor: Create vector from iternal intrinsic type.
  /// \param[in] x The intrinsic variable to use to initialize the internal 
  ///              vector.
  svec(const data_type& x);

  /// Constructor: Broadcasts a single 8 bit int of type DType into the vector. 
  /// \param[in] x The 8 bit int to broadcast.
  svec(intern_dtype x);

  /// Constructor: Sets a pointer to an array of elements as vector elements.
  /// \param[in] p A pointer to the start of the elements to load into to
  ///              vector.
  svec(intern_dtype* p);

  // ---- Operators -------------------------------------------------------- //
  
  /// Cast operator: Allow conversion to the intrinsic type.
  /// \return The internal intrinsic vector.
  operator __m128i() const;
  
  /// Assignment operator: Allows the conversion from intrinsic types.
  /// \param[in] x The intrinsic variable to use ti set the internal vector.
  /// \return      A reference to the vector.
  svec_type& operator=(const data_type& x);

  /// Access operator: Allows a specific element of the vector to be fetched.
  /// This does not check bounds due to performance implications.
  /// \param[in] idx The index of the element to fetch.
  intern_dtype operator[](uint8_t idx) const;

  // ---- General Operations ----------------------------------------------- //
 
  /// Load operation: Allows a pointer to contiguous, aligned or unaligned 
  /// memory to be loaded as a vector data type. This can be slower than loada
  /// in some cases.
  /// \param[in] p A pointer to the start of the contiguous aligned/unaligned 
  ///              memory to load as a vector of elements.
  void load(void* p);

  /// Load operation: Allows a pointer to contiguous, aligned memory to be
  /// loaded as a vector data type. This can be faster than load, but must only 
  /// be used when the memory is definitely 16-byte aligned.
  /// \param[in] p A pointer to the start of the contiguous aligned memory to 
  ///              load as a vector of elements.
  void loada(void* p);

  /// Store operation: Allows the vector to be stored in contiguous memory. The 
  /// memory needs to be aligned on a 16 byte boundary.
  /// \param[in] p A pointer to the start of the contiguous aligned memory.
  void store(void* p) const;

  /// Store operation: Allows the vector to be stored in contiguous memory. The
  /// memory does not need to be aligned on a 16 byte boundary.
  /// \param[in] p A pointer to the start of the contiguous aligned or
  ///              unaligned memory.
  void storeu(void* p) const;

  /// Set operation: Sets a specific element of the vector to the specified
  /// value.
  /// \param[in] idx The index of the element to set the value of.
  /// \param[in] val The value to set the element to.
  void set(uint8_t idx, intern_dtype val);

} SNAP_ALIGNED;

// ---- Implementation ----------------------------------------------------- //

template <typename DT> SNAP_INLINE
svec<DT, 16>::svec(const data_type& x) {
  data = x;
}

template <typename DT> SNAP_INLINE 
svec<DT, 16>::svec(DT x) {
  data = _mm_set1_epi8(x);
}

template <typename DT> SNAP_INLINE 
svec<DT, 16>::svec(DT* p) {
  load(p);
}

template <typename DT> SNAP_INLINE
svec<DT, 16>::operator __m128i() const {
  return data;
}

template <typename DT> SNAP_INLINE 
svec<DT, 16>& svec<DT, 16>::operator=(const data_type& x) {
  data = x;
  return *this;
}

template <typename DT> SNAP_INLINE
DT svec<DT, 16>::operator[](uint8_t idx) const {
  SNAP_ALIGN(16) DT data_array[16];
  store(data_array);
  return data_array[idx];
}

template <typename DT> SNAP_INLINE
void svec<DT, 16>::load(void* p) {
  data = _mm_loadu_si128(reinterpret_cast<data_type const*>(p));
}

template <typename DT> SNAP_INLINE 
void svec<DT, 16>::loada(void* p) {
  data = _mm_load_si128(reinterpret_cast<data_type const*>(p));
}

template <typename DT> SNAP_INLINE 
void svec<DT, 16>::store(void* p) const {
  _mm_store_si128(reinterpret_cast<data_type*>(p), data);
}

template <typename DT> SNAP_INLINE 
void svec<DT, 16>::storeu(void* p) const {
  _mm_storeu_si128(reinterpret_cast<data_type*>(p), data);
}

template <typename DT> SNAP_INLINE 
void svec<DT, 16>::set(uint8_t idx, DT val) {
  SNAP_ALIGN(16) DT tmp[16];
  _mm_store_si128(reinterpret_cast<data_type*>(tmp), data);
  tmp[idx] = val;
  data = _mm_load_si128(reinterpret_cast<data_type const*>(tmp));
}

} // namespace snap

#endif // SNAP_SVECTOR_SVECTOR_SSE_HPP

