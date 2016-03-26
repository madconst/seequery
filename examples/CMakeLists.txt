set (examples
    grid
    rects
)

foreach (example ${examples})
    add_executable (${example} ${example}.cpp)
    target_link_libraries (${example} seequery)
#    add_dependencies (examples ${example})
endforeach ()
