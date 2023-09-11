#pragma once

#include <vector>
#include <string>

class FieldInfo;

class QStructType
{
public:
    explicit QStructType(std::string&& inName, std::vector<FieldInfo>&& inFields);
    std::string name;

    const FieldInfo& getField(const std::string& fieldName) const;

protected:
    std::vector<FieldInfo> fields;
};

#define GEN_QSTRUCT_FIELD_ENTRY(type,name) {#name, offsetof(type, name), getType<typeof(type::name)>()},
