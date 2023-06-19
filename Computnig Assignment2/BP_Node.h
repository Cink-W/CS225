#ifndef BP_NODE_H
#define BP_NODE_H

const int M = 5;
const int MINNUM_KEY = M - 1;
const int MAXNUM_KEY = 2 * M - 1;
const int MINNUM_CHILD = MINNUM_KEY + 1;
const int MAXNUM_CHILD = MAXNUM_KEY + 1;
const int MINNUM_LEAVES = MINNUM_KEY;
const int MAXNUM_LEAVES = MAXNUM_KEY;

using namespace std;
// basic class
template <typename K, typename D> class Node
{
  public:
    Node();
    virtual ~Node();

    void set_type(int type);
    int get_type() const;
    void set_num(int num);
    int get_num() const;
    K get_key(int i) const;
    int get_key_index(K key) const;
    void set_key(int i, K new_value);

    // the functions below are defined for subclasses, so they are pure virtual
    virtual void clear() = 0;
    virtual void remove_key(int key_index, int child) = 0;
    virtual void split(Node<K, D> *parent, int child) = 0;
    virtual void merge(Node<K, D> *parent, Node<K, D> *child, int index) = 0;
    virtual void borrow(Node<K, D> *dest, Node<K, D> *parent, int key_index, int sibling) = 0;
    // 0 for left, 1 for right
    virtual int get_child_index(K key, int index) = 0;

  protected:
    int type;  // 0 for internal, 1 for leaf
    int key_num;
    K key_value[MAXNUM_KEY];
};

// class for internal nodes
template <typename K, typename D> class INode : public Node<K, D>
{
  public:
    INode();
    virtual ~INode();

    Node<K, D> *get_child(int i) const;
    void set_child(int i, Node<K, D> *child);
    void insert(int key_index, int child_index, K key, Node<K, D> *child);

    virtual void clear();
    virtual void remove_key(int key_index, int child);
    virtual void split(Node<K, D> *parent, int child);
    virtual void merge(Node<K, D> *parent, Node<K, D> *child, int index);
    virtual void borrow(Node<K, D> *dest, Node<K, D> *parent, int key_index, int sibling);
    // 0 for left, 1 for right
    virtual int get_child_index(K key, int index);

  private:
    Node<K, D> *children[MAXNUM_CHILD];
};

// class for leaf nodes
template <typename K, typename D> class LNode : public Node<K, D>
{
  public:
    LNode();
    virtual ~LNode();

    void set_lsibling(LNode<K, D> *node);
    LNode<K, D> *get_lsibling() const;
    void set_rsibling(LNode<K, D> *node);
    LNode<K, D> *get_rsibling() const;
    D get_data(int i);
    void set_data(int i, const D &data);
    D *get_data_addr(int i);

    void insert_main(K key, const D data);
    void insert(K key, const D data);
    virtual void clear();
    virtual void remove_key(int key_index, int child);
    virtual void split(Node<K, D> *parent, int child);
    virtual void merge(Node<K, D> *parent, Node<K, D> *child, int index);
    virtual void borrow(Node<K, D> *dest, Node<K, D> *parent, int key_index, int sibling);
    // 0 for left, 1 for right
    virtual int get_child_index(K key, int index);

    void pour();
    void sort();
    void overflow_ins(K key, D data);

  private:
    LNode<K, D> *lsibling;
    LNode<K, D> *rsibling;
    D data[MAXNUM_LEAVES];
    K overflow_keys[M / 2];
    D overflow_datas[M / 2];
    int overflow_size;
};
#endif