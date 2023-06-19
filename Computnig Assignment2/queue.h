
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "registry.h"

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Local_Q
{
  public:
    Local_Q(int registry);
    void add(int identification, string name, float *addr, string phone, string wechat, string email, int profession,
             Time *birth, int risk, Time *deadline, Time *timestamp, int registry, int type);
    void set_registry(int reg);
    queue<Registration *> *q;
    int get_registry();
    void set_numitems(int numitems);

    int get_numitems();

  private:
    int numitems;
    int registry;
};

#endif