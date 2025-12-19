#include <EntryPoint/Application.h>
#include <iostream>

#include <refl.hpp>
#include <vector>

#include "Experiments/TypeTree.h"

class A{};
REFL_AUTO(type(A))
class B:A{};
REFL_AUTO(type(B,bases<A>))
class C:A{};
REFL_AUTO(type(C,bases<A>))
class D:B{};
REFL_AUTO(type(D,bases<B>))
class E:B{};
REFL_AUTO(type(E,bases<B>))
class F:C{};
REFL_AUTO(type(F,bases<C>))
class G:E{};
REFL_AUTO(type(G,bases<E>))
class H:E{};
REFL_AUTO(type(H,bases<E>))
class I:H{};
REFL_AUTO(type(I,bases<H>))

void Engine::Application::Initialize()
{
    std::cout<<"Hello, World!\n";

    TypeTree typeTree;

    typeTree.AddType<D>();
    typeTree.AddType<F>();
    typeTree.AddType<G>();
    typeTree.AddType<I>();

    Type* type = typeTree.getType<A>();

    typeTree.PrintTree(type);

}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

