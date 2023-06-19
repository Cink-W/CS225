//
//  alist.h
//
//
//  Created by KD on 14.01.21.
//  Modified by KD on 22.01.21
//

#ifndef alist_h
#define alist_h

#include "time.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
using std::cin;
using std::cout;
using std::rand;
using std::srand;

template <class T> class AList
{
  public:
    AList(int size = 20);
    virtual ~AList();
    T &operator[](int index);
    int getlength(void);
    void setitem(int index, T value);
    T getitem(int index);
    void append(T value);
    void insert(int index, T value);
    T remove(int index);
    void concat(AList<T> &list);
    int getidx(T value);
    int findidx(T value);
    bool equal(AList<T> &list);
    bool sublist(AList<T> &list);
    void delete_last(int k);
    int get_num();

  private:
    int numitems;
    T *reprarray;
    int maxsize, minsize;
    void allocate(void);
    void deallocate(void);
};

template <class T> AList<T>::AList(int size)
{
    maxsize = size;
    if (size < 20)
        minsize = size;
    else
        minsize = 20;
    numitems = 0;
    reprarray = new T[maxsize];
}

template <class T> AList<T>::~AList()
{
    delete[] reprarray;
}

template <class T> T &AList<T>::operator[](int index)
{
    return reprarray[index];
}

template <class T> int AList<T>::getlength(void)
{
    return numitems;
}

template <class T> void AList<T>::setitem(int index, T value)
{
    if ((index >= 0) && (index < numitems)) {
        reprarray[index] = value;
        return;
    }
    else
        cout << "Index error: index " << index << " out of range in setitem()\n";
}
template <class T> int AList<T>::get_num()
{
    return numitems;
}
template <class T> T AList<T>::getitem(int index)
{
    if ((index >= 0) && (index < numitems)) {
        return reprarray[index];
    }
    else {
        cout << "Index error: index " << index << " out of range in getitem()\n";
        exit(EXIT_FAILURE);
    }
}

template <class T> void AList<T>::append(T value)
{
    if (numitems == maxsize)
        allocate();
    reprarray[numitems] = value;
    ++numitems;
    return;
}

template <class T> void AList<T>::insert(int index, T value)
{
    if (numitems == maxsize)
        allocate();
    if (index < numitems) {
        for (int j = numitems - 1; j >= index; --j) {
            reprarray[j + 1] = reprarray[j];
        }
        reprarray[index] = value;
        ++numitems;
        return;
    }
    else {
        append(value);
        return;
    }
}

template <class T> void AList<T>::allocate(void)
{
    int newsize = 2 * maxsize;
    T *newarray = new T[newsize];
    for (int i = 0; i < numitems; ++i) {
        newarray[i] = reprarray[i];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    return;
}

template <class T> T AList<T>::remove(int index)
{
    if ((numitems == maxsize / 4) && (maxsize > minsize))
        deallocate();
    if (index < numitems) {
        T value = reprarray[index];
        for (int j = index + 1; j < numitems; ++j) {
            reprarray[j - 1] = reprarray[j];
        }
        --numitems;
        return value;
    }
    else
        cout << "Index error: list index "
             << " out of range in remove()\n";
    return 0;
}

template <class T> void AList<T>::deallocate(void)
{
    int newsize = maxsize / 2;
    T *newarray = new T[newsize];
    for (int i = 0; i < numitems; ++i) {
        newarray[i] = reprarray[i];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    return;
}

template <class T> void AList<T>::concat(AList<T> &list)
{
    int length = list.getlength();
    for (int i = 0; i < length; ++i) {
        append(list[i]);
    }
    return;
}

template <class T> int AList<T>::getidx(T value)
{
    int result = -1;
    for (int i = 0; i < numitems; ++i) {
        if (reprarray[i] == value) {
            result = i;
            break;
        }
    }
    return result;
}

template <class T> int AList<T>::findidx(T value)
{
    int i = 0;
    for (; i < numitems; ++i) {
        if (reprarray[i] >= value) {
            return i;
        }
    }
    return i;
}

template <class T> bool AList<T>::equal(AList<T> &list)
{
    bool result = true;
    if (numitems == list.getlength()) {
        for (int i = 0; i < numitems; ++i) {
            if (reprarray[i] == list[i + 1])
                continue;
            else {
                result = false;
                break;
            }
        }
    }
    else
        result = false;
    return result;
}

template <class T> bool AList<T>::sublist(AList<T> &list)
{
    int length = list.getlength();
    bool result = true;
    for (int i = 1, j = 0; (i <= length) && (j < numitems); ++i, ++j) {
        if (list[i] == reprarray[j]) {
            if ((j == numitems - 1) && (i < length)) {
                result = false;
                break;
            }
        }
        else if (j == numitems - 1) {
            result = false;
            break;
        }
        else
            --i;
    }
    return result;
}

template <class T> void AList<T>::delete_last(int k)
{
    // this member function needs to be implemented
    if (k < 0) {
        cout << "Number to be deleted must not be negative." << std::endl;
        return;
    }
    numitems -= k;
    if (numitems < 0) {
        cout << "Number to be deleted larger than the length of the list, "
             << "delete all elements." << std::endl;
        numitems = 0;
    }
    while (numitems <= maxsize / 4 && maxsize > minsize) {
        deallocate();
    }
}

#endif /* alist_h */
