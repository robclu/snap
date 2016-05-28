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

static constexpr uint8_t elem_u8 = 244;  // 8 bit unsigned test element.
static constexpr int8_t  elem_s8 = -110; // 8 bit signed test element.

// Fixture struct for svec testing.
struct svec16x8_fixture {
  SNAP_ALIGN(16) uint8_t uint16x8[16] =  {  // 16 byte aligned array.
    0, 1, 2 , 3 , 4 , 5 , 6 , 7 ,
    8, 9, 10, 11, 12, 13, 14, 15
  };

  uint8_t elem_u8 = 244;                   // Test unsigned int, 8 bits.
  int8_t  elem_s8 = -110;                  // Test signed int, 8 bits.
        
  int8_t sint16x8[16] = {                  // Non 16 byte aligned array.
    -8, -7, -6, -5, -4, -3, -2, -1, 
     0,  1,  2,  3,  4,  5,  6,  7
  };
};

BOOST_FIXTURE_TEST_SUITE(snap_svec16x8_suite, svec16x8_fixture)

BOOST_AUTO_TEST_CASE(can_broadcast_construct_unsigned) {
  svec16x8u vec_uint8(elem_u8);

  for (auto i = 0; i < 15; ++i) 
    BOOST_CHECK(vec_uint8[i] == elem_u8);
}

BOOST_AUTO_TEST_CASE(can_broadcast_construct_signed) {
  svec16x8s vec_sint8(elem_s8);

  for (auto i = 0; i < 15; ++i) 
    BOOST_CHECK(vec_sint8[i] == elem_s8);
}

BOOST_AUTO_TEST_CASE(can_construct_from_aligned_array) {
  svec16x8u vec_from_aligned(uint16x8);
  svec16x8s vec_from_unaligned(sint16x8);

  for (auto i = 0; i < 16; ++i) {
    BOOST_CHECK(vec_from_aligned[i]   == uint16x8[i]);
    BOOST_CHECK(vec_from_unaligned[i] == sint16x8[i]);
  }
}

BOOST_AUTO_TEST_CASE(can_store_into_aligned_memory) {
  svec16x8u vec_uint8(uint16x8);

  // Create 16-byte aligned 16 element array.
  SNAP_ALIGN(16) uint8_t elems[16];

  // Store svec elements into element array.
  vec_uint8.store(elems);

  for (auto i = 0; i < 16; ++i) 
    BOOST_CHECK(elems[i] == vec_uint8[i]);
}

BOOST_AUTO_TEST_CASE(can_store_into_unaligned_memory) {
  svec16x8s vec_sint8(sint16x8);

  // Create non 16 byte aligned element array.
  int8_t elems[16];

  // Store svec elements into element array.
  vec_sint8.storeu(elems);

  for (auto i = 0; i < 16; ++i) 
    BOOST_CHECK(elems[i] == vec_sint8[i]);
}
  
BOOST_AUTO_TEST_SUITE_END()


