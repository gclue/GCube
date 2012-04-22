//
//  MathExpression.h
//  GCube
//
//  Created by  on 12/04/20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef GCube_MathExpression_h
#define GCube_MathExpression_h

#include <string>

class MathExpression {
private:
    std::string expressionStr;
    std::string curToken;
    int curPosision;
    
    std::string getToken();
    
    float expression();
    float term();
    float factor();
    float number();
public:
    MathExpression(std::string expression);
    ~MathExpression();
    
    float eval();
};

#endif
