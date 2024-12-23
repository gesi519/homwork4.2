#ifndef PARSER 
#define PARSER

// parser of myscheme 

#include "value.hpp"
#include "RE.hpp"
#include "Def.hpp"
#include "syntax.hpp"
#include "expr.hpp"
#include <map>
#include <cstring>
#include <iostream>
#define mp make_pair
using std :: string;
using std :: vector;
using std :: pair;

extern std :: map<std :: string, ExprType> primitives;
extern std :: map<std :: string, ExprType> reserved_words;

Expr Syntax :: parse(Assoc &env) {
    return ptr -> parse(env);//自动调用它指向的Syntaxbase
}

Expr Number :: parse(Assoc &env) {
    return Expr(new Fixnum(n));
}

Expr Identifier :: parse(Assoc &env) {
    return Expr(new Var(s));
}

Expr TrueSyntax :: parse(Assoc &env) {
    return Expr(new True());
}

Expr FalseSyntax :: parse(Assoc &env) {
    return Expr(new False());
}

//主
Expr List :: parse(Assoc &env) {
    int n = stxs.size();
    if(n==0) {
        return Expr(new Quote(Syntax(new List)));
    }
    if(List* first = dynamic_cast<List*>(stxs[0].get())) {
        std::vector<Expr> ex_v;
        for(int i = 1;i < stxs.size();++i) {
            ex_v.push_back(stxs[i]->parse(env));
        }
        return Expr(new Apply(first ->parse(env),ex_v));
    }
    Expr first = stxs[0]->parse(env);

    if(first->e_type == E_VAR) {
        Var* var = dynamic_cast<Var*>(first.get());
        if(var -> x == "quote") {
            if(n != 2) {
                throw RuntimeError("");
            }
            return Expr(new Quote(stxs[1]));
        }else if(var->x == "lambda") {
            if(n != 3) {
                throw RuntimeError("");
            }
            std::vector<std::string> v_str;
            if(List* list_ =dynamic_cast<List*>(stxs[1].get())) {
                for(int i = 0;i < list_->stxs.size();++i) {
                    if(Identifier* ide = dynamic_cast<Identifier*>(list_->stxs[i].get())) {
                        v_str.push_back(ide->s);
                        continue;
                    }
                    throw RuntimeError("");
                }
            }else if(Identifier* ide =dynamic_cast<Identifier*>(stxs[1].get())) {
                v_str.push_back(ide->s);
            }
            Expr ex = stxs[2]->parse(env);
            return Expr(new Lambda(v_str,ex));
        }else if(var->x == "begin") {
            std::vector<Expr> content;
            for(int i = 1;i < n;++i) {
                content.push_back(stxs[i].parse(env));
            }
            return Expr(new Begin(content));
        }else if(var->x == "let") {
            if(n != 3) {
                throw RuntimeError("");
            }
            std::vector<std::pair<std::string, Expr>> bind;
            if(List* list_l = dynamic_cast<List*>(stxs[1].get())) {
                int size = list_l->stxs.size();
                for(int i = 0;i < size; ++i) {
                    if(List* list_li = dynamic_cast<List*>(list_l->stxs[i].get())) {
                        int size_2 = list_li->stxs.size();
                        if(size_2 != 2) {
                            throw RuntimeError("");
                        }
                        if(Identifier* iden = dynamic_cast<Identifier*>(list_li->stxs[0].get())) {
                            bind.push_back({iden->s,list_li->stxs[1].parse(env)});
                        }else {
                            throw RuntimeError("");
                        }
                    }else {
                        throw RuntimeError("");
                    }
                }
            }else {
                throw RuntimeError("");
            }
            Expr body = stxs[2]->parse(env);
            return Expr(new Let(bind,body));
        }else if(var->x == "letrec") {
            if(n != 3) {
                throw RuntimeError("");
            }
            std::vector<std::pair<std::string, Expr>> bind;
            if(List* list_l = dynamic_cast<List*>(stxs[1].get())) {
                int size = list_l->stxs.size();
                for(int i = 0;i < size; ++i) {
                    if(List* list_li = dynamic_cast<List*>(list_l->stxs[i].get())) {
                        int size_2 = list_li->stxs.size();
                        if(size_2 != 2) {
                            throw RuntimeError("");
                        }
                        if(Identifier* iden = dynamic_cast<Identifier*>(list_li->stxs[0].get())) {
                            bind.push_back({iden->s,list_li->stxs[1].parse(env)});
                        }else {
                            throw RuntimeError("");
                        }
                    }else {
                        throw RuntimeError("");
                    }
                }
            }else {
                throw RuntimeError("");
            }
            Expr body = stxs[2]->parse(env);
            return Expr(new Letrec(bind,body));
        }else if(var->x == "if") {
            if(n != 4) {
                throw RuntimeError("");
            }
            std::vector<Expr> content;
            for(int i = 1;i < n;++i) {
                content.push_back(stxs[i].parse(env));
            }
            return Expr(new If(content[0],content[1],content[2]));
        }            
        std::vector<Expr> ex_v;
        for(int i = 1;i < stxs.size();++i) {
            ex_v.push_back(stxs[i]->parse(env));
        }
        return Expr(new Apply(first,ex_v));
    }

    throw RuntimeError("RuntimeError List");
}

#endif