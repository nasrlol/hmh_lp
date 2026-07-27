#include "base/base_include.h"

int ENTRYPOINT(HINSTANCE instance,
               HINSTANCE prev_instance,
               LPSTR cmdline,
               int cmdshow)
{

    MemArena *global_arena = ArenaCreate(GiB(1));
    PlatformCreateWindow(instance, 100, 100, 100, 100);

    Engine *engine = PushStruct(global_arena, Engine);
    engine->name   = "TB";
    engine->width  = 800;
    engine->height = 600;

    RenderEntrypoint(engine, global_arena);

    ArenaDestroy(global_arena);
    return (0);
}

