#ifndef _HTML_DOM_H
#define _HTML_DOM_H

#include <string>
#include "html_node.h"

namespace SeeQuery
{
    class Dom: public HtmlNode
    {
    public:
        Dom();
        std::string serialize(size_t depth = 0) const;
    private:
        std::string doctype;
    };
}

#endif // _HTML_DOM_H