//
// Created by Messi on 24-4-22.
//

#ifndef CORE_WRAPPER_JUDGECLIENT_H
#define CORE_WRAPPER_JUDGECLIENT_H

#include <string>
#include "../common/define.h"
#include "../common/json/json.h"

/*
 * core调用实例
 * 初始化之后先读取测试用例
 */
class JudgeClient {
    struct RunConfig _run_config;
    int _max_cpu_time;
    int _max_real_time;
    int _max_memory;
    std::string _exe_path;
    std::string _test_case_dir;
    std::string _log_path;
    std::string _submission_dir;
    std::string _io_mode;

    Json::Value _test_case_info; // 从info文件加载的测试用例信息
    bool _is_test_case_info_loaded = false;

public:
    explicit JudgeClient(struct RunConfig run_config, int max_cpu_time, int max_real_time, int max_memory,
                         std::string exe_path, std::string log_path, std::string test_case_dir,
                         std::string submission_dir,
                         std::string io_mode);
    void run();

private:
    void _load_test_case_info();
    void _get_test_case_file_info(int test_case_file_id);
    void _judge_one(int test_case_file_id);
    bool _compare_output(int test_case_file_id, const std::string& user_output_file);

private:
    static std::string readFileContent(const std::filesystem::path &filePath);
};

namespace cmd_template {
    static std::string compile_cmd_cpp = "/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++11 {src_path} -lm -o {exe_path}";
    static std::string run_cmd_cpp = "{exe_path}";
}

#endif //CORE_WRAPPER_JUDGECLIENT_H
