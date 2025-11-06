#ifndef H_COMPONENT_T
#define H_COMPONENT_T

#include "component.h"

typedef struct SceneFrame {
    float frameScale;
}SceneFrame; 

// init(
#define COMPONENTS(init)\
    init(SceneFrame, SCENE_FRAME)

#define SCENE_FRAME_UI_FIELDS(X)\
    X(0, frameScale, FLOAT_VAR, 0, 1)


#define TAGS(init)\
    init(FLOOR_COLLIDER)\

GEN_COMPONENT_TABLE(COMPONENTS, TAGS)

#endif
