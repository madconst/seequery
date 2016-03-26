#ifndef _HTML_NODE_H
#define _HTML_NODE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include "node.h"

namespace SeeQuery
{
    struct Attribute
    {
        Attribute(const std::string& k, const std::string& v) :
            key(k),
            value(v)
        {}
        Attribute(const std::string& k, const char* v) :
            key(k),
            value(v)
        {}
        template <class T>
        Attribute(const std::string& k, T v) :
            key(k),
            value(std::to_string(v))
        {}
        operator std::pair<std::string,std::string>()
        {
            return std::make_pair(key, value);
        }
        std::string key;
        std::string value;
    };

    class HtmlNode: public Node
    {
    public:
        HtmlNode(const std::string& tag_name, std::initializer_list<Attribute> attributes = {});
        HtmlNode(const HtmlNode& other);
        HtmlNode& operator=(const HtmlNode& other);

        Node* getElementById(const std::string& id);
        std::list<Node*> getElementsByTagName(const std::string& tag_name);
        std::list<Node*> getElementsByClassName(const std::string& class_name);

        Node* clone() const;

        std::string serialize(size_t depth = 0) const;
        std::string tagName() const;
        std::string text() const;
        std::string html() const;

        std::string attr(const std::string& key) const;
        void attr(const std::string& key, const std::string& value);

        Node* append(Node* child);
        Node* prepend(Node* child);
        void attr(Attribute attr);
    protected:
        std::string tag_name_;
        std::unordered_map<std::string, std::string> attributes_;
    };
}

#endif // _HTML_NODE_H
