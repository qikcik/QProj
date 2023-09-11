#include "qstructType.hpp"
#include "qstructField.hpp"
#include <algorithm>

QStructType::QStructType(std::string&& inName, std::vector<FieldInfo>&& inFields)
:   name(inName), fields(inFields)
{

}


const FieldInfo& QStructType::getField(const std::string& fieldName) const
{
    auto fieldMather = [&fieldName](const FieldInfo& it){ return it.name == fieldName;};
    if (auto it = std::find_if(begin(fields), end(fields), fieldMather); it != end(fields))
    {
        return *it;
    }
    return {};
}