#ifndef BASE_RENDER
#define BASE_RENDER

typedef struct Engine Engine;
struct Engine
{
    const char *name;
    int width;
    int height;
    GLFWwindow            *gl_window;
    VkExtensionProperties *vk_properties;
    VkInstance            *vk_instance;
};

internal void vk_render_entrypoint(Engine *engine, MemArena *arena);

#define RenderEntrypoint vk_render_entrypoint


#endif