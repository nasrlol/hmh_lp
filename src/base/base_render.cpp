internal void
vk_render_entrypoint(Engine *engine, MemArena *arena)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: disable resizing for now

    GLFWwindow* window = glfwCreateWindow((int)engine->width, (int)engine->height, engine->name,NULL, NULL);

    VkApplicationInfo app_info{};
    VkInstanceCreateInfo create_info{};

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

	// extensions filled by opengl
	{
	    u32 glfw_extensions_count = 0;
	    const char** glfw_extensions;

	    glfw_extensions= glfwGetRequiredInstanceExtensions(&glfw_extensions_count);

	    create_info.enabledExtensionCount   = glfw_extensions_count;
	    create_info.ppEnabledExtensionNames = glfw_extensions;
	}
    }

#if BASE_DEBUG
    b32 enable_validation_layer = true;
#else
    b32 enable_validation_layer = false;
#endif


    b32 layer_found  = false;
    // check validation support
    ScratchArena scratch = scratch_start(arena);


    // get the layer count
    u32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties *available_layers;
    String8List       *validation_layers = PushStructZero(arena, String8List);

    // gather the validation layers
    {
	String8Node vk_layer_khronos_validation_node = ToString8Node(str8("VK_LAYER_KHRONOS_validation"));
	str8_list_push_node(validation_layers, &vk_layer_khronos_validation_node);
    }

    // gather the validation layers
    {
	available_layers = PushArrayZero(arena, VkLayerProperties, layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
    }

    breakpoint();
    for(String8Node *current_node = validation_layers->first; !is_nil_str8_node(current_node); current_node = current_node->next)
    {
	for(u32 index = 0; index < layer_count; ++index)
	{
	    VkLayerProperties *current_layer = &available_layers[index];
	    String8 current_layer_name = str8(current_layer->layerName);
	    if(str8_is_match(current_node->value, current_layer_name))
	    {
		layer_found = true;
		break;
	    }
	}
    }
      
    // check if we found that specific layer and we can continue

    if(enable_validation_layer && !layer_found) // check the avalaiobe layers after creating the instance
    {
	if(!layer_found)
	{
	    Log("requested validation layer not found!");
	    goto cleanup;
	}


	// extract names, null terminated
	String8 *names_in_one_str8_lit = PushStructZero(arena, String8);
	for(u32 index = 0; index < layer_count; ++index)
	{

	    VkLayerProperties *current_layer = &available_layers[index];
	    String8 current_layer_name       = str8(current_layer->layerName);
	    int res = str8_append(names_in_one_str8_lit, &current_layer_name);
	    if(res != 0)
	    {
		breakpoint();
	    }
	}

	create_info.enabledLayerCount   = layer_count;
	create_info.ppEnabledLayerNames = (const char *const *)names_in_one_str8_lit->data; // TODO: maybe use the String8 to get a single buffer that we can pass to available layers
	breakpoint();

    } 


    VkInstance instance = {};
    VkResult result = vkCreateInstance(&create_info, NULL, &instance);

    if(result != VK_SUCCESS) // check the return code of the instance creation
    {
	Log("Failed to create a VkInstance");
	goto cleanup;
    }

    scratch_end(&scratch);

    // main loop
    while(!glfwWindowShouldClose(window))
    {
	glfwPollEvents();
    }



  cleanup:
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}
