#include <ofxCore.h>
#include <ofxImageEffect.h>

#include <cstring>

#ifdef _WIN32
#define YOROZU_EXPORT __declspec(dllexport)
#else
#define YOROZU_EXPORT __attribute__((visibility("default")))
#endif

namespace {
OfxHost* host = nullptr;

void setHost(OfxHost* suppliedHost) {
    host = suppliedHost;
}

OfxStatus mainEntry(const char* action, const void*, OfxPropertySetHandle, OfxPropertySetHandle) {
    if (!action) {
        return kOfxStatErrValue;
    }
    if (std::strcmp(action, kOfxActionLoad) == 0) {
        return kOfxStatFailed;
    }
    return kOfxStatReplyDefault;
}

OfxPlugin plugin{kOfxImageEffectPluginApi, 1, "org.yorozufx.buildstub", 0, 1, setHost, mainEntry};
}

extern "C" YOROZU_EXPORT int OfxGetNumberOfPlugins() {
    return 1;
}

extern "C" YOROZU_EXPORT OfxPlugin* OfxGetPlugin(int index) {
    return index == 0 ? &plugin : nullptr;
}
