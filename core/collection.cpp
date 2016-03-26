#include "collection.h"
#include "dom.h"

namespace SeeQuery
{
    Collection::Collection(const Collection& other)
    {
        for (auto child: other.children_) {
            push_back(child);
        }
    }
    Collection::~Collection()
    {
        for (auto child: children_) {
            decrement_root(get_root(child));
        }
    }
    Collection& Collection::operator=(const Collection& other)
    {
        if (this == &other) {
            return *this;
        }
        for (auto child: other.children_) {
            push_back(child);
        }
        return *this;
    }
    std::string Collection::serialize() const
    {
        std::ostringstream oss;
        for (auto& child: children_) {
            oss << child->serialize() << std::endl;
        }
        return oss.str();
    }
    Collection& Collection::append(const Collection& collection)
    {
        if (children_.empty()) {
            return *this;
        }
        auto last = --children_.end();
        // Append all elements from `collection` to every child of 
        // this collection, removing it from the source location.
        for (auto it = children_.begin(); it != last; ++it) {
            for (Node* new_child: collection.children_) {
                (*it)->append(new_child->clone()); // NEW, embeds
            }
        }
        // All copied, the latest moved:
        for (Node* new_child: collection.children_) {
            Node* old_root = get_root(new_child);
            (*last)->append(new_child);
            // new_child changed root:
            increment_root(get_root(new_child)); // increment new root
            decrement_root(old_root); // decrement old root
        }
        return *this;
    }
    Collection& Collection::prepend(const Collection& collection)
    {
        if (children_.empty()) {
            return *this;
        }
        auto last = --children_.end();
        // Append all elements from `collection` to every child of 
        // this collection, removing it from the source location.
        for (auto it = children_.begin(); it != last; ++it) {
            for (Node* new_child: collection.children_) {
                (*it)->prepend(new_child->clone());
            }
        }
        // All copied, the latest moved:
        for (Node* new_child: collection.children_) {
            Node* old_root = get_root(new_child);
            (*last)->prepend(new_child);
            // new_child changed root:
            increment_root(get_root(new_child)); // increment new root
            decrement_root(old_root); // decrement old root
        }
        return *this;
    }
    Collection& Collection::after(const Collection& collection)
    {
        if (children_.empty()) {
            return *this;
        }
        auto last = --children_.end();
        // All copied, the latest moved:
        for (auto it = children_.begin(); it != last; ++it) {
            for (Node* new_child: collection.children_) {
                (*it)->nextSibling(new_child->clone());
            }
        }
        for (Node* new_child: collection.children_) {
            Node* old_root = get_root(new_child);
            new_child->detach();
            (*last)->nextSibling(new_child);
            increment_root(get_root(new_child)); // increment new root
            decrement_root(old_root); // decrement old root
        }
        return *this;
    }
    Collection& Collection::before(const Collection& collection)
    {
        if (children_.empty()) {
            return *this;
        }
        auto last = --children_.end();
        // All copied, the latest moved:
        for (auto it = children_.begin(); it != last; ++it) {
            for (Node* new_child: collection.children_) {
                (*it)->prevSibling(new_child->clone());
            }
        }
        for (Node* new_child: collection.children_) {
            Node* old_root = get_root(new_child);
            new_child->detach();
            (*last)->prevSibling(new_child);
            increment_root(get_root(new_child)); // increment new root
            decrement_root(old_root); // decrement old root
        }
        return *this;
    }
    Collection& Collection::remove()
    {
        for (auto element: children_) {
            // remove the element from where it was:
            decrement_root(get_root(element));
            element->detach();
            increment_root(element);
        }
        return *this;
    }
    size_t Collection::size() const
    {
        return children_.size();
    }
    Collection Collection::operator()(std::string query, 
        std::initializer_list<Attribute> attributes/* = {}*/)
    {
        if (query.empty()) {
            return *this;
        }

        // Check if the query string is something like: '<tag/>' or '<tag></tag>':
        std::regex re_single_tag(R"(^<([\w-]+)\s*\/?>(?:<\/\1>|)$)");
        std::smatch single_tag_match;
        std::regex_match(query, single_tag_match, re_single_tag);
        // If it is, create new element:
        if (!single_tag_match.empty()) {
            Collection result;
            result.push_back(new HtmlNode(single_tag_match[1], attributes));
            return result;
        }

        std::string tag_name, id, class_name;

        std::regex re_quick_expr(R"(^(?:#([\w-]+)|(\w+)|\.([\w-]+))$)");
        std::smatch quick_expr_match;
        std::regex_match(query, quick_expr_match, re_quick_expr);

        if (quick_expr_match.size() >= 3) {
            std::string id = quick_expr_match[1];
            std::string tag_name = quick_expr_match[2];
            std::string class_name = quick_expr_match[3];
            Collection result;
            if (!id.empty()) {
                for (auto& element: children_) {
                    auto node = element->getElementById(id);
                    if (node) {
                        result.push_back(node);
                    }
                }
                return result;
            } else if (!tag_name.empty()) {
                for (auto& element: children_) {
                    for (Node* node: element->getElementsByTagName(tag_name)) {
                        result.push_back(node);
                    }
                }
                return result;
            } else if (!class_name.empty()) {
                for (auto& element: children_) {
                    for (Node* node: element->getElementsByClassName(class_name)) {
                        result.push_back(node);
                    }
                }
                return result;
            }
        }

        // If nothing has been found, return empty collection:
        Collection result;
        return result;
    }
    Collection Collection::operator[](size_t index) const
    {
        Collection result;
        if (index < children_.size()) {
            auto it = children_.begin();
            std::advance(it, index);
            result.push_back(*it);
        }
        return result;
    }
    Collection Collection::children() const
    {
        Collection result;
        for (auto node: children_) {
            for (auto child: node->getChildren()) {
                result.push_back(child);
            }
        }
        return result;
    }
    std::string Collection::attr(const std::string& key) const
    {
        if (children_.empty()) {
            return std::string();
        }
        return children_.front()->attr(key);
    }
    Collection& Collection::attr(const std::string& key, const std::string& value)
    {
        for (auto node: children_) {
            node->attr(key, value);
        }
        return *this;
    }

    void Collection::push_back(Node* node)
    {
        children_.push_back(node);
        increment_root(get_root(node));
    }

    /* Static methods: */
    size_t Collection::roots()
    {
        return root_count.size();
    }
    void Collection::increment_root(Node* root)
    {
        ++root_count[root];
    }
    void Collection::decrement_root(Node* root)
    {
        if (root->parent()) {
            root_count.erase(root);
            return;
        }
        --root_count[root];
        if (root_count[root] == 0) {
            root_count.erase(root);
            delete root;
        }
    }
    Node* Collection::get_root(Node* node)
    {
        while (Node* p = node->parent()) {
            node = p;
        }
        return node;
    }

    std::unordered_map<Node*, int> Collection::root_count;

    SeeQuery::SeeQuery()
    {
        push_back(new Dom);
    }

    std::ostream& operator<<(std::ostream& out, const Collection& collection)
    {
        out << collection.serialize();
        return out;
    }
}