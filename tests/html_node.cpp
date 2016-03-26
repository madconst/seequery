#include "catch.hpp"
#include "../core/html_node.h"
#include "../core/node.h"

using SeeQuery::HtmlNode;
using SeeQuery::Node;

TEST_CASE("Set/get next/previous siblings", "[html_node][sibling]")
{
    // Create a node:
    std::unique_ptr<HtmlNode> node{new HtmlNode("node")};
    REQUIRE(node->nextSibling() == nullptr);
    REQUIRE(node->prevSibling() == nullptr);
    REQUIRE(node->serialize() == "<node/>");

    // Insert next sibling:
    node->nextSibling(new HtmlNode("first_sibling"));
    REQUIRE(node->nextSibling()->serialize() == "<first_sibling/>");
    REQUIRE(node->prevSibling() == nullptr);

    // Insert one more sibling:
    auto s1 = node->nextSibling();
    s1->nextSibling(new HtmlNode("second_sibling"));
    auto s2 = s1->nextSibling();
    REQUIRE(s2->serialize() == "<second_sibling/>");
    REQUIRE(s1->prevSibling()->serialize() == "<node/>");

    SECTION("Inserting in the middle with `nextSibling()`")
    {
        s1->nextSibling(new HtmlNode("after_1"));
        auto after_1 = s1->nextSibling();
        REQUIRE(s1->nextSibling()->serialize() == "<after_1/>");
        REQUIRE(s2->prevSibling()->serialize() == "<after_1/>");
        REQUIRE(after_1->nextSibling() == s2);
        REQUIRE(after_1->prevSibling() == s1);
    }
    SECTION("Inserting in the end with `nextSibling()`")
    {
        s2->nextSibling(new HtmlNode("after_2"));
        auto after_2 = s2->nextSibling();
        REQUIRE(s2->nextSibling()->serialize() == "<after_2/>");
        REQUIRE(after_2->nextSibling() == nullptr);
        REQUIRE(after_2->prevSibling() == s2);
    }
    SECTION("Inserting in the middle with `prevSibling()`")
    {
        s2->prevSibling(new HtmlNode("between_1_and_2"));
        auto between_1_and_2 = s2->prevSibling();
        REQUIRE(between_1_and_2->serialize() == "<between_1_and_2/>");
        REQUIRE(between_1_and_2->prevSibling()->serialize() == "<first_sibling/>");
        REQUIRE(between_1_and_2->nextSibling()->serialize() == "<second_sibling/>");
        REQUIRE(s1->nextSibling()->serialize() == "<between_1_and_2/>");
        REQUIRE(s2->prevSibling()->serialize() == "<between_1_and_2/>");
    }
    SECTION("Inserting in the beginning with `prevSibling()`")
    {
        node->prevSibling(new HtmlNode("before_node"));
        auto before_node = node->prevSibling();
        REQUIRE(before_node != nullptr);
        REQUIRE(before_node->prevSibling() == nullptr);
        REQUIRE(before_node->nextSibling() == node.get());
    }
}
TEST_CASE("Set/get parent/child", "[html_node][parent_child]")
{
    // Create a node:
    std::unique_ptr<HtmlNode> node{new HtmlNode("node")};
    REQUIRE(node->nextSibling() == nullptr);
    REQUIRE(node->prevSibling() == nullptr);
    REQUIRE(node->serialize() == "<node/>");

    // Append 1st child:
    HtmlNode* child1 = new HtmlNode("child1");
    node->append(child1);
    REQUIRE(node->firstChild() == child1);
    REQUIRE(node->lastChild() == child1);
    REQUIRE(node->firstChild()->prevSibling() == nullptr);
    REQUIRE(node->firstChild()->nextSibling() == nullptr);
    REQUIRE(node->firstChild()->firstSibling() == child1);
    REQUIRE(node->firstChild()->lastSibling() == child1);

    // Append 2nd child:
    HtmlNode* child2 = new HtmlNode("child2");
    node->append(child2);
    REQUIRE(node->firstChild() == child1);
    REQUIRE(node->lastChild() == child2);

    // Check relations between children:
    REQUIRE(child1->prevSibling() == nullptr);
    REQUIRE(child1->nextSibling() == child2);
    REQUIRE(child2->nextSibling() == nullptr);
    REQUIRE(child2->prevSibling() == child1);
    REQUIRE(child1->parent() == node.get());
    REQUIRE(child2->parent() == node.get());

    // Prepend 1 child:
    HtmlNode* child0 = new HtmlNode("child0");
    node->prepend(child0);
    // Check relations:
    REQUIRE(child0->parent() == node.get());
    REQUIRE(child0->prevSibling() == nullptr);
    REQUIRE(child0->nextSibling() == child1);
    REQUIRE(child1->prevSibling() == child0);
    REQUIRE(child1->nextSibling() == child2);
    REQUIRE(child2->nextSibling() == nullptr);
    REQUIRE(child2->prevSibling() == child1);
}
TEST_CASE("Get elements by tag name", "[html_node][get_by_tag]")
{
    std::unique_ptr<HtmlNode> node{new HtmlNode("some_tag")};
    node->append(new HtmlNode("some_other_tag"));
    node->append(new HtmlNode("some_other_tag"));
    node->append(new HtmlNode("yet_another_tag"));
    REQUIRE(node->getElementsByTagName("some_other_tag").size() == 2);
    REQUIRE(node->getElementsByTagName("yet_another_tag").size() == 1);
    REQUIRE(node->getElementsByTagName("non_existing_tag").empty());
}
TEST_CASE("Get elements by class name", "[html_node][get_by_class]")
{
    std::unique_ptr<HtmlNode> node(new HtmlNode("some_tag", {
        {"class", "some class"}, // TODO: spaces are not supported
        {"id", "42"}
    }));
    node->append(new HtmlNode("some_other_tag", {
        {"class", "foo"}
    }));
    node->append(new HtmlNode("some_other_tag", {
        {"class", "bar"}
    }));
    node->append(new HtmlNode("yet_another_tag", {
        {"class", "foo"}
    }));
    REQUIRE(node->getElementsByClassName("foo").size() == 2);
    REQUIRE(node->getElementsByClassName("bar").size() == 1);
    REQUIRE(node->getElementsByClassName("non-existing-class").empty());
}
TEST_CASE("Get element by id", "[html_node][get_by_id]")
{
    std::unique_ptr<HtmlNode> node(new HtmlNode("some_tag", {
        {"class", "some class"}, // TODO: spaces are not supported
        {"id", "element42"}
    }));
    node->append(new HtmlNode("some_other_tag", {
        {"class", "foo"},
        {"id", "element43"}
    }));
    node->append(new HtmlNode("some_other_tag", {
        {"class", "bar"},
        {"id", "element44"}
    }));
    node->append(new HtmlNode("yet_another_tag", {
        {"class", "foo"},
        {"id", "element45"}
    }));
    REQUIRE(node->getElementById("element42"));
    REQUIRE(node->getElementById("element43"));
    REQUIRE(node->getElementById("element44"));
    REQUIRE(node->getElementById("element45"));
    REQUIRE(node->getElementById("element46") == nullptr);
}
TEST_CASE("Deep copy", "[html_node][deep_copy]")
{
    HtmlNode node("root");
    node.append(new HtmlNode("child1"));
    node.append(new HtmlNode("child2"));
    REQUIRE(node.getChildren().size() == 2);

    SECTION("Copy constructor")
    {
        HtmlNode copy(node);
        REQUIRE(copy.getChildren().size() == 2);
    }
    SECTION("Copy assignment operator")
    {
        HtmlNode copy("copy");
        REQUIRE(copy.getChildren().empty());
        copy = node;
        REQUIRE(copy.getChildren().size() == 2);
    }
}
TEST_CASE("Detaching a node", "[html_node][detach]")
{
    std::unique_ptr<HtmlNode> node(new HtmlNode("root"));
    node->append(new HtmlNode("child1"));
    node->append(new HtmlNode("child2"));
    node->append(new HtmlNode("child3"));
    REQUIRE(node->getChildren().size() == 3);

    SECTION("Detaching the 1st child")
    {
        Node* first_child = node->firstChild();
        first_child->detach();
        REQUIRE(node->getChildren().size() == 2);
        REQUIRE(node->firstChild() != first_child);
        REQUIRE(first_child->parent() == nullptr);
        REQUIRE(first_child->nextSibling() == nullptr);
        REQUIRE(first_child->prevSibling() == nullptr);
        delete first_child;
    }
    SECTION("Detaching the 2nd child")
    {
        Node* second_child = node->firstChild()->nextSibling();
        second_child->detach();
        REQUIRE(node->getChildren().size() == 2);
        REQUIRE(second_child->parent() == nullptr);
        REQUIRE(second_child->nextSibling() == nullptr);
        REQUIRE(second_child->prevSibling() == nullptr);
        delete second_child;
    }
}
TEST_CASE("Reattaching a node", "[html_node][reattach]")
{
    std::unique_ptr<HtmlNode> node(new HtmlNode("root"));
    node->append(new HtmlNode("child1", {
        {"class", "child"}
    }));
    node->append(new HtmlNode("child2", {
        {"class", "child"}
    }));
    node->append(new HtmlNode("child3", {
        {"class", "child"}
    }));
    REQUIRE(node->getElementsByClassName("child").size() == 3);

    auto child2_list = node->getElementsByTagName("child2");
    REQUIRE(child2_list.size() == 1);
    Node* child2 = child2_list.front();
    child2->detach();
    REQUIRE(node->getElementsByClassName("child").size() == 2);
    node->append(child2);
    REQUIRE(node->getElementsByClassName("child").size() == 3);
}
TEST_CASE("Performance test: appending/prepending items in a cycle", "[html_node][performance]")
{
    const size_t NUM_OF_ITEMS = 100 * 1000;
    HtmlNode node("root");
    SECTION("Appending items")
    {
        for (size_t i = 0; i < NUM_OF_ITEMS; ++i) {
            node.append(new HtmlNode(std::to_string(i)));
        }
        REQUIRE(node.getChildren().size() == NUM_OF_ITEMS);
    }
    SECTION("Prepending items")
    {
        for (size_t i = 0; i < NUM_OF_ITEMS; ++i) {
            node.prepend(new HtmlNode(std::to_string(i)));
        }
        REQUIRE(node.getChildren().size() == NUM_OF_ITEMS);
    }
}