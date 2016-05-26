//---- examples/svec_examples.cc --------------------------- -*- C++ -*- ----//
//
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
/// \file  svec_examples.cc
/// \brief Examples files for Snap svec class.
//
//---------------------------------------------------------------------------//

#include "snap/config/simd_instruction_detect.h"

// Test class
template <typename T, uint8_t W>
class svec;

template <typename T>
class svec<T, 16> {
 public:
   using data_type = __m128i;
   using svec_type = svec<T, 16>;

   enum { elements = 16 / sizeof(T), bytes = 16 };
   svec() {}
   svec(const data_type& x) { data = x; }
   svev_type& operator=(const data_type& x) { data = x; return *this; }
   operator __m128i() const { return data; }

   // Loads an array into the vector.
   template <typename U>
   svec_type& load(U const* u) 
     data = _mm_load_si128(u);
     return *this;
   }
   
 protected:
   data_type data;
};
 

int main() {
  svec<uint8_t, 16> byte_vec;
  uint8_t data[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  // Load data into the vector
  byte_vec.load(data);
}
