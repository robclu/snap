#include <vector>
#include "timer.hpp"

using namespace chaos;

template <uint8_t Start, uint8_t End, typename L, typename...As>
static inline constexpr void unroll(L l, As&&... as, 
typename std::enable_if<Start != End, void*>::type = nullptr) {
  l(Start, std::forward<As>(as)...);
  unroll<Start + 1, End>(l, as...);
}

template <uint8_t Start, uint8_t End, typename L, typename...As>
static inline constexpr void unroll(L l, As&&... as, 
typename std::enable_if<Start == End, void*>::type = nullptr) {
  l(Start, std::forward<As>(as)...);
//  unroll<Start + 1, End>(l, as...);
}



template <typename T>
static inline void arr_basic(T& a) {
  // Modify array basic
  for (size_t i = 0; i < a.size(); ++i) {
    a[i] = a[i] * i + a[i];
  } 
}

template <typename T>
static inline void vec_basic(T& v) {
  // Modify vec
  for (size_t i = 0; i < v.size() - 3; i += 4) {
    v[i] = v[i] * i + v[i];
    v[i + 1] = v[i + 1] * (i + 1) + v[i + 1];
    v[i + 2] = v[i + 2] * (i + 2) + v[i + 2];
    v[i + 3] = v[i + 3] * (i + 3) + v[i + 3];
  }
}

template <typename T>
static inline void unroll_basic(T& u) {
  for (size_t i = 0; i < u.size() - 7; i += 8) {  
    unroll<0, 7>([&u, i] (const int offset) {
      u[i + offset] = u[i + offset] * (i + offset) + u[i + offset];
    });
  }
};

int main() {

  std::vector<int> a(100000000);
  std::vector<int> v(100000000);
  std::vector<int> u(100000000);
  Timer<3>                    t;

  t.startTimepoint("BSC");

  // Basic mode
  arr_basic(a);

  t.stopTimepoint();
  t.printTimepoint();
  
  t.startTimepoint("VEC");

  // Vec mode 
  vec_basic(v);

  t.stopTimepoint();
  t.printTimepoint();

  t.startTimepoint("URL");

  unroll_basic(u);

  t.stopTimepoint();
  t.printTimepoint();

  return 0;
}

