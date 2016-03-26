#ifndef _COLLECTION_H
#define _COLLECTION_H

#include <list>
#include <string>
#include <memory>
#include <sstream>
#include <regex>
#include "node.h"
#include "text_node.h"
#include "html_node.h"
#include "dom.h"

namespace SeeQuery
{
    class Collection
    {
    public:
        virtual ~Collection();
        Collection() = default;
        Collection(const Collection& other);
        Collection& operator=(const Collection& other);

        std::string serialize() const;

        Collection& append(const Collection& element);
        Collection& prepend(const Collection& element);
        Collection& after(const Collection& collection);
        Collection& before(const Collection& collection);

        Collection& remove();

        size_t size() const;

        Collection operator()(std::string selector, std::initializer_list<Attribute> attributes = {});
        Collection operator[](size_t index) const;
        Collection children() const;

        std::string attr(const std::string& key) const;
        Collection& attr(const std::string& key, const std::string& value);

        static size_t roots();

    protected:
        void push_back(Node* node);

    private:
        std::list<Node*> children_;

        /* Root reference count methods: */
        static void increment_root(Node* root);
        static void decrement_root(Node* root);
        static Node* get_root(Node* node);
        static std::unordered_map<Node*, int> root_count;
    };

    class SeeQuery: public Collection
    {
    public:
        SeeQuery();
    };

    std::ostream& operator<<(std::ostream& out, const Collection& collection);
}

#endif // _COLLECTION_H