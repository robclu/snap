//---- tests/utility_tests.cc ------------------------------ -*- C++ -*- ----//
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
/// \file  utility_tests.cc
/// \brief Test file to test the snap utility features.
//
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE snap_utility_tests

#include <boost/test/unit_test.hpp>
#include "snap/utility/utility.hpp"
#include <array>
#include <iostream>

// NOTE: The utility.hpp header simply provides all of the specific utilities
// which are tested here. Each of the test suites tested here have their own
// header file in the snap/utility directory.

using namespace snap;

static constexpr uint64_t UNROLL_SIZE = 8;

struct performance_fixture {
  std::array<uint64_t, 10000> elements = {0};
};

BOOST_FIXTURE_TEST_SUITE(snap_utility_performance_suite, performance_fixture)

BOOST_AUTO_TEST_CASE(can_unroll_and_access_unroll_index) {
  const auto end = elements.size() - UNROLL_SIZE + 1;

  // Set each element in the array to its index value, but unroll it.
  for (size_t i = 0; i < end; i += UNROLL_SIZE) {
    util::perf::unroll<0, UNROLL_SIZE - 1>(
      [&] (const unroll_index uidx) {
        elements[i + uidx] = i + uidx;
      }
    );
  }

  for (size_t i = 0; i < elements.size(); ++i)
    BOOST_CHECK(elements[i] == i);
} 


BOOST_AUTO_TEST_CASE(can_provide_parameters_for_unroll) {
  const auto    end    = elements.size() - UNROLL_SIZE + 1;
  const int     offset_a = 12;
  const int16_t offset_b = 123;

  // Set each element to it's index plus the offset valud.
  for (size_t i = 0; i < end; i += UNROLL_SIZE) {
    util::perf::unroll<0, UNROLL_SIZE - 1>(
      [&] (const unroll_index uidx, const int off_a, const int16_t off_b) {
        elements[i + uidx] = i + uidx + off_a + off_b;
      }, 
      offset_a,       // Value of off_a in unrolled lambda.
      offset_b        // Value of off_b in unrolled lambda
    );
  }

  for (size_t i = 0; i < elements.size(); ++i) 
    BOOST_CHECK(elements[i] == i + offset_a + offset_b);
}

BOOST_AUTO_TEST_CASE(can_unroll_without_unroll_index_access) {
  size_t           x   = 0;
  constexpr size_t END = 10;
  for (size_t i = 0; i < END; ++i) {
    util::perf::unroll<0, UNROLL_SIZE - 1>([&] () {
        x += 1;
      }
    );
  }

  BOOST_CHECK(x == (END * UNROLL_SIZE));
}

BOOST_AUTO_TEST_SUITE_END()  
