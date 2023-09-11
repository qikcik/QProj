#include "qstructField.hpp"

template<> FieldType getType<float>() { return FieldType::Float; }
template<> FieldType getType<std::string>() { return FieldType::StdString; }
