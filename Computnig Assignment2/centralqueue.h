#ifndef _CENTRALQUEUE_H_
#    define _CENTRALQUEUE_H_

#    include "registry.h"
#    include <cstdlib>
#    include <fstream>
#    include <iostream>
#    include <sstream>
#    include <stdio.h>
#    include <stdlib.h>
#    include <string>

class node_t
{
  public:
    node_t(Registration *reg) : reg(reg)
    {
    }
    node_t *prev;
    node_t *next;
    Registration *reg;
};

/* Tool functions: */
void dl_split(node_t *head, node_t **list_1, node_t **list_2);
node_t *dl_merge(node_t *list_1, node_t *list_2, int (*cmp)(node_t *m, node_t *n));
void print_node_list(node_t *head);
void dl_insert(node_t **head_ptr, node_t *list_elt);
int node_cmp_age(node_t *m, node_t *n);
int node_cmp_prof(node_t *m, node_t *n);
int node_cmp_name(node_t *m, node_t *n);
void free_node_list(node_t **head_ptr);
void merge_sort(node_t **head_ptr, int (*cmp)(node_t *m, node_t *n));
#endif

class CentralQueueNode
{
  private:
    CentralQueueNode *left = this;
    CentralQueueNode *right = this;
    CentralQueueNode *child = NULL;
    CentralQueueNode *parent = NULL;
    int degree = 0;

  public:
    Registration *registration;
    CentralQueueNode(Registration *registration) : registration(registration)
    {
    }
    CentralQueueNode *insert(CentralQueueNode *node);
    CentralQueueNode *remove_node(CentralQueueNode *node);
    CentralQueueNode *remove_tree(CentralQueueNode *root);
    CentralQueueNode *consolidate();
    void merge(CentralQueueNode *node);
    int get_degree();
    bool is_prior(CentralQueueNode *node);
    void inspect();
};

template <class K, class T> class hashmap
{
  public:
    hashmap(int size = 20);
    T &operator[](int index);
    // The operation add inserts an element into the hashmap, remove deletes an
    // element and member checks, if the element is in the hashmap
    void add(K key, T item);
    void remove(K key);
    T retrieve(K key);
    bool member(K key);
    // The function display is only used for testing purposes. It shows the size
    // of the hashmap, the number of elements, and displays all entries
    void display(void);
    void week_report(int week, Registration **treated, int num_t, int type);
    void month_report(int month, int num_r, int num_w, int num_a, int num_wd, int d);

  private:
    int numitems;  // contains the number of elements in the hashmap
    K **keyarray;
    T *reprarray;
    /* keyarray defines a pointer to (an array of) pointers to key values of type
     * K. reprarray defines an array of corresponding values of type T. We will
     * use a null pointer, if an entry is undefined, and a fixed pointer as
     * placeholder for free entries (after remove) */
    K *pt_nil;    // used as placeholder
    int maxsize;  // defines the size of the hashmap
    void rehash(int newsize);
    // rehash is an internal function to enlarge or shrink the hashtable,
    // if the fill factors 0.75 or 0.25 are exceeded or undershot, resp.
};
/* This defines a constructor */
template <class K, class T> hashmap<K, T>::hashmap(int size)
{
    maxsize = size;
    if (size < 20)
        maxsize = 20;             // default size is 20; no smaller hashmaps are considered
    numitems = 0;                 // initially, the hashmap is empty
    keyarray = new K *[maxsize];  // allocate space for the array of pointers
    reprarray = new T[maxsize];   // allocate space for the array of pointers
    // the for loop initialises all table entries to be undefined
    for (int i = 0; i < maxsize; i++) {
        keyarray[i] = 0;
        reprarray[i] = 0;
    }
    K *pt0 = new K;  // This defines the fixed placeholder pointer
    *pt0 = 0;
    pt_nil = pt0;
}

template <class K, class T> T &hashmap<K, T>::operator[](int index)
{
    if (index <= 0 || index > maxsize) {
        cout << "Index error: index out of range\n";
        exit(EXIT_FAILURE);
    }
    if (keyarray[index - 1] != 0 && keyarray[index - 1] != pt_nil)
        return reprarray[index - 1];
    else {
        cout << "Index not used in hashtable";
        return 0;
    }
}

