//---- snap/vector/vector_sse.hpp -------------------------- -*- C++ -*- ----//
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
/// \file  vector_sse.hpp
/// \brief Defiition of Vector class SSE implementation. The possible options 
///        for snap vector types when using SSE instructions are the following:
///
///        -) Vec<uint8_t|int8_t, 16> : Vec of 16 8 bit ints. 
///
///\note   The aliases are defined in vector_see.hpp.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_VECTOR_VECTOR_SSE_HPP
#define SNAP_VECTOR_VECTOR_SSE_HPP

#include "vector_general.hpp"
#include "snap/config/simd_instruction_detect.h"

namespace snap {

/// Implementation of Vector class for integer cases and SSE instructions where
/// the width of the vector is 16 elements and the data type can be either an
/// 8-bit signed or unsigned integer.
/// \tparam DType The type of the data elements.
template <typename DType>
class Vector<DType, 16> {
 public:
  using VecDType = __m128i;               //!< Alias for the vector data type.
  using VecType  = Vector<DType, 16>;     //!< Alias for the type of vector.

  static constexpr uint8_t width = 16;    //!< Width of the vector.

  // ---- Constructors ----------------------------------------------------- //

  /// Default constructor: does nothing.
  Vector() {}

  /// Constructor: Create vector from iternal intrinsic type.
  /// \param[in] x The intrinsic variable to use to initialize the internal 
  ///              vector.
  Vector(const VecDType& x);

  /// Constructor: Broadcasts a single 8 bit int of type DType into the vector. 
  /// \param[in] x The 8 bit int to broadcast.
  Vector(DType x);

  /// Constructor: Sets a pointer to an array of elements as vector elements.
  /// \param[in] p A pointer to the start of the elements to load into to
  ///              vector.
  Vector(DType* p);

  // ---- Operators -------------------------------------------------------- //
  
  /// Cast operator: Allow conversion to the intrinsic type.
  /// \return The internal intrinsic vector.
  operator __m128i() const;
  
  /// Assignment operator: Allows the conversion from intrinsic types.
  /// \param[in] x The intrinsic variable to use ti set the internal vector.
  /// \return      A reference to the vector.
  VecType& operator=(const VecDType& x);

  /// Access operator: Allows a specific element of the vector to be fetched.
  /// This does not check bounds due to performance implications.
  /// \param[in] idx The index of the element to fetch.
  DType operator[](uint8_t idx) const;

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
  void set(uint8_t idx, DType val);

 private:
  VecDType Data;                            //!< Data for the vector.

} SNAP_ALIGNED;

// ---- Implementation ----------------------------------------------------- //

template <typename DT> SNAP_INLINE
Vector<DT, 16>::Vector(const VecDType& x) {
  Data = x;
}

template <typename DT> SNAP_INLINE 
Vector<DT, 16>::Vector(DT x) {
  Data = _mm_set1_epi8(x);
}

template <typename DT> SNAP_INLINE 
Vector<DT, 16>::Vector(DT* p) {
  load(p);
}

template <typename DT> SNAP_INLINE
Vector<DT, 16>::operator __m128i() const {
  return Data;
}

template <typename DT> SNAP_INLINE 
Vector<DT, 16>& Vector<DT, 16>::operator=(const VecDType& x) {
  Data = x;
  return *this;
}

template <typename DT> SNAP_INLINE
DT Vector<DT, 16>::operator[](uint8_t idx) const {
  SNAP_ALIGN(16) DT dataArray[16];
  store(dataArray);
  return dataArray[idx];
}

template <typename DT> SNAP_INLINE
void Vector<DT, 16>::load(void* p) {
  Data = _mm_loadu_si128(reinterpret_cast<VecDType const*>(p));
}

template <typename DT> SNAP_INLINE 
void Vector<DT, 16>::loada(void* p) {
  Data = _mm_load_si128(reinterpret_cast<VecDType const*>(p));
}

template <typename DT> SNAP_INLINE 
void Vector<DT, 16>::store(void* p) const {
  _mm_store_si128(reinterpret_cast<VecDType*>(p), Data);
}

template <typename DT> SNAP_INLINE 
void Vector<DT, 16>::storeu(void* p) const {
  _mm_storeu_si128(reinterpret_cast<VecDType*>(p), Data);
}

template <typename DT> SNAP_INLINE 
void Vector<DT, 16>::set(uint8_t idx, DT val) {
  SNAP_ALIGN(16) DT tmp[16];
  _mm_store_si128(reinterpret_cast<VecDType*>(tmp), Data);
  tmp[idx] = val;
  Data = _mm_load_si128(reinterpret_cast<VecDType const*>(tmp));
}

} // namespace snap

#endif // SNAP_VECTOR_VECTOR_SSE_HPP

