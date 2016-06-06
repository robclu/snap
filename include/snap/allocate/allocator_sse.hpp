//---- snap/allocate_allocator_sse.hpp --------------------- -*- C++ -*- ----//
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
/// \file  allocator_sse.hpp
/// \brief Definition of SSE specific allocator class,
//
//---------------------------------------------------------------------------//

#ifndef SNAP_ALLOCATE_ALLOCATOR_SSE_HPP
#define SNAP_ALLOCATE_ALLOCATOR_SSE_HPP

#include "snap/config/simd_instruction_detect.h"

namespace snap {

/// Defines a specialization of the allocator class to allocate aligned
/// vectorized data when the data type is 8-bit greyscale.
/// \tparam DataType The type of data to allocate in an aligned manner.
template <typename DataType>
struct aligned_allocator {
  /// Defines a function to allocate \p bytes of \p alignment aligned data.
  /// \param bytes     The number of bytes to allocate.
  /// \param alignment The alignment of the allocated data.
  static inline DataType* alloc(size_t bytes, size_t alignment) {
    return static_cast<DataType*>(_mm_malloc(bytes, alignment));
  }

  /// Frees aligned data.
  /// \param p A pointer to the data to free.
  static inline void free(DataType* p) {
    _mm_free(static_cast<void*>(p));
  }
};

} // namespace snap

#endif //  SNAP_ALLOCATE_ALLOCATOR_SSE_HPP


