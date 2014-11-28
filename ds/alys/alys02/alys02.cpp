#include <iostream>
using namespace std;

template <class ElemType>
class BSTree
{
private:
    class BSTNode
    {
    public:
        BSTNode* left;
        BSTNode* right;
        ElemType data;
        BSTNode() :left(NULL), right(NULL) {}
        BSTNode(ElemType _data) :data(_data), left(NULL), right(NULL) {}
    };
    typedef BSTNode* NodeP;
    NodeP root;

public:
    BSTree() :root(NULL) {}
    ~BSTree() { delete_node(root); }
    bool empty() const { return root == NULL; }
    void insert(const ElemType &_data) { insert_node(root, _data); }
    void inorder(ostream &out) const { inorder_node(out, root); }

protected:

    inline void delete_node(NodeP _node)
    {
        if (_node->left != NULL) {
            delete_node(_node->left);
        } else if (_node->right != NULL) {
            delete_node(_node->right);
        } else if (_node != NULL) {
            delete _node;
            _node = NULL;
        }
    }

    inline void insert_node(NodeP &_root, const ElemType &_data)
    {
        if (_root == NULL) {
            _root = new BSTNode(_data);
        } else if (_data <= _root->data) {
            /* 小的放左边 */
            insert_node(_root->left, _data);
        } else {
            /* 大的放右边 */
            insert_node(_root->right, _data);
        }
    }

    inline void inorder_node(ostream &out, NodeP _root) const
    {
        if (_root != NULL) {
            inorder_node(out, _root->left); // 左
            out << _root->data << " "; // 根
            inorder_node(out, _root->right);// 右
        }
    }
};

int main()
{
    BSTree<int> T;
    int n, a;
    
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> a;
        T.insert(a);
    }
    T.inorder(cout);
    cout << endl;

    return 0;
}
