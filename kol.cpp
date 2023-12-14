#include "plist.h"
#include "kol.h"
#include <vector>
#include <iostream>

struct interesant
{ 
    size_t num;
    plib::list<interesant*>::const_iterator it;
};

struct city_hall
{
    using size_type = std::size_t;

    size_type counter;
    std::vector<plib::list<interesant*>> queues;

    city_hall() = default;
    city_hall(const city_hall&) = default;
    city_hall(city_hall&&) = default;

    city_hall(size_type size) : counter{0}, queues{size}
    { }

    city_hall& operator=(const city_hall&) = default;
    city_hall& operator=(city_hall&&) = default;

    ~city_hall() = default;
};

static thread_local city_hall main_hall;

void otwarcie_urzedu(int m)
{ main_hall = city_hall(m); }

interesant *nowy_interesant(int k)
{
    interesant* out = (interesant*)malloc(sizeof(interesant));
    out->num = main_hall.counter++;
    out->it = main_hall.queues[k].push_back(out);
    return out;
}

int numerek(interesant *i)
{ return static_cast<int>((*i).num); }

interesant *obsluz(int k)
{
    if(!main_hall.queues[k].empty())
        return main_hall.queues[k].pop_front();
    else
        return nullptr;
}

void zmiana_okienka(interesant *i, int k)
{
    main_hall.queues[k].erase(i->it);
    i->it = main_hall.queues[k].emplace_back(i);
}

void zamkniecie_okienka(int k1, int k2)
{ main_hall.queues[k2].push_back(main_hall.queues[k1]); }

std::vector<interesant *> fast_track(interesant *i1, interesant *i2)
{
    typename plib::list<interesant*>::const_iterator it = plib::list<interesant*>().direct(i1->it, i2->it);
    std::vector<interesant*> out;
    
    while(it != i2->it)
    {
        out.push_back(*it);
        it = plib::list<interesant*>().erase(it);
    }

    out.push_back(*it);
    it = plib::list<interesant*>().erase(it);

    return out;
}

void naczelnik(int k)
{ main_hall.queues[k].reverse(); }

std::vector<interesant *> zamkniecie_urzedu()
{
    std::vector<interesant*> out;
    for(auto& queue : main_hall.queues)
    {
        while(!queue.empty())
            out.push_back(queue.pop_front());
    }
    return out;
}

void print(int callno)
{
    std::cerr << callno << '\n';
    for(int i = 0; i < main_hall.queues.size(); ++i)
    {
        std::cerr << i << " size: " << main_hall.queues[i].size() << ": { ";
        for(auto x : main_hall.queues[i])
            std::cerr << (*x).num << ' ';
        std::cerr << " }\n";
    }
    std::cerr << "\n";
}