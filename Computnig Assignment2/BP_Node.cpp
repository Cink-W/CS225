#include "BP_Node.h"
#include <iostream>
using namespace std;

template <typename K, typename D> Node<K, D>::Node()
{
    set_type(1);
    set_num(0);
}

template <typename K, typename D> Node<K, D>::~Node()
{
    set_num(0);
}

template <typename K, typename D> void Node<K, D>::set_type(int type)
{
    this->type = type;
}

template <typename K, typename D> int Node<K, D>::get_type() const
{
    return type;
}
template <typename K, typename D> int Node<K, D>::get_num() const
{
    return key_num;
}

template <typename K, typename D> void Node<K, D>::set_num(int num)
{
    key_num = num;
}

template <typename K, typename D> void Node<K, D>::set_key(int i, K new_value)
{
    key_value[i] = new_value;
}

template <typename K, typename D> K Node<K, D>::get_key(int i) const
{
    return key_value[i];
}

template <typename K, typename D> int Node<K, D>::get_key_index(K key) const
{
    if (get_num() == 0)
        return 0;

    int index = this->get_num() - 1;
    for (int i = 0; i < this->get_num(); i++) {
        if (this->key_value[i] >= key) {
            index = i;
            break;
        }
    }
    return index;
}

// INode
template <typename K, typename D> INode<K, D>::INode()
{
    this->set_type(0);
}

template <typename K, typename D> INode<K, D>::~INode()
{
}

template <typename K, typename D> Node<K, D> *INode<K, D>::get_child(int i) const
{
    return children[i];
}

template <typename K, typename D> void INode<K, D>::set_child(int i, Node<K, D> *child)
{
    children[i] = child;
}

template <typename K, typename D> void INode<K, D>::insert(int key_index, int child_index, K key, Node<K, D> *child)
{
    int i;
    for (i = this->get_num(); i > key_index; --i)  //将父节点中的child_index后的所有关键字的值和子树指针向后移一位
    {
        set_child(i + 1, children[i]);
        this->set_key(i, this->key_value[i - 1]);
    }
    if (i == child_index) {
        set_child(i + 1, children[i]);
    }
    set_child(child_index, child);
    this->set_key(key_index, key);
    this->set_num(this->key_num + 1);
}

template <typename K, typename D> void INode<K, D>::clear()
{
    for (int i = 0; i <= this->key_num; ++i) {
        if (!children[i]) {
            children[i]->clear();
            delete children[i];
            children[i] = NULL;
        }
    }
}

template <typename K, typename D> void INode<K, D>::split(Node<K, D> *parent, int child)
{
    INode<K, D> *newNode = new INode<K, D>;  //分裂后的右节点
    newNode->set_num(MINNUM_KEY);
    int i;
    for (i = 0; i < MINNUM_KEY; ++i)  // 拷贝关键字的值
    {
        newNode->set_key(i, this->key_value[i + MINNUM_CHILD]);
    }
    for (i = 0; i < MINNUM_CHILD; ++i)  // 拷贝孩子节点指针
    {
        newNode->set_child(i, children[i + MINNUM_CHILD]);
    }
    this->set_num(MINNUM_KEY);  //更新左子树的关键字个数
    ((INode<K, D> *)parent)->insert(child, child + 1, this->key_value[MINNUM_KEY], newNode);
}

template <typename K, typename D> void INode<K, D>::merge(Node<K, D> *parent, Node<K, D> *child, int index)
{
    // 合并数据
    insert(MINNUM_KEY, MINNUM_KEY + 1, parent->get_key(index), ((INode<K, D> *)child)->get_child(0));
    int i;
    for (i = 1; i <= child->get_num(); ++i) {
        insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, child->get_key(i - 1), ((INode<K, D> *)child)->get_child(i));
    }
    //父节点删除index的key
    parent->remove_key(index, index + 1);
    delete ((INode<K, D> *)parent)->get_child(index + 1);
}

template <typename K, typename D> void INode<K, D>::remove_key(int key_index, int child)
{
    for (int i = 0; i < this->get_num() - key_index - 1; ++i) {
        this->set_key(key_index + i, this->get_key(key_index + i + 1));
        set_child(child + i, get_child(child + i + 1));
    }
    this->set_num(this->get_num() - 1);
}

template <typename K, typename D>
void INode<K, D>::borrow(Node<K, D> *dest, Node<K, D> *parent, int key_index, int sibling)
{
    switch (sibling) {
        case 0:  // 从左兄弟结点借
        {
            insert(0, 0, parent->get_key(key_index), ((INode<K, D> *)dest)->get_child(dest->get_num()));
            parent->set_key(key_index, dest->get_key(dest->get_num() - 1));
            dest->remove_key(dest->get_num() - 1, dest->get_num());
        } break;
        case 1:  // 从右兄弟结点借
        {
            insert(this->get_num(), this->get_num() + 1, parent->get_key(key_index),
                   ((INode<K, D> *)dest)->get_child(0));
            parent->set_key(key_index, dest->get_key(0));
            dest->remove_key(0, 0);
        } break;
        default:
            break;
    }
}

template <typename K, typename D> int INode<K, D>::get_child_index(K key, int index)
{
    if (key >= this->get_key(index)) {
        return index + 1;
    }
    else {
        return index;
    }
}

// LNode
template <typename K, typename D> LNode<K, D>::LNode()
{
    this->set_type(1);
    set_lsibling(NULL);
    set_rsibling(NULL);

    overflow_size = 0;
}

template <typename K, typename D> LNode<K, D>::~LNode()
{
}

template <typename K, typename D> LNode<K, D> *LNode<K, D>::get_lsibling() const
{
    return lsibling;
}

