
/// Defines a general vector class which can be overloaded for specific
/// platforms, data types, and widths.
/// \tparam Type  The data type being used.
/// \tparam Width The number of Type elements in the vector.
template <typename Type, size_t Width>
class svector;
