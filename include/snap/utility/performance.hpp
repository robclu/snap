//---- snap/utility/performance.hpp ------------------------ -*- C++ -*- ----//
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
/// \file  performance.hpp
/// \brief Defines utility functions related to improving performance.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_UTILITY_PERFORMANCE_HPP  
#define SNAP_UTILITY_PERFORMANCE_HPP  

#include "traits.hpp"

namespace snap {

/// Defines a type which can be used in unrolled lambda functions to access the
/// value of the unrolling index. This is designed specificallt for the unroll
/// function defined below.
class unroll_index {
 private:
  uint8_t value;  //!< The value of the unrolling index.

 public:
  /// Constructor: Sets the value of the unrolling index to 0.
  constexpr unroll_index() : value(0) {}

  /// Constructor: Allows the value of the unrolling index to be set.
  /// \param[in] val The value to set the unrolling index to.
  constexpr unroll_index(uint8_t val) : value(val) {}

  /// Operator uint8_t: Allows the unrolling constant to be used as an 8-bit
  /// unsigneg integer.
  constexpr operator uint8_t() const { return value; }
};

namespace util   {
namespace perf   {
namespace detail {

/// Defines a struct to detect which version of the loop unrolling functions is
/// valid.
/// \tparam Type  The type to check for an unroll const.
/// \tparam Start The starting value of the span.
/// \tparam End   The end value of the span.
template <typename T, uint8_t Start, uint8_t End>
struct unroll_enable {
  /// Returns truwe if T is an unroll_index type and if the span given by 
  /// Start -> End is valid.
  static constexpr bool valid_span_with_unroll_idx() {
    return std::is_same<T, unroll_index>::value && 
           traits::check_unroll_span<Start, End>::is_valid();
  }

  /// Returns true if T is an unroll_index type and if the span given by Start 
  /// -> End is the end of the span.
  static constexpr bool end_span_with_unroll_idx() {
    return std::is_same<T, unroll_index>::value && 
           traits::check_unroll_span<Start, End>::is_end();
  }

  /// Returns true if T is not an unroll_index and if the span given by Start 
  /// -> End is valid.
  static constexpr bool valid_span_wo_unroll_idx() {
    return !std::is_same<T, unroll_index>::value && 
            traits::check_unroll_span<Start, End>::is_valid();
  }

  /// Returns true if T is not an unroll_index and if the span given by Start 
  /// -> End is the end of the span.
  static inline constexpr bool end_span_wo_unroll_idx() {
    return !std::is_same<T, unroll_index>::value && 
            traits::check_unroll_span<Start, End>::is_end();
  }
};

} // namespace detail

/// Defines a function that can unroll a lambda function to improve performance 
/// in critical sections of code. The function is valid for a span (the range
/// of Start -> End template paramters) where the start of the span is less
/// than or equal to the end of the span.
///
/// The function will pass the constant value (unroll index) as the first 
/// argument in the lambda, if the lambda has a parameter (const
/// snap::unroll_index) or (snap::unroll_index), which is how the unrolling
/// index can be accessed. For example, the following:
///
///   snap::util::perf::unroll<0, 3>( [&some_array] (snap::unroll_index u) {
///     some_array[u] = u * u;
///   });
///
/// is equivalent to:
///
///   some_array[0] = 0 * 0;
///   some_array[1] = 1 * 1;
///   some_array[2] = 2 * 2;
///   some_array[3] = 3 * 3;
///
/// The values of any additional parameters for the lambda must be provided
/// after the lambda. For example, the following (doesn't currently work):
///
/// snap::util::perf::unroll<0, 3>( 
///   [&some_array] (snap::unroll_index u, int init_offset) {
///     some_array[init_offset + u] = init_offset + u;
///   }, 
///   1000  // Value of init_offset
/// );
///
/// is equivalent to:
///
///   some_array[1000 + 0] = 1000 + 0;
///   some_array[1000 + 1] = 1000 + 1;
///   some_array[1000 + 2] = 1000 + 2;
///   some_array[1000 + 3] = 1000 + 3;
///
/// However, the same functionality can be achieved by declaring the variable
/// outside the lambda and capturing it:
///
/// const int init_offset = 1000;
///
/// snap::util::perf::unroll<0, 3>( 
///   [&some_array, init_offset] (snap::unroll_index u) {
///     some_array[init_offset + u] = init_offset + u;
///   }
/// );
///
/// is equivalent to:
///
///   some_array[1000 + 0] = 1000 + 0;
///   some_array[1000 + 1] = 1000 + 1;
///   some_array[1000 + 2] = 1000 + 2;
///   some_array[1000 + 3] = 1000 + 3;
/// 
///
/// Lastly, if unroll index will not be used then simply omit the (const
/// snap::unroll_index) or (snap::unroll_index) parameter for the lambda, and
/// specify other arguments after the lambda as in the example above.
///
/// \tparam Start  The start valud of the unroll_index.
/// \tparam End    The end valud of the unroll index (inclusive).
/// \tparam Lambda The type of the lambda function.
/// \tparam Args   The type of the arguments for the lambda functions.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, Args&&... args, typename std::enable_if_t<
    detail::unroll_enable<
      typename traits::function_traits<decltype(l)>::template arg<0>::type,  
      Start, End
    >::end_span_with_unroll_idx(), void*> = nullptr) {
  l(unroll_index(Start), std::forward<Args&&>(args)...);
}

// This case is enabled when the span is valid and the first argument of the
// lambda function is of type unroll_index.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, Args&&... args, typename std::enable_if_t<
    detail::unroll_enable<
      typename traits::function_traits<decltype(l)>::template arg<0>::type,  
      Start, End
    >::valid_span_with_unroll_idx(), void*> = nullptr) {
  l(unroll_index(Start), std::forward<Args>(args)...);
  unroll<Start + 1, End>(l, std::forward<Args>(args)...);
}

// This case is enabled when the span is the end of the span and the first
// argument of the lambda is not of unroll_index type.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, Args&&... args, typename std::enable_if_t<
    detail::unroll_enable<
      typename traits::function_traits<decltype(l)>::template arg<0>::type,  
      Start, End
    >::end_span_wo_unroll_idx(), void*> = nullptr) {
  l(std::forward<Args&&>(args)...);
}

// This case is enabled when the span is a valid span and the first argument of
// the lamda is not of unroll_index type.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, Args&&... args, typename std::enable_if_t<
    detail::unroll_enable<
      typename traits::function_traits<decltype(l)>::template arg<0>::type,  
      Start, End
    >::valid_span_wo_unroll_idx(), void*> = nullptr) {
  l(std::forward<Args&&>(args)...);
  unroll<Start + 1, End> (l, std::forward<Args&&>(args)...);
}

} // namespace perf
} // namespace util
} // namespace snap

#endif // SNAP_UTIL_PERFORMANCE_HPP
