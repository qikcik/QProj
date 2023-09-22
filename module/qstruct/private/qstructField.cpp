#include "qstructField.hpp"
#include "qstructType.hpp"

template<> FieldType::type getType<float>() { return FieldType::Float{}; }
template<> FieldType::type getType<std::string>() { return FieldType::StdString{}; }
