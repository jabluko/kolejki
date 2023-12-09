#include "plist.h"
#include <iostream>
#include <memory>

using pi = std::pair<int,int>;
int main()
{
	plib::list<pi>::iterator a;
    plib::list<pi> l;
    l.push_back({1, 2});
    l.push_front({2, 3});
    plib::list<pi>::iterator bgn = l.begin();
    std::cout << (bgn->first);
}