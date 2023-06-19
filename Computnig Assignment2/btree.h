#ifndef BTREE_H
#define BTREE_H
#include "alist.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

template <class KeyType, class DataType> class KeyStruct
{
  public:
    KeyStruct(KeyType k, DataType d) : key(k)
    {
        data = new AList<DataType>();
        data->append(d);
    };
    ~KeyStruct()
    {
        if (data != 0)
            delete data;
    }
    void appenddata(DataType d)
    {
        if (data == 0) {
            data = new AList<DataType>();
        }
        data->append(d);
    }
    void removedata(int i)
    {
        if (data != 0)
            data->remove(i);
    }
    KeyType key;
    AList<DataType> *data = 0;
};

template <class KeyType, class DataType> class BTree;

template <class KeyType, class DataType> class BTreeNode
{
    friend class BTree<KeyType, DataType>;

  public:
    BTreeNode(KeyType k, DataType d);
    BTreeNode(KeyStruct<KeyType, DataType> *kstruct);
    BTreeNode();
    ~BTreeNode();
    KeyStruct<KeyType, DataType> *getkey(int i);
    AList<DataType> *getdata(int i);
    int getkeyidx(KeyType k);
    int findkeyidx(KeyType k);
    BTreeNode *getchild(int i);
    int insertkey(KeyStruct<KeyType, DataType> *k);
    void insertkey(int i, KeyStruct<KeyType, DataType> *k);
    KeyStruct<KeyType, DataType> *removekey(int i);
    DataType removedata(int keyidx, int dataidx);
    BTreeNode *removechild(int i);
    BTreeNode *getparent();
    BTreeNode *split();
    void merge_right(BTreeNode *rsib);
    int getnumkeys();
    int getnumchildren();
    void print();
    void insertchild(int i, BTreeNode *c);
    void setkey(int i, KeyStruct<KeyType, DataType> *k);
    void setchild(int i, BTreeNode *c);
    BTreeNode *getlsibling();
    BTreeNode *getrsibling();
    KeyStruct<KeyType, DataType> *getpred(int i, BTreeNode **dest);
    KeyStruct<KeyType, DataType> *getsucc(int i, BTreeNode **dest);
    KeyStruct<KeyType, DataType> *getlargest(BTreeNode **dest);
    KeyStruct<KeyType, DataType> *getsmallest(BTreeNode **dest);

  private:
    AList<KeyStruct<KeyType, DataType> *> *keys = 0;
    AList<BTreeNode<KeyType, DataType> *> *children = 0;
    BTreeNode<KeyType, DataType> *parent = 0;
    int idx = -1;
    void free_all_children();
};

template <class KeyType, class DataType> class BTree
{
  public:
    BTree() : m(100){};
    BTree(int m) : m(m){};
    BTree(int m, KeyType k, DataType d) : m(m)
    {
        root = new BTreeNode<KeyType, DataType>(k, d);
    };
    ~BTree();
    void insert(KeyType k, DataType d);
    void remove(KeyType k, DataType d);
    void remove(KeyType k);
    AList<DataType> *find(KeyType k);
    void print();

  private:
    int m;
    int height = 0;
    int numnodes = 0;
    int numkeys = 0;
    BTreeNode<KeyType, DataType> *root = 0;
    void _remove_from_leaf(int i, BTreeNode<KeyType, DataType> *leaf, bool del);
    void _remove_from_nonleaf(int i, BTreeNode<KeyType, DataType> *non_leaf);
    void merge(BTreeNode<KeyType, DataType> *n);
    void split(BTreeNode<KeyType, DataType> *n);
    void _print_recurse(BTreeNode<KeyType, DataType> *n, unsigned long long offset);
    int _search(KeyType k, BTreeNode<KeyType, DataType> **dest);
};

