#include <stdlib.h>
#include "program.h"
#include "raylib.h"

#define P_STR(...) #__VA_ARGS__
#define STR(a) P_STR(a)

#if defined(__EMSCRIPTEN__)
    #include <emscripten/emscripten.h>
#endif

#if defined(DYNAMIC_LIB)
    #include <dlfcn.h>
#endif

#include "reload.h"

const int initialScreenWidth = 1280;
const int initialScreenHeight = 720;

ReloadCallbacks calls;
void* handle = NULL;

bool editorMode = false;

int main(void) {
    #if defined(DYNAMIC_LIB)
        char* shared[2] = { "./build/game.so","./build/editor.so" };
        char* dataFiles[2] = { "./assets/runtime_game_data","./assets/game_data" };
        char* buildCommands[2] = { "make reload-lib", "make editor-reload-lib" };
        calls.names[RELOAD_CALLBACK_INIT] = "Init";
        calls.names[RELOAD_CALLBACK_INIT_STATE] = "InitState";
        calls.names[RELOAD_CALLBACK_UPDATE] = "UpdateDrawFrame";
        calls.names[RELOAD_CALLBACK_FREE] = "Free";
        calls.names[RELOAD_CALLBACK_SAVE] = "Save";
        calls.names[RELOAD_CALLBACK_LOAD] = "Load";
        int r = ReloadCode(shared[editorMode], &calls);
        if(r) return r;
    #else 
        calls.init = Init;
        calls.initState = InitState;
        calls.update = UpdateDrawFrame;
        calls.free = Free;
        calls.save = Save;
        calls.load = Load;
    #endif
    InitWindow(initialScreenWidth, initialScreenHeight, "clamber game");
    calls.init();
    calls.initState();

    #if defined(__EMSCRIPTEN__)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
    SetTargetFPS(60);   

    while (!WindowShouldClose())    
    {
        #if defined(DYNAMIC_LIB)
            if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) {
                editorMode = !editorMode;
                int err = system(buildCommands[editorMode]);
                if(!err) {
                    calls.save("./assets/runtime_game_data");
                    int r = ReloadCode(shared[editorMode], &calls);
                    if(r) return r;
                    calls.free();
                    calls.init();
                    calls.load("./assets/runtime_game_data");
                }
            }
            if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_P)) {
                calls.free();
                calls.init();
                calls.load("./assets/game_data");
            }
            if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
                calls.free();
                calls.init();
                calls.load("./assets/game_data");
            }
            if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
                calls.save(dataFiles[editorMode]);
            }
            if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) {
                int err = system(buildCommands[editorMode]);
                if(!err) {
                    calls.save("./assets/runtime_game_data");
                    int r = ReloadCode(shared[editorMode], &calls);
                    if(r) return r;
                    calls.free();
                    calls.init();
                    calls.load("./assets/runtime_game_data");
                }
            }
        #endif
        calls.update();
    }
    #endif
    CloseWindow();        
    calls.free();

    #if defined(DYNAMIC_LIB)
        if(handle) {
            dlclose(handle);
        }
    #endif

    return 0;
}
