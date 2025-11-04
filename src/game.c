#include "program.h"
#include <raylib.h>
#include <raymath.h>
#include "ecs.h"
#include "component_types.h"
#include "scene.h"
#include <stdio.h>

ECS ecs = {};
Scene* currentScene;
Scene firstScene = (Scene){};
const int maxEntities = 100;
int screenWidth = 800;
int screenHeight = 450;

Rectangle module = (Rectangle){20,20,300,200};
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
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
}

void InitState(void) {
    LoadSceneFromFile(&ecs, &firstScene, "assets/game_data");
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
        DrawSquircleLines(module, 12, 2);
    EndDrawing();
}


void Free() {
    printf("Freeing game...\n");
    FreeECS(&ecs);
    FreeScene(&firstScene);
}
