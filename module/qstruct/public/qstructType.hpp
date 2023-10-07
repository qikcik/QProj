#pragma once

#include <vector>
#include <string>
#include "qstructField.hpp"
#include <functional>

class FieldInfo;


class QStructType
{
public:

    enum class ImplType {
        Plain,
        Dynamic
    };

    explicit QStructType(std::string&& inName,
                         size_t inSize,
                         std::vector<FieldInfo>&& inFields,
                         const ImplType&& implType,
                         size_t inDynamicTypeOffset,
                         const std::function<void(void*)>&& inInitInstance,
                         const std::function<void(void*)>&& inDeInitInstance,
                         const QStructType* inBase = nullptr);

    const size_t size;
    const std::string name;
    const ImplType implType;
    const size_t dynamicTypeOffset;

    const std::function<void(void*)> initInstance;
    const std::function<void(void*)> deInitInstance;

    const FieldInfo& getField(const std::string& fieldName) const;
    const std::vector<FieldInfo> getAllFields() const;

protected:
    const QStructType* base;
    std::vector<FieldInfo> fields;
};


template<typename T>
const QStructType* getBaseClass() {
    return nullptr;
}

template<typename T>
concept QStructDerivered = requires (T t)
{
    typename T::base_class;
};

template<QStructDerivered T>
const QStructType* getBaseClass() {
    return &T::base_class::staticType;
}

#define GEN_QSTRUCT_FIELD_ENTRY(type,name) FieldInfo{#name, offsetof(type, name), getType<typeof(type::name)>()}

#define GEN_QSTRUCT_TYPE(type,entries...) \
const QStructType type::staticType{ \
        #type, \
        sizeof(type),          \
        entries,                       \
        getQStructImplType<type>(),       \
        getQStructDynamicTypeOffset<type>(), \
        [](void* addr) { new (addr) type(); }, \
        [](void* addr) { reinterpret_cast<type*>(addr)->~type(); }, \
        getBaseClass<type>()\
};

template<typename T>
concept QStruct = requires (T t)
{
    { T::staticType } -> std::convertible_to<const QStructType>;
};

template<QStruct T>
FieldType::type getType() {
    return FieldType::QStruct { &T::staticType };
}


template<typename T>
concept QStructDynamic = requires (T t)
{
    { T::staticType } -> std::convertible_to<const QStructType>;
    { t.qStructType } -> std::convertible_to<const QStructType*>;
};

template<typename T>
QStructType::ImplType getQStructImplType() {
    return QStructType::ImplType::Plain;
}


template<QStructDynamic T>
QStructType::ImplType getQStructImplType() {
    return QStructType::ImplType::Dynamic;
}


template<typename T>
size_t getQStructDynamicTypeOffset() {
    return -1;
}


template<QStructDynamic T>
size_t getQStructDynamicTypeOffset() {
    return offsetof(T,qStructType);
}

#include "dynamicArray.hpp"

template<typename T>
concept TDynamicArray = requires(T a) {
    requires std::same_as<T, qstd::DynamicArray<typename T::element_type>>;
};

//TODO: implement

template<TDynamicArray T>
FieldType::type getType() {
    using value_type = typename T::element_type;
    return FieldType::DynamicArray{
        std::make_unique<FieldType::type>(
            getType<value_type>()
        )
    };
}

template<typename T>
concept TStdUniquePtr = requires(T a) {
    requires std::same_as<T, std::unique_ptr<typename T::element_type>>;
};

//TODO: implement

template<TStdUniquePtr T>
FieldType::type getType() {
    using value_type = typename T::element_type;
    return FieldType::StdUniquePtr{
        &T::element_type::staticType
    };
}
