/**
 * @file kol.cpp
 * @author cs.pawelmieszkowski@gmail.com
 * @brief implementation of a hypothetical city_hall
 * @version 0.1
 * @date 2023-12-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "plist.h"
#include "kol.h"
#include <vector>
#include <iostream>


struct interesant
{ 
    int num;
    plib::list<interesant*>::const_iterator it;
};

/**
 * @struct city_hall
 * @brief represent a city hall with queues and a machine giving numbers
 * 
 */
struct city_hall: public std::vector<plib::list<interesant*>>
{
    using std::vector<plib::list<interesant*>>::vector;
    int counter;
};

static thread_local city_hall main_hall;

void otwarcie_urzedu(int m)
{ main_hall.resize(m); }

interesant *nowy_interesant(int k)
{
    interesant* out = (interesant*)malloc(sizeof(interesant));
    out->num = main_hall.counter++;
    out->it = main_hall[k].push_back(out);
    return out;
}

int numerek(interesant *i)
{ return i->num; }

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
    i->it = main_hall[k].push_back(i);
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