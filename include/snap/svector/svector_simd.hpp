#include "svector_general.hpp"

namespace snap {

/// General implementation of svector for SSE types.
template <typename Type, uint8_t Width>
SNAP_ALIGN class svector {
 public:
  /// Alias for the type of data.
  using data_type = __mm128i;

 protected:
  data_type data; //!< Data for the vector.

 public:
  /// Default constructor: does nothing.
  svector() {}

  /// Constructor to convert from sse intrinsics.
  svector(const data_type& other_data) {
    data = other_data;
  }
};

} // namespace snap
