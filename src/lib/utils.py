import os
import sys
import re
import chardet
from collections import Counter
from .test_data import docstrings, docstring, formatted_docstring

type_dict = {
    "unsigned char *": "uchar*",
    "unsigned char*": "uchar*",
    "const char*": "string",
    "const char *": "string",
}


def detect_encoding(str):
    """侦测字符编码格式"""
    return chardet.detect(str)["encoding"]


def get_file_content(path):
    """用正确的编码格式打开文件"""
    with open(path, "rb") as f:
        data = f.read()
    with open(path, "r", encoding=detect_encoding(data)) as f:
        return f.read()


def strip_header_file(header_str):
    """从头文件中截取导出函数部分"""
    in_target = False  # 是否目标行
    in_comm = False  # 是否为注释
    curly_braces = []
    target_lines = []
    for line in header_str.splitlines():
        if 'extern "C" {' in line:
            in_target = True
            curly_braces.append("{")
            continue
        if in_target:
            if "#endif" in line or "#ifdef" in line:
                continue
            target_lines.append(line.strip())
            if "/*" in line:
                in_comm = True
            if "*/" in line:
                in_comm = False
            if not in_comm:
                for ch in line:
                    if ch == "{":
                        curly_braces.append(ch)
                    if ch == "}":
                        curly_braces.pop()
                    if len(curly_braces) == 0:
                        in_target = False
    strs = "\n".join(target_lines[:-1])
    return strs


def remove_extra_spaces(text):
    """将多个空白字符替换为单个空格"""
    return re.sub(r"\s+", " ", text).strip()


def parse_decl(decl):
    """解析函数声明"""
    decl = remove_extra_spaces(decl)
    result = re.findall(r"(\S+)\s(\S+)\s([^\(]+)\((.*)\);", decl)[0]
    ret_type = result[1]
    func_name = result[2]
    types_str = result[3]
    try:
        params_decl = [type_str.strip() for type_str in types_str.split(",")]
        type_arr = []
        for param_decl in params_decl:
            param_decl = param_decl.split("=")[0].strip()
            if param_decl == "":
                continue
            (type, _) = re.findall(r"(.*[*\s])(.*)", param_decl)[0]
            type = type.strip()
            type = type_dict.get(type) or type
            type_str = f"'{type}'"
            type_arr.append(type_str)
    except:
        type_arr = [f"'something went wrong'"]

    type_arr_str = f"[{', '.join(type_arr)}]"
    ret_type_str = f"'{ret_type}'"
    func_decl_content = f"[{', '.join([ret_type_str, type_arr_str])}]"
    result = f"{func_name}: {func_decl_content}"
    return result


def export_comm_and_decl(comm, decl, skip_comm=False):
    """导出注释和函数声明"""
    if not skip_comm:
        if comm != "" and decl != "":
            return f"{comm}\n{decl},\n\n"
        elif comm != "":
            return f"{comm}\n"
        else:
            return f"{decl},\n\n"
    else:
        if comm != "" and decl != "":
            return f"{decl},\n\n"
        elif comm != "":
            return ""
        else:
            return f"{decl},\n\n"


def remove_single_line_comm(docstrings):
    """移除双斜杠注释"""
    lines = [
        remove_extra_spaces(line)
        for line in docstrings.split("\n")
        if not remove_extra_spaces(line).startswith("//")
    ]
    return "\n".join(lines)


def remove_multiline_comm(docstrings):
    """移除多行注释"""
    total_len = len(docstrings)
    i = 0
    in_comm = False
    acc = ""
    while i < total_len:
        curr_ch = docstrings[i]
        if i + 1 >= total_len:
            nxt_ch = ""
        else:
            nxt_ch = docstrings[i + 1]
        if curr_ch == "/" and nxt_ch == "*":
            # 进入注释部分
            in_comm = True
        if curr_ch == "*" and nxt_ch == "/":
            in_comm = False
            i += 2
            continue
        if not in_comm and curr_ch != "\n":
            acc += curr_ch
        i += 1
    return acc


def remove_comm(docstrings):
    """移除所有注释"""
    docstrings = remove_single_line_comm(docstrings)
    return remove_multiline_comm(docstrings)


def get_export_key_from_docstrings(docstrings):
    """根据数量统计获取函数导出关键字"""
    decls = remove_comm(docstrings)
    decls = re.sub(r"\([\s\S]*?\);", " ", decls)
    token_filter_list = ["", "int"]
    tokens = [
        token for token in re.split("\s+", decls) if token not in token_filter_list
    ]
    counter = Counter(tokens)
    return counter.most_common(1)[0][0]


def convert(docstrings, skip_comm=False):
    export_key = get_export_key_from_docstrings(docstrings)
    return replace_all_decls(docstrings, export_key, skip_comm)


def write_content_to_template(content, output_path, output_name):
    """将转换后的内容填充到模板后导出至文件"""
    res = fill_template(output_name, content)
    output_path = os.path.join(output_path, f"{output_name}.ts")
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(res)
    return os.path.abspath(output_path)


def convert_file(input_path, output_path, output_name, skip_comm=False):
    """读取头文件并且输出转换后的文件"""
    data = get_file_content(input_path)
    striped_header_file = strip_header_file(data)
    content = convert(striped_header_file, skip_comm)
    write_content_to_template(content, output_path, output_name)


def get_static_file_path(filename):
    """获取静态资源路径"""
    if getattr(sys, "frozen", False):
        # 如果程序被打包
        base_path = sys._MEIPASS
    else:
        # 如果在开发环境中运行
        base_path = os.path.join(os.path.dirname(__file__), "../..")
    return os.path.join(base_path, "assets", filename)


def padding_left(strs, n):
    """在多行字符串左侧填充空格符"""
    padding = " " * n
    return "\n".join([f"{padding}{line}" for line in strs.split("\n")])


def fill_template(dll_name, content):
    """填充模板"""
    dll_temp_path = get_static_file_path("dll_template.txt")
    content = padding_left(content, 4)
    with open(dll_temp_path, "r") as f:
        data = f.read()
        data = data.replace("$DLL_NAME$", dll_name)
        data = data.replace("$CONTENT$", content)
        return data


def replace_all_decls(docstrings, export_key, skip_comm):
    """转化函数声明并替换"""

    def process(match):
        return parse_decl(match.group()) + ","

    pattern = rf"{export_key}[\s\S]*?\);"
    if not skip_comm:
        return re.sub(pattern, process, docstrings)
    else:
        decls = re.findall(pattern, docstrings)
        return ",\n\n".join([parse_decl(decl) for decl in decls])


if __name__ == "__main__":
    print(get_export_key_from_docstrings(docstrings))
