#include <ofxCore.h>
#include <ofxImageEffect.h>

#include <climits>
#include <cstring>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace {
int suiteRequests = 0;

const void* fetchSuite(OfxPropertySetHandle, const char*, int) {
    ++suiteRequests;
    return nullptr;
}

bool check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << '\n';
    }
    return condition;
}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ofx_abi_smoke MODULE\n";
        return 2;
    }
#ifdef _WIN32
    const auto library = LoadLibraryW(std::filesystem::path(argv[1]).c_str());
#else
    const auto library = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
#endif
    if (!library) {
        std::cerr << "Cannot load module: " << argv[1] << '\n';
        return 1;
    }
#ifdef _WIN32
    int (*count)() = nullptr;
    OfxPlugin* (*get)(int) = nullptr;
    const auto countSymbol = GetProcAddress(library, "OfxGetNumberOfPlugins");
    const auto getSymbol = GetProcAddress(library, "OfxGetPlugin");
    static_assert(sizeof(count) == sizeof(countSymbol));
    static_assert(sizeof(get) == sizeof(getSymbol));
    std::memcpy(&count, &countSymbol, sizeof(count));
    std::memcpy(&get, &getSymbol, sizeof(get));
#else
    const auto count = reinterpret_cast<int (*)()>(dlsym(library, "OfxGetNumberOfPlugins"));
    const auto get = reinterpret_cast<OfxPlugin* (*)(int)>(dlsym(library, "OfxGetPlugin"));
#endif
    bool ok = check(count && get, "Missing bootstrap exports");
    if (ok) {
        ok &= check(count() == 1, "Expected one plugin");
        for (const int index : {-1, 1, INT_MIN, INT_MAX}) {
            ok &= check(get(index) == nullptr, "Invalid index returned a plugin");
        }
        auto* plugin = get(0);
        ok &= check(plugin != nullptr, "Missing plugin descriptor");
        if (plugin) {
            ok &= check(plugin->pluginApi && std::strcmp(plugin->pluginApi, kOfxImageEffectPluginApi) == 0,
                        "Unexpected plugin API");
            ok &= check(plugin->apiVersion == 1, "Unexpected API version");
            ok &= check(plugin->pluginIdentifier && std::strcmp(plugin->pluginIdentifier, "org.yorozufx.buildstub") == 0,
                        "Unexpected provisional identifier");
            ok &= check(plugin->pluginVersionMajor == 0 && plugin->pluginVersionMinor == 1,
                        "Unexpected stub version");
            ok &= check(plugin->setHost && plugin->mainEntry, "Missing callbacks");
            if (plugin->setHost && plugin->mainEntry) {
                OfxHost host{nullptr, fetchSuite};
                plugin->setHost(&host);
                ok &= check(plugin->mainEntry(kOfxActionLoad, nullptr, nullptr, nullptr) == kOfxStatFailed,
                            "Build stub must reject host Load");
                ok &= check(plugin->mainEntry(kOfxActionDescribe, nullptr, nullptr, nullptr) == kOfxStatReplyDefault,
                            "Describe is not implemented");
                ok &= check(plugin->mainEntry(kOfxImageEffectActionRender, nullptr, nullptr, nullptr) == kOfxStatReplyDefault,
                            "Render is not implemented");
                ok &= check(plugin->mainEntry("yorozu.test.unknown", nullptr, nullptr, nullptr) == kOfxStatReplyDefault,
                            "Unknown actions must return default");
                ok &= check(plugin->mainEntry(nullptr, nullptr, nullptr, nullptr) == kOfxStatErrValue,
                            "Null action must fail validation");
                ok &= check(suiteRequests == 0, "Build stub requested a host suite");
                plugin->setHost(nullptr);
            }
        }
    }
#ifdef _WIN32
    ok &= check(FreeLibrary(library) != 0, "Cannot unload module");
#else
    ok &= check(dlclose(library) == 0, "Cannot unload module");
#endif
    return ok ? 0 : 1;
}
