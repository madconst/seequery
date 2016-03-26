#include <iostream>
#include "core/collection.h"
#include "core/html_node.h"

const size_t WIDTH = 800;
const size_t HEIGHT = 600;

int main()
{
    using SeeQuery::SeeQuery;
    // Create document:
    SeeQuery $;
    // Add elements:
    $("body")
    .append($("<svg/>", {
        {"id", "chart"},
        {"width", std::to_string(WIDTH)},
        {"height", std::to_string(HEIGHT)}
    }));

    $("head")
    .append($("<style/>", {
        {"text", R"(
            line {
                stroke: rgb(0,0,0);
                stroke-width: 1;
            }
        )"}
    }));

    auto svg = $("svg");

    for (size_t x = 0; x <= WIDTH; x += WIDTH / 20) {
        auto line = $("<line/>", {
            {"x1", std::to_string(x)},
            {"y1", "0"},
            {"x2", std::to_string(x)},
            {"y2", std::to_string(HEIGHT)}
        });
        svg.append(line);
    }
    for (size_t y = 0; y <= HEIGHT; y += HEIGHT / 20) {
        auto line = $("<line/>", {
            {"x1", "0"},
            {"y1", std::to_string(y)},
            {"x2", std::to_string(WIDTH)},
            {"y2", std::to_string(y)}
        });
        svg.append(line);
    }
    std::cout << $ << std::endl;

}
