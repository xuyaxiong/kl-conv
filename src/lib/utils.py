import os
import sys
import re
import chardet
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


def is_comm(str):
    """是否为注释"""
    return re.search(r"/\*[\s\S]*?\*/", str)


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


def preprocess(strs):
    lines = [line for line in strs.split("\n") if not line.strip().startswith("//")]
    return "\n".join(lines)


def split_to_docstring_list(strs):
    docstring_list = [
        item[0]
        for item in re.findall(
            r"((/\*[\s\S]*?\*/\s*)?((\w*\s*)?\w*\s*\w+\(.*?\);)?)", strs
        )
    ]
    arr = [
        docstring.strip() for docstring in docstring_list if len(docstring.strip()) > 0
    ]
    return arr


def split_docstring_to_comm_and_decl(docstring):
    """将字符串分割成注释和函数声明两部分"""
    if docstring != "":
        parts = re.split(r"(?<=\*/)", docstring)
        if len(parts) == 2:
            [comm, decl] = parts
        else:
            first_part = parts[0]
            if is_comm(first_part):
                comm = first_part
                decl = ""
            else:
                comm = ""
                decl = first_part
        comm = comm.strip()
        decl = re.sub(r"[\n\t]", " ", decl).strip()
        return (comm, decl)
    else:
        return None


def parse_decl(decl):
    """解析函数声明"""
    result = re.findall(r"(.* )?(\S+)\s+(\S+)\((.*)\);", decl)[0]
    ret_type = result[1]
    func_name = result[2]
    types_str = result[3]
    params_decl = [type_str.strip() for type_str in types_str.split(",")]
    type_arr = []
    for param_decl in params_decl:
        param_decl = param_decl.split("=")[0].strip()
        (type, _) = re.findall(r"(.*[*\s])(.*)", param_decl)[0]
        type = type.strip()
        type = type_dict.get(type) or type
        type_str = f"'{type}'"
        type_arr.append(type_str)

    type_arr_str = f"[{', '.join(type_arr)}]"
    ret_type_str = f"'{ret_type}'"
    func_decl_content = f"[{', '.join([ret_type_str, type_arr_str])}]"
    result = f"{func_name}: {func_decl_content}"
    return result


def format_comm(comm):
    """格式化注释"""
    lines = [line.strip() for line in comm.split("\n") if line.strip() != ""]
    return "\n".join(
        line if line.startswith(("@", "/*", "*/")) else "\t" + line for line in lines
    )


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


def convert(strs, skip_comm=False):
    strs = preprocess(strs)
    docstring_list = split_to_docstring_list(strs)
    result = []
    for docstring in docstring_list:
        res = split_docstring_to_comm_and_decl(docstring)
        if res:
            (comm, decl) = res
            comm = format_comm(comm)
            decl = parse_decl(decl) if decl != "" else ""
            out_str = export_comm_and_decl(comm, decl, skip_comm)
            result.append(out_str)
        else:
            continue
    return "".join(result)


def convert_file(input_path, output_path, output_name, skip_comm=False):
    """读取头文件并且输出转换后的文件"""
    data = get_file_content(input_path)
    striped_header_file = strip_header_file(data)
    content = convert(striped_header_file, skip_comm)
    res = fill_template(output_name, content)
    output_path = os.path.join(output_path, f"{output_name}.ts")
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(res)


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


if __name__ == "__main__":
    print(split_to_docstring_list(docstrings))
