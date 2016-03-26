#include <iostream>
#include <sstream>
#include <random>
#include "core/collection.h"
#include "core/html_node.h"

const size_t WIDTH = 1600;
const size_t HEIGHT = 900;
const size_t NUM_OF_RECTS = 256;

std::random_device rd;
std::default_random_engine re(rd());
std::uniform_int_distribution<size_t> color_dist(0,255);

std::string random_color()
{
    std::ostringstream oss;
    oss << "rgb("
        << color_dist(re) << ", "
        << color_dist(re) << ", "
        << color_dist(re) << ")";
    return oss.str();
}

int main()
{
    using SeeQuery::SeeQuery;

    // Create document:
    SeeQuery $;
    // Add elements:
    $("body")
    .append($("<svg/>", {
        {"id", "chart"},
        {"width", WIDTH},
        {"height", HEIGHT}
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

    std::uniform_int_distribution<size_t> x_dist(0, WIDTH);
    std::uniform_int_distribution<size_t> y_dist(0, HEIGHT);
    std::uniform_int_distribution<size_t> w_dist(10, WIDTH / 5);
    std::uniform_int_distribution<size_t> h_dist(10, HEIGHT / 5);

    for (size_t i = 0; i < NUM_OF_RECTS; ++i) {
        size_t x = x_dist(re);
        size_t y = y_dist(re);
        size_t width = w_dist(re);
        size_t height = h_dist(re);

        std::ostringstream oss;
        oss << "fill: " << random_color() << "; "
            << "stroke: " << random_color() << "; "
            << "stroke-width: 1; "
            << "fill-opacity: 0.5; "
            << "stroke-opacity: 1.0;";

        svg.append($("<rect/>", {
            {"x", x},
            {"y", y},
            {"width", width},
            {"height", height},
            {"style", oss.str()}
        }));
    }

    std::cout << $ << std::endl;

}
