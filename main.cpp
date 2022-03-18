#include <iostream>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <GLFW/glfw3.h>

// These functions are defined in glfw_bridge.mm
// metal-cpp does not cover NSWindow nor CAMetalLayer. We need to use Objective-C++
// to interface with those two types as they're also required for creating drawable
// textures for the window. While I have written my own implementations for these,
// it'd be overkill to introduce those headers for this simple test case.
void* createLayer(GLFWwindow* window, double width, double height, void* device);
void* nextDrawable(void* layer);

int main() {
    if (!glfwInit())
        return -1;

    auto window = glfwCreateWindow(1920, 1080, "Test", nullptr, nullptr);
    
    auto device = MTL::CreateSystemDefaultDevice();
    auto queue = device->newCommandQueue();

    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // The layer requires the scaled window size in order to properly allocate a drawable
    // texture. If the scaling is not done, we get a "allocation failed" error message
    // from calling [CAMetalLayer nextDrawable].
    void* layer = createLayer(window, width * xscale, height * yscale, device);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        CA::MetalDrawable* drawable = reinterpret_cast<CA::MetalDrawable*>(nextDrawable(layer));

        if (drawable == nullptr) {
            std::cout << "Failed to acquire drawable" << std::endl;
            break;
        }

        MTL::RenderPassDescriptor* mainPass = MTL::RenderPassDescriptor::alloc();
        auto colorAttachment = mainPass->colorAttachments()->object(0);

        colorAttachment->setClearColor(MTL::ClearColor::Make(0, 0, 0, 0.25));
        colorAttachment->setLoadAction(MTL::LoadActionClear);
        colorAttachment->setStoreAction(MTL::StoreActionStore);
        colorAttachment->setTexture(drawable->texture());

        MTL::CommandBuffer* buffer = queue->commandBuffer();

        // This will segfault, as the renderCommandEncoderWithDescriptor: selector is 0x0
        // in the MTL::Private::Selector namespace. The selector was not loaded through
        // sel_registerName from objc/runtime.h.
        MTL::RenderCommandEncoder* encoder = buffer->renderCommandEncoder(mainPass);
    
        encoder->endEncoding();
        buffer->presentDrawable(drawable);
        buffer->commit();

        buffer->release();
    }

    glfwTerminate();

    return 0;
}
