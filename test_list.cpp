#include <iostream>
#include "plist.h"

int main()
{
    plib::list<int> l{1, 2, 4, 5};
    {
        plib::list<int> l2{1, 3, 4};
        std::swap(l, l2);
        l = std::move(l2);
    }
    auto it2 = l.begin();
    plib::list<int>::const_iterator it3 = it2;
    l = {1, 7, 9, 15};
    auto it = l.begin();
    ++it;
    plib::list<int>().erase(it);
    for(int x : l)
        std::cout << x << '\n';
}