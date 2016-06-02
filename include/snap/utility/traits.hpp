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

/// Defines a struct which can get the traits of a function, such as the 
/// return type, and the argument types.
/// \tparam T The type to get the traits for.
template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

// Specialization for a pointer to a member function.
// \tparam ClassType  The type of the class the member function is part of.
// \tparam ReturnType The return type of the function.
// \tparam Args...    The argument types for the function.
template <typename ClassType, typename ReturnType, typename A, typename... Args>
struct function_traits<ReturnType(ClassType::*)(A, Args...) const> {
  /// Returns the number of arguments for the function.
  static constexpr size_t arity = sizeof...(Args) + 1;

  /// Defines the return type of the function.
  using return_type = ReturnType;

  /// Defines a struct which allows the type of the argument at a specific
  /// index to be determined.
  /// \tparam Idx     The index of the argument to get the type of.
  /// \tparam Enable  If the struct is enabled.
  template <size_t Idx>
  struct arg {
    /// Defines the type of the argument in position Idx.
    using type = typename std::tuple_element<Idx, std::tuple<A, Args...>>::type;
  };
};

template <typename ClassType, typename ReturnType>
struct function_traits<ReturnType(ClassType::*)() const> {
  /// Returns the number of arguments for the function.
  static constexpr size_t arity = 0;

  /// Defines the return type of the function.
  using return_type = ReturnType;

  template <size_t Idx>
  struct arg {
    using type = void;
  };
};

/// Defines a struct to check if a span (range of numbers) is valid for
/// unrolling, or if the span specifies the end of the range of numbers.
/// \tparam Start The start value of the range to unroll.
/// tparam  End   The end value of the range to unroll.
template <uint8_t Start, uint8_t End>
struct check_unroll_span {

  /// Checks if the span is valid for unrolling (Start < End).
  static constexpr bool is_valid() {
    return Start < End;
  }

  /// Checks if anspan specifies the end of the span.
  static constexpr bool is_end() {
    return Start == End;
  }
};
 
} // namespace traits
} // namespace util
} // namespace snap

#endif // SNAP_UTILITY_TRAITS_HPP
