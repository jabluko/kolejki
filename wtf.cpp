#include <list>
struct A
{
    int xd;
};
struct B: A
{
    int xdd;
};
int main()
{
    std::list<int> l{1, 4, 2, 7, 0};
    l.erase(l.begin());
}