#include "wrapper.h"

void wrapper::judge(struct config* language_config, std::string& src) {

}

void wrapper::compile() {
    // null
}

/*
 * ret = judgercore.run(max_cpu_time=1000,
                  max_real_time=2000,
                  max_memory=128 * 1024 * 1024,
                  max_process_number=200,
                  max_output_size=10000,
                  max_stack=32 * 1024 * 1024,
                  # five args above can be judgercore.UNLIMITED
                  exe_path="main",
                  input_path="1.in",
                  output_path="1.out",
                  error_path="1.out",
                  args=[],
                  # can be empty list
                  env=[],
                  log_path="judger.log",
                  # can be None
                  seccomp_rule_name="c_cpp",
                  uid=0,
                  gid=0)
 */
void wrapper::run(struct config* language_config) {
    struct result* result = (struct result*)malloc(sizeof(struct result));
    ::run(language_config, result);
    
}
