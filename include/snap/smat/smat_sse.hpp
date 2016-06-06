//---- snap/smat/smat_sse.hpp ------------------------------ -*- C++ -*- ----//
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
/// \file  smat_sse.hpp
/// \brief Defiition of smat class SSE implementation. The possible options 
///        for snap matrix types when using SSE instructions are the following:
//
//---------------------------------------------------------------------------//

#ifndef SNAP_SMAT_SMAT_SSE_HPP
#define SNAP_SMAT_SMAT_SSE_HPP

#include "smat_general.hpp"
#include "snap/allocate/allocator.hpp"
#include "snap/config/simd_instruction_detect.h"
#include "snap/svec/svec.hpp"

namespace snap {

// Specialization for when the format is greyscale.
template <>
struct get_format_type<mat::FMT_GREY_8> {
  /// Defines the data type used for 8-bit greyscale values.
  using type = svec16x8u;
};

/// Defines a matrix class for which SIMD operations can be used to improve
/// processing performance. The constructor always allocates data for the
/// elements, and the data is stored at arrays of svec types to ensure
/// alignment and faster processing. The initial overhead of the memory
/// allocation is insignificant relative to the additional performance gained
/// by the vectorized and correctly aligned data.
///
/// The alignment requirement of the data means that extra data might be used
/// if the number of matrix elements is not a multiple of the alignment
/// requirement for the vectorized data types. This is typically a very small
/// overhead, as the max number of additional elements will be less than 32.
///
/// The matrix should be allocated initiallly, and the data can then be
/// overwritten using the load operations.
///
/// \tparam Format    The format of the matrix.
/// \tparam Allocator The allocator for the data.
template <
  uint8_t  Format, 
  typename Allocator = aligned_allocator<
                        typename get_format_type<Format>::type>
  >
class smat {
 public:
  /// Alias for the type of allocator to use to allocate data.
  using allocator_type = Allocator;

  /// The data type of each vectorized element in the matrix.
  using data_type = typename get_format_type<Format>::type;

  /// Constructor: Creates an empty matrix.
  smat();

  /// Constructor: Creates a matrix with a specific size.
  smat(size_t rows, size_t cols);

  /// Destructor: Cleans up matrix memory.
  ~smat();

  /// Row size operation: Gets the number of rows in the matrix.
  size_t rows() const { return _rows; }

  /// Col size operation: Gets the number of columns in the matrix.
  size_t cols() const { return _cols; }

  /// Size operation: Gets the total number of elements in the matrix.
  /// TODO: Modify to reflect actual size.
  size_t size() const { 
    return (_rows * _cols % ALIGNMENT == 0) ?
      _rows * _cols : _rows * _cols / ALIGNMENT + 1; 
  }

 private:
  data_type*  _data;    //!< Pointer to the raw matrix vectorized data. 
  size_t      _rows;    //!< Number of rows in the matrix.
  size_t      _cols;    //!< Number of columns in the matrix.
};


// ---- Implementation ----------------------------------------------------- //


template <uint8_t F, typename A>
smat<F, A>::smat() : _data(nullptr), _rows(0), _cols(0) {}

template <uint8_t F, typename A>
smat<F, A>::smat(size_t rows, size_t cols)
    : _rows(rows), _cols(cols) {
  const size_t total_elements = _rows * cols;

  _data = allocator_type::alloc(
            total_elements % ALIGNMENT == 0 
            ? total_elements : _rows * _cols / ALIGNMENT + 1,
            ALIGNMENT
          );
}

template <uint8_t F, typename A>
smat<F, A>::~smat() {
  if (_data != nullptr) 
    allocator_type::free(_data);
}

} // namespace snap

#endif // SNAP_SMAT_SMAT_SSE_HPP


