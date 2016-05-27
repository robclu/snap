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

// Struct with an off aligned array.
struct unaligned {
  int16_t offset;       // Offset of 2 bytes      , bytes 0  - 1.
  uint8_t uint16x8[16]; // 16 byte array of uints , bytes 2  - 17.
  int8_t  sint16x8[16]; // 16 byte array of int8  , bytes 18 - 33.
};

BOOST_AUTO_TEST_SUITE(snap_svec16x8_suite)

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

// TODO: Improve this test so that the order of the elements is ensured.
BOOST_AUTO_TEST_CASE(can_store_into_aligned_memory) {
  svec16x8u vec_uint8(elem_u8);

  // Create 16-byte aligned 16 element array.
  SNAP_ALIGN(16) uint8_t elems[16];

  // Store svec elements into element array.
  vec_uint8.store(elems);

  for (auto i = 0; i < 16; ++i) 
    BOOST_CHECK(elems[i] == vec_uint8[i]);
}

// TODO: Improve this test so that the order of the elements is ensured.
BOOST_AUTO_TEST_CASE(can_store_into_unaligned_memory) {
  svec16x8s vec_sint8(elem_s8);

  unaligned unaligned_test_struct;

  // Store svec elements into element array.
  vec_sint8.storeu(&unaligned_test_struct.sint16x8[0]);

  for (auto i = 0; i < 16; ++i) 
    BOOST_CHECK(unaligned_test_struct.sint16x8[i] == vec_sint8[i]);
}
  

BOOST_AUTO_TEST_SUITE_END()


