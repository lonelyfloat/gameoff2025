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
    SaveSceneToFile(&ecs, &firstScene, file);
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
        DrawText("INSERT GAME HERE", screenWidth*0.5 - 0.5*MeasureText("INSERT GAME HERE", 20), screenHeight/2.0 - 10,20, LIGHTGRAY);
    EndDrawing();
}

void Free() {
    printf("Freeing game...\n");
    FreeECS(&ecs);
    FreeScene(&firstScene);
}
