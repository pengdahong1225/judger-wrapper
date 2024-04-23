#include "JudgeWrapper.h"
#include <fstream>
#include <iostream>

/*
 * language_config：配置
 * src：提交的源代码
 * submission_id：唯一提交id
 */
struct result JudgeWrapper::judge(LangConfig *language_config, std::string &src, int submission_id, int test_case_id,
                                  const std::string &test_case) {
    // init
    auto &compile_config = language_config->compile_config;
    auto &run_config = language_config->run_config;
    std::string work_dir = judger_dir + std::to_string(submission_id);

    // 源代码文件路径：/judger/submission_id/src_name
    std::string src_path = work_dir + "/" + compile_config.src_name;
    // write source code into file
    try {
        writeUtf8ToFile(src_path, src);
        std::cout << "Text successfully written to file.\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    // compile source code, return exe file path
    auto exe_path = compile(&compile_config, src_path, work_dir);

    // 解析测试用例json，保存到文件中

}

void JudgeWrapper::writeUtf8ToFile(const std::string &filePath, const std::string &content) {
    std::ofstream file(filePath, std::ios_base::in);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    file << content;
    file.close();
}

std::string
JudgeWrapper::compile(CompileConfig *compile_config, const std::string &src_path, const std::string &work_dir) {
    std::string exe_path = work_dir + "/" + compile_config->exe_name;
    std::string compiler_out = work_dir + "/" + "compiler.out";
    std::string log_path = work_dir + "/" + "compile.log";
    // 编译
    struct config cfg{
            // 限制
            .max_cpu_time = static_cast<int>(compile_config->max_cpu_time),
            .max_real_time = compile_config->max_real_time,
            .max_memory = compile_config->max_memory,
            .max_stack = 128 * 1024 * 1024,
            .max_output_size = 20 * 1024 * 1024,
            .max_process_number = UNLIMITED,
            // 执行参数
            .exe_path = exe_path.data(),
            .input_path = const_cast<char *>(src_path.data()),
            .output_path = compiler_out.data(),
            .error_path = compiler_out.data(),
            .log_path = log_path.data(),
            .seccomp_rule_name = nullptr,
            .args = nullptr,
            .env = nullptr,
            .uid = 0,
            .gid = 0
    };
    struct result result{};
    ::run(&cfg, &result);
    if (result.result == SUCCESS) {
        if (std::filesystem::exists(compiler_out)) {
            std::string content;
            try {
                content = readFileContent(compiler_out);
                std::cout << "compiler_out content: " << content << '\n';
            } catch (const std::exception &e) {
                std::cerr << "Error: " << e.what() << '\n';
                return "";
            }

            if (!content.empty()) {
                std::cout << "compiler_out content: " << content << '\n';
                return "";
            }
        }
        std::filesystem::remove(compiler_out);
        return exe_path;
    }
    else {
        std::cout << "Compile failed: " << result.error << '\n';
        std::filesystem::remove(compiler_out);
        return "";
    }
}

std::string JudgeWrapper::readFileContent(const std::filesystem::path &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return content;
}
