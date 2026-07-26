#include "base/base_include.h"

int ENTRYPOINT(HINSTANCE instance,
               HINSTANCE prev_instance,
               LPSTR cmdline,
               int cmdshow)
{

    MemArena *global_arena = ArenaCreate(MiB(100));
    PlatformCreateWindow(instance, 100, 100, 100, 100);

    Engine *engine = PushStruct(global_arena, Engine);
    engine->name   = "TB";
    engine->width  = 800;
    engine->height = 600;

    render_entrypoint(engine);

    ArenaDestroy(global_arena);
    return (0);
}

