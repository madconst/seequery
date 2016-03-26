#include "dom.h"
#include "text_node.h"

namespace SeeQuery
{
    Dom::Dom() :
        HtmlNode("html"),
        doctype("<!DOCTYPE html>")
    {
        append(new HtmlNode("head"));
        append(new HtmlNode("body"));
    }

    std::string Dom::serialize(size_t depth /*= 0*/) const
    {
        return doctype + "\n"
            + HtmlNode::serialize(depth);
    }
}
