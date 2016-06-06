//---- tests/smat_tests.cc --------------------------------- -*- C++ -*- ----//
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
/// \file  smat_tests.cc
/// \brief Test file to test the snap smat class.
//
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE snap_smat_tests

#include <boost/test/unit_test.hpp>
#include "snap/smat/smat.hpp"

using namespace snap;

BOOST_AUTO_TEST_SUITE(snap_smat_suite)

BOOST_AUTO_TEST_CASE(can_default_construct_smat_grey_8) {
  smat<mat::FMT_GREY_8> mat_grey;

  BOOST_CHECK(mat_grey.rows() == 0);
  BOOST_CHECK(mat_grey.cols() == 0);
  BOOST_CHECK(mat_grey.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()

