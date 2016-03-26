#include <string>
#include "text_node.h"

namespace SeeQuery
{
    TextNode::TextNode(const std::string& text) :
        text_(text)
    {}
    std::string TextNode::serialize(size_t depth /*= 0*/) const
    {
        return std::string(depth * INDENT_WIDTH, ' ') + text_;
    }
    std::string TextNode::text() const
    {
        return text_;
    }
    std::string TextNode::html() const
    {
        return serialize();
    }
    std::string TextNode::attr(const std::string& key) const
    {
        return std::string(); // return empty string
    }
    void TextNode::attr(const std::string& key, const std::string& value)
    { /* Do nothing. TextNode ignores setting attribute */ }
    Node* TextNode::getElementById(const std::string& id)
    {
        return nullptr;
    }
    std::list<Node*> TextNode::getElementsByTagName(const std::string& tag_name)
    {
        return std::list<Node*>();
    }
    std::list<Node*> TextNode::getElementsByClassName(const std::string& class_name)
    {
        return std::list<Node*>();
    }
    Node* TextNode::clone() const
    {
        return new TextNode(text_);
    }
    Node* TextNode::append(Node* child)
    {
        // Do nothing. TextNode ignores appending.
        return this; 
    }
    Node* TextNode::prepend(Node* child)
    {
        // Do nothing. TextNode ignores appending.
        return this; 
    }
}
