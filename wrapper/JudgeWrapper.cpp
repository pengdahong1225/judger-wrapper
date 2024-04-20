#include "JudgeWrapper.h"
#include <fstream>
#include <iostream>

/*
 * language_config：配置
 * src：提交的源代码
 * submission_id：唯一提交id
 */
struct result JudgeWrapper::judge(LangConfig *language_config, std::string &src, int64 submission_id) {
    // init
    auto &compile_config = language_config->compile_config;
    auto &run_config = language_config->run_config;
    std::string work_dir = JUDGER_WORKSPACE_BASE + std::to_string(submission_id);

    // 源代码文件路径：/judger/run/submission_id/src_name
    std::string src_path = work_dir + "/" + compile_config.src_name;
    // write source code into file
    try {
        writeUtf8ToFile(src_path, src);
        std::cout << "Text successfully written to file.\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    // compile source code, return exe file path
    auto exe_path = this->compile(&compile_config, src_path, work_dir);

}

void JudgeWrapper::writeUtf8ToFile(const std::string &filePath, const std::string &content) {
    std::ofstream file(filePath, std::ios_base::in);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    file << content;
    file.close();
}

std::string JudgeWrapper::compile(CompileConfig *compile_config, std::string &src_path, std::string &output_dir) {

}

void JudgeWrapper::run(struct config *language_config, std::string &src) {

}


struct result JudgeWrapper::judge_one(struct config *language_config, int test_case_file_id) {
    // 获取测试用例


    // 编译运行
    struct result *result = (struct result *) malloc(sizeof(struct result));
    ::run(language_config, result);
    // 返回结果(copy)
    return *result;
}


