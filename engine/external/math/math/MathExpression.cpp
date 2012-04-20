//
//  MathExpression.cpp
//  GCube
//
//  Created by  on 12/04/20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MathExpression.h"
#include <ctype.h>

MathExpression::MathExpression(std::string expression){
    this->expressionStr = expression;
    curPosision = 0;
}

MathExpression::~MathExpression(){
}

float MathExpression::eval(){
    std::string token;
    while(true){
        token = getToken();
        if(token.size() == 0) break;
        
        return expression();
        
    }
    
    return 0;
}

std::string MathExpression::getToken(){
    std::string::iterator it;
    
    it = expressionStr.begin();
    it += curPosision;
    
    int start = curPosision;
    int len = 0;
    
    while(*it == ' ' || *it == '\t') { //スペースは無視
        it++;
        start++;
    }
    if(*it == '\0'){//何もなければ空の文字列を返す
        std::string result = "";
        return result;
    }

    if(isalpha(*it)){//アルファベット
        do{
            it++;
            len++;
        }while(isalnum(*it) && *it !='\0');
    }
    else if(isdigit(*it)){
        do{
            it++;
            len++;
        }while((isdigit(*it) || *it == '.') && *it !='\0');
    }
    else {
        len++;
    }
    
    curPosision = start + len;
    
    curToken = expressionStr.substr(start, len);
    return curToken;
}

float MathExpression::expression(){
    float ret;
    
    ret = term();
    while(true){
        if(curToken.compare("+")==0){
            getToken();
            ret = ret + term();
        }else if(curToken.compare("-")==0){
            getToken();
            ret = ret - term();
        }else
            break;
    }
    return ret;
}

float MathExpression::term(){
    float ret;
    
    ret = factor();
    while(true){
        if(curToken.compare("*")==0){
            getToken();
            ret = ret * factor();
        }else if(curToken.compare("/")==0){
            getToken();
            float t = factor();
            if(t == 0){//0除算
                break;
            }
            ret = ret / t;
        }else
            break;
    }
    return ret;
}

float MathExpression::factor(){
    float ret;
    
    if(curToken.compare("(")==0){
        getToken();
        ret = expression();
        if(curToken.compare(")")!=0){
            // ) の数が合わない
        }
        getToken();
        return ret;
    }
    else{
        return number();
    }
}

float MathExpression::number() {
    float ret;
    std::string sign;
    
    if(curToken.compare("+")==0 || curToken.compare("-")==0){
        sign = curToken;
        getToken();
    }
    ret = atof(curToken.c_str());
    getToken();
    if(sign == "-"){
        return -ret;
    }
    else {
        return ret;
    }
}

