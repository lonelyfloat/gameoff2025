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
const int screenWidth = 800;
const int screenHeight = 450;
int size;

const float distance = 10;
const float speed = 20;

void Init(void) {
    printf("Initializing game...\n");
    InitECS(&ecs, 100, COMPONENT_COUNT);
    InitComponentBlocks(&ecs);

    Font f = GetFontDefault();
    size = f.baseSize;
    printf("%d\n", size);
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
        DrawText("INSERT GAME HERE", screenWidth - 0.5*MeasureText("INSERT GAME HERE", 20), screenHeight - 10,20, LIGHTGRAY);
    EndDrawing();
}

void Free() {
    printf("Freeing game...\n");
    FreeECS(&ecs);
    FreeScene(&firstScene);
}
