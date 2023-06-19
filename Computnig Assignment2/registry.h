#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#define YEAR 2022
using namespace std;

class Time
{
  public:
    Time(int year, int month, int day);  // format: yyyy/mm/dd such as 20220326
    Time(int year, int month, int day, int hour, int minute);
    ~Time();
    int interval(Time *start);
    int to_days();
    bool earlier(Time *a);  // return 1 if a is earlier that b

    int year;
    int month;
    int day;
    int hour;
    int minute;
};

class Registration
{
  public:
    Registration();
    Registration(int identification, string name, float *addr, string phone, string wechat, string email,
                 int profession, Time *birth, int risk, Time *deadline, Time *timestamp, int type);
    ~Registration();

    bool operator>(Registration *to_compare);
    bool operator<(Registration *to_compare);
    bool operator>=(Registration *to_compare);
    bool operator<=(Registration *to_compare);
    bool operator==(Registration *to_compare);
    /* bool operator!=(Registration *to_compare);*/

    int set_timestamp(Time *timestamp);
    void set_id(int numitem, int registry);
    int set_name(string name);
    int set_addr(float *addr);
    int set_phone(string phone);
    int set_wechat(string wechat);
    int set_email(string email);
    int set_profession(int prof);
    int set_birth(Time *birth);
    int set_risk(int risk);
    int set_deadline(Time *deadline);
    void set_identification(int identification);
    void set_age(Time *birth);

    Time *get_timestamp();
    int get_id();
    string get_name();
    float *get_addr();
    string get_phone();
    string get_wechat();
    string get_email();
    int get_profession();
    int get_age();
    int get_birth();  // return the year of birth
    int get_risk();
    Time *get_deadline();
    int get_identification();
    int get_appointed();
    void appoint();                        // set it as appointed
    Time *treat_time = new Time(0, 0, 0);  // set it as treated with time
    int withdrawn;                         // 0 is not withdrawn, 1 is was once withdrawn, 2 is just have been withdrawn
    int waittime;
    int treated;  // 1 for done
    int place;    // 0 for new registration, 1 is in wait list, 2 is in centralqueue.
    int type;

  private:
    /* information provided by registry */
    Time *timestamp;  // the date of registration
    int id;           // a unique number of each regisration, we assign the first 3 numbers
                      // to identify the registry and the last 8 numbers for each registry
                      // this should be enough to creat 2 registration for each of the 5
                      // billion people

    /* personal information that is almosst useless */
    int identification;  // idenfication number of a person
    string name;
    string phone;
    string wechat;
    string email;

    /* personal information that is useful */
    float addr[2];
    int profession;  // from 1 to 8
    Time *birth;     // should be later than 1900 earlier than current time
    int risk;        // 0,1,2,3
    int age;         // age group number
    Time *deadline;  // priority by letter with deadline, no later than now

    // current status
    int appointed;  // 0 for not
};

#endif
