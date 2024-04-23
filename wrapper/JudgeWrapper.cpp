#include "JudgeWrapper.h"
#include <fstream>
#include <iostream>
#include "../common/json/json.h"

/*
 * language_config：配置
 * src：提交的源代码
 * submission_id：唯一提交id
 * test_case_json：测试用例，需要解析
 */
JudgeResult JudgeWrapper::judge(LangConfig *language_config, std::string &src, int submission_id,
                                const std::string &test_case_json) {
    JudgeResult ret{};

    // init
    auto &compile_config = language_config->compile_config;
    auto &run_config = language_config->run_config;
    std::string work_dir = judger_dir + std::to_string(submission_id);

    // TODO write source code into file
    // 源代码文件路径：/app/judger/{submission_id}/{src_name}
    std::string src_path = work_dir + "/" + compile_config.src_name;
    try {
        writeUtf8ToFile(src_path, src);
    } catch (const std::exception &e) {
        sprintf(ret.error_message.data(), "Failed to write source code into file[%s],err=%s", src_path.c_str(),
                e.what());
        return ret;
    }

    // TODO compile source code, return exe file path
    std::string exe_path;
    try {
        exe_path = compile(&compile_config, src_path, work_dir);
    } catch (const std::exception &e) {
        sprintf(ret.error_message.data(), "Failed to compile source code,err=%s", e.what());
        return ret;
    }

    // TODO 解析测试用例json，保存到文件中
    try {
        initTestCaseEnv(work_dir, test_case_json);
    } catch (const std::exception &e) {
        sprintf(ret.error_message.data(), "Failed to parse test case json,err=%s", e.what());
        return ret;
    }
    // TODO 循环运行测试用例

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
    if (result.result != SUCCESS) {
        std::filesystem::remove(compiler_out);
        std::string msg;
        // todo 将result对象序列化为json

        throw std::runtime_error(sprintf("Compile failed: %s", result.error));
        std::cout << "Compile failed: " << result.error << '\n';
        std::filesystem::remove(compiler_out);
        return "";
    }

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

/*
 * 初始化测试用例环境
 * 1.创建test_case目录
 * 2.解析test_case_json
 * 3.创建文件
 */
void JudgeWrapper::initTestCaseEnv(const std::string &work_dir, const std::string &test_case_json) {
    // work_dir = /app/judger/{submission_id}
    // 创建test_case目录
    if (std::filesystem::create_directory(work_dir + "/test_case")) {
        throw std::runtime_error("Failed to create test_case directory: " + work_dir + "/test_case");
    }

    // 解析json

}
