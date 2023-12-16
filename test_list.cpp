#include <iostream>
#include "plist.h"

int main()
{
    const plib::list<int> l = {1, 2, 4, 5};
    auto it = l.begin();
    std::cout << it._const_linking << "   ";
    ++it;
    std::cout << it._const_linking << "   ";
    plib::list<int>().erase(it);
    for(int x : l)
        std::cout << x << '\n';
}