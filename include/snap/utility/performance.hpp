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

/// Defines a type which can be used in unrolled lambda functions as a variable
/// . The class is designed spcifically for use with the unroll functions, but
/// can be used as an 8 bit unsigned integer.
class UnrollIndex {
 public:
  /// Constructor: Sets the value of the unrolling index to 0.
  constexpr UnrollIndex() : Value(0) {}

  /// Constructor: Allows the value of the unrolling index to be set.
  /// \param[in] value The value to set the unrolling index to.
  constexpr UnrollIndex(uint8_t value) : Value(value) {}

  /// Operator uint8_t: Allows the unrolling constant to be used as an 8-bit
  /// unsigned integer, and as a constexpr value where it is defined to be
  /// such.
  constexpr operator uint8_t() const { return Value; }

 private:
  uint8_t Value;  //!< The value of the unrolling index.
};

namespace util   {
namespace perf   {

/// Defines the types of unroll functions. The following bits are used to
/// determine the type of the unroll function:
///
///         -----------------------------------------
/// Bit     |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
///         -----------------------------------------
/// Field   | NA | NA | NA | NA | NA | AR | ST | UI |
///         -----------------------------------------
///
/// Where
///         ST = Span Type    : The type of span, options are:
///            - 1  : Valid span (Start < End).
///            - 0  : End span (Start = End).
///
///         UI = Unroll Index : If an unroll index is being used, options are:
///            - 1  : Unroll index is being used.
///            - 0  : No unroll index is being used.
enum UnrollType : uint8_t {
  END_SPAN_WO_UROLL = 0,
  END_SPAN_WI_UROLL = 1,
  VAL_SPAN_WO_UROLL = 2, 
  VAL_SPAN_WI_UROLL = 3
};

namespace detail {

/// Defines a struct to help select the correct version of the unrolling
/// functions.
/// \tparam F     The function type to check for an UnrollIndex type.
/// \tparam Start The starting value of the span.
/// \tparam End   The end value of the span.
template <typename F, uint8_t Start, uint8_t End>
struct unroll_enable {
  /// Defines the type of the function.
  using Arg0Type = typename traits::function_traits<F>::template arg<0>::type;

  /// Defines the arity (number of arguments) of the function.
  static constexpr uint8_t arity = traits::function_traits<F>::arity;

  /// Returns the UnrollType of the function by bit shifting the properties of
  /// the function to the correct positions.
  static constexpr uint8_t value = 
    (std::is_same<Arg0Type, UnrollIndex>::value)            |   // Bit 0
    (traits::check_unroll_span<Start, End>::isValid() << 1);    // Bit 1
};

} // namespace detail

/// Defines a function that can unroll a lambda function to improve performance 
/// in critical sections of code. The function is valid for a span (the range
/// of Start -> End template paramters) where the start of the span is less
/// than or equal to the end of the span. The span is non inclusive, so the
/// number of unrolls is: 
///
///   unrolls = End - Start + 1
///
/// The function will pass the span value as the first argument in the lambda, 
/// if the lambda has a first parameter of type const snap::UnrollIndex or 
/// snap::UnrollIndex, which is how the unrolling index can be accessed inside
/// the unrolled lambda. For example, the following:
///
///   snap::util::perf::unroll<0, 3>( [&someArray] (snap::UnrollIndex u) {
///     someArray[u] = u * u;
///   });
///
/// is equivalent to:
///
///   someArray[0] = 0 * 0;
///   someArray[1] = 1 * 1;
///   someArray[2] = 2 * 2;
///   someArray[3] = 3 * 3;
///
/// The values of any additional parameters for the lambda must be provided
/// after the lambda. For example, the following:
///
/// snap::util::perf::unroll<0, 3>( 
///   [&someArray] (snap::UnrollIndex u, int offset) {
///     someArray[offset + u] = offset + u;
///   }, 
///   1000  // Value of offset
/// );
///
/// is equivalent to:
///
///   someArray[1000 + 0] = 1000 + 0;
///   someArray[1000 + 1] = 1000 + 1;
///   someArray[1000 + 2] = 1000 + 2;
///   someArray[1000 + 3] = 1000 + 3;
///
/// Lastly, if lambda function does not need to access the value of the
/// unrolled index, then simply omit the (const snap::UnrollIndex) or 
/// (snap::UnrollIndex) parameter from the lambda, and specify other arguments 
/// after the lambda as in the example above. For example:
///
/// size_t someVar = 0; 
/// size_t offset  = 10;
///
/// snap::util::perf::unroll<0, 15>([&someVar] (const size_t off) {
///     someVar += off;
///
///     // Some other code ...
///   },
///   offset
/// );
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
    arg = traits::function_traits<decltype(l)>::template arg<1>::defaultArg()
  ) {
  // Call the lambda and give pass it the unroll_index value.
  l(UnrollIndex(Start));
}

template <uint8_t Start, uint8_t End, typename Lambda>
static inline constexpr 
void unroll(Lambda l, 
    typename std::enable_if_t<
        (detail::unroll_enable<decltype(l), Start, End>::arity == 1) &&
        (detail::unroll_enable<decltype(l), Start, End>::value 
         == VAL_SPAN_WI_UROLL),
        typename traits::function_traits<decltype(l)>::template arg<1>::type> 
    arg = traits::function_traits<decltype(l)>::template arg<1>::defaultArg()
  ) {
  // Call the lambda and pass it the unroll_index_value.
  l(UnrollIndex(Start));

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
    arg = traits::function_traits<decltype(l)>::template arg<1>::defaultArg(), 
    Args&&... args) {
  // Define the type of the first argument.
  using ArgType = 
    typename traits::function_traits<decltype(l)>::template arg<1>::type;

  // Call the lambda and pass it the unroll const as well as the args.
  l(UnrollIndex(Start)        , 
    std::forward<ArgType>(arg), 
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
    arg = traits::function_traits<decltype(l)>::template arg<1>::defaultArg(), 
    Args&&... args) {
  // Define the type of the first argument.
  using ArgType = 
    typename traits::function_traits<decltype(l)>::template arg<1>::type;

  // Call the lambda and pass it the unroll const as well as the args.
  l(UnrollIndex(Start)         , 
    std::forward<ArgType>(arg) , 
    std::forward<Args>(args)...
  );

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End>(
    l                            , 
    std::forward<ArgType&&>(arg) , 
    std::forward<Args&&>(args)...
  );
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
    arg = traits::function_traits<decltype(l)>::template arg<0>::defaultArg()
  ) { 
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
    arg = traits::function_traits<decltype(l)>::template arg<0>::defaultArg()
  ) {
  l();

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End>(l);
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
    arg = traits::function_traits<decltype(l)>::template arg<0>::defaultArg(),
    Args&&... args) {
  // Define the type of the first argument.
  using ArgType = 
    typename traits::function_traits<decltype(l)>::template arg<0>::type;

  // Call the lambda and pass the arguments.
  l(std::forward<ArgType>(arg));
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
    arg = traits::function_traits<decltype(l)>::template arg<0>::defaultArg(),
    Args&&... args) {
  // Define the type of the first argument.
  using ArgType = 
    typename traits::function_traits<decltype(l)>::template arg<0>::type;

  // Call the lambda and pass it the unroll const as well as the args.
  l(std::forward<ArgType>(arg), std::forward<Args>(args)...);

  // Invoke the next iteration of the unrolling.
  unroll<Start + 1, End>(l, std::forward<ArgType&&>(arg), 
    std::forward<Args&&>(args)...);
}

} // namespace perf
} // namespace util
} // namespace snap

#endif // SNAP_UTIL_PERFORMANCE_HPP
