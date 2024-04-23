//
// Created by Messi on 24-4-22.
//

#include <fstream>
#include <iostream>
#include <filesystem>

#include "JudgeClient.h"
#include "../core/src/runner.h"

JudgeClient::JudgeClient(struct RunConfig run_config, int max_cpu_time, int max_real_time, int max_memory,
                         std::string exe_path, std::string log_path, std::string test_case_dir,
                         std::string submission_dir,
                         std::string io_mode) : _run_config(std::move(run_config)), _max_cpu_time(max_cpu_time),
                                                _max_real_time(max_real_time), _max_memory(max_memory),
                                                _exe_path(std::move(exe_path)),
                                                _log_path(std::move(log_path)),
                                                _test_case_dir(std::move(test_case_dir)),
                                                _submission_dir(std::move(submission_dir)),
                                                _io_mode(std::move(io_mode)) {
    // 加载测试用例info
    _load_test_case_info();
}

void JudgeClient::_load_test_case_info() {
    // 打开info文件并读取到_test_case_info中
    std::ifstream info_file(_test_case_dir + "/info.json");
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

// 运行一个测试用例
JudgeResult JudgeClient::_judge_one(int test_case_file_id) {
    JudgeResult ret{};

    if (!_is_test_case_info_loaded) {
        ret.error_message = "Error: Test case info not loaded";
        return ret;
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
        ret.error_message = "Error: _io_mode";
        return ret;
    }

    // 构建命令
    std::string cmd = cmd_template::run_cmd_cpp;
    cmd.replace(cmd.find("{exe_path}"), strlen("{exe_path}"), _exe_path);

    // 执行
    struct config cfg{
            // 限制
            .max_cpu_time = _max_cpu_time,
            .max_real_time = _max_real_time,
            .max_memory = _max_memory,
            .max_stack = 128 * 1024 * 1024,
            .max_process_number = UNLIMITED,
            .max_output_size = std::max(1024 * 1024 * 16,
                                        test_cases[test_case_file_id].get("output_size", 0).asInt() * 2),
            // 执行参数
            .exe_path =cmd.data(),
            .input_path = in_file.data(),
            .output_path = real_user_output_file.data(),
            .error_path = real_user_output_file.data(),
            .args = {nullptr},
            .env = {nullptr},
            .log_path = _log_path.data(),
            .seccomp_rule_name = _run_config.seccomp_rule_name.data(),
            .uid = 0,
            .gid = 0
    };
    struct result result{};
    ::run(&cfg, &result);

    // if progress exited normally, then we should check output result
    ret.code = result.result;
    ret.cpu_time = result.cpu_time;
    ret.real_time = result.real_time;
    ret.memory = result.memory;
    ret.signal = result.signal;
    ret.exit_code = result.exit_code;
    ret.error = result.error;
    if (result.result != SUCCESS) {
        ret.error_message = "Error: progress exited normally";
        return ret;
    }
    else {
        if (!std::filesystem::exists(user_output_file)) {
            ret.error_message = "Error: user output file not found";
            return ret;
        }
        if (_compare_output(test_case_file_id, user_output_file)) {
            ret.error_message = "Error: output failed";
            return ret;
        }
        ret.error_message = "Success";
        return ret;
    }
}

bool JudgeClient::_compare_output(int test_case_file_id, const std::string &user_output_file) {
    // 读取用户执行输出文件
    std::string user_content;
    try {
        user_content = readFileContent(user_output_file);
        std::cout << "user_output_file user_content: " << user_content << '\n';
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }
    if (!user_content.empty()) {
        std::cout << "user_output_file empty: " << '\n';
        return false;
    }

    // 读取测试用例的输出文件
    std::string case_out_content;
    auto &test_cases = _test_case_info["test_cases"];
    std::string case_out_file = _test_case_dir + "/" + test_cases[test_case_file_id].get("output_name", "").asString();
    try {
        case_out_content = readFileContent(case_out_file);
        std::cout << "case_out_file case_out_content: " << case_out_content << '\n';
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }
    if (!case_out_content.empty()) {
        std::cout << "case_out_file empty: " << '\n';
        return false;
    }

    // 比较
    return user_content == case_out_content;
}

std::string JudgeClient::readFileContent(const std::filesystem::path &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return content;
}
