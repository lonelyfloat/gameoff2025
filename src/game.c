#include "program.h"
#include <raylib.h>
#include <raymath.h>
#include "ecs.h"
#include "component_types.h"
#include "scene.h"
#include <stdio.h>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

ECS ecs = {};
Scene* currentScene;
Scene firstScene = (Scene){};
Scene minigames[1] = {};
const int maxEntities = 100;
int screenWidth = 1280;
int screenHeight = 720;

RenderTexture2D minigameTexture;
Texture2D textureMask;
Shader maskShader;
int maskTextureLoc;

const float scale = 0.7;
Rectangle module = (Rectangle){(1280/2.0) - (0.5*1280*scale),(720/2.0) - (0.5*720*scale),1280*scale,720*scale};
float radius = 80;
float thickness = 2;

void DrawSquircleLines(Rectangle rect, float roundness, float line) {
    DrawLineEx((Vector2){rect.x+roundness, rect.y}, (Vector2){rect.x+rect.width-roundness, rect.y}, line, LIGHTGRAY);
    DrawLineEx((Vector2){rect.x+roundness, rect.y+rect.height}, (Vector2){rect.x+rect.width-roundness, rect.y+rect.height}, line, LIGHTGRAY);
    DrawLineEx((Vector2){rect.x, rect.y+roundness}, (Vector2){rect.x, rect.y+rect.height-roundness}, line, LIGHTGRAY);
    DrawLineEx((Vector2){rect.x+rect.width, rect.y+roundness}, (Vector2){rect.x+rect.width, rect.y+rect.height-roundness}, line, LIGHTGRAY);
    DrawRing((Vector2){rect.x+roundness, rect.y+roundness}, roundness-line, roundness, 180, 270, 20, LIGHTGRAY);
    DrawRing((Vector2){rect.x+rect.width-roundness, rect.y+roundness}, roundness-line, roundness, 270, 360, 20, LIGHTGRAY);
    DrawRing((Vector2){rect.x+rect.width-roundness, rect.y+rect.height-roundness}, roundness-line, roundness, 0, 90, 20, LIGHTGRAY);
    DrawRing((Vector2){rect.x+roundness, rect.y+rect.height-roundness}, roundness-line, roundness, 90, 180, 20, LIGHTGRAY);
}

void Init(void) {
    printf("Initializing game...\n");
    InitECS(&ecs, 100, COMPONENT_COUNT);
    InitComponentBlocks(&ecs);
    printf("GLSL Version: %i\n", GLSL_VERSION);
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    Image imageMask = GenImageColor(screenWidth, screenHeight, BLACK);
    float roundness = 12;
    ImageDrawCircleV(&imageMask,(Vector2){module.x+roundness, module.y+roundness}, roundness, WHITE);
    ImageDrawCircleV(&imageMask,(Vector2){module.x+module.width-roundness, module.y+roundness}, roundness, WHITE);
    ImageDrawCircleV(&imageMask,(Vector2){module.x+module.width-roundness, module.y+module.height-roundness}, roundness, WHITE);
    ImageDrawCircleV(&imageMask,(Vector2){module.x+roundness, module.y+module.height-roundness}, roundness, WHITE);
    ImageDrawRectangleRec(&imageMask,(Rectangle){module.x, module.y+roundness,module.width, module.height-roundness*2}, WHITE);
    ImageDrawRectangleRec(&imageMask,(Rectangle){module.x+roundness, module.y,module.width-roundness*2, module.height}, WHITE);
    textureMask = LoadTextureFromImage(imageMask);
    UnloadImage(imageMask);
    maskShader = LoadShader(0, TextFormat("assets/shaders/glsl%i/minigame_mask.glsl", GLSL_VERSION));
    // Get an additional sampler2D location to be enabled on drawing
    maskTextureLoc = GetShaderLocation(maskShader, "texture1");
    SetShaderValueTexture(maskShader,maskTextureLoc, textureMask);
    minigameTexture = LoadRenderTexture(screenWidth, screenHeight);

}

void InitState(void) {
    LoadSceneFromFile(&ecs, &firstScene, "assets/game_data");
    DeactivateScene(&firstScene, &ecs);
    LoadSceneFromFile(&ecs, &minigames[0], "assets/minigame1");
    currentScene = &firstScene;
}

void Save(char* file) {
    SaveWorldAsScene(&ecs, &firstScene, file);
}

void Load(char* file) {
    LoadSceneFromFile(&ecs, &firstScene, file);
    currentScene = &firstScene;
}

void UpdateDrawFrame(void) {
    // Update
    //----------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------
    // Draw
    // ---------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(WHITE);
        BeginTextureMode(minigameTexture);
        ClearBackground(PINK);
        for(int i = 0; i < ecs.blocks[COLLIDER_COMPONENT].count; ++i) {
            Entity e = GetEntity(&ecs, COLLIDER_COMPONENT, i); 
            if(!HasComponent(&ecs, e, POSITION_COMPONENT)) {
                continue;
            }
            Vector2 offset = (Vector2){};
            float scale = 1;
            if(HasComponent(&ecs, e, SCENE_FRAME_COMPONENT)) {
                SceneFrame* sf = GetComponent(&ecs, e, SCENE_FRAME_COMPONENT);
                offset = (Vector2){(1280/2.0) - (0.5*1280*sf->frameScale),(720/2.0) - (0.5*720*sf->frameScale)};
                scale = sf->frameScale;
            }

            Position* p = GetComponent(&ecs, e, POSITION_COMPONENT);
            Collider* c = &IndexComponent(&ecs, Collider, COLLIDER_COMPONENT, i);
            switch(c->shape) {
                case RECT_COLLIDER: 
                    DrawRectangleV(Vector2Add(*p, offset), c->sizeData.bounds, BLUE);
                    break;
                case CIRCLE_COLLIDER: 
                    DrawCircleV(Vector2Add(*p, offset), c->sizeData.radius, GREEN);
                    break;
            }
        }
        EndTextureMode();

        BeginShaderMode(maskShader);
            SetShaderValueTexture(maskShader,maskTextureLoc, textureMask);
            DrawTextureRec(minigameTexture.texture, (Rectangle){0,0,minigameTexture.texture.width, -minigameTexture.texture.height}, (Vector2){0}, WHITE);
        EndShaderMode();
        DrawSquircleLines(module, 12, 2);
    EndDrawing();
}


void Free() {
    printf("Freeing game...\n");
    FreeECS(&ecs);
    FreeScene(&firstScene);
    UnloadShader(maskShader);
    UnloadRenderTexture(minigameTexture);
}
