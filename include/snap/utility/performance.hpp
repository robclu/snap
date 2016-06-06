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

/// Defines the types of unroll functions. The following bits are used to
/// determine the type of the unroll:
///
///         -----------------------------------------
/// Bit     |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
///         -----------------------------------------
/// Field   | NA | NA | NA | NA | NA | AR | ST | UI |
///         -----------------------------------------
///
/// Where
///         ST = Span Type    : The type of span, options are:
///            - 1  : Valis span (Start < End).
///            - 0  : End span (Start = End).
///
///         UI = Unroll Index : If an unroll index is being used, options are:
///            - 1  : Unroll index is being used.
///            - 0  : No unroll index is being used.
enum unroll_type : uint8_t {
  END_SPAN_WO_UROLL = 0,
  END_SPAN_WI_UROLL = 1,
  VAL_SPAN_WO_UROLL = 2, 
  VAL_SPAN_WI_UROLL = 3
};

namespace detail {

/// Defines a struct to detect which version of the loop unrolling functions is
/// valid.
/// \tparam F     The function type to check for an unroll_index const.
/// \tparam Start The starting value of the span.
/// \tparam End   The end value of the span.
template <typename F, uint8_t Start, uint8_t End>
struct unroll_enable {
  /// Defines the type of the function.
  using arg_type = typename traits::function_traits<F>::template arg<0>::type;

  /// Defines the arity (number of arguments) of the function.
  static constexpr uint8_t arity = traits::function_traits<F>::arity;

  /// Returns the unroll_type of the function by bit shifting the properties of
  /// the function to the correct positions.
  static constexpr uint8_t value = 
    (traits::check_unroll_span<Start, End>::is_valid() << 1) |  // Bit 1
    (std::is_same<arg_type, unroll_index>::value);              // Bit 0
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
/// after the lambda. For example, the following:
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
 
// This case is enabled when the unrolling uses an unroll_index constant but
// when there are no additional arguments for the lambda function. This
// terminates the unrolling.
template <uint8_t Start, uint8_t End, typename Lambda>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity == 1) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == END_SPAN_WI_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<1>::type> 
    arg = traits::function_traits<decltype(l)>::template arg<1>::default_arg()
  ) {
  // Call the lambda and give pass it the unroll_index value.
  l(unroll_index(Start));
}

template <uint8_t Start, uint8_t End, typename Lambda>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity == 1) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == VAL_SPAN_WI_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<1>::type> 
    arg = traits::function_traits<decltype(l)>::template arg<1>::default_arg()
  ) {
  // Call the lambda and pass it the unroll_index_value.
  l(unroll_index(Start));

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End>(l);
}

// This case is enabled when the span is an end span, the first argument of the
// lambda function is of type unroll_index, and the lambda has additional
// arguments.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity > 1) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == END_SPAN_WI_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<1>::type>
    arg = traits::function_traits<decltype(l)>::template arg<1>::default_arg(), 
    Args&&... args) {
  // Define the type of the first argument.
  using arg_type = 
    typename traits::function_traits<decltype(l)>::template arg<1>::type;

  // Call the lambda and pass it the unroll const as well as the args.
  l(unroll_index(Start)         , 
    std::forward<arg_type>(arg) , 
    std::forward<Args>(args)...
  );
}

// This case is enabled when the span is valid, the first argument of the
// lambda function is of type unroll_index, and the lambda function has
// additional arguments.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity > 1) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == VAL_SPAN_WI_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<1>::type>
    arg = traits::function_traits<decltype(l)>::template arg<1>::default_arg(), 
    Args&&... args) {
  // Define the type of the first argument.
  using arg_type = 
    typename traits::function_traits<decltype(l)>::template arg<1>::type;

  // Call the lambda and pass it the unroll const as well as the args.
  l(unroll_index(Start)         , 
    std::forward<arg_type>(arg) , 
    std::forward<Args>(args)...
  );

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End>(l, std::forward<arg_type&&>(arg), 
    std::forward<Args&&>(args)...);
}

// This case is enabled when the span is the end of the span and the first
// argument of the lambda is not an unroll_idx, and the lambda does not take
// any arguments.
template <uint8_t Start, uint8_t End, typename Lambda>
static inline constexpr 
void unroll(Lambda l,
    typename std::enable_if_t<
    (detail::unroll_enable<decltype(l), Start, End>::arity == 0) &&
    (detail::unroll_enable<decltype(l), Start, End>::value 
     == END_SPAN_WO_UROLL), 
    typename traits::function_traits<decltype(l)>::template arg<0>::type> 
    arg = traits::function_traits<decltype(l)>::template arg<0>::default_arg()
  ) { 
  // Call the lambda function with no arguments.
  l();
}

// This case is enabled when the span is a valid span and the first argument of
// the lamda is not an unroll_index type, and the lambda has no arguments.
template <uint8_t Start, uint8_t End, typename Lambda>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity == 0) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == VAL_SPAN_WO_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<0>::type>
    arg = traits::function_traits<decltype(l)>::template arg<0>::default_arg()
  ) {
  // Call the lambda with no arguments.
  l();

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End> (l);
}

// This case is enabled when the span is the end of the span and the first
// argument of the lambda is not an unroll_idx, and the lambda takes additional
// arguments.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l,
    typename std::enable_if_t<
    (detail::unroll_enable<decltype(l), Start, End>::arity >= 1) &&
    (detail::unroll_enable<decltype(l), Start, End>::value 
     == END_SPAN_WO_UROLL), 
    typename traits::function_traits<decltype(l)>::template arg<0>::type> 
    arg = traits::function_traits<decltype(l)>::template arg<0>::default_arg(),
    Args&&... args) {
  // Define the type of the first argument.
  using arg_type = 
    typename traits::function_traits<decltype(l)>::template arg<0>::type;

  // Call the lambda and pass the arguments.
  l(std::forward<arg_type>(arg));
}

// This case is enabled when the span is a valid span and the first argument of
// the lamda is not an unroll_index type, and the lambda has no arguments.
template <uint8_t Start, uint8_t End, typename Lambda, typename... Args>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity >= 1) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == VAL_SPAN_WO_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<0>::type>
    arg = traits::function_traits<decltype(l)>::template arg<0>::default_arg(),
    Args&&... args) {
  // Define the type of the first argument.
  using arg_type = 
    typename traits::function_traits<decltype(l)>::template arg<0>::type;

  // Call the lambda and pass it the unroll const as well as the args.
  l(std::forward<arg_type>(arg), std::forward<Args>(args)...);

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End>(l, std::forward<arg_type&&>(arg), 
    std::forward<Args&&>(args)...);
}

} // namespace perf
} // namespace util
} // namespace snap

#endif // SNAP_UTIL_PERFORMANCE_HPP
