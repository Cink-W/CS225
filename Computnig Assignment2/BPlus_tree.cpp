
#include "BPlus_tree.h"

#include <algorithm>
using namespace std;

template <typename K, typename D> CBPlusTree<K, D>::CBPlusTree()
{
    root = NULL;
    m_DataHead = NULL;
}

template <typename K, typename D> CBPlusTree<K, D>::~CBPlusTree()
{
    clear();
}

template <typename K, typename D> bool CBPlusTree<K, D>::insert(K key, const D data)
{  // cout << search(key) << endl;
    // 是否已经存在
    if (search(key)) {
        return false;
    }
    // 找到可以插入的叶子结点，否则创建新的叶子结点
    if (root == NULL) {
        // cout << "check2" << endl;
        root = new LNode<K, D>;
        m_DataHead = (LNode<K, D> *)root;
        m_MaxKey = key;
    }
    if (root->get_num() >= MAXNUM_KEY)  // 根结点已满，分裂
    {
        // cout << "check3" << endl;
        INode<K, D> *newNode = new INode<K, D>;  //创建新的根节点
        newNode->set_child(0, root);
        root->split(newNode, 0);  // 叶子结点分裂
        root = newNode;           //更新根节点指针
    }
    if (key > m_MaxKey)  // 更新最大键值
    {
        // cout << "check4" << endl;
        m_MaxKey = key;
    }
    rec_insert(root, key, data);
    return true;
}

template <typename K, typename D> void CBPlusTree<K, D>::rec_insert(Node<K, D> *parentNode, K key, const D data)
{
    if (parentNode->get_type() == 1)  // 叶子结点，直接插入
    {
        ((LNode<K, D> *)parentNode)->insert(key, data);
    }
    else {
        // 找到子结点
        int keyIndex = parentNode->get_key_index(key);
        int childIndex = parentNode->get_child_index(key, keyIndex);  // 孩子结点指针索引
        Node<K, D> *childNode = ((INode<K, D> *)parentNode)->get_child(childIndex);
        if (childNode->get_num() >= MAXNUM_LEAVES)  // 子结点已满，需进行分裂
        {
            childNode->split(parentNode, childIndex);
            if (parentNode->get_key(childIndex) <= key)  // 确定目标子结点
            {
                childNode = ((INode<K, D> *)parentNode)->get_child(childIndex + 1);
            }
        }
        rec_insert(childNode, key, data);
    }
}

template <typename K, typename D> void CBPlusTree<K, D>::clear()
{
    if (root != NULL) {
        root->clear();
        delete root;
        root = NULL;
        m_DataHead = NULL;
    }
}

template <typename K, typename D> bool CBPlusTree<K, D>::search(K key) const
{
    return rec_search(root, key);
}

template <typename K, typename D> bool CBPlusTree<K, D>::rec_search(Node<K, D> *pNode, K key) const
{
    if (pNode == NULL)  //检测节点指针是否为空，或该节点是否为叶子节点
    {
        return false;
    }
    else {
        int keyIndex = pNode->get_key_index(key);
        int childIndex = pNode->get_child_index(key, keyIndex);  // 孩子结点指针索引
        if (keyIndex < pNode->get_num() && key == pNode->get_key(keyIndex)) {
            return true;
        }
        else {
            if (pNode->get_type() == 1)  //检查该节点是否为叶子节点
            {
                return false;
            }
            else {
                return rec_search(((INode<K, D> *)pNode)->get_child(childIndex), key);
            }
        }
    }
}

template <typename K, typename D> D *CBPlusTree<K, D>::get_data(K key) const
{
    return rec_get_data(root, key);
}

template <typename K, typename D> D *CBPlusTree<K, D>::rec_get_data(Node<K, D> *pNode, K key) const
{
    // cout << "first" << endl;
    if (pNode == NULL)  //检测节点指针是否为空
    {
        return nullptr;
    }

    int keyIndex = pNode->get_key_index(key);
    int childIndex = pNode->get_child_index(key, keyIndex);  // 孩子结点指针索引

    // cout << "second" << endl;
    if (pNode->get_type() == 1) {
        // ((LNode<K, D> *)pNode)->pour();

        // cout << "now \"pnode\" points to a 1." << endl;
        // cout << "key to get handle: " << key << endl;
        // cout << "keyindex: " << keyIndex << endl;
        // cout << "childindex: " << childIndex << endl;
        // cout << "pNode->get_num(): " << pNode->get_num() << endl;
        // cout << "pNode->get_key(keyIndex): " << pNode->get_key(keyIndex) << endl;

        if (keyIndex < pNode->get_num() && key == pNode->get_key(keyIndex)) {
            // cout << "third" << endl;
            return ((LNode<K, D> *)pNode)->get_data_addr(childIndex);
        }
        else {
            // cout << "fourth" << endl;
            return nullptr;
        }
    }
    else
        // cout << "\"pnode\" still points to an internal node." << endl;

        return rec_get_data(((INode<K, D> *)pNode)->get_child(childIndex), key);
}

