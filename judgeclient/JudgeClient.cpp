//
// Created by Messi on 24-4-22.
//

#include "JudgeClient.h"

#include <utility>

JudgeClient::JudgeClient(struct RunConfig run_config, int max_cpu_time, int max_real_time, int max_memory,
                         std::string exe_path, std::string test_case_dir, std::string submission_dir,
                         std::string io_mode) : _run_config(std::move(run_config)), _max_cpu_time(max_cpu_time), ,
                                                _max_real_time(max_real_time), _max_memory(max_memory),
                                                _exe_path(std::move(exe_path)),
                                                _test_case_dir(std::move(test_case_dir)),
                                                _submission_dir(std::move(submission_dir)),
                                                _io_mode(std::move(io_mode)) {
    // 加载测试用例
    _load_test_case_info();
}

void JudgeClient::_load_test_case_info() {

}

void JudgeClient::run() {

}

void JudgeClient::_judge_one(int test_case_file_id) {

}

void JudgeClient::_compare_output(int test_case_file_id, std::string user_output_file) {

}
