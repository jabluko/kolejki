#include "plist.h"
#include "kol.h"
#include <vector>
#include <iostream>

struct interesant
{ 
    size_t num;
    plib::list<interesant*>::const_iterator it;
};

struct city_hall: public std::vector<plib::list<interesant*>>
{
    using std::vector<plib::list<interesant*>>::vector;
    using size_type = std::size_t;

    size_type counter;
};

static thread_local city_hall main_hall;

void otwarcie_urzedu(int m)
{ main_hall = city_hall(m); }

interesant *nowy_interesant(int k)
{
    interesant* out = (interesant*)malloc(sizeof(interesant));
    out->num = main_hall.counter++;
    out->it = main_hall[k].push_back(out);
    return out;
}

int numerek(interesant *i)
{ return static_cast<int>((*i).num); }

interesant *obsluz(int k)
{
    if(!main_hall[k].empty())
        return main_hall[k].pop_front();
    else
        return nullptr;
}

void zmiana_okienka(interesant *i, int k)
{
    plib::list<interesant*>().erase(i->it);
    i->it = main_hall[k].emplace_back(i);
}

void zamkniecie_okienka(int k1, int k2)
{ main_hall[k2].merge_back(main_hall[k1]); }

std::vector<interesant *> fast_track(interesant *i1, interesant *i2)
{
    auto it = direct(i1->it, i2->it);
    std::vector<interesant*> out;
    
    while(it != i2->it)
    {
        out.push_back(*it);
        it = plib::list<interesant*>().erase(it);
    }

    out.push_back(i2);
    plib::list<interesant*>().erase(i2->it);

    return out;
}

void naczelnik(int k)
{ main_hall[k].reverse(); }

std::vector<interesant *> zamkniecie_urzedu()
{
    std::vector<interesant*> out;
    for(auto& queue : main_hall)
        while(!queue.empty())
            out.push_back(queue.pop_front());

    return out;
}