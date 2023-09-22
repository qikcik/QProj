#pragma once

#include <vector>
#include <string>

class FieldInfo;

class QStructType
{
public:
    explicit QStructType(std::string&& inName, std::vector<FieldInfo>&& inFields);
    const std::string name;

    const FieldInfo& getField(const std::string& fieldName) const;
    const std::vector<FieldInfo>& getFields() const;

protected:
    std::vector<FieldInfo> fields;
};

#define GEN_QSTRUCT_FIELD_ENTRY(type,name) {#name, offsetof(type, name), getType<typeof(type::name)>()},


template<typename T>
concept QStruct = requires (T t)
{
    { T::staticType } -> std::convertible_to<const QStructType>;
};
