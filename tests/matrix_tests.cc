//---- tests/matrix_tests.cc ------------------------------- -*- C++ -*- ----//
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
/// \file  matrix_tests.cc
/// \brief Test file to test the snap Matrix class.
//
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE SnapMatrixTests

#include <boost/test/unit_test.hpp>
#include "snap/matrix/matrix.hpp"

using namespace snap;

BOOST_AUTO_TEST_SUITE(SnapMatrixSuite)

BOOST_AUTO_TEST_CASE(canDefaultConstructMatGrey8) {
  Matrix<mat::FM_GREY_8> mat;

  BOOST_CHECK(mat.rows() == 0);
  BOOST_CHECK(mat.cols() == 0);
  BOOST_CHECK(mat.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