template <class KeyType, class DataType> BTreeNode<KeyType, DataType>::BTreeNode()
{
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType>::BTreeNode(KeyType k, DataType d)
{
    KeyStruct<KeyType, DataType> *kstruct = new KeyStruct<KeyType, DataType>(k, d);
    insertkey(kstruct);
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType>::BTreeNode(KeyStruct<KeyType, DataType> *kstruct)
{
    insertkey(kstruct);
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType>::~BTreeNode()
{
    if (keys != 0) {
        delete keys;
    }
    if (children != 0) {
        delete children;
    }
}

template <class KeyType, class DataType> KeyStruct<KeyType, DataType> *BTreeNode<KeyType, DataType>::getkey(int i)
{
    if (keys != 0) {
        return keys->getitem(i);
    }
    return 0;
}

template <class KeyType, class DataType> AList<DataType> *BTreeNode<KeyType, DataType>::getdata(int i)
{
    if (keys != 0) {
        return keys->getitem(i)->data;
    }
    return 0;
}

template <class KeyType, class DataType> int BTreeNode<KeyType, DataType>::getkeyidx(KeyType k)
{
    if (keys == 0) {
        return -1;
    }
    for (int i = 0; i < keys->getlength(); i++) {
        if (keys->getitem(i)->key == k) {
            return i;
        }
    }
}

template <class KeyType, class DataType> int BTreeNode<KeyType, DataType>::findkeyidx(KeyType k)
{
    if (keys == 0) {
        return -1;
    }
    for (int i = 0; i < keys->getlength(); i++) {
        if (keys->getitem(i)->key >= k) {
            return i;
        }
    }
    return keys->getlength();
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType> *BTreeNode<KeyType, DataType>::getchild(int i)
{
    if (children != 0) {
        return children->getitem(i);
    }
    return 0;
}

template <class KeyType, class DataType>
void BTreeNode<KeyType, DataType>::setkey(int i, KeyStruct<KeyType, DataType> *k)
{
    if (keys != 0) {
        keys->setitem(i, k);
    }
}

template <class KeyType, class DataType>
void BTreeNode<KeyType, DataType>::setchild(int i, BTreeNode<KeyType, DataType> *child)
{
    if (children != 0) {
        children->setitem(i, child);
        child->parent = this;
        child->idx = i;
    }
}

template <class KeyType, class DataType>
void BTreeNode<KeyType, DataType>::insertkey(int i, KeyStruct<KeyType, DataType> *key)
{
    if (keys != 0) {
        keys->insert(i, key);
    }
    else {
        keys = new AList<KeyStruct<KeyType, DataType> *>();
        keys->insert(i, key);
    }
}

template <class KeyType, class DataType> int BTreeNode<KeyType, DataType>::insertkey(KeyStruct<KeyType, DataType> *k)
{
    if (keys != 0) {
        for (int i = 0; i < getnumkeys(); ++i) {
            if (getkey(i)->key > k->key) {
                keys->insert(i, k);
                return i;
            }
        }
        keys->insert(keys->getlength(), k);
    }
    else {
        keys = new AList<KeyStruct<KeyType, DataType> *>();
        keys->insert(0, k);
    }
    return 0;
}

template <class KeyType, class DataType>
void BTreeNode<KeyType, DataType>::insertchild(int i, BTreeNode<KeyType, DataType> *c)
{
    if (children != 0) {
        children->insert(i, c);
        c->parent = this;
        c->idx = i;
        for (int j = i + 1; j < getnumchildren(); ++j) {
            ++(children->getitem(j)->idx);
        }
    }
    else {
        children = new AList<BTreeNode<KeyType, DataType> *>();
        children->append(c);
        c->parent = this;
        c->idx = 0;
    }
}

template <class KeyType, class DataType> KeyStruct<KeyType, DataType> *BTreeNode<KeyType, DataType>::removekey(int i)
{
    if (keys != 0) {
        KeyStruct<KeyType, DataType> *removed_key = keys->remove(i);
        return removed_key;
    }
    else {
        return 0;
    }
}

template <class KeyType, class DataType> DataType BTreeNode<KeyType, DataType>::removedata(int keyidx, int dataidx)
{
    if (keys != 0) {
        KeyStruct<KeyType, DataType> *target_key = getkey(keyidx);
        return target_key->data->remove(dataidx);
    }
    return 0;
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType> *BTreeNode<KeyType, DataType>::removechild(int i)
{
    if (children != 0) {
        BTreeNode<KeyType, DataType> *removed_child = children->remove(i);
        removed_child->parent = 0;
        removed_child->idx = -1;
        for (; i < children->getlength(); ++i) {
            --(children->getitem(i)->idx);
        }
        return removed_child;
    }
    else {
        return 0;
    }
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType> *BTreeNode<KeyType, DataType>::getparent()
{
    return parent;
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType> *BTreeNode<KeyType, DataType>::split()
{
    if (keys == 0)
        return 0;
    BTreeNode<KeyType, DataType> *new_node = new BTreeNode<KeyType, DataType>();
    // Move the latter half of the keys to the new node
    int numkeys_tot = getnumkeys();
    for (int i = numkeys_tot - 1; i > (numkeys_tot - 1) / 2; --i) {
        new_node->insertkey(keys->remove(i));
    }

    // Move the latter half of the children to the new node
    if (children != 0) {
        int numchildren_tot = getnumchildren();
        for (int j = (numchildren_tot + 1) / 2; j > 0; --j) {
            new_node->insertchild(0, children->remove(getnumchildren() - 1));
        }
    }

    // Update in parent node if has one
    if (parent != 0) {
        parent->insertkey(idx, removekey(getnumkeys() - 1));
        parent->insertchild(idx + 1, new_node);
    }

    return new_node;
}

template <class KeyType, class DataType> void BTreeNode<KeyType, DataType>::merge_right(BTreeNode *rsib)
{
    if (rsib == 0 || rsib->parent == 0 || parent != rsib->parent)
        return;

    // Merge keys
    if (rsib->keys != 0) {
        insertkey(parent->removekey(idx));
        for (int i = 0; i < rsib->getnumkeys(); ++i) {
            insertkey(rsib->getkey(i));
        }
    }

    // Merge children
    if (rsib->children != 0) {
        for (int j = 0; j < rsib->getnumchildren(); ++j) {
            insertchild(getnumchildren(), rsib->getchild(j));
        }
    }

    // Update in parent
    rsib->parent->removechild(rsib->idx);
    // Free memory
    delete rsib;
}

template <class KeyType, class DataType> int BTreeNode<KeyType, DataType>::getnumkeys()
{
    if (keys != 0) {
        return keys->getlength();
    }
    return 0;
}

template <class KeyType, class DataType> int BTreeNode<KeyType, DataType>::getnumchildren()
{
    if (children != 0) {
        return children->getlength();
    }
    return 0;
}

template <class KeyType, class DataType> void BTreeNode<KeyType, DataType>::print()
{
    if (keys == 0)
        return;
    cout << "Keys: " << endl;
    for (int i = 0; i < getnumkeys(); ++i) {
        cout << getkey(i) << " ";
    }
    cout << endl;

    if (children == 0)
        return;
    cout << "Children: " << endl;
    for (int j = 0; j < getnumchildren(); ++j) {
        if (getchild(j) != 0) {
            cout << getchild(j)->getkey(0) << " ";
        }
        else {
            cout << "null ";
        }
    }
    cout << endl;
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType> *BTreeNode<KeyType, DataType>::getlsibling()
{
    if (parent == 0 || idx == 0)
        return 0;
    return parent->getchild(idx - 1);
}

template <class KeyType, class DataType> BTreeNode<KeyType, DataType> *BTreeNode<KeyType, DataType>::getrsibling()
{
    if (parent == 0 || idx == parent->getnumchildren() - 1)
        return 0;
    return parent->getchild(idx + 1);
}

template <class KeyType, class DataType>
KeyStruct<KeyType, DataType> *BTreeNode<KeyType, DataType>::getpred(int i, BTreeNode<KeyType, DataType> **dest)
{
    *dest = 0;
    if (children == 0)
        return 0;
    return getchild(i)->getlargest(dest);
}

template <class KeyType, class DataType>
KeyStruct<KeyType, DataType> *BTreeNode<KeyType, DataType>::getsucc(int i, BTreeNode<KeyType, DataType> **dest)
{
    *dest = 0;
    if (children == 0)
        return 0;
    return getchild(i + 1)->getsmallest(dest);
}

template <class KeyType, class DataType>
KeyStruct<KeyType, DataType> *BTreeNode<KeyType, DataType>::getlargest(BTreeNode<KeyType, DataType> **dest)
{
    *dest = 0;
    if (children == 0) {
        *dest = this;
        return getkey(getnumkeys() - 1);
    }
    return getchild(getnumchildren() - 1)->getlargest(dest);
}

template <class KeyType, class DataType>
KeyStruct<KeyType, DataType> *BTreeNode<KeyType, DataType>::getsmallest(BTreeNode<KeyType, DataType> **dest)
{
    *dest = 0;
    if (children == 0) {
        *dest = this;
        return getkey(0);
    }
    return getchild(0)->getsmallest(dest);
}

template <class KeyType, class DataType> void BTreeNode<KeyType, DataType>::free_all_children()
{
    if (children == 0)
        return;
    for (int i = 0; i < getnumchildren(); ++i) {
        BTreeNode<KeyType, DataType> *child = getchild(i);
        if (child->children == 0) {
            delete child;
        }
        else {
            child->free_all_children();
        }
    }
}

template <class KeyType, class DataType> BTree<KeyType, DataType>::~BTree()
{
    if (root == 0)
        return;
    root->free_all_children();
    delete root;
}

template <class KeyType, class DataType> void BTree<KeyType, DataType>::insert(KeyType k, DataType d)
{
    if (root == 0) {
        root = new BTreeNode<KeyType, DataType>(k, d);
        height = 1;
        numkeys = 1;
        numnodes = 1;
        return;
    }

    BTreeNode<KeyType, DataType> *dest = 0;
    int idx = _search(k, &dest);
    // Key already exists
    if (idx >= 0 && idx < dest->getnumkeys() && dest->getkey(idx)->key == k) {
        // Data already exists
        AList<DataType> *dlist = dest->getdata(idx);
        for (int i = 0; i < dlist->getlength(); ++i) {
            if (dlist->getitem(i) == d) {
                cout << "Key " << k << " and data " << d << " already exist" << endl;
                return;
            }
        }
        // Update data in that key if key exists but data doesn't
        dest->getkey(idx)->appenddata(d);
        return;
    }
    // Else key does not exist, create a new KeyStruct instance then insert
    KeyStruct<KeyType, DataType> *newkey = new KeyStruct<KeyType, DataType>(k, d);
    dest->insertkey(newkey);
    ++numkeys;
    // Overflow
    if (dest->getnumkeys() > m - 1) {
        split(dest);
    }
}

template <class KeyType, class DataType> void BTree<KeyType, DataType>::remove(KeyType k, DataType d)
{
    if (root == 0)
        return;
    BTreeNode<KeyType, DataType> *dest = 0;
    int idx = _search(k, &dest);
    if (idx == -1 || idx >= dest->getnumkeys() || dest->getkey(idx)->key != k) {
        cout << "Key " << k << " not found" << endl;
        return;
    }

    KeyStruct<KeyType, DataType> *kstruct = dest->getkey(idx);
    int dataidx = kstruct->data->getidx(d);

    if (dataidx == -1) {
        cout << "Data " << d << " not found in key " << k << endl;
        return;
    }

    if (kstruct->data->getlength() <= 1) {
        remove(k);
    }
    else {
        kstruct->data->remove(dataidx);
    }
}

template <class KeyType, class DataType> void BTree<KeyType, DataType>::remove(KeyType k)
{
    if (root == 0)
        return;
    BTreeNode<KeyType, DataType> *dest = 0;
    int idx = _search(k, &dest);
    if (idx == -1 || idx >= dest->getnumkeys() || dest->getkey(idx)->key != k) {
        cout << "Key " << k << " not found" << endl;
        return;
    }
    // Remove key from Leaf node
    if (dest->getnumchildren() == 0) {
        _remove_from_leaf(idx, dest, true);
    }
    else {
        // Remove key from Non-Leaf node
        _remove_from_nonleaf(idx, dest);
    }
    --numkeys;
}

template <class KeyType, class DataType>
void BTree<KeyType, DataType>::_remove_from_leaf(int i, BTreeNode<KeyType, DataType> *leaf, bool del)
{
    KeyStruct<KeyType, DataType> *removed_kstruct = leaf->removekey(i);
    if (del == true) {
        delete removed_kstruct;
    }
    // Underflow
    if (leaf->getnumkeys() < (m + 1) / 2 - 1) {
        // Special case: leaf is the root
        if (leaf == root)
            return;

        BTreeNode<KeyType, DataType> *rsib = leaf->getrsibling();
        BTreeNode<KeyType, DataType> *lsib = leaf->getlsibling();
        // Try borrowing from right sibling
        if (rsib != 0 && rsib->getnumkeys() > (m + 1) / 2 - 1 &&
            (lsib == 0 || rsib->getnumkeys() > lsib->getnumkeys())) {
            leaf->insertkey(leaf->getnumkeys(), leaf->parent->getkey(leaf->idx));
            leaf->parent->setkey(leaf->idx, rsib->removekey(0));
            return;
        }
        // Try borrowing from left sibling
        if (lsib != 0 && lsib->getnumkeys() > (m + 1) / 2 - 1 &&
            (rsib == 0 || lsib->getnumkeys() > rsib->getnumkeys())) {
            leaf->insertkey(0, leaf->parent->getkey(leaf->idx - 1));
            leaf->parent->setkey(leaf->idx - 1, lsib->removekey(lsib->getnumkeys() - 1));
            return;
        }

        // Merge with right or left sibling
        // (recursively merges upward, if necessary)
        merge(leaf);
    }
}

template <class KeyType, class DataType>
void BTree<KeyType, DataType>::_remove_from_nonleaf(int i, BTreeNode<KeyType, DataType> *non_leaf)
{
    KeyStruct<KeyType, DataType> *kstruct = non_leaf->getkey(i);
    BTreeNode<KeyType, DataType> *predk_leaf = 0;
    BTreeNode<KeyType, DataType> *succk_leaf = 0;
    KeyStruct<KeyType, DataType> *predkey = non_leaf->getpred(i, &predk_leaf);
    KeyStruct<KeyType, DataType> *succkey = non_leaf->getsucc(i, &succk_leaf);
    if (predk_leaf->getnumchildren() > succk_leaf->getnumchildren()) {
        non_leaf->setkey(i, predkey);
        _remove_from_leaf(predk_leaf->getnumkeys() - 1, predk_leaf, false);
    }
    else {
        non_leaf->setkey(i, succkey);
        _remove_from_leaf(0, succk_leaf, false);
    }
    delete kstruct;
}

template <class KeyType, class DataType> void BTree<KeyType, DataType>::merge(BTreeNode<KeyType, DataType> *n)
{
    if (n == 0)
        return;

    // Special case: merging touches the root
    if (n == root || n->parent == 0) {
        root = n->getchild(0);
        root->parent = 0;
        root->idx = -1;
        --height;
        --numnodes;
        delete n;
        return;
    }

    BTreeNode<KeyType, DataType> *lsib = n->getlsibling();
    BTreeNode<KeyType, DataType> *rsib = n->getrsibling();
    // Try merging with right sibling
    if (lsib == 0 || rsib != 0 && rsib->getnumkeys() < lsib->getnumkeys()) {
        n->merge_right(rsib);
        --numnodes;
        // Recursively merge up if parent underflows
        if ((n->parent == root && n->parent->getnumkeys() == 0) ||
            (n->parent != root && n->parent->getnumkeys() < (m + 1) / 2 - 1)) {
            merge(n->parent);
        }
    }
    else {  // Else try merging into left sibling
        lsib->merge_right(n);
        --numnodes;
        if ((lsib->parent == root && lsib->parent->getnumkeys() == 0) ||
            (lsib->parent != root && lsib->parent->getnumkeys() < (m + 1) / 2 - 1)) {
            merge(lsib->parent);
        }
    }
}

template <class KeyType, class DataType> void BTree<KeyType, DataType>::split(BTreeNode<KeyType, DataType> *n)
{
    if (n == 0)
        return;

    BTreeNode<KeyType, DataType> *newnode = n->split();
    ++numnodes;
    // Special case: splitting touches the root
    if (n == root || n->parent == 0) {
        BTreeNode<KeyType, DataType> *newroot = new BTreeNode<KeyType, DataType>(n->removekey(n->getnumkeys() - 1));
        newroot->insertchild(0, n);
        newroot->insertchild(1, newnode);
        root = newroot;
        ++height;
        ++numnodes;
        return;
    }

    // Recursively split up if parent overflows
    if (n->parent->getnumkeys() > m - 1) {
        split(n->parent);
    }
}

template <class KeyType, class DataType>
int BTree<KeyType, DataType>::_search(KeyType k, BTreeNode<KeyType, DataType> **dest)
{
    // Exceptions
    if (dest == 0)
        return -1;
    *dest = 0;
    if (root == 0)
        return -1;

    BTreeNode<KeyType, DataType> *current = root;
    while (current->getnumchildren() > 0) {  // While current is not a leaf
        int idx = current->findkeyidx(k);
        // Found key
        if (idx < current->getnumkeys() && current->getkey(idx)->key == k) {
            *dest = current;
            return idx;
        }
        current = current->getchild(idx);
    }
    *dest = current;
    return current->findkeyidx(k);
}

template <class KeyType, class DataType> AList<DataType> *BTree<KeyType, DataType>::find(KeyType k)
{
    BTreeNode<KeyType, DataType> *dest = 0;
    int idx = _search(k, &dest);
    if (idx < 0 || idx >= dest->getnumkeys())
        return 0;
    AList<DataType> *orig_list = dest->getkey(idx)->data;
    // Create a new list to return, since the original list
    // in the key struct can be deleted when the key is removed
    AList<DataType> *result = new AList<DataType>();
    for (int i = 0; i < orig_list->getlength(); ++i) {
        result->append(orig_list->getitem(i));
    }
    return result;
}

template <class KeyType, class DataType> void BTree<KeyType, DataType>::print()
{
    if (root == 0 || root->getnumkeys() == 0) {
        cout << "Empty tree" << endl;
    }
    else {
        cout << "Tree: " << this << endl;
        cout << "m: " << m << endl;
        cout << "height: " << height << endl;
        cout << "numnodes: " << numnodes << endl;
        cout << "numkeys: " << numkeys << endl;
        _print_recurse(root, 1);
    }
    cout << "------------------------------------------------------" << endl;
}

template <class KeyType, class DataType>
void BTree<KeyType, DataType>::_print_recurse(BTreeNode<KeyType, DataType> *n, unsigned long long offset)
{
    if (n == 0)
        return;
    int offset_cpy = offset;
    while (offset_cpy != 1) {
        if (offset_cpy & 1) {
            cout << "| ";
        }
        else {
            cout << "  ";
        }
        offset_cpy >>= 1;
    }
    if (n->parent) {
        unsigned long long mask = 1;
        if (n->idx == n->parent->getnumchildren() - 1) {
            cout << "└ ";
            if (n->getnumchildren() > 0) {
                offset_cpy = offset;
                while (offset_cpy != 1) {
                    offset_cpy >>= 1;
                    mask <<= 1;
                }
                offset_cpy <<= 1;
                mask >>= 1;
                while (mask != 0) {
                    offset_cpy <<= 1;
                    offset_cpy += ((mask & offset) != 0);
                    mask >>= 1;
                }
            }
        }
        else {
            cout << "├ ";
            if (n->getnumchildren() > 0) {
                offset_cpy = offset;
                while (offset_cpy != 1) {
                    offset_cpy >>= 1;
                    mask <<= 1;
                }
                offset_cpy <<= 1;
                offset_cpy += 1;
                mask >>= 1;
                while (mask != 0) {
                    offset_cpy <<= 1;
                    offset_cpy += ((mask & offset) != 0);
                    mask >>= 1;
                }
            }
        }
    }
    for (int j = 0; j < n->getnumkeys(); ++j) {
        cout << n->getkey(j)->key << " ";
    }
    cout << endl;
    if (n->getnumchildren() > 0) {
        for (int i = 0; i < n->getnumchildren(); ++i) {
            _print_recurse(n->getchild(i), offset_cpy);
        }
    }
}

#endif
