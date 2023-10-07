#pragma once
#include "dynamicArray.hpp"
#include "qstructType.hpp"

struct Entry
{
    std::string title {};
    std::string status {}; // TODO: Create enum

    static const QStructType staticType;
};

//TODO: make generate by Header Tool
const QStructType Entry::staticType{"Entry",sizeof(Entry),{
        GEN_QSTRUCT_FIELD_ENTRY(Entry,title),
        GEN_QSTRUCT_FIELD_ENTRY(Entry,status),
}};
