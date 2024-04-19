#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string>

#include "../core/src/runner.h"

static std::string standard = "Standard IO";
static std::string file = "File IO";

class wrapper{
public:
    void judge(struct config* language_config, std::string& src);
private:
    void compile();
    void run(struct config* language_config);
};