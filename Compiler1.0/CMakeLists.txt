add_executable(compiler
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

target_compile_features(compiler PRIVATE cxx_std_17)

target_include_directories(compiler PRIVATE src)