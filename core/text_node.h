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

        Node* getElementById(const std::string&);
        std::list<Node*> getElementsByTagName(const std::string&);
        std::list<Node*> getElementsByClassName(const std::string&);

        Node* clone() const;

        std::string serialize(size_t depth = 0) const;
        std::string text() const;
        std::string html() const;

        std::string attr(const std::string&) const;
        void attr(const std::string&, const std::string&);

        Node* append(Node*);
        Node* prepend(Node*);

    private:
        std::string text_;
    };
}

#endif // _TEXT_NODE_H
