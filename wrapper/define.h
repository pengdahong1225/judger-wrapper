//
// Created by Messi on 2024-04-20.
//

#ifndef CORE_WRAPPER_DEFINE_H
#define CORE_WRAPPER_DEFINE_H

#include <string>
#include <vector>

using int64 = int64_t;
using int32 = int32_t;

#define JUDGER_WORKSPACE_BASE "/judger/run"
#define LOG_BASE "/log"

static std::vector<std::string> DefaultEnv{"LANG=en_US.UTF-8", "LANGUAGE=en_US:en", "LC_ALL=en_US.UTF-8"};


typedef struct CompileConfig {
    std::string src_name;
    std::string exe_name;
    int64 max_cpu_time;
    int64 max_real_time;
    int64 max_memory;
    std::string compile_command;
} CompileConfig;

typedef struct RunConfig {
    std::string command;
    std::string seccomp_rule;
    std::vector<std::string> env;
    int32 memory_limit_check_only;
} RunConfig;

typedef struct LangConfig {
    CompileConfig compile_config;
    RunConfig run_config;
} LangConfig;

#endif //CORE_WRAPPER_DEFINE_H
