# judger-JudgeWrapper

## test_case文件
info：测试用例描述，json格式
```json
{
  "test_case_number": 1,
  "spj": false,
  "test_cases": {
    "1": {
      "input_name": "1.in",
      "output_name": "1.out"
    }
  }
}
```

x.in：输入文件
```
1 2
```

x.out：输出文件
```
3
```

## command template
cpp：<br>
`/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++11 {src_path} -lm -o {exe_path}`<br>
`{exe_path}`<br>

## 容器目录
**根目录**

container_workspace = /app

judger_dir = /app/judger

work_dir = /app/judger/{submission_id}

**编译**

源代码 = /judger/{submission_id}/{src_name}

编译输出 = /app/judger/{submission_id}/compiler.out

可执行文件 = /app/judger/{submission_id}/{exe_name}

日志 = /app/judger/{submission_id}/compiler.log

**运行**

测试用例 = /app/judger/{submission_id}/test_case

运行输出 = /app/judger/{submission_id}/{test_case_file_id}.out
