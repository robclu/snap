//---- snap/config/opencv.h -------------------------------- -*- C++ -*- ----//
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
/// \file  opencv.h
/// \brief Configuration file for opencv to include the relevant headers if 
///        opencv is enabled.
//
//---------------------------------------------------------------------------//

#ifndef SNAP_CONFIG_OPENCV_H
#define SNAP_CONFIG_OPENCV_H

#ifdef OPENCV
  #include <opencv2/opencv.hpp>
#endif

#endif // SNAP_CONFIG_OPENCV_H


