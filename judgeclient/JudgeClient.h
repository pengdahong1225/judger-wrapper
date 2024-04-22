//
// Created by Messi on 24-4-22.
//

#ifndef CORE_WRAPPER_JUDGECLIENT_H
#define CORE_WRAPPER_JUDGECLIENT_H

#include <string>

/*
 * cor调用客户端
 */
class JudgeClient {
public:


private:
    struct RunConfig *_run_config;
    int _max_cpu_time;
    int _max_real_time;
    int _max_memory;
    std::string _exe_path;
    std::string _test_case_dir;
    std::string _submission_dir;
    std::string _io_mode;
};


#endif //CORE_WRAPPER_JUDGECLIENT_H
