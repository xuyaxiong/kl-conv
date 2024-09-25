import re
from .test_data import docstrings, docstring

type_dict = {
    "unsigned char *": "uchar*",
    "unsigned char*": "uchar*",
    "const char*": "string",
    "const char *": "string",
}


def preprocess(strs):
    lines = [line for line in strs.split("\n") if not line.strip().startswith("//")]
    return "\n".join(lines)


def split_to_docstring(strs):
    docstring_list = [
        item[0]
        for item in re.findall(
            r"((/\*[\s\S]*?\*/\s*)?(\w*\s*)?\w*\s*\w+\(.*?\);)", strs
        )
    ]
    arr = [
        docstring.strip() for docstring in docstring_list if len(docstring.strip()) > 0
    ]
    return arr


def split_docstring_to_comm_and_decl(docstring):
    if docstring != "":
        parts = re.split(r"(?<=\*/)", docstring)
        if len(parts) == 2:
            [comm, decl] = parts
        else:
            comm = ""
            decl = parts[0]
        comm = comm.strip() if comm else ""
        decl = re.sub(r"[\n\t]", " ", decl).strip()
        return (comm, decl)
    else:
        return None


def parse_decl(decl):
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
    lines = [line.strip() for line in comm.split("\n") if line.strip() != ""]
    return "\n".join(
        line if line.startswith(("@", "/*", "*/")) else "\t" + line for line in lines
    )


def export_comm_and_decl(comm, decl, skip_comm=False):
    return f"{comm}\n{decl}" if not skip_comm else f"{decl}"


def convert(strs, skip_comm=False):
    strs = preprocess(strs)
    docstring_list = split_to_docstring(strs)
    result = []
    for docstring in docstring_list:
        res = split_docstring_to_comm_and_decl(docstring)
        if res:
            (comm, decl) = res
            comm = format_comm(comm)
            decl = parse_decl(decl)
            out_str = export_comm_and_decl(comm, decl, skip_comm)
            result.append(out_str)
        else:
            continue
    return ",\n\n".join(result) + ","


if __name__ == "__main__":
    print(split_to_docstring(docstrings))