template <typename K, typename D> void CBPlusTree<K, D>::print() const
{
    ofstream BP;
    BP.open("B+ Tree.csv");
    BP << endl;
    BP << endl;
    BP << "Now print the keys in tree form:" << endl;
    printInConcavo(root, 10, BP);
    BP << endl;
    BP.close();
}

template <typename K, typename D>
void CBPlusTree<K, D>::printInConcavo(Node<K, D> *pNode, int count, ofstream &BP) const
{
    if (pNode->get_type() == 1)
        ((LNode<K, D> *)pNode)->pour();
    if (pNode != NULL) {
        int i, j;
        for (i = 0; i < pNode->get_num(); ++i) {
            if (pNode->get_type() != 1) {
                printInConcavo(((INode<K, D> *)pNode)->get_child(i), count - 2, BP);
            }
            for (j = count; j >= 0; --j) {
                BP << "---";
            }
            BP << pNode->get_key(i) << endl;
        }
        if (pNode->get_type() != 1) {
            printInConcavo(((INode<K, D> *)pNode)->get_child(i), count - 2, BP);
        }
    }
}

template <typename K, typename D> void CBPlusTree<K, D>::printData() const
{
    cout << endl;
    cout << "Now print all the data stored in B+ tree:" << endl << endl;
    LNode<K, D> *itr = m_DataHead;
    itr->pour();
    while (itr != NULL) {
        cout << "block size: " << itr->get_num() << endl;
        cout << "[ ";
        for (int i = 0; i < itr->get_num(); ++i) {
            cout << itr->get_data(i) << " , ";
        }
        cout << " ]" << endl;
        itr = itr->get_lsibling();
    }
    cout << endl;
}

template <typename K, typename D> bool CBPlusTree<K, D>::remove(K key)
{
    if (!search(key))  //不存在
    {
        return false;
    }
    if (root->get_num() == 1)  //特殊情况处理
    {
        if (root->get_type() == 1) {
            clear();
            return true;
        }
        else {
            Node<K, D> *pChild1 = ((INode<K, D> *)root)->get_child(0);
            Node<K, D> *pChild2 = ((INode<K, D> *)root)->get_child(1);
            if (pChild1->get_num() == MINNUM_KEY && pChild2->get_num() == MINNUM_KEY) {
                pChild1->merge(root, pChild2, 0);
                delete root;
                root = pChild1;
            }
        }
    }
    rec_remove(root, key);
    return true;
}

// parentNode中包含的键值数>MINNUM_KEY
template <typename K, typename D> void CBPlusTree<K, D>::rec_remove(Node<K, D> *parentNode, K key)
{
    int keyIndex = parentNode->get_key_index(key);
    int childIndex = parentNode->get_child_index(key, keyIndex);  // 孩子结点指针索引
    if (parentNode->get_type() == 1)                              // 找到目标叶子节点
    {
        if (key == m_MaxKey && keyIndex > 0) {
            m_MaxKey = parentNode->get_key(keyIndex - 1);
        }
        parentNode->remove_key(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex == 0 && root->get_type() != 1 && parentNode != m_DataHead) {
            changeKey(root, key, parentNode->get_key(0));
        }
    }
    else  // 内结点
    {
        cout << "call" << endl;
        Node<K, D> *pChildNode = ((INode<K, D> *)parentNode)->get_child(childIndex);  //包含key的子树根节点
        if (pChildNode->get_num() == MINNUM_KEY)  // 包含关键字达到下限值，进行相关操作
        {
            Node<K, D> *pLeft =
                childIndex > 0 ? ((INode<K, D> *)parentNode)->get_child(childIndex - 1) : NULL;  //左兄弟节点
            Node<K, D> *pRight = childIndex < parentNode->get_num()
                                     ? ((INode<K, D> *)parentNode)->get_child(childIndex + 1)
                                     : NULL;  //右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft && pLeft->get_num() > MINNUM_KEY)  // 左兄弟结点可借
            {
                pChildNode->borrow(pLeft, parentNode, childIndex - 1, 0);
            }
            else if (pRight && pRight->get_num() > MINNUM_KEY)  //右兄弟结点可借
            {
                pChildNode->borrow(pRight, parentNode, childIndex, 1);
            }
            //左右兄弟节点都不可借，考虑合并
            else if (pLeft)  //与左兄弟合并
            {
                pLeft->merge(parentNode, pChildNode, childIndex - 1);
                pChildNode = pLeft;
            }
            else if (pRight)  //与右兄弟合并
            {
                pChildNode->merge(parentNode, pRight, childIndex);
            }
        }
        rec_remove(pChildNode, key);
    }
}

