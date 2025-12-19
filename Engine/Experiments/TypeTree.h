#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "refl.hpp"


struct Type
{
    Type(std::string  name):name{std::move(name)} {}
    std::string name;
    std::vector<Type*> children;
};

class TypeTree
{
public:
    TypeTree(){}

    void PrintTree(Type* type, int indentCount=0);

    template<typename T>
    Type* getType();

    template<typename T>
    void AddType();
private:
    std::unordered_map<std::string,Type> types;

    template<typename T>
    void WalkParents(Type* base, refl::type_descriptor<T> descriptor = refl::reflect<T>());
};

void TypeTree::PrintTree(Type* type, int indentCount)
{
    if (type==nullptr)
    {
        return;
    }

    //Nicer spacing
    for (int i=0;i<indentCount-1;i++)
    {
        std::cout<<'\t';
    }

    if (indentCount>0)
    {
        std::cout<<"L___";
    }

    std::cout<<type->name;

    if (type->children.size()==0)
    {
        std::cout<<"]";
    }
    std::cout<<'\n';

    for (auto it:type->children)
    {
        PrintTree(it,indentCount+1);
    }
}

template<typename T>
Type * TypeTree::getType()
{
    refl::type_descriptor<T> type;
    return &types.at(type.name.str());
}

template<typename T>
void TypeTree::AddType()
{
    //Get name of the type
    refl::type_descriptor<T> type;
    std::string name = type.name.str();
    if (types.contains(name)){return;}

    //Add the type to the tree
    types.insert(std::make_pair(name,Type{name}));

    WalkParents(&types.at(name),type);
}



template<typename T>
void TypeTree::WalkParents(Type* type, refl::type_descriptor<T> descriptor)
{
    refl::util::for_each(refl::util::reflect_types(descriptor.declared_bases),[&](auto x)
    {
        //Find parent type (if doesn't exist, add it)
        auto parent = types.find(x.name.str());
        if (parent==types.end())
        {
            types.insert(std::make_pair(x.name.str(),Type{x.name.str()}));
            parent=types.find(x.name.str());
        }

        bool found=false;
        for (auto it:parent->second.children)
        {
            if (it==type)
            {
                found=true;
                break;
            }
        }
        if (!found)
        {
            parent->second.children.push_back(type);
            WalkParents(&parent->second,x);
            return;
        }
    });
}
