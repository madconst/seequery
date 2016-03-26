#include <list>
#include <algorithm>
#include "html_node.h"
#include "text_node.h"

namespace SeeQuery
{
    HtmlNode::HtmlNode(const std::string& tag_name, 
            std::initializer_list<Attribute> attributes) :
        tag_name_(tag_name)
    {
        for (auto& attr: attributes)
        {
            if (attr.key.compare("text") == 0) {
                append(new TextNode(attr.value));
            } else {
                attributes_.insert(std::make_pair(attr.key, attr.value));
            }
        }
    }
    HtmlNode::HtmlNode(const HtmlNode& other) :
        tag_name_(other.tag_name_)
    {
        if (&other == this) {
            return;
        }
        Node* child = other.firstChild();
        while (child) {
            append(child->clone());
            child = child->nextSibling();
        }
    }
    HtmlNode& HtmlNode::operator=(const HtmlNode& other)
    {
        if (&other == this) {
            return *this;
        }
        Node* first_child = other.firstChild();
        while (first_child) {
            append(first_child->clone());
            first_child = first_child->nextSibling();
        }
        return *this;
    }
    Node* HtmlNode::append(Node* child)
    {
        // Detach the child if it is already embedded somewhere:
        child->detach();
        child->parent(this);
        if (firstChild() == nullptr) {
            firstChild(child);
        } else {
            Node* node = lastChild();
            node->nextSibling(child);
        }
        return this;
    }
    Node* HtmlNode::prepend(Node* child)
    {
        // Detach the child if it is already embedded somewhere:
        child->detach();
        child->parent(this);
        Node* first_child = firstChild();
        if (first_child == nullptr) {
            firstChild(child);
        } else {
            first_child->prevSibling(child);
        }
        return this;
    }
    void HtmlNode::attr(Attribute attr)
    {
        attributes_.insert(std::make_pair(attr.key, attr.value));
    }
    std::string HtmlNode::serialize(size_t depth /*default: 0*/) const
    {
        std::string indent(depth * INDENT_WIDTH, ' ');
        std::ostringstream oss;
        oss << indent << "<" << tag_name_;
        for (auto& attr: attributes_) {
            oss << " " << attr.first << "=\"" << attr.second << "\"";
        }
        if (firstChild() == nullptr) {
            oss << "/>";
        } else {
            oss << ">" << std::endl;
            Node* node = firstChild();
            while (node) {
                oss << node->serialize(depth + 1) << std::endl;
                node = node->nextSibling();
            }
            oss << indent << "</" << tag_name_ << ">";
        }
        return oss.str();
    }
    std::string HtmlNode::tagName() const
    {
        return tag_name_;
    }
    std::string HtmlNode::text() const
    {
        std::ostringstream oss;
        Node* node = firstChild();
        while (node) {
            oss << node->serialize() << std::endl;
            node = node->nextSibling();
        }
        return oss.str();
    }
    std::string HtmlNode::html() const
    {
        return serialize();
    }
    std::string HtmlNode::attr(const std::string& key) const
    {
        auto it = attributes_.find(key);
        if (it != attributes_.cend()) {
            return it->second;
        }
        return "";
    }
    void HtmlNode::attr(const std::string& key, const std::string& value)
    {
        attributes_[key] = value;
    }
    Node* HtmlNode::getElementById(const std::string& id)
    {
        auto it = attributes_.find("id");
        if (it != attributes_.end() && id.compare(it->second) == 0) {
            return this;
        }
        Node* node = firstChild();
        while (node) {
            if (Node* result = node->getElementById(id)) {
                return result;
            }
            node = node->nextSibling();
        }
        return nullptr;
    }
    std::list<Node*> HtmlNode::getElementsByTagName(const std::string& tag_name)
    {
        std::list<Node*> result;
        // Check if this node tag name matches:
        if (tag_name_ == tag_name) {
            result.emplace_back(this);
            return result;
        }
        // Otherwise search in children:
        Node* node = firstChild();
        while (node) {
            result.splice(
                result.end(),
                node->getElementsByTagName(tag_name));
            node = node->nextSibling();
        }
        return result;
    }
    std::list<Node*> HtmlNode::getElementsByClassName(const std::string& class_name)
    {
        std::list<Node*> result;
        // Check if this node class matches:
        auto it = attributes_.find("class");
        if (it != attributes_.end() && class_name.compare(it->second) == 0) {
            result.push_back(this);
            return result;
        }
        // Otherwise, search in children:
        Node* node = firstChild();
        while (node) {
            result.splice(
                result.end(),
                node->getElementsByClassName(class_name));
            node = node->nextSibling();
        }
        return result;
    }
    Node* HtmlNode::clone() const
    {
        HtmlNode* copy = new HtmlNode(tag_name_);
        copy->attributes_ = attributes_;
        Node* node = firstChild();
        while (node) {
            copy->append(node->clone());
            node = node->nextSibling();
        }
        return copy;
    }
}