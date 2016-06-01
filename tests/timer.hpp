//---- include/chaos/timer.hpp ----------------------------- -*- C++ -*- ----//
//
///                                Chaos
//                          
//                      Copyright (c) 2016 Rob Clucas        
//                    Distributed under the MIT License
//                (See accompanying file LICENSE or copy at
//                   https://opensource.org/licenses/MIT)
//
// ========================================================================= //
//
/// \file  timer.hpp
/// \brief Defines a class to do timing.
//
//---------------------------------------------------------------------------//

#ifndef CHAOS_TIMER_TIMER_HPP
#define CHAOS_TIMER_TIMER_HPP

#include <chrono>
#include <iostream>
#include <array>

namespace chrono = std::chrono;

namespace chaos {

/// Struct to represent duration with a label.
struct Duration {
  using TimepointType = chrono::time_point<chrono::high_resolution_clock>;
  /// Defult constructor.
  Duration() : label("") {}
 
  TimepointType start;   //!< Start time for the duration.
  TimepointType end;     //!< End time for the duration.
  std::string   label;   //!< Label for the duration.
};

/// Timer class to add time points with an optional label. Internally the class
/// uses chrono::time_point and duration, but the class returns time as number,
/// in nano, micro, or milliseconds. The class uses a circular buffer to store
/// the timepoints, rewriting the first once the last has been written.
/// \tparam NumDurations The number of durations to store.
/// \tparam Resolution   An std::duration representing the default resolution.
template <size_t NumDurations, typename Resolution = chrono::nanoseconds>
class Timer {
 public:
  /// Constructor to set the default resolution.
  Timer()
  : LastTimepoint(0) {}

  /// Adds a timepoint as the last timepoint, starting the timer for the
  /// timepoint.
  /// \param label The label for the timepoint.
  void startTimepoint(const std::string& label) {
    Timepoints[LastTimepoint].label = label;
    Timepoints[LastTimepoint].start = chrono::high_resolution_clock::now();
    LastTimepoint                   = (LastTimepoint + 1) % NumDurations;
  }

  /// Stops the last started timepoint.
  void stopTimepoint() {
    const auto index      = LastTimepoint == 0 ? Timepoints.size() - 1
                                               : LastTimepoint - 1;
    Timepoints[index].end = chrono::high_resolution_clock::now();
  }

  /// Prints the last timepoint.
  void printTimepoint() {
    const auto  index     = LastTimepoint == 0 ? Timepoints.size() - 1 
                                               : LastTimepoint - 1;
    const auto& timepoint = Timepoints[index];
    const auto  duration  = timepoint.end - timepoint.start;

    std::cout << timepoint.label << " : " << 
      chrono::duration_cast<Resolution>(duration).count() << "\n";
  }

 private:
  using BufferType = std::array<Duration, NumDurations>;

  BufferType  Timepoints;     //!< Buffer of timepoints.
  size_t      LastTimepoint;  //!< The index of the last timepoint.
};

} // namespace chaos

#endif // CHAOS_TIMER_TIMER_HPP

