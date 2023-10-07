#pragma once
#include "dynamicArray.tpp"
#include "qstructType.hpp"

#include "entry.hpp"

struct List
{
    std::string title {};
    qstd::DynamicArray<Entry> entries {};

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType List::staticType{"List",sizeof(List),{
        GEN_QSTRUCT_FIELD_ENTRY(List,title),
        GEN_QSTRUCT_FIELD_ENTRY(List,entries),
}};
