#include "registry.h"

using namespace std;
using std ::cin;
using std ::cout;
using std ::endl;

Time::Time(int year, int month, int day)
{
    if ((year + month + day == 0) || (year > 1800 && year < 2100 && month > 0 && month < 13 && day > 0 && day < 32)) {
        this->year = year;
        this->month = month;
        this->day = day;
        this->hour = -1;
        this->minute = -1;
    }

    else {
        cout << "Wrong time format!" << endl;
    }
}

Time::Time(int year, int month, int day, int hour, int minute)
{
    if (year > 1800 && year < 2100 && month > 0 && month < 13 && day > 0 && day < 32 && hour > -1 && hour < 25 &&
        minute > -1 && minute < 61) {
        this->year = year;
        this->month = month;
        this->day = day;
        this->hour = hour;
        this->minute = minute;
    }

    else {
        cout << "Wrong time format!" << endl;
    }
}
Time::~Time()
{
    return;
}

// to judge is this earlier than a
bool Time::earlier(Time *a)
{
    if (this->year < a->year) {
        return true;
    }
    else if (this->year > a->year) {
        return false;
    }
    if (this->month < a->month) {
        return true;
    }
    else if (this->month > a->month) {
        return false;
    }
    if (this->day < a->day) {
        return true;
    }
    return false;
}
// to calculate the time interval between two date in day
int Time::interval(Time *start)
{
    int total = (this->month - start->month - 1) * 30 + (30 - start->day) + this->day;
    return total;
}
// to calculate how much days has past in this year
int Time::to_days()
{
    int days = (this->month - 1) * 30 + this->day;
    return days;
}
// REGESTRATION

bool Registration::operator>(Registration *to_compare)
{
    if (id > to_compare->get_id()) {
        return 1;
    }
    else {
        return 0;
    }
}

bool Registration::operator<(Registration *to_compare)
{
    if (id < to_compare->get_id()) {
        return 1;
    }
    else {
        return 0;
    }
}

bool Registration::operator>=(Registration *to_compare)
{
    if (id >= to_compare->get_id()) {
        return 1;
    }
    else {
        return 0;
    }
}

bool Registration::operator<=(Registration *to_compare)
{
    if (id <= to_compare->get_id()) {
        return 1;
    }
    else {
        return 0;
    }
}
/*
bool Registration::operator==(Registration *to_compare)
{
    if (id == to_compare->get_id()) {
        return 1;
    }
    else {
        return 0;
    }
}
*/
int Registration::set_timestamp(Time *timestamp)
{
    this->timestamp = new Time(timestamp->year, timestamp->month, timestamp->day, timestamp->hour, timestamp->minute);

    return 1;
}

void Registration::set_identification(int identification)
{
    this->identification = identification;
}

int Registration::set_name(string name)
{
    this->name = name;
    return 1;
}

int Registration::set_addr(float *address)
{
    this->addr[0] = address[0];
    this->addr[1] = address[1];
    return 1;
}

int Registration::set_profession(int prof)
{
    this->profession = prof;
    return 1;
}

int Registration::set_birth(Time *birth)
{
    this->birth = new Time(birth->year, birth->month, birth->day);
    set_age(birth);
    return 1;
}

int Registration::set_risk(int risk)
{
    this->risk = risk;
    return 1;
}

int Registration::set_phone(string phone)
{
    this->phone = phone;
    return 1;
}

int Registration::set_wechat(string wechat)
{
    this->wechat = wechat;
    return 1;
}

int Registration::set_email(string email)
{
    this->email = email;
    return 1;
}

int Registration::set_deadline(Time *deadline)
{
    this->deadline = new Time(deadline->year, deadline->month, deadline->day);
    return 1;
}
Time *Registration::get_timestamp()
{
    return this->timestamp;
}
int Registration::get_id()
{
    return this->id;
}
string Registration::get_name()
{
    return this->name;
}

float *Registration::get_addr()
{
    return &(this->addr[0]);
}  // there may be a problem ?????????????????????????????????????????

string Registration::get_phone()
{
    return this->phone;
}

string Registration::get_wechat()
{
    return this->wechat;
}

string Registration::get_email()
{
    return this->email;
}

int Registration::get_profession()
{
    return this->profession;
}

int Registration::get_age()
{
    return this->age;
}

int Registration::get_risk()
{
    return this->risk;
}

Time *Registration::get_deadline()
{
    return this->deadline;
}
int Registration::get_identification()
{
    return this->identification;
}

int Registration::get_appointed()
{
    return this->appointed;
}
void Registration::set_age(Time *birth)
{
    int age = YEAR - birth->year;
    if (age <= 12) {
        this->age = 0;
    }
    else if (age <= 18) {
        this->age = 1;
    }
    else if (age <= 35) {
        this->age = 2;
    }
    else if (age <= 50) {
        this->age = 3;
    }
    else if (age <= 65) {
        this->age = 4;
    }
    else if (age <= 75) {
        this->age = 5;
    }
    else {
        this->age = 6;
    }
}
int Registration::get_birth()
{
    return this->birth->year;
}
void Registration::set_id(int numitem, int registry)
{
    int b = (1000000 - 1) / 9;
    if (numitem <= b) {
        this->id = (registry * 1000000) + numitem;
    }
    else {
        cout << "too many records" << endl;
    }
}
Registration::Registration()
{
    return;
}
Registration::Registration(int identification, string name, float *addr, string phone, string wechat, string email,
                           int profession, Time *birth, int risk, Time *deadline, Time *timestamp, int type)
{
    set_identification(identification);
    set_name(name);
    set_addr(addr);

    set_phone(phone);
    set_wechat(wechat);
    set_email(email);

    set_profession(profession);
    set_birth(birth);
    set_risk(risk);
    set_timestamp(timestamp);
    set_deadline(deadline);
    this->waittime = 0;
    this->appointed = 0;
    this->treated = 0;
    this->treat_time = new Time(0, 0, 0);
    this->withdrawn = 0;
    this->type = type;
}
Registration::~Registration()
{
    delete this->birth;
    delete this->deadline;
    delete this->treat_time;
    delete this->timestamp;
}
