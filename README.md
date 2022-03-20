# metal-cpp-bug

This is a small test case to showcase a bug with Apple's official metal-cpp wrappers. In short,
it tries to call `objc_msgSend` with a null selector, causing a segfault. This happens when trying
to create a MTLRenderCommandEncoder, arguably one of the most vital classes when rasterising with
the Metal API. Read more [here](https://developer.apple.com/forums/thread/702578#702578021).

## Building

To build, simply install glfw through vcpkg or homebrew and unzip the metal-cpp file into `metal-cpp-bug/metal-cpp/`.
Then configure and build through cmake like you would normally do and the project should build.
