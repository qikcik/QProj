#include "qstructField.hpp"
#include "qstructType.hpp"
#include "overload.hpp"

template<> FieldType::type getType<float>() { return FieldType::Float{}; }
template<> FieldType::type getType<std::string>() { return FieldType::StdString{}; }

size_t getSizeOf(const FieldType::type& ref)
{
    return std::visit(qstd::overloaded{
            [&](const FieldType::Unimplemented& field)  {
                return size_t(0);
            },
            [&](const FieldType::DynamicArray& field)  {
                return sizeof(qstd::DynamicArray<uint8_t>);
            },
            [&](const FieldType::Float& field)  {
                return sizeof(float);
            },
            [&](const FieldType::StdString& field)  {
                return sizeof(std::string);
            },
            [&](const FieldType::QStruct& field)  {
                return field.type->size;
            }
    },ref);
}