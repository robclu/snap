//---- tests/config.cc ------------------------------------- -*- C++ -*- ----//
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
/// \file  config.cc
/// \brief Test file to test that snap can create a valid configuration.
//
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE SnapConfigTests

#include <boost/test/unit_test.hpp>
#include "snap/config/simd_instruction_detect.h"

BOOST_AUTO_TEST_SUITE(SnapConfigSuite)

BOOST_AUTO_TEST_CASE(simdTypeDetected) {
  BOOST_CHECK(snap::SIMD_TYPE >= 0 && snap::SIMD_TYPE <= 10);
}

BOOST_AUTO_TEST_CASE(alignmentSet) {
  BOOST_CHECK(snap::ALIGNMENT % 2 == 0 && snap::ALIGNMENT != 0);
}

BOOST_AUTO_TEST_SUITE_END()
