internal void
              render_entrypoint(Engine *engine)
    {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: disable resizing for now

    GLFWwindow* window = glfwCreateWindow((int)engine->width, (int)engine->height, engine->name, glfwGetPrimaryMonitor(), NULL);

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

    VkInstance instance = {};
    VkResult result = vkCreateInstance(&create_info, NULL, &instance);
    if(result != VK_SUCCESS)
    {
    Log("Failed to create a VkInstance");
    goto cleanup;
    }

    u32 extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

    // cleanup
    cleanup:
    glfwDestroyWindow(window);
    glfwTerminate();
}
