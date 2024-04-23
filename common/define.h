//
// Created by Messi on 2024-04-20.
//

#ifndef CORE_WRAPPER_DEFINE_H
#define CORE_WRAPPER_DEFINE_H

#include <string>
#include <vector>

static std::string judger_dir = "/app/judger";
static std::string standard = "Standard IO";
static std::string file = "File IO";
static std::vector<std::string> DefaultEnv{"LANG=en_US.UTF-8", "LANGUAGE=en_US:en", "LC_ALL=en_US.UTF-8"};

typedef struct CompileConfig {
    std::string src_name;
    std::string exe_name;
    int max_cpu_time;
    int max_real_time;
    int max_memory;
} CompileConfig;

typedef struct RunConfig {
    std::string seccomp_rule_name;
    std::vector<std::string> env;
    int memory_limit_check_only;
} RunConfig;

typedef struct LangConfig {
    CompileConfig compile_config;
    RunConfig run_config;
} LangConfig;

#endif //CORE_WRAPPER_DEFINE_H
