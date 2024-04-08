#include "lib/vulkan_gate.h"

int main() {
    NSApplication::NSCompute::CVulkanGate gate;
    gate.isAvailable();
}
