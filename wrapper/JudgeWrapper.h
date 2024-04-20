#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string>

#include "../core/src/runner.h"
#include "define.h"

static std::string standard = "Standard IO";
static std::string file = "File IO";

class JudgeWrapper{
public:
    struct result judge(LangConfig* language_config, std::string& src, int64 submission_id);

    void run(struct config* language_config, std::string& src);

private:
    void writeUtf8ToFile(const std::string& filePath, const std::string& content);

    std::string compile(CompileConfig* compile_config, std::string& src_path, std::string& output_dir);
    struct result judge_one(struct config* language_config, int test_case_file_id);
};

class JudgeClient{

};