//---- tests/svec_tests.cc --------------------------------- -*- C++ -*- ----//
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
/// \file  svec_tests.cc
/// \brief Test file to test the snap svec class.
//
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE snap_svec_tests

#include <boost/test/unit_test.hpp>
#include "snap/svec/svec.hpp"

using namespace snap;

BOOST_AUTO_TEST_SUITE(snap_svec16x8_suite)

BOOST_AUTO_TEST_CASE(can_broadcast_construct_unsigned) {
  constexpr uint8 elem = 244;
  svec16x8u vec_uint8(elem);

  for (auto i = 0; i < 15; ++i) 
    BOOST_CHECK(vec_uint8[i] == elem);
}

BOOST_AUTO_TEST_CASE(can_broadcast_construct_signed) {
  constexpr sint8 elem = -100;
  svec16x8s vec_sint8(elem);

  for (auto i = 0; i < 15; ++i) 
    BOOST_CHECK(vec_sint8[i] == elem);
}

BOOST_AUTO_TEST_SUITE_END()


