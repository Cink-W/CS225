#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H
#include "BP_Node.cpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

const int INVALID_INDEX = -1;

template <typename K, typename D> struct SelectResult {
    int keyIndex;
    LNode<K, D> *targetNode;
};

template <typename K, typename D> class CBPlusTree
{
  public:
    // 构造函数 & 析构函数
    CBPlusTree();
    ~CBPlusTree();

    // 接口
    bool insert(K key, const D data);
    bool remove(K key);
    bool update(K oldKey, K newKey);

    // 查找是否存在
    bool search(K key) const;

    // 获取key对应data的指针
    D *get_data(K key) const;

    // 清空
    void clear();

    // 打印所有key（树的形式）
    void print() const;

    // 打印所有data
    void printData() const;

  private:
    void rec_insert(Node<K, D> *parentNode, K key, const D data);
    void rec_remove(Node<K, D> *parentNode, K key);
    void printInConcavo(Node<K, D> *pNode, int count, ofstream &BP) const;
    bool rec_search(Node<K, D> *pNode, K key) const;
    void rec_search(Node<K, D> *pNode, K key, SelectResult<K, D> &result);
    D *rec_get_data(Node<K, D> *pNode, K key) const;
    void changeKey(Node<K, D> *pNode, K oldKey, K newKey);
    void search(K key, SelectResult<K, D> &result);
    void remove(K key, D &dataValue);
    void rec_remove(Node<K, D> *parentNode, K key, D &dataValue);

  private:
    LNode<K, D> *m_DataHead;
    K m_MaxKey;  // B+树中的最大键
    Node<K, D> *root;
};

#endif
