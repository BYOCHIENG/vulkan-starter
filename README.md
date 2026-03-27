# Vulkan Starter

- Getting Started
    - Linux Setup

## Getting Started

### Linux (Ubuntu) Environment Setup

#### Vulkan Packages

Install packages
```[bash]
sudo apt install vulkan-tools
sudo apt install libvulkan-dev
sudo apt install vulkan-validationlayers spirv-tools
```

Confirm install
```[bash]
vulkaninfo && vkcube
```

Output
```[bash]
==========
VULKANINFO
==========

Vulkan Instance Version: 1.X.XYZ


Instance Extensions: count = X
===============================
...
```

Cube
![vkcube](/public/vkcube.png)

#### GLFW Packages

Install packages
```[bash]
sudo apt install libglfw3-dev
```

#### GLM Packages

Install packages
```[bash]
sudo apt install libglm-dev
```

#### Google Shaderc library (glslc)

Install packages
```[bash]
sudo apt install glslc
```

Confirm install
```[bash]
glslc
```

Expected output
```[bash]
glslc: error: no input files
```