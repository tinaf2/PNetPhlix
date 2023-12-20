#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
#include <iostream>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator(std::vector<ValueType>* values = nullptr)
        {
            m_values = values;
            m_index = 0;
        }

        ValueType& get_value() const
        {
            if (!is_valid())
                throw 1;
            return m_values->at(m_index);
        }

        bool is_valid() const
        {
//            if (m_values == nullptr)
//            {
//                std::cerr << "ERROR: No values were found in Iterator" << std::endl;
//                return false;
//            }
//            if (m_index >= m_values->size())
//            {
//                std::cerr << "ERROR: Index out of range for Iterator" << std::endl;
//                return false;
//            }
            if (m_values == nullptr || m_index >= m_values->size())
            {
                return false;
            }
            return true;
        }

        void advance()
        {
            if (!is_valid())
                throw 1;
            m_index++;
        }

      private:
        std::vector<ValueType>* m_values;
        int m_index;
    };

    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        deleteHelper(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        KeyType keyCopy = key;
        ValueType valueCopy = value;
        
        if (m_root == nullptr)
        {
            m_root = new Node(keyCopy);
            m_root->values.push_back(valueCopy);
            return;
        }
        
        Node* curr = m_root;
        for (;;)
        {
            if (keyCopy == curr->key)
            {
                curr->values.push_back(valueCopy);
                return;
            }
            if (keyCopy < curr->key)
            {
                if (curr->left != nullptr)
                    curr = curr->left;
                else
                {
                    curr->left = new Node(keyCopy);
                    curr->left->values.push_back(valueCopy);
                    return;
                }
            }
            else if (keyCopy > curr->key)
            {
                if (curr->right != nullptr)
                    curr = curr->right;
                else
                {
                    curr->right = new Node(keyCopy);
                    curr->right->values.push_back(valueCopy);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* curr = m_root;
        while (curr != nullptr)
        {
            if (key == curr->key)
                return Iterator(&(curr->values)); // TODO: Check if this is right
            else if (key < curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return Iterator();
    }

  private:
    struct Node
    {
        Node(const KeyType &val)
        {
            key = val;
            left = nullptr;
            right = nullptr;
        }
        KeyType key;
        std::vector<ValueType> values;
        Node *left, *right;
    };
    Node* m_root;
    
    void deleteHelper(Node* curr)
    {
        if (curr == nullptr)
            return;
        deleteHelper(curr->left);
        deleteHelper(curr->right);
        delete curr;
    }
};

#endif // TREEMULTIMAP_INCLUDED