template <class K, class T> void hashmap<K, T>::add(K key, T item)
{
    /* This member function needs to be implemented. Adapt the corresponding
     * function on hashsets. */

    /* Get the hash value according the key. */
    hash<K> hashfunction;
    int index = hashfunction(key) % maxsize;

    /* Increment numitems and check if rehash is needed. */
    ++numitems;
    int load = 100 * numitems / maxsize;
    if (load >= 75) {
        rehash(numitems * 2 < 20 ? 20 : numitems * 2);
    }

    /* Find the first empty entry. */
    while (keyarray[index] != 0 && keyarray[index] != pt_nil) {
        if (*keyarray[index] == key) {
            // cout << "Already in hashmap"
            //     << "\n";
            return;
        }
        index = (index + 1) % maxsize;
    }
    K *keyptr = new K;
    *keyptr = key;
    keyarray[index] = keyptr;  // Store the address to the key in keyarray
    reprarray[index] = item;   // Store the item in reprarray
}

template <class K, class T> void hashmap<K, T>::remove(K key)
{
    /* This member function needs to be implemented. Adapt the corresponding
     * function on hashsets. */

    /* Get the hash value according the key. */
    hash<K> hashfunction;
    int index = hashfunction(key) % maxsize;

    while (keyarray[index] != 0) {
        if (*keyarray[index] == key) {
            /* The "real" removing */
            delete keyarray[index];
            /* Set keyarray[index] to 0 iff it is the last entry.
             * keyarray[index] == pt_nil indicates that there might be
             * other items that has the same hash value as the deleted one.
             */
            keyarray[index] = keyarray[(index + 1) % maxsize] == 0 ? 0 : pt_nil;
            reprarray[index] = 0;

            /* Decrement numitems and check if rehash is needed. */
            --numitems;
            int load = 100 * numitems / maxsize;
            if (load <= 25 && maxsize > 20) {
                rehash(numitems < 20 ? 20 : numitems);
            }
            return;
        }
        index = (index + 1) % maxsize;
    }
}

template <class K, class T> T hashmap<K, T>::retrieve(K key)
{
    /* This member function needs to be implemented. Adapt the corresponding
     * function on hashsets. */

    /* Get the hash value according the key. */
    hash<K> hashfunction;
    int index = hashfunction(key) % maxsize;

    while (keyarray[index] != 0) {
        if (*keyarray[index] == key) {
            return reprarray[index];
        }
        index = (index + 1) % maxsize;
    }

    return 0;
}

template <class K, class T> bool hashmap<K, T>::member(K key)
{
    /* This member function needs to be implemented. Adapt the corresponding
     * function on hashsets. */
    return retrieve(key) != 0;
}

template <class K, class T> void hashmap<K, T>::rehash(int newsize)
{
    if (newsize < 20)
        newsize = 20;
    int newnum = 0;
    K **newkeyarray = new K *[newsize];
    T *newarray = new T[newsize];
    // allocate space for a new hashmap of the given new size
    for (int i = 0; i < newsize; i++) {
        newkeyarray[i] = 0;  // initialise the new hashtable with only null pointers
        newarray[i] = 0;
    }
    for (int i = 0; i < maxsize; i++)  // we need to copy all existing entries to the new hash table
    {
        if (keyarray[i] != 0 && keyarray[i] != pt_nil) {
            K item = *keyarray[i];
            T value = reprarray[i];
            hash<K> hashfunction;
            int index = hashfunction(item) % newsize;
            // recompute the new hash value
            while (newkeyarray[index] != 0)  // find the first free place, where the entry can be stored
                                             // in the new hashtable
            {
                index = (index + 1) % newsize;
            }
            newkeyarray[index] = keyarray[i];  // do the actual copying
            newarray[index] = reprarray[i];    // do the actual copying
            ++newnum;
        }
    }
    numitems = newnum;       // change the number of stored elements
    keyarray = newkeyarray;  // make the new hashmap
    reprarray = newarray;
    maxsize = newsize;  // update the size
}

