//---- snap/utility/traits.hpp ----------------------------- -*- C++ -*- ----//
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
/// \file  traits.hpp
/// \brief Defines utility functions related to type traits.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_UTILITY_TRAITS_HPP
#define SNAP_UTILITY_TRAITS_HPP

#include <tuple>

namespace snap   {
namespace util   {
namespace traits {
namespace detail {

/// Defines a struct to get the type of an argument with a specific index from
/// a parameter pack. The struct is specialized for the case that the index is
/// too large for the number of aruments in the parameter pack so that that
/// case can be handled.
/// \tparam IdxValid  If the index is valid (Idx < sizeof(Args)).
/// \tparam Idx       The index of the argument to get the type of.
/// \tparam Args      The arguments to a type of.
template <bool IdxValid, size_t Idx, typename... Args>
struct arg_extract;

/// Specialization for a valid index.
template <size_t Idx, typename... Args>
struct arg_extract<true, Idx, Args...> {
  /// Defines the type of the argument at index Idx.
  using type = typename std::tuple_element<Idx, std::tuple<Args...>>::type;
};

/// Specialization for an invalid index.
template <size_t Idx, typename... Args>
struct arg_extract<false, Idx, Args...> {
  /// Defines the type of the argument at the (invalid) index Idx.
  using type = void*;
};
  
/// Defines a struct which can return the type of a variadic argument pack at a
/// specific index, and can handle out-of-range index by returning a void*
/// type.
/// /tparam Idx   The index of the element to get the traits of.
/// /tparam Args  The variadic argument pack of all arguments.
template <size_t Idx, typename... Args>
struct arg_traits;

/// Specialization for at least one arg.
template <size_t Idx, typename Arg, typename... Args>
struct arg_traits<Idx, Arg, Args...> {
  /// Gets the type of the argument at index Idx if Idx is less than the number
  /// of args (Idx is in range), otherwise returns a void*.
  using type = typename 
    arg_extract<(Idx < sizeof...(Args) + 1), Idx, Arg, Args...>::type;
  
  /// Gets the default value of the type of the argument at index Idx if Idx is
  /// less than the number of args (idx is valid).
  template <size_t I = Idx>
  static constexpr typename 
  std::enable_if_t<(I < sizeof...(Args) + 1), type>  defaultArg() {
    return type();
  }

  /// Gets the default value of the type of the argument when Idx is greated
  /// than the number of arguments, since the index is invalid it return a
  /// nullptr.
  template <size_t I = Idx>
  static constexpr typename
  std::enable_if_t<(I >= sizeof...(Args) + 1), type> defaultArg() {
    return nullptr;
  }
};

/// Specialization for no args.
template <size_t Idx>
struct arg_traits<Idx> {
  /// Gets the type as a void* since there are no arguments.
  using type = void*;

  /// Returns the default value as a nullptr because the default type for this
  /// case is defined to be a void*.
  static constexpr type defaultArg() { return nullptr; }
};

} // namespace detail

/// Defines a struct which can get the traits of a function, such as the 
/// return type, and the argument types.
/// \tparam T The type to get the traits for.
template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

// Specialization for a pointer to a member function.
// \tparam ClassType  The type of the class the member function is part of.
// \tparam ReturnType The return type of the function.
// \tparam Args...    The argument types for the function.
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const> {
  /// Returns the number of arguments for the function.
  static constexpr size_t arity = sizeof...(Args);

  /// Defines the return type of the function.
  using return_type = ReturnType;

  /// Defines a struct which allows the type of the argument at a specific
  /// index to be determined.
  /// \tparam Idx     The index of the argument to get the type of.
  /// \tparam Enable  If the struct is enabled.
  template <size_t Idx>
  struct arg {
    /// Defines the type of the argument in position Idx.
    using type = typename detail::arg_traits<Idx, Args...>::type;

    /// Defines the default type for the argument at Idx.
    static constexpr type defaultArg() { 
      return detail::arg_traits<Idx, Args...>::defaultArg();
    }
  };
};

/// Defines a struct to check if a span (range of numbers) is valid for
/// unrolling, or if the span specifies the end of the range of numbers.
/// \tparam Start The start value of the range to unroll.
/// tparam  End   The end value of the range to unroll.
template <uint8_t Start, uint8_t End>
struct check_unroll_span {

  /// Checks if the span is valid for unrolling (Start < End).
  static constexpr bool isValid() {
    return Start < End;
  }

  /// Checks if anspan specifies the end of the span.
  static constexpr bool isEnd() {
    return Start == End;
  }
};
 
} // namespace traits
} // namespace util
} // namespace snap

#endif // SNAP_UTILITY_TRAITS_HPP
