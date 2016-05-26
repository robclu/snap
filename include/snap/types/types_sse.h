//---- snap/types/types_sse.h ------------------------------ -*- C++ -*- ----//
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
/// \file  types_sse.h
/// \brief Defiition of types supported when using SSE.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_TYPES_TYPES_SSE_H
#define SNAP_TYPES_TYPES_SSE_H

namespace snap {

using uint8   = uint8_t ;   //!< Unsigned 8 bit integer.
using sint8   = int8_t  ;   //!< Signed 8 bit integer.
using uint16  = uint16_t;   //!< Unsigned 16 bit integer.
using sint16  = int16_t ;   //!< Signed 16 bit integer.
using uint32  = uint32_t;   //!< Unsigned 32 bit integer.
using sint32  = int32_t ;   //!< Signed 32 bit integer.
using uint64  = uint64_t;   //!< Unsigned 64 bit integer.
using sint64  = int64_t ;   //!< Signed 64 bit integer.
using sfloat  = float   ;   //!< Single precision floating point number.
using sdouble = double  ;   //!< Double precision floating point number.

} // namespace snap

#endif // SNAP_TYPES_TYPES_SSE_H
