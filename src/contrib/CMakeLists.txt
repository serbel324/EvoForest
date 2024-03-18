include(FetchContent)
# SFML

set(SFML_STATIC_LIBRARIES TRUE)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
)

add_subdirectory(SFML)
