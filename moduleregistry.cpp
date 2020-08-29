#include "moduleregistry.h"

#include <boost/algorithm/string/replace.hpp>

using namespace std;

const string BASE = "/usr/lib/pulse-13.0/modules/";

inline string get_symbol(string mod, string func) {
    return boost::replace_all_copy(boost::replace_all_copy(mod, ".so", ""), "-", "_") + "_LTX_" + func;
}

inline optional<string> safe_call(const char* (*ptr)()) {
    if (ptr == NULL) return nullopt;
    return string(ptr());
}

ModuleRegistry::ModuleRegistry()
{
    cout << "construction" << endl;
    DIR* dirp = opendir(BASE.c_str());
    struct dirent* dp;
    while ((dp = readdir(dirp)) != NULL) {
        if (strstr(dp->d_name, "module") == NULL) continue;
        string mod = dp->d_name;
        string target = BASE + mod;
        void *handle = dlopen(target.c_str(), RTLD_LAZY);
        string init = get_symbol(mod, "pa__init");
        if (dlsym(handle, init.c_str()) != NULL) {
            cout << mod << " -> " << safe_call((const char* (*)())dlsym(handle, get_symbol(mod, "pa__get_usage").c_str())).value_or("NONE") << endl;
        } else {
            cout << target << " did not have " << init << endl;
        }
        dlclose(handle);
    }
}
