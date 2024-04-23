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
public:
    explicit JudgeClient(struct RunConfig run_config, int max_cpu_time, int max_real_time, int max_memory,
                         std::string exe_path, std::string test_case_dir, std::string submission_dir,
                         std::string io_mode);

private:
    void _load_test_case_info();
    void _get_test_case_file_info(int test_case_file_id);
    void run();
    void _judge_one(int test_case_file_id);
    void _compare_output(int test_case_file_id, std::string user_output_file);
private:
    struct RunConfig _run_config;
    int _max_cpu_time;
    int _max_real_time;
    int _max_memory;
    std::string _exe_path;
    std::string _test_case_dir;
    std::string _submission_dir;
    std::string _io_mode;

    Json::Value _test_case_info; // 从info文件加载的测试用例信息
    bool _is_test_case_info_loaded = false;
};


#endif //CORE_WRAPPER_JUDGECLIENT_H
