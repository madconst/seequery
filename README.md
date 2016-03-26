# SeeQuery

SeeQuery is a C++ mimic of jQuery, the most popular JavaScript library. SeeQuery is implemented in C++11 and takes advantage of its syntax features to make the code look very much like jQuery. Consider the following example:

```cpp
// Create document:
SeeQuery::SeeQuery $;

// Add elements:
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
    {"text", "I'm standing after '.container'"}
}));

std::cout << $ << std::endl;
```

While most parts of it look very similar to jQuery, this is a valid C++11 code.

SeeQuery will be useful for simple creation and manipulation of XHTML documents in C++ for those who have Web development background and are used to jQuery's syntax.

## Key Features

* automatic memory management (reference counting)
* support of most popular jQuery DOM selection and manipulation methods

## Build

```
git clone https://github.com/madconst/seequery
cd seequery
mkdir build && cd $_
cmake ..
make
make test
```

## Run examples

```
examples/rects > rects.html
```

Then open `rects.html` using any up-to-date Web browser. You will see something like this:

![Rects Example]
(https://raw.githubusercontent.com/madconst/seequery/master/rects_view_in_browser.png)