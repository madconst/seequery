#ifndef _TEXT_NODE_H
#define _TEXT_NODE_H

#include <string>
#include "node.h"

namespace SeeQuery
{
    class TextNode: public Node
    {
    public:
        TextNode(const std::string& text);

        Node* getElementById(const std::string& id);
        std::list<Node*> getElementsByTagName(const std::string& tag_name);
        std::list<Node*> getElementsByClassName(const std::string& class_name);

        Node* clone() const;

        std::string serialize(size_t depth = 0) const;
        std::string text() const;
        std::string html() const;

        std::string attr(const std::string& key) const;
        void attr(const std::string& key, const std::string& value);

        Node* append(Node* child);
        Node* prepend(Node* child);

    private:
        std::string text_;
    };
}

#endif // _TEXT_NODE_H
