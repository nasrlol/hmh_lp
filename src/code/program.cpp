#include "base/base_include.h"

ENTRYPOINT(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmdline, int cmdshow)
{
    MemArena *global_arena = ArenaCreate(GiB(1));

    Engine *engine = PushStructZero(global_arena, Engine);
    engine->name   = "TB";
    engine->width  = 800;
    engine->height = 600;

    RenderEntrypoint(engine, global_arena);

    ArenaDestroy(global_arena);
    return (0);
}