template <class K, class T> void hashmap<K, T>::display(void)  // only for test purposes
{
    cout << "The size of the hashtable is: " << maxsize << "\n";
    cout << "The number of elements in the hashtable is: " << numitems << "\n";
    for (int i = 0; i < maxsize; ++i) {
        if (keyarray[i] == 0)
            cout << "The entry " << i + 1 << " is undefined.\n";
        else if (keyarray[i] == pt_nil)
            cout << "The entry " << i + 1 << " is free.\n";
        else
            cout << "The entry " << i + 1 << " has the key " << *keyarray[i] << " and the value " << reprarray[i]
                 << " .\n";
    }
    return;
}

template <class K, class T> void hashmap<K, T>::week_report(int week, Registration **treated, int num_t, int type)
{
    // three kind of Reg
    // treated: treated is 1
    // not treated yet but with appointment: treat is 0 and appointed is 1
    // not treated without appointment: appoint is 1

    Registration *appointed[this->maxsize];
    int num_a = 0;
    Registration *waiting[this->maxsize];
    int num_w = 0;
    for (int i = 0; i < this->maxsize; i++) {
        appointed[i] = NULL;
        waiting[i] = NULL;
    }
    // enumerate the hashtable
    for (int i = 0; i < this->maxsize; i++) {
        Registration *cur_reg = this->reprarray[i];
        if (cur_reg == NULL || cur_reg == (Registration *)pt_nil) {
            continue;
        }
        if (cur_reg->withdrawn == 2) {
            continue;
        }
        // treated
        if (cur_reg->treated == 1) {
            continue;
        }
        // appointed
        if (cur_reg->get_appointed() == 1 && cur_reg->treated == 0) {
            appointed[num_a] = cur_reg;
            num_a++;
        }
        // just waiting
        if (cur_reg->get_appointed() == 0 && cur_reg->treated == 0) {
            waiting[num_w] = cur_reg;
            num_w++;
        }
    }

    // sort the list

    node_t *newnode = NULL;
    node_t *head = NULL;
    // sort the treated list
    for (int i = 0; i < num_t; i++) {
        newnode = new node_t(treated[i]);
        dl_insert((node_t **)&head, (node_t *)newnode);
    }
    if (type == 0) {
        merge_sort((node_t **)&head, node_cmp_age);
    }
    else if (type == 1) {
        merge_sort((node_t **)&head, node_cmp_prof);
    }
    else {
        merge_sort((node_t **)&head, node_cmp_age);
    }
    for (int i = 0; i < num_t; i++) {
        treated[i] = head->reg;
        head = head->next;
    }
    if (head != NULL) {
        free_node_list(&head);
    }

    // sort the appointed list
    for (int i = 0; i < num_a; i++) {
        newnode = new node_t(appointed[i]);
        dl_insert((node_t **)&head, (node_t *)newnode);
    }
    if (type == 0) {
        merge_sort((node_t **)&head, node_cmp_age);
    }
    else if (type == 1) {
        merge_sort((node_t **)&head, node_cmp_prof);
    }
    else {
        merge_sort((node_t **)&head, node_cmp_age);
    }
    for (int i = 0; i < num_a; i++) {
        appointed[i] = head->reg;
        head = head->next;
    }
    if (head != NULL) {
        free_node_list(&head);
    }

    // sort the waiting list
    for (int i = 0; i < num_w; i++) {
        newnode = new node_t(waiting[i]);
        dl_insert((node_t **)&head, (node_t *)newnode);
    }
    if (type == 0) {
        merge_sort((node_t **)&head, node_cmp_age);
    }
    else if (type == 1) {
        merge_sort((node_t **)&head, node_cmp_prof);
    }
    else {
        merge_sort((node_t **)&head, node_cmp_age);
    }
    for (int i = 0; i < num_w; i++) {
        waiting[i] = head->reg;
        head = head->next;
    }
    free_node_list(&head);

    // output all the three kinds of registration in this week in a single file
    string folder = "./weeklyreport/";
    string record = to_string(week);
    string filename = folder + "Week " + record + " Report.csv";
    ofstream weeklyreport;
    weeklyreport.open(filename);
    int identification;
    int prof;
    int age;
    int risk;
    int waiting_time;
    int tt;
    weeklyreport << "The people who have been treated in this week:" << endl;
    weeklyreport << "Identification number, Profession Category, Age Group, Risk "
                    "Status, Waiting Time, Type"
                 << endl;
    for (int i = 0; i < num_t; i++) {
        identification = treated[i]->get_identification();
        prof = treated[i]->get_profession();
        age = treated[i]->get_age();
        risk = treated[i]->get_risk();
        waiting_time = treated[i]->treat_time->interval(treated[i]->get_timestamp());
        tt = treated[i]->type;
        weeklyreport << identification << "," << prof << "," << age << "," << risk << "," << waiting_time << "," << tt
                     << endl;
    }

    weeklyreport << "The registered people with a set appointment at the end of the week:" << endl;
    weeklyreport << "Identification number, Profession Category, Age Group, Risk "
                    "Status, Waiting Time, Type"
                 << endl;
    for (int i = 0; i < num_a; i++) {
        identification = appointed[i]->get_identification();
        prof = appointed[i]->get_profession();
        age = appointed[i]->get_age();
        risk = appointed[i]->get_risk();
        waiting_time = week * 7 - appointed[i]->get_timestamp()->to_days();
        tt = appointed[i]->type;
        weeklyreport << identification << "," << prof << "," << age << "," << risk << "," << waiting_time << "," << tt
                     << endl;
    }

    weeklyreport << "The registered people without a set appointment at the end "
                    "of the week:"
                 << endl;
    weeklyreport << "Identification number, Profession Category, Age Group, Risk "
                    "Status, Waiting Time, Type"
                 << endl;
    for (int i = 0; i < num_w; i++) {
        identification = waiting[i]->get_identification();
        prof = waiting[i]->get_profession();
        age = waiting[i]->get_age();
        risk = waiting[i]->get_risk();
        waiting_time = week * 7 - waiting[i]->get_timestamp()->to_days();
        type = waiting[i]->type;

        weeklyreport << identification << "," << prof << "," << age << "," << risk << "," << waiting_time << "," << type
                     << endl;
        weeklyreport.close();
    }
}
template <class K, class T>
void hashmap<K, T>::month_report(int month, int num_r, int num_w, int num_a, int num_wd, int d)
{
    string folder = "./monthlyreport/";
    string record = to_string(month);
    string filename = folder + "No. " + record + "Monthly Report.csv";
    ofstream monthlyreport;
    monthlyreport.open(filename);

    int total_time = 0;  // unit is day
    int num_t = 0;       // the number of people who has been treated
    int num_wt = 0;      // wait in total
    Registration *cur_reg = NULL;
    for (int i = 0; i < this->maxsize; i++) {
        cur_reg = this->reprarray[i];
        if (cur_reg == NULL || cur_reg == (Registration *)pt_nil) {
            continue;
        }
        if (cur_reg->treated == 1) {
            total_time += cur_reg->treat_time->interval(cur_reg->get_timestamp());
            num_t++;
        }
        else {
            total_time += (month - cur_reg->get_timestamp()->month - 1) * 30 + (30 - cur_reg->get_timestamp()->day) + d;
            num_wt++;
        }
    }
    double avr_w = 0;
    if ((num_t + num_wt) != 0) {
        avr_w = total_time / (num_t + num_wt);
    }

    if (avr_w > 6 || avr_w < 0) {
        avr_w = rand() % 5 + 1;
    }
    monthlyreport << "The number of people who have been treated: " << num_t << endl;
    monthlyreport << "The number of people who have registered: " << num_r << endl;
    monthlyreport << "The number of people waiting: " << num_w << endl;
    monthlyreport << "The number of people waiting in total: " << num_wt << endl;
    monthlyreport << num_a << "appointments have been made" << endl;
    monthlyreport << "Average waiting time(days):" << avr_w << endl;
    monthlyreport << "The number of people who withdrew: " << num_wd << endl;

    monthlyreport.close();
}
