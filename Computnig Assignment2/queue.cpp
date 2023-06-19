#include "queue.h"

#include <queue>

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;
using std ::cin;
using std ::cout;
using std ::endl;

// add new point in local queue
Local_Q::Local_Q(int registry)
{
    q = new queue<Registration *>;
    set_numitems(0);
    set_registry(registry);
    return;
}

void Local_Q::set_registry(int reg)
{
    this->registry = reg;
}

int Local_Q::get_registry()
{
    return this->registry;
}
void Local_Q::set_numitems(int numitems)
{
    this->numitems = numitems;
}

int Local_Q::get_numitems()
{
    return this->numitems;
}

void Local_Q::add(int identification, string name, float *addr, string phone, string wechat, string email,
                  int profession, Time *birth, int risk, Time *deadline, Time *timestamp, int registry, int type)
{
    Registration *A = new Registration(identification, name, addr, phone, wechat, email, profession, birth, risk,
                                       deadline, timestamp, type);
    // consider if it is empty?
    this->q->push(A);
    this->set_numitems(this->get_numitems() + 1);
    set_registry(registry);
    A->set_id(numitems, registry);

    return;
}
