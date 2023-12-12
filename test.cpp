#include "plist.h"
#include <iostream>
#include <memory>
#include <list>

using pi = std::pair<int,int>;
int main()
{
    plib::list<pi> l = (std::initializer_list<pi>({{4, 5}, {9, 10}, {15, 18}}));
    l.push_back({1, 2});
    l.push_front({2, 3});

    l.reverse();

    l.pop_back();

    l.push_back({0, -1});
    l.push_front({-10, -8});

    l.reverse();

    l.pop_front();

    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
        
    for(plib::list<pi>::const_reverse_iterator it = l.rbegin(); it != l.rend(); ++it)
        std::cout << it->first << ' ' << it->second << '\n';

    _Exit(0);
}