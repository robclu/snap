//---- snap/svector/svector_sse.hpp ----------------------- -*- C++ -*- ----//
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
/// \file  svector_sse.hpp
/// \brief Defiition of svector class SSE implementation.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SVECTOR_SVECTOR_SSE_HPP
#define SNAP_SVECTOR_SVECTOR_SSE_HPP

#include "snap/config/simd_instruction_detect.h"

namespace snap {

/// General implementation of svector for SSE types.
template <typename Type, uint8_t Width>
SNAP_ALIGN class svector {
 public:
  /// Alias for the type of data.
  using data_type = __mm128i;

 protected:
  data_type data; //!< Data for the vector.

 public:
  /// Default constructor: does nothing.
  svector() {}

  /// Constructor to convert from sse intrinsics.
  svector(const data_type& other_data);

  /// Constructor to broadcast a single value.
  /// \param[in] val The value to boradcast.
  
};

// ---- Implementation ----------------------------------------------------- //

template <typename T, uint8_t W>
svector<T, W>::svector(const svector::data_type& other_data) {
  data = other_data;
}

} // namespace snap

#endif // SNAP_SVECTOR_SVECTOR_SSE_HPP

