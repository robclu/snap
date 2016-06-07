//---- tests/vector_tests.cc ------------------------------- -*- C++ -*- ----//
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
/// \file  vector_tests.cc
/// \brief Test file to test the snap Vector class.
//
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE SnapVectorTests

#include <boost/test/unit_test.hpp>
#include "snap/vector/vector.hpp"

using namespace snap;

static constexpr uint8_t elemU8 = 244;  // 8 bit unsigned test element.
static constexpr int8_t  elemS8 = -110; // 8 bit signed test element.

// Fixture struct for svec testing.
struct Vec16x8Fixture {
  SNAP_ALIGN(16) uint8_t uint16x8a[16] = { // 16 byte aligned unsigned array.
    0, 1, 2 , 3 , 4 , 5 , 6 , 7 ,
    8, 9, 10, 11, 12, 13, 14, 15
  };

  SNAP_ALIGN(16) int8_t sint16x8a[16] = {  // 16 byte aligned signed array.
    -8, -7, -6, -5, -4, -3, -2, -1, 
     0,  1,  2,  3,  4,  5,  6,  7
  };

  uint8_t uint16x8u[16] = {                // 16 byte unaligned unsigned array.
    0, 1, 2 , 3 , 4 , 5 , 6 , 7 ,
    8, 9, 10, 11, 12, 13, 14, 15
  };
        
  int8_t sint16x8u[16] = {                 // 16 byte unaligned signed array.
    -8, -7, -6, -5, -4, -3, -2, -1, 
     0,  1,  2,  3,  4,  5,  6,  7
  };
};

BOOST_FIXTURE_TEST_SUITE(SnapVec16x8Suite, Vec16x8Fixture)

BOOST_AUTO_TEST_CASE(canBroadcastConstructUnsigned) {
  Vec16x8u vecU8(elemU8);

  for (auto i = 0; i < 15; ++i) 
    BOOST_CHECK(vecU8[i] == elemU8);
}

BOOST_AUTO_TEST_CASE(canBroadcastConstructSigned) {
  Vec16x8s vecS8(elemS8);

  for (auto i = 0; i < 15; ++i) 
    BOOST_CHECK(vecS8[i] == elemS8);
}

BOOST_AUTO_TEST_CASE(canConstructFromArray) {
  Vec16x8u vecFromAligned(uint16x8a);
  Vec16x8s vecFromUnaligned(sint16x8u);

  for (auto i = 0; i < 16; ++i) {
    BOOST_CHECK(vecFromAligned[i]   == uint16x8a[i]);
    BOOST_CHECK(vecFromUnaligned[i] == sint16x8a[i]);
  }
}

BOOST_AUTO_TEST_CASE(canLoadFromAlignedMemory) {
  Vec16x8u vecU;
  Vec16x8s vecS;

  vecU.loada(uint16x8a);
  vecS.loada(sint16x8a);

  for (auto i = 0; i < 16; ++i) {
    BOOST_CHECK(vecU[i] == uint16x8a[i]);
    BOOST_CHECK(vecS[i] == sint16x8a[i]);
  }
}

BOOST_AUTO_TEST_CASE(canLoadFromUnalignedMemory) {
  Vec16x8u vecU;
  Vec16x8s vecS;

  vecU.load(uint16x8u);
  vecS.load(sint16x8u);

  for (auto i = 0; i < 16; ++i) {
    BOOST_CHECK(vecU[i] == uint16x8u[i]);
    BOOST_CHECK(vecS[i] == sint16x8u[i]);
  }
}

BOOST_AUTO_TEST_CASE(canStoreIntoAlignedMemory) {
  Vec16x8u vecU8(uint16x8a);

  // Create 16-byte aligned 16 element array.
  SNAP_ALIGN(16) uint8_t elems[16];

  // Store svec elements into element array.
  vecU8.store(elems);

  for (auto i = 0; i < 16; ++i) 
    BOOST_CHECK(vecU8[i] == elems[i]);
}

BOOST_AUTO_TEST_CASE(canStoreIntoUnalignedMemory) {
  Vec16x8s vecS8(sint16x8u);

  // Create non 16 byte aligned element array.
  int8_t elems[16];

  // Store svec elements into element array.
  vecS8.storeu(elems);

  for (auto i = 0; i < 16; ++i) 
    BOOST_CHECK(vecS8[i] == elems[i]);
}

BOOST_AUTO_TEST_CASE(canSetSingleElement) {
  Vec16x8u vec(4);   // Set all elements to the value 4.

  // Set the value of each of the elements to their index.
  for (auto i = 0; i < 16; ++i) {
    vec.set(i, i);
    BOOST_CHECK(vec[i] == i);
  }
}
  
BOOST_AUTO_TEST_SUITE_END()
