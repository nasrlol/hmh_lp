#if 0
#include <vector>
#endif

 internal void
    vk_render_entrypoint(Engine *engine, MemArena *arena)
    {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: disable resizing for now

    GLFWwindow* window = glfwCreateWindow((int)engine->width, (int)engine->height, engine->name,NULL , NULL);

    // main loop
    while(!glfwWindowShouldClose(window))
    {
    glfwPollEvents();
    }

    VkApplicationInfo app_info{};
    VkInstanceCreateInfo create_info{};

    {
    //  fill in app info
    {
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = engine->name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "TB Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;
    }

    // fill in create info
    {
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledLayerCount = 0;

    // extensions filled by opengl
    {
    u32 glfw_extensions_count = 0;
    const char** glfw_extensions;

    glfw_extensions= glfwGetRequiredInstanceExtensions(&glfw_extensions_count);

    create_info.enabledExtensionCount   = glfw_extensions_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;
    }
    }
    }

    {
    //  TODO: validate extension layer properties

    }

    #if BASE_DEBUG
    b32 enable_validation_layer = true;
    #else
    b32 enable_validation_layer = false;
    #endif

    VkInstance instance = {};
    // create an instance
    {
    VkResult result = vkCreateInstance(&create_info, NULL, &instance);
    if(result != VK_SUCCESS) // check the return code of the instance creation
    {
    Log("Failed to create a VkInstance");
    goto cleanup;
    }

    b32 validation_support  = false;
    // check validation support
    {
    ScratchArena scratch = scratch_start(arena);


    #if 1
    // get the layer count
    u32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties* available_layers;
    #endif
    String8List *validation_layers      = PushStruct(arena, String8List);

    // gather the validation layers
    {
    String8Node vk_layer_khronos_validation_node =  ToString8Node(str8("VK_LAYER_KHRONOS_validation"));
    str8_list_push_node(validation_layers, &vk_layer_khronos_validation_node);
    }

    // gather the validatoin layers
    {

    #if 1

    available_layers = PushArray(arena, VkLayerProperties, layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    #endif

    #if 0
    u32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
    #endif
    }

    {
    breakpoint();
    for(String8Node *current_node = validation_layers->first;
    !is_nil_str8_node(current_node); current_node = current_node->next)
    {
    bool layerFound = false;

    #if 0
    for (const auto& layerProperties : availableLayers) {
    if (cstring_is_match(layerName, layerProperties.layerName) == 0) {
    validation_support = true;
    break;
    }
    }
    #endif

    if (!layerFound) {
    validation_support = false;
    }
    }
    }

    scratch_end(&scratch);
    }

    if(enable_validation_layer && validation_support) // check the avalaiobe layers after creating the instance
    {

    }
    }

    // cleanup
    cleanup:
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}