template <typename K, typename D> void CBPlusTree<K, D>::changeKey(Node<K, D> *pNode, K oldKey, K newKey)
{
    if (pNode != NULL && pNode->get_type() != 1) {
        int keyIndex = pNode->get_key_index(oldKey);
        if (keyIndex < pNode->get_num() && oldKey == pNode->get_key(keyIndex))  // 找到
        {
            pNode->setKeyValue(keyIndex, newKey);
        }
        else  // 继续找
        {
            changeKey(((INode<K, D> *)pNode)->get_child(keyIndex), oldKey, newKey);
        }
    }
}

template <typename K, typename D> bool CBPlusTree<K, D>::update(K oldKey, K newKey)
{
    if (search(newKey))  // 检查更新后的键是否已经存在
    {
        return false;
    }
    else {
        int dataValue;
        remove(oldKey, dataValue);
        if (dataValue == INVALID_INDEX) {
            return false;
        }
        else {
            return insert(newKey, dataValue);
        }
    }
}

template <typename K, typename D> void CBPlusTree<K, D>::remove(K key, D &dataValue)
{
    if (!search(key))  //不存在
    {
        dataValue = INVALID_INDEX;
        return;
    }
    if (root->get_num() == 1)  //特殊情况处理
    {
        if (root->get_type() == 1) {
            dataValue = ((LNode<K, D> *)root)->get_data(0);
            clear();
            return;
        }
        else {
            Node<K, D> *pChild1 = ((INode<K, D> *)root)->get_child(0);
            Node<K, D> *pChild2 = ((INode<K, D> *)root)->get_child(1);
            if (pChild1->get_num() == MINNUM_KEY && pChild2->get_num() == MINNUM_KEY) {
                pChild1->merge(root, pChild2, 0);
                delete root;
                root = pChild1;
            }
        }
    }
    rec_remove(root, key, dataValue);
}

template <typename K, typename D> void CBPlusTree<K, D>::rec_remove(Node<K, D> *parentNode, K key, D &dataValue)
{
    int keyIndex = parentNode->get_key_index(key);
    int childIndex = parentNode->get_child_index(key, keyIndex);  // 孩子结点指针索引
    if (parentNode->get_type() == 1)                              // 找到目标叶子节点
    {
        if (key == m_MaxKey && keyIndex > 0) {
            m_MaxKey = parentNode->get_key(keyIndex - 1);
        }
        dataValue = ((LNode<K, D> *)parentNode)->get_data(keyIndex);
        parentNode->remove_key(keyIndex, childIndex);  // 直接删除
        // 如果键值在内部结点中存在，也要相应的替换内部结点
        if (childIndex == 0 && root->get_type() != 1 && parentNode != m_DataHead) {
            changeKey(root, key, parentNode->get_key(0));
        }
    }
    else  // 内结点
    {
        Node<K, D> *pChildNode = ((INode<K, D> *)parentNode)->get_child(childIndex);  //包含key的子树根节点
        if (pChildNode->get_num() == MINNUM_KEY)  // 包含关键字达到下限值，进行相关操作
        {
            Node<K, D> *pLeft =
                childIndex > 0 ? ((INode<K, D> *)parentNode)->get_child(childIndex - 1) : NULL;  //左兄弟节点
            Node<K, D> *pRight = childIndex < parentNode->get_num()
                                     ? ((INode<K, D> *)parentNode)->get_child(childIndex + 1)
                                     : NULL;  //右兄弟节点
            // 先考虑从兄弟结点中借
            if (pLeft && pLeft->get_num() > MINNUM_KEY)  // 左兄弟结点可借
            {
                pChildNode->borrow(pLeft, parentNode, childIndex - 1, 0);
            }
            else if (pRight && pRight->get_num() > MINNUM_KEY)  //右兄弟结点可借
            {
                pChildNode->borrow(pRight, parentNode, childIndex, 1);
            }
            //左右兄弟节点都不可借，考虑合并
            else if (pLeft)  //与左兄弟合并
            {
                pLeft->merge(parentNode, pChildNode, childIndex - 1);
                pChildNode = pLeft;
            }
            else if (pRight)  //与右兄弟合并
            {
                pChildNode->merge(parentNode, pRight, childIndex);
            }
        }
        rec_remove(pChildNode, key, dataValue);
    }
}