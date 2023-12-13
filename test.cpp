#include "plist.h"
#include <iostream>
#include <memory>
#include <list>

using pi = std::pair<int,int>;
int main()
{
    plib::list<pi> l = (std::initializer_list<pi>({{4, 5}, {9, 10}, {15, 18}}));
    plib::list<pi> k({{-1, -2}, {-8, -6}, {-3, -5}});

    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    k.reverse();

    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    auto copy = k.push_back({-100, -5});
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    k.insert(copy, l);
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    k.push_back({5, 7});
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    k.pop_front();
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    k.reverse();
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    k.push_back({1, 15});
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    
    k.push_front({3, 10});
    
    std::cout << "l\n";
    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';

    std::cout << "k\n";
    for(const auto& x : k)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
    

    for(const auto& x : l)
        std::cout << x.first << ' ' << x.second << '\n';
    std::cout << '\n';
        
    for(plib::list<pi>::const_reverse_iterator it = l.rbegin(); it != l.rend(); ++it)
        std::cout << it->first << ' ' << it->second << '\n';
    std::cout << '\n';

    _Exit(0);
}