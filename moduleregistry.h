#ifndef MODULEREGISTRY_H
#define MODULEREGISTRY_H

#include <dlfcn.h>
#include <pulse/pulseaudio.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <optional>
#include <algorithm>

using namespace std;

typedef optional<string> maybestring;

typedef struct {
    string name;
    string target;
    maybestring description;
    maybestring usage;
} module;

class ModuleRegistry
{
public:
    ModuleRegistry();
    std::vector<module> modules = std::vector<module>();;
};

#endif // MODULEREGISTRY_H
