//
// Created by Messi on 24-4-22.
//

#include <fstream>
#include <iostream>

#include "JudgeClient.h"

JudgeClient::JudgeClient(struct RunConfig run_config, int max_cpu_time, int max_real_time, int max_memory,
                         std::string exe_path, std::string test_case_dir, std::string submission_dir,
                         std::string io_mode) : _run_config(std::move(run_config)), _max_cpu_time(max_cpu_time),
                                                _max_real_time(max_real_time), _max_memory(max_memory),
                                                _exe_path(std::move(exe_path)),
                                                _test_case_dir(std::move(test_case_dir)),
                                                _submission_dir(std::move(submission_dir)),
                                                _io_mode(std::move(io_mode)) {
    // 加载测试用例info
    _load_test_case_info();
}

void JudgeClient::_load_test_case_info() {
    // 打开info文件并读取到_test_case_info中
    std::ifstream info_file(_test_case_dir + "/info");
    std::stringstream file_contents_stream;
    std::string line;

    if (!info_file) {
        std::cerr << "Error: Unable to open info file " << std::endl;
        return;
    }
    file_contents_stream << info_file.rdbuf(); // 使用rdbuf()直接读取文件内容
    info_file.close();
    std::string file_contents = file_contents_stream.str();

    // json解析
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(file_contents.c_str(), file_contents.c_str() + file_contents.size(),
                                           &_test_case_info,
                                           &errors);
    delete reader;
    if (!parsingSuccessful) {
        std::cerr << "Failed to parse JSON: " << errors << std::endl;
        return;
    }

    _is_test_case_info_loaded = true;
}

void JudgeClient::run() {

}

// 运行一个测试用例
void JudgeClient::_judge_one(int test_case_file_id) {
    if (!_is_test_case_info_loaded) {
        std::cout << "Error: Test case info not loaded" << std::endl;
        return;
    }

    // 读取id对应的case
    auto &test_cases = _test_case_info["test_cases"];
    std::string in_file = _test_case_dir + "/" + test_cases[test_case_file_id]["input_name"].asString();

    // 创建运行需要的文件
    std::string real_user_output_file;
    std::string user_output_file;
    if (_io_mode == standard) {
        real_user_output_file = user_output_file = _submission_dir + "/" + std::to_string(test_case_file_id) + ".out";
    }
    else {
        std::cout << "Error: _io_mode" << std::endl;
        return;
    }

    // 构建命令

}

void JudgeClient::_compare_output(int test_case_file_id, std::string user_output_file) {

}

void JudgeClient::_get_test_case_file_info(int test_case_file_id) {

}
