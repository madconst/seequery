#include "catch.hpp"
#include "../core/collection.h"
#include "../core/node.h"

#include <iostream> // del

TEST_CASE("Test selection by id/tag/class", "[collection][selection]")
{
    // Create document:
    SeeQuery::SeeQuery $;

    // Append elements:
    $("body")
    .append($("<p/>", {
        {"class", "my-class"},
        {"id", "unique-id"},
        {"text", "Some text"}
    }))
    .append($("<p/>", {
        {"class", "my-class"},
        {"id", "another-unique-id"},
        {"text", "Some other text"}
    }));

    REQUIRE($(".my-class").size() == 2);
    REQUIRE($("#unique-id").size() == 1);
    REQUIRE($("#another-unique-id").size() == 1);
    REQUIRE($("#non-existing-id").size() == 0);
    REQUIRE($("p").size() == 2);
    REQUIRE($("body").size() == 1);
    REQUIRE($("nothing").size() == 0);
}
TEST_CASE("Test collection appending", "[collection][append]")
{
    // Create document:
    SeeQuery::SeeQuery $;

    REQUIRE($("body").size() == 1);
    REQUIRE($(".container").size() == 0);

    // Append elements:
    $("body")
    .append($("<div/>", {
        {"class", "container"}
    }));
    REQUIRE($(".container").size() == 1);

    $(".container")
    .append($("<div/>", {
        {"class", "appeal"},
        {"id", "hello"},
        {"text", "Hello"}
    }))
    .append($("<div/>", {
        {"class", "appeal"},
        {"id", "goodbye"},
        {"text", "Goodbye"}
    }));
    REQUIRE($(".appeal").size() == 2);

    SECTION("Remove one div")
    {
        $("#hello").remove();
        REQUIRE($(".appeal").size() == 1);
    }

    SECTION("Append a div to all elements with class 'appeal'")
    {
        auto new_div = $("<div/>", {
            {"class", "new-div"},
            {"test", "Yo!"}
        });

        $(".appeal").append(new_div);
        REQUIRE($(".new-div").size() == 2);
    }
}
TEST_CASE("Inserting after", "[collection][after]")
{
    // Create document:
    SeeQuery::SeeQuery $;
    $("body")
    .append($("<div/>", {
        {"class", "container"},
        {"id", "one"}
    }))
    .append($("<div/>", {
        {"class", "container"},
        {"id", "two"}
    }));

    $(".container").after($("<p/>", {
        {"class", "after-container"},
        {"test", "I'm standing after '.container'"}
    }));

    auto body_children = $("body").children();

    REQUIRE(body_children.size() == 4);
    REQUIRE(body_children[0].attr("id") == "one");
    REQUIRE(body_children[1].attr("class") == "after-container");
    REQUIRE(body_children[2].attr("id") == "two");
    REQUIRE(body_children[3].attr("class") == "after-container");
}

TEST_CASE("Inserting before", "[collection][before]")
{
    // Create document:
    SeeQuery::SeeQuery $;
    $("body")
    .append($("<div/>", {
        {"class", "container"},
        {"id", "one"}
    }))
    .append($("<div/>", {
        {"class", "container"},
        {"id", "two"}
    }));

    $(".container").before($("<p/>", {
        {"class", "before-container"},
        {"test", "I'm standing before '.container'"}
    }));

    auto body_children = $("body").children();

    REQUIRE(body_children.size() == 4);
    REQUIRE(body_children[0].attr("class") == "before-container");
    REQUIRE(body_children[1].attr("id") == "one");
    REQUIRE(body_children[2].attr("class") == "before-container");
    REQUIRE(body_children[3].attr("id") == "two");
}

TEST_CASE("Root-reference count", "[collection][reference_count]")
{
    using SeeQuery::SeeQuery;

    // Create document:
    SeeQuery $;
    REQUIRE(SeeQuery::roots() == 1);

    auto container = $("<div/>", {
        {"class", "container"}
    });
    REQUIRE(SeeQuery::roots() == 2);

    $("body")
    .append(container);
    // container changed changed its root (was root, became embedded),
    // so it must be deleted from the root counter table
    REQUIRE(SeeQuery::roots() == 1);
}