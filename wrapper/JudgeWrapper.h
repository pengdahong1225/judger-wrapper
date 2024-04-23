#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string>
#include <filesystem>

#include "../core/src/runner.h"
#include "../common/define.h"

/*
 * 作为judger-core的外部调用接口类
 */
class JudgeWrapper {
public:
    static struct result
    judge(LangConfig *language_config, std::string &src, int submission_id, int test_case_id,
          const std::string &test_case);

private:
    static void writeUtf8ToFile(const std::string &filePath, const std::string &content);
    static std::string readFileContent(const std::filesystem::path &filePath);

    static std::string compile(CompileConfig *compile_config, const std::string &src_path, const std::string &work_dir);
};
