#!/bin/bash
g++ -Wall -o main main.cpp repl.cpp Parser/parser.cpp Object/object.cpp Evaluator/evaluator.cpp Ast/ast.cpp Lexer/lexer.cpp Lexer/token.cpp Object/environment.cpp
