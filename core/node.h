#ifndef _NODE_H
#define _NODE_H

#include <string>
#include <list>
#include <memory>

namespace SeeQuery
{
    constexpr size_t INDENT_WIDTH = 2; // 2 spaces

    class Node
    {
    public:
        virtual ~Node();

        virtual Node* getElementById(const std::string& id) = 0;
        virtual std::list<Node*> getElementsByTagName(const std::string& tag_name) = 0;
        virtual std::list<Node*> getElementsByClassName(const std::string& class_name) = 0;
        virtual std::list<Node*> getChildren() const;

        virtual Node* clone() const = 0; /** Performs deep copy of the current node */

        virtual std::string serialize(size_t depth = 0) const = 0; /** Serialize the node */
        virtual std::string text() const = 0; /** Get test content of the node */
        virtual std::string html() const = 0; /** Get HTML content of the node */
        virtual std::string attr(const std::string& key) const = 0; /** Get attribute value for the given key */
        virtual void attr(const std::string& key, const std::string& value) = 0; /** Set attribute value for the given key */

        virtual Node* append(Node* child) = 0; /** Append child node */
        virtual Node* prepend(Node* child) = 0; /** Prepend child node */

        virtual Node* parent() const; /** Get parent of the current node. Return `nullptr` if the node is the root */
        virtual void parent(Node* p); /** Set parent of the current node */

        virtual Node* nextSibling() const; /** Get next sibling of this node */
        virtual void nextSibling(Node* s); /** Insert next sibling of this node */

        virtual Node* prevSibling() const; /** Get previous sibling of this node */
        virtual void prevSibling(Node* s); /** Set previous sibling of this node */

        virtual const Node* firstSibling() const; /** Get the first sibling of this node (const version) */
        virtual Node* firstSibling(); /** Get the first sibling of this node */
        virtual void firstSibling(Node* s); /** Set the first sibling of this node */

        virtual const Node* lastSibling() const; /** Get the last sibling of this node (const version) */
        virtual Node* lastSibling();  /** Get the last sibling of this node */
        virtual void lastSibling(Node* s); /** Set the last sibling of this node */

        virtual void firstChild(Node* c); /** Set the first child pointer of this node */
        virtual Node* firstChild() const; /** Get the first child of this node */
        virtual Node* lastChild() const; /** Get the last child of this node */

        virtual bool isFirst() const; /** Return true if this node is the first sibling, otherwise false */
        virtual bool isLast() const; /** Return true if this node is the last sibling, otherwise false */

        virtual Node* detach(); /** Detach this node from its emplacement */
    private:
        Node* parent_ = nullptr;
        Node* next_sibling_ = nullptr;
        // `prev_sibling` will always point to the last element in order to speed up appending:
        Node* prev_sibling_ = this;
        Node* first_child_ = nullptr;
    };

    std::ostream& operator<<(std::ostream& out, const Node& node);
}

#endif // _NODE_H
