#include "qstructType.hpp"
#include "qstructField.hpp"
#include <algorithm>

QStructType::QStructType(std::string&& inName,
                         size_t inSize,
                         std::vector<FieldInfo>&& inFields,
                         const ImplType&& inImplType,
                         size_t inDynamicTypeOffset,
                         const std::function<void(void*)>&& inInitInstance,
                         const std::function<void(void*)>&& inDeInitInstance,
                         const QStructType* inBase)
:
name(inName),
size(inSize),
fields(std::move(inFields)),
base(inBase),
implType(inImplType),
dynamicTypeOffset(inDynamicTypeOffset),
initInstance(inInitInstance),
deInitInstance(inDeInitInstance)
{

}


const FieldInfo& QStructType::getField(const std::string& fieldName) const
{
    auto fieldMather = [&fieldName](const FieldInfo& it){ return it.name == fieldName;};
    if (auto it = std::find_if(begin(fields), end(fields), fieldMather); it != end(fields))
    {
        return *it;
    }

    if(base)
        return base->getField(fieldName);

    return {};
}

const std::vector<FieldInfo> QStructType::getAllFields() const
{
    if(!base)
        return fields;

    std::vector<FieldInfo> result = base->getAllFields();
    result.reserve( fields.size() + result.size() );
    for(auto& it : fields)
        result.push_back(it);

    return fields;
}
