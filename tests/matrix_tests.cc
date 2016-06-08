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
#include "snap/matrix/convert.hpp"

using namespace snap;

BOOST_AUTO_TEST_SUITE(SnapMatrixSuite)

BOOST_AUTO_TEST_CASE(canDefaultConstructMatGrey8) {
  Matrix<mat::FM_GREY_8> mat;

  BOOST_CHECK(mat.rows() == 0);
  BOOST_CHECK(mat.cols() == 0);
  BOOST_CHECK(mat.size() == 0);
  BOOST_CHECK(mat.step() == 0);
}

BOOST_AUTO_TEST_CASE(canConstructWithAlignedSize) {
  size_t rows = ALIGNMENT, cols = ALIGNMENT;

  Matrix<mat::FM_GREY_8> mat(rows, cols);

  // For an aligned matrix, the size should be rows * cols.
  // The number of columns should also match the step size.
  BOOST_CHECK(mat.rows()             == rows       );
  BOOST_CHECK(mat.cols()             == cols       );
  BOOST_CHECK(mat.step() % ALIGNMENT == 0          );
  BOOST_CHECK(mat.step()             == mat.cols() );
  BOOST_CHECK(mat.size()             == rows * cols);
}

BOOST_AUTO_TEST_CASE(canConstructWithUnalignedSize) {
  size_t rows = 5, cols = ALIGNMENT + 1;

  // The matrix constructor allocates aligned data even if the specified
  // rows and columns will not produce aligned data, so the size should be 
  // larger than rows * cols in this case, and the step size will indicate
  // the number of elements per row.
  Matrix<mat::FM_GREY_8> mat(rows, cols);

  // For unaligned data the step size should be larger than the number of 
  // columns, and the size will hence be larger than rows * cols.
  BOOST_CHECK(mat.rows()             == rows       );
  BOOST_CHECK(mat.cols()             == cols       );
  BOOST_CHECK(mat.step() % ALIGNMENT == 0          );
  BOOST_CHECK(mat.step()             >  mat.cols() );
  BOOST_CHECK(mat.size()             >  rows * cols);
}

// OpenCV conversion tests
#ifdef OPENCV

BOOST_AUTO_TEST_CASE(canConvertCVMatUCharToSnapGrey) {
  const int rows = 480, cols = 640;

  cv::Mat                 cvMat(rows  , cols, CV_8UC1);
  Matrix<mat::FM_GREY_8>  snapMat(rows, cols);

  // Convert the opencv unsigned char mat to a snap one.
  mat::convert(cvMat, snapMat);

  BOOST_CHECK(0 == 0);
}

BOOST_AUTO_TEST_CASE(canCostructSnapMatrixFromCVmat) {
  const int rows = 480, cols = 640;

  cv::Mat                 cvMat(rows, cols, CV_8UC1);
  Matrix<mat::FM_GREY_8>  snapMat(cvMat);

  BOOST_CHECK(snapMat.rows() == cvMat.rows);
  BOOST_CHECK(snapMat.cols() == cvMat.cols);

  // TODO: Check that the data ias correct.
}

#endif

BOOST_AUTO_TEST_SUITE_END()
