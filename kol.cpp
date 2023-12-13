#include "plist.h"
#include "kol.h"
#include <vector>

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
{ return (*i).num;}

interesant *obsluz(int k)
{
    auto copy = *main_hall.queues[k].begin();
    main_hall.queues[k].pop_front();
    return copy;
}

void zmiana_okienka(interesant *i, int k)
{

}