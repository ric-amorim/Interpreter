#include <cstdlib>
#include <iostream>
#include <string>
#include "Evaluator/evaluator.h"
#include <vector>

#define PROMPT ">> "

void printParserErrors(std::vector<std::string> errors){
    std::cout<<"Woops! We ran into some monkey business here!\n";
    std::cout<<" parser errors:\n";
    for(auto msg : errors)
        std::cout<<"\t"<<msg<<"\n";
}

void repl::start(void) noexcept{
    auto s = std::unordered_map<std::string,object*>();
    environment* env = new environment(s,nullptr);
    for(;;){
        std::string str;

        std::cout<<PROMPT;
        std::getline(std::cin,str);
        
        lexer lex(str);
        std::vector<std::string> v;
        parser pars(lex,v);

        program* p = pars.parseProgram(); 
        if(pars.error().size() !=0){
            printParserErrors(pars.error());
            continue;
        }
        evaluator eval;
        auto evaluated = eval.eval(p,env);
        if(evaluated != nullptr){
        std::cout<<evaluated->inspect()<<std::endl;

        }


    }
    exit(EXIT_SUCCESS);
}
