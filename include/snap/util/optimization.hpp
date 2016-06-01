
#ifndef SNAP_UTIL_OPTIMIZATION_HPP  
#define SNAP_UTIL_OPTIMIZATION_HPP  

namespace snap {

class unroll_const {
  uint8_t val;

 public:
  constexpr unroll_const() : val(0) {}
  constexpr unroll_const(uint8_t value) : val(value) {}
  constexpr uint8_t value() { return val; }
  constexpr operator uint8_t() const { return val; }
};

namespace util {
namespace detail {

/// Checks if a range is valid for unrolling.
/// \param start The start value of the range to unroll.
/// \param end   The end value of the range to unroll.
static inline constexpr 
bool is_valid_unroll_span(const size_t start, const size_t end) {
  return start < end;
}

/// Checks if an unroll  span specifies the end of the span.
/// \param start The start value of the span to unroll.
/// \param end   The end value of the span to unroll.
static inline constexpr 
bool is_unroll_span_end(const size_t start, const size_t end) {
  return start == end;
}

template <typename... Ts>
struct is_unroll_const;

// Checks if the type is an unroll_const.
template <typename T, typename... Ts>
struct is_unroll_const<T, Ts...> {
  static inline constexpr bool value() {
    return std::is_same<T, unroll_const>::value;
  }
};

template <>
struct is_unroll_const<> {
  static inline constexpr bool value() {
    return false;
  }
};

} // namespace detail


/// Defines a struct to detect which version of the loop unrolling functions is
/// valid.
/// \tparam Type  The type to check for an unroll const.
/// \tparam Start The starting value of the span.
/// \tparam End   The end value of the span.
template <typename T, uint8_t Start, uint8_t End>
struct unroll_enable {
  /// Returns truw if the unrolling is using an unroll constant and if the span 
  /// given by Start -> End is valid (Start < End).
  static inline constexpr bool valid_span_with_uroll_const() {
    return detail::is_unroll_const<T>::value() && 
           detail::is_valid_unroll_span(Start, End);
  }

  /// Returns true if the unrolling is using an unroll const and if the span 
  /// given by Start -> End is the end of the span (Start == End).
  static inline constexpr bool end_span_with_uroll_const() {
    return detail::is_unroll_const<T>::value() && 
           detail::is_unroll_span_end(Start, End);
  }

  /// Returns true if the unrolling is not using an unroll const and if the
  /// span given by Start -> End is valid (Start < End).
  static inline constexpr bool valid_span_wo_uroll_const() {
    return !detail::is_unroll_const<T>::value() && 
            detail::is_valid_unroll_span(Start, End);
  }

  /// Returns true if the unrolling is not using an unroll const and if the
  /// span given by Start -> End is the end of the span (Start == End).
  static inline constexpr bool end_span_wo_uroll_const() {
    return !detail::is_unroll_const<T>::value() && 
            detail::is_unroll_span_end(Start, End);
  }
};

/// Defines a function that can unroll a lambda function to improve performance 
/// in critical sections of code. This version is enabled when the template
/// parameters are not equal and the Start parameter is less than the End
/// parameter. 
///
/// The function will pass the constant value (unroll index) as the first 
/// argument in the lambda
/// To access the template paramaeters inside the lamda, the lambda
/// must specify that the first argument is of type (const) uint8_t, which will 
/// be the template value (offset in the example to follow).
///
/// The function can be used as follows, for example to unroll the setting of
/// (8) elements in an array:
/// 
///   // Define an array of 8 elements.
///   std::vector<uint64_t> v(8);
///
///   snap::util:unroll<0, 7>([&v] (const uint8__t offset) {
///     v[offset] = offset;
///   });
///
/// Which effectively results in the following code (when compiled):
///
///   v[0] = 0;
///   v[1] = 1;
///   v[2] = 2;
///   v[3] = 3;
///   v[4] = 4;
///   v[5] = 5; 
///   v[6] = 6;
///   v[7] = 7;
//template <uint8_t Start, uint8_t End, typename LType, typename...LArgs>
//static inline constexpr 
//void unroll(LType lambda, LArgs&&... args,
//    typename std::enable_if<
//      sizeof...(LArgs) == 0, void*
//    >::type = nullptr) {
//  lambda(std::forward<LArgs&&>(args)...);
//}

// Span and const
template <
  uint8_t Start   , 
  uint8_t End     , 
  typename LType  , 
  typename LArg   ,
  typename... LArgs,
  typename std::enable_if<
      unroll_enable<LArg, Start, End>::valid_span_with_uroll_const(), void*
    >::type = nullptr
> static inline constexpr 
void unroll(LType lambda, LArg arg, LArgs... args) {
  lambda(unroll_const(Start), std::forward<LArgs>(args)...);
  unroll<Start + 1, End>(lambda, std::forward<LArg&&>(arg), 
    std::forward<LArgs&&>(args)...);
}

// Span and no const 
template <
  uint8_t Start   , 
  uint8_t End     , 
  typename LType  , 
  typename LArg   ,
  typename... LArgs
> static inline constexpr 
void unroll(LType lambda, LArg&& arg, LArgs&&... args,
    typename std::enable_if<
      unroll_enable<LArg, Start, End>::valid_span_wo_uroll_const(), void*
    >::type = nullptr) {
  lambda(std::forward<LArg&&>(arg), std::forward<LArgs>(args)...);
  unroll<Start + 1, End>(lambda, std::forward<LArg&&>(arg), 
    std::forward<LArgs&&>(args)...);
}

// End and const
template <
  uint8_t Start   ,
  uint8_t End     ,
  typename LType  ,
  typename LArg   ,
  typename... LArgs
> static inline constexpr 
void unroll(LType lambda, LArg&& arg, LArgs&&... args,
    typename std::enable_if<
      unroll_enable<LArg, Start, End>::end_span_with_uroll_const(), void*
    >::type = nullptr) {
  lambda(unroll_const(Start), std::forward<LArgs&&>(args)...);
}

// End and no const
template <
  uint8_t Start   ,
  uint8_t End     ,
  typename LType  ,
  typename LArg   ,
  typename... LArgs
> static inline constexpr 
void unroll(LType lambda, LArg&& arg, LArgs&&... args,
    typename std::enable_if<
      unroll_enable<LArg, Start, End>::end_span_wo_uroll_const(), void*
    >::type = nullptr) {
  lambda(std::forward<LArg&&>(arg), std::forward<LArgs&&>(args)...);
}

// When there are no args
/*
template <
  uint8_t Start   ,
  uint8_t End     ,
  typename LType  ,
  typename... LArgs
> static inline constexpr 
void unroll(LType lambda, LArgs&&... args,
    typename std::enable_if<
      unroll_enable<LArg, Start, End>::end_span_wo_uroll_const(), void*
    >::type = nullptr) {
  lambda(std::forward<LArg&&>(arg), std::forward<LArgs&&>(args)...);
}
*/
} // namespace util
} // namespace snap

#endif // SNAP_UTIL_OPTIMIZATION_HPP  
