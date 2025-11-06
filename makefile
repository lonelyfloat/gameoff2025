CFLAGS = -Os -std=c99 -Wall 
LDFLAGS = -lraylib 
LIBLOC =
DYNLIBLOC = 
PROJECT_NAME = game
EDITOR_NAME = editor
HOT_RELOAD_NAME = game
LDNORAY = -lGL -lm -lpthread -ldl -lrt -lX11
EDITOR_LIB_FILT = find . -name *.c -a ! -path "./src/game.c" -a ! -name main.c
GAME_LIB_FILT = find . -name *.c -a ! -path "./src/editor.c" -a ! -name main.c
FILES =

ifeq ($(OS),Windows_NT)
	FILES += $(shell dir /s /b src\*.c)
	FILES += $(shell dir /s /b framework\*.c)
	LDFLAGS += -opengl32 -lgdi32 -lwinmm 
	LIBLOC = -Llib/windows
else # Linux
	FILES += $(shell find . -name *.c -a ! -path "./src/editor.c")
	LDFLAGS +=  -lGL -lm -lpthread -ldl -lrt -lX11
	LIBLOC = -Llib/linux
	DYNLIBLOC = lib/linux/shared
endif

all:
	gcc $(FILES) -o build/$(PROJECT_NAME) $(CFLAGS) -DPLATFORM_DESKTOP -Iinclude -Isrc -Iframework $(LIBLOC) $(LDFLAGS)

editor-reload-lib: 
	gcc -shared -o build/$(EDITOR_NAME).so $(shell $(EDITOR_LIB_FILT)) -DPLATFORM_DESKTOP -Isrc -Iinclude -Iframework -L$(DYNLIBLOC) $(CFLAGS) $(LDFLAGS) -fPIC

reload-lib: 
	gcc -shared -o build/$(PROJECT_NAME).so $(shell $(GAME_LIB_FILT))  -DPLATFORM_DESKTOP -Isrc -Iinclude -Iframework -L$(DYNLIBLOC) $(CFLAGS) $(LDFLAGS) -fPIC

reload-head: 
	gcc -shared -o build/$(EDITOR_NAME).so $(shell $(EDITOR_LIB_FILT)) -DPLATFORM_DESKTOP -Isrc -Iinclude -Iframework -L$(DYNLIBLOC) $(CFLAGS) $(LDFLAGS) -fPIC
	gcc -shared -o build/$(PROJECT_NAME).so $(shell $(GAME_LIB_FILT))  -DPLATFORM_DESKTOP -Isrc -Iinclude -Iframework -L$(DYNLIBLOC) $(CFLAGS) $(LDFLAGS) -fPIC
	gcc src/main.c framework/reload.c -o build/$(PROJECT_NAME)_dynamic -Iinclude -Iframework $(CFLAGS) -Wl,-rpath=$(DYNLIBLOC),-rpath=build -L$(DYNLIBLOC) $(LDFLAGS) -DDYNAMIC_LIB=./build/$(PROJECT_NAME).so 

web: 
	emcc -o build/$(PROJECT_NAME).html $(FILES) lib/web/libraylib.a $(CFLAGS) -Iinclude -Iframework -Isrc -Llib/web -s USE_GLFW=3  -DPLATFORM_WEB --shell-file lib/web/template.html --preload-file assets

clean:
	rm -rf build
	mkdir build
