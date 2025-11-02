#ifndef H_COMPONENT_T
#define H_COMPONENT_T

#include "component.h"

typedef struct Test {
    float param;
} Test;

#define COMPONENTS(init)\
    init(Test, TEST)\

#define TEST_UI_FIELDS(X)\
    X(0,param,FLOAT_VAR,0,200)

#define TAGS(init)\
    init(ORBIT_CENTER)\

GEN_COMPONENT_TABLE(COMPONENTS, TAGS)

#endif
