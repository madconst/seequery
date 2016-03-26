#include "node.h"

namespace SeeQuery
{
    Node::~Node()
    {
        if (parent_ == nullptr) {
            // If this is the root-level then recursively delete
            // children and siblings:
            delete first_child_;
            delete nextSibling();
        } else if (parent_->first_child_ == this) {
            // Otherwise delete children and siblings iteratively:
            Node* node = next_sibling_;
            while (node) {
                Node* current = node;
                node = node->next_sibling_;
                delete current->first_child_;
                delete current;
            }
            delete first_child_;
        }
    }
    std::list<Node*> Node::getChildren() const
    {
        std::list<Node*> result;
        Node* node = firstChild();
        while (node) {
            result.push_back(node);
            node = node->nextSibling();
        }
        return result;
    }
    Node* Node::parent() const
    {
        return parent_;
    }
    void Node::parent(Node* p)
    {
        parent_ = p;
    }
    Node* Node::nextSibling() const
    {
        return next_sibling_;
    }
    void Node::nextSibling(Node* s)
    {
        if (s == nullptr) {
            return; // do nothing
        }
        s->detach();
        s->prev_sibling_ = this;
        s->next_sibling_ = next_sibling_;
        if (next_sibling_) {
            // Insertion in the middle:
            next_sibling_->prev_sibling_ = s;
        } else {
            // Insertion at the end: need to update `prev_sibling_` of the 1st element:
            Node* first = firstSibling();
            first->prev_sibling_ = s;
        }
        next_sibling_ = s;
        // Set new parent:
        s->parent_ = parent_;
    }
    Node* Node::prevSibling() const
    {
        if (this == firstSibling()) {
            return nullptr;
        }
        return prev_sibling_;
    }
    void Node::prevSibling(Node* s)
    {
        if (s == nullptr) {
            return; // do nothing
        }
        s->detach();
        s->prev_sibling_ = prev_sibling_;
        s->next_sibling_ = this;

        if (isFirst()) { // insertion into the beginning
            if (parent_) {
                parent_->first_child_ = s;
            }
        } else { // insertion in the middle
            prev_sibling_->next_sibling_ = s;
        }

        prev_sibling_ = s;
        s->parent_ = parent_;
    }
    const Node* Node::firstSibling() const
    {
        if (parent_) {
            return parent_->first_child_;
        } else {
            const Node* last = lastSibling();
            const Node* first = this;
            while (first->prev_sibling_ != last) {
                first = first->prev_sibling_;
            }
            return first;
        }
    }
    Node* Node::firstSibling()
    {
        return const_cast<Node*>(
            static_cast<const Node&>(*this).firstSibling()
        );
    }
    void Node::firstSibling(Node* s)
    {
        Node* first = firstSibling();
        first->prevSibling(s);
    }
    const Node* Node::lastSibling() const
    {
        if (parent_) {
            return parent_->first_child_->prev_sibling_;
        } else {
            const Node* last = this;
            while (last->next_sibling_) {
                last = last->next_sibling_;
            }
            return last;
        }
    }
    Node* Node::lastSibling()
    {
        return const_cast<Node*>(
            static_cast<const Node&>(*this).lastSibling()
        );
    }
    void Node::lastSibling(Node* s)
    {
        Node* last = lastSibling();
        last->nextSibling(s);
    }
    Node* Node::firstChild() const
    {
        return first_child_;
    }
    void Node::firstChild(Node* node)
    {
        first_child_ = node;
    }
    Node* Node::lastChild() const
    {
        return first_child_->prev_sibling_;
    }
    bool Node::isFirst() const
    {
        // The first node always points back to the last one:
        return prev_sibling_->isLast();
    }
    bool Node::isLast() const
    {
        return next_sibling_ == nullptr;
    }
    Node* Node::detach()
    {
        Node* next = nextSibling();
        Node* prev = prevSibling();
        if (prev) {
            // If this is not the 1st child, then just connect `prev` with `next`:
            prev->next_sibling_ = next;
            if (next) {
                next->prev_sibling_ = prev;
            }
        } else {
            // If this is the 1st child, then change parent's pointer to the 1st child:
            if (parent_) {
                parent_->first_child_ = next;
            }
            // Detach this node from next:
            if (next) {
                next->prev_sibling_ = prev_sibling_;
            }
        }
        // Detach from parent and siblings:
        parent_ = nullptr;
        next_sibling_ = nullptr;
        prev_sibling_ = this;
        // And return the detached node:
        return this;
    }
    std::ostream& operator<<(std::ostream& out, const Node& node)
    {
        out << node.serialize();
        return out;
    }
}