template <typename K, typename D> void LNode<K, D>::set_lsibling(LNode<K, D> *node)
{
    lsibling = node;
}

template <typename K, typename D> LNode<K, D> *LNode<K, D>::get_rsibling() const
{
    return rsibling;
}

template <typename K, typename D> void LNode<K, D>::set_rsibling(LNode<K, D> *node)
{
    rsibling = node;
}

template <typename K, typename D> D LNode<K, D>::get_data(int i)
{
    this->pour();
    return data[i];
}

template <typename K, typename D> void LNode<K, D>::set_data(int i, const D &data)
{
    this->data[i] = data;
}

template <typename K, typename D> D *LNode<K, D>::get_data_addr(int i)
{
    this->pour();
    return &(data[i]);
}

template <typename K, typename D> void LNode<K, D>::clear()
{
    for (int i = 0; i < this->key_num; ++i) {
        if (data[i] != NULL) {
            delete data[i];
            data[i] = NULL;
        }
    }
}

template <typename K, typename D> void LNode<K, D>::insert_main(K key, const D data)
{
    int i;
    for (i = this->key_num; i >= 1 && this->key_value[i - 1] > key; --i) {
        this->set_key(i, this->key_value[i - 1]);
        set_data(i, this->data[i - 1]);
    }
    this->set_key(i, key);
    set_data(i, data);
    this->set_num(this->key_num + 1);
}

template <typename K, typename D> void LNode<K, D>::insert(K key, const D data)
{
    // insert_main(key, data);

    if (this->key_num + M / 2 > MAXNUM_LEAVES) {
        insert_main(key, data);
    }
    else {
        overflow_ins(key, data);
    }
}

template <typename K, typename D> void LNode<K, D>::split(Node<K, D> *parent, int child)
{
    this->pour();
    LNode<K, D> *newNode = new LNode<K, D>;  //分裂后的右节点
    this->set_num(MINNUM_LEAVES);
    newNode->set_num(MINNUM_LEAVES + 1);
    newNode->set_rsibling(get_rsibling());
    set_rsibling(newNode);
    newNode->set_lsibling(this);
    int i;
    for (i = 0; i < MINNUM_LEAVES + 1; ++i)  // 拷贝关键字的值
    {
        newNode->set_key(i, this->key_value[i + MINNUM_LEAVES]);
    }
    for (i = 0; i < MINNUM_LEAVES + 1; ++i)  // 拷贝数据
    {
        newNode->set_data(i, data[i + MINNUM_LEAVES]);
    }
    ((INode<K, D> *)parent)->insert(child, child + 1, this->key_value[MINNUM_LEAVES], newNode);
}

template <typename K, typename D> void LNode<K, D>::merge(Node<K, D> *parent, Node<K, D> *child, int key_index)
{
    this->pour();
    // 合并数据
    for (int i = 0; i < child->get_num(); ++i) {
        insert(child->get_key(i), ((LNode *)child)->get_data(i));
    }
    set_rsibling(((LNode *)child)->get_rsibling());
    //父节点删除index的key，
    parent->remove_key(key_index, key_index + 1);
}

template <typename K, typename D> void LNode<K, D>::remove_key(int key_index, int child_index)
{
    this->pour();
    for (int i = key_index; i < this->get_num() - 1; ++i) {
        this->set_key(i, this->get_key(i + 1));
        set_data(i, get_data(i + 1));
    }
    this->set_num(this->get_num() - 1);
}

template <typename K, typename D>
void LNode<K, D>::borrow(Node<K, D> *sibling, Node<K, D> *parent, int key_index, int d)
{
    this->pour();
    switch (d) {
        case 0:  // 从左兄弟结点借
        {
            insert(sibling->get_key(sibling->get_num() - 1), ((LNode *)sibling)->get_data(sibling->get_num() - 1));
            sibling->remove_key(sibling->get_num() - 1, sibling->get_num() - 1);
            parent->set_key(key_index, this->get_key(0));
        } break;
        case 1:  // 从右兄弟结点借
        {
            insert(sibling->get_key(0), ((LNode *)sibling)->get_data(0));
            sibling->remove_key(0, 0);
            parent->set_key(key_index, sibling->get_key(0));
        } break;
        default:
            break;
    }
}

template <typename K, typename D> int LNode<K, D>::get_child_index(K key, int key_index)
{
    this->pour();
    return key_index;
}

template <typename K, typename D> void LNode<K, D>::pour()
{
    if (overflow_size == 0)
        return;
    // cout << "overflow size is: " << overflow_size << endl;
    sort();
    // cout << "again, overflow size is: " << overflow_size << endl;
    for (int k = 0; k < overflow_size; k++) {
        K key = overflow_keys[k];
        D data = overflow_datas[k];

        insert_main(key, data);
    }
    overflow_size = 0;
}

template <typename K, typename D> void LNode<K, D>::sort()
{
    int end_of_sorted = 0;

    while (end_of_sorted < overflow_size - 1) {
        int start = 0;

        while (overflow_keys[start] < overflow_keys[end_of_sorted + 1]) {
            start++;
        }

        int temp = overflow_keys[end_of_sorted + 1];

        for (int i = end_of_sorted; i >= start; i--) {
            overflow_keys[i + 1] = overflow_keys[i];
            overflow_datas[i + 1] = overflow_datas[i];
        }

        overflow_keys[start] = temp;
        end_of_sorted++;
    }
}

template <typename K, typename D> void LNode<K, D>::overflow_ins(K key, const D data)
{
    if (overflow_size >= M / 2)
        pour();
    overflow_keys[overflow_size] = key;
    overflow_datas[overflow_size] = data;
    overflow_size++;
}