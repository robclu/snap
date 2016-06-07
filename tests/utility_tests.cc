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

#define BOOST_TEST_MODULE SnapUtilityTests

#include <boost/test/unit_test.hpp>
#include "snap/utility/utility.hpp"
#include <array>
#include <iostream>

// NOTE: The utility.hpp header simply provides all of the specific utilities
// which are tested here. Each of the test suites tested here have their own
// header file in the snap/utility directory.

using namespace snap;

static constexpr uint64_t UNROLL_SIZE = 8;

struct PerformanceFixture {
  std::array<uint64_t, 10000> elements = {0};
};

BOOST_FIXTURE_TEST_SUITE(SnapUtilityPerformanceSuite, PerformanceFixture)

BOOST_AUTO_TEST_CASE(canUnrollWithUnrollIndex) {
  const auto end = elements.size() - UNROLL_SIZE + 1;

  // Set each element in the array to its index value, but unroll it.
  for (size_t i = 0; i < end; i += UNROLL_SIZE) {
    util::perf::unroll<0, UNROLL_SIZE - 1>(
      [&] (const UnrollIndex unrollIdx) {
        elements[i + unrollIdx] = i + unrollIdx;
      }
    );
  }

  for (size_t i = 0; i < elements.size(); ++i)
    BOOST_CHECK(elements[i] == i);
} 


BOOST_AUTO_TEST_CASE(canUnrollWithUnrollIndexAndOtherParams) {
  const auto    end     = elements.size() - UNROLL_SIZE + 1;
  const int     offsetA = 12;
  const int16_t offsetB = 123;

  // Set each element to it's index plus the offset values.
  for (size_t i = 0; i < end; i += UNROLL_SIZE) {
    util::perf::unroll<0, UNROLL_SIZE - 1>(
      [&] (const UnrollIndex unrollIdx, const int offA, const int16_t offB) {
        elements[i + unrollIdx] = i + unrollIdx + offA + offB;
      }, 
      offsetA,       // Value of offA in unrolled lambda.
      offsetB        // Value of offB in unrolled lambda.
    );
  }

  for (size_t i = 0; i < elements.size(); ++i) 
    BOOST_CHECK(elements[i] == i + offsetA + offsetB);
}

BOOST_AUTO_TEST_CASE(canUnrollWithoutUnrollIndex) {
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

BOOST_AUTO_TEST_CASE(canUnrollWithoutUnrollIndexAndtherParams) {
  size_t           x       =  0;
  uint8_t          offsetB = 20;
  constexpr size_t END     = 10;

  for (size_t i = 0; i < END; ++i) {
    util::perf::unroll<0, UNROLL_SIZE - 1>(
      [&] (const uint8_t offB) {
        x += 1 + offB;
      }, 
      offsetB  // Value of offB in unrolled lambda.
    );
  }

  BOOST_CHECK(x == (END * UNROLL_SIZE * (1 + offsetB)));
}

BOOST_AUTO_TEST_SUITE_END()  
