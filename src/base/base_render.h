#ifndef BASE_RENDER
#define BASE_RENDER

typedef struct Engine Engine;
struct Engine
{
    const char *name;
    int width;
    int height;
    GLFWwindow *window;
    VkExtensionProperties *properties;
};

#define RenderEntrypoint vk_render_entrypoint
internal Engine vk_render_entrypoint(Engine *engine);


#endif