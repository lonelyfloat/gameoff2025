#include "program.h"
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include "ecs.h"
#include "internal_ui.h"
#include "component_types.h"
#include "scene.h"
#include "systems.h"


ECS ecs = {};
Scene* currentScene;
Scene firstScene = (Scene){};
int screenWidth = 0;
int screenHeight = 0;
Rectangle entityWidgets;

Camera2D camera = (Camera2D) {
    .offset = (Vector2){},
    .rotation = 0,
    .target = (Vector2){},
    .zoom = 1.0
};

Entity entitySelected = NULL_ENTITY;
Entity dragEntity = NULL_ENTITY;

const int maxEntities = 100;

const Color foreground = (Color){0xcc,0xcc,0xcc,0xFF};
const Color midtone = (Color){0x50,0x50,0x50,0xFF};
const Color background = (Color){0x17,0x17,0x17,0xFF};

Font pixelOperator;
bool fontLoaded = false;

void Init(void) {
    printf("Initializing editor...\n");
    InitECS(&ecs, 100, COMPONENT_COUNT);
    InitComponentBlocks(&ecs);
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    entityWidgets = (Rectangle){screenWidth*(1000.0/1280), 0, screenWidth*(280.0/1280), screenHeight};
    GuiLoadStyle("assets/pinkdark/pinkdark.rgs");
    if(!fontLoaded) { 
        pixelOperator = LoadFont("assets/pinkdark/PixelOperator.ttf");
        fontLoaded = true;
    }
    GuiSetFont(pixelOperator);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 0);
    GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
}

Vector2 padding = (Vector2){10,5};
bool showEntityWidgets = false;


void InitState(void) {
    LoadSceneFromFile(&ecs, &firstScene, "assets/game_data");
    currentScene = &firstScene;
    pixelOperator = LoadFont("assets/pinkdark/PixelOperator.ttf");
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
    SelectEntity(&ecs, entityWidgets, &showEntityWidgets, &entitySelected, &dragEntity, &camera);

    // ---------------------------------------------------------------------------------
    // Draw
    // ---------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(background);
        Vector2 o = GetWorldToScreen2D((Vector2){0,0}, camera);

        Vector2 offset = (Vector2) {
            o.x - 32*camera.zoom*ceil(o.x/(32*camera.zoom)),
            o.y - 32*camera.zoom*ceil(o.y/(32*camera.zoom))};
        GuiGrid((Rectangle){
                offset.x, offset.y,
                screenWidth-offset.x, screenHeight-offset.y},
                "", 32 * camera.zoom, 1, NULL);

        BeginMode2D(camera);
        DrawRectangleLines(0,0,screenWidth,screenHeight, YELLOW);
        DrawDebugColliders(&ecs);
        for(int i = 0; i < ecs.blocks[POSITION_COMPONENT].count; ++i) {
            Color c = foreground;
            if(GetEntity(&ecs, POSITION_COMPONENT, i) == entitySelected) {
                c = PINK;
            }
            Vector2 p = IndexComponent(&ecs, Position, POSITION_COMPONENT, i);
            DrawCircleLinesV(p, 10, c);
            DrawLineV((Vector2){p.x-10, p.y}, (Vector2){p.x+10, p.y}, c);
            DrawLineV((Vector2){p.x, p.y-10}, (Vector2){p.x, p.y+10}, c);
        }
        EndMode2D();

        if(showEntityWidgets) {
            EntityWidget(&ecs, entityWidgets, screenWidth, screenHeight, padding, &entitySelected, &showEntityWidgets, background, midtone);
        }
    EndDrawing();
}

void Free() {
    printf("Freeing editor...\n");
    FreeECS(&ecs);
    FreeScene(&firstScene);
    FreeUIState();
}
