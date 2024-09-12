import re

type_dict = {
    "unsigned char *": "uchar*",
    "unsigned char*": "uchar*",
    "const char*": "string",
    "const char *": "string",
}


def split_to_docstring(strs):
    return re.split(r"(?<=\);)", strs)  # 正向后视断言


def split_docstring_to_comm_and_decl(docstring):
    docstring = docstring.strip()
    if docstring != "":
        groups = re.findall(r"(/\*[\s\S]*\*/\n)?\n?([\s\S]*)", docstring)
        (comm, decl) = groups[0]
        comm = comm.strip() + "\n" if comm else ""
        decl = re.sub(r"[\n\t]", "", decl)
        return (comm, decl)
    else:
        return None


def parse_decl(decl):
    result = re.findall(r"(.* )?(.*) (.*)\((.*)\);", decl)[0]
    ret_type = result[1]
    func_name = result[2]
    types_arr = result[3:]
    params_decl = types_arr[0].split(",")
    type_arr = []
    for param_decl in params_decl:
        (type, name) = re.findall(r"(.*[*\s])(.*)", param_decl.strip())[0]
        type = type.strip()
        type = type_dict.get(type.strip()) or type
        type_str = f"'{type}'"
        name = name.strip()
        type_arr.append(type_str)

    type_arr_str = f"[{', '.join(type_arr)}]"
    ret_type_str = f"'{ret_type}'"
    func_decl_content = f"[{', '.join([ret_type_str, type_arr_str])}]"
    result = f"{func_name}: {func_decl_content}"
    return result


def export_comm_and_decl(comm, decl):
    return f"{comm}{decl}"


def convert(strs):
    docstring_list = split_to_docstring(strs)
    result = []
    for docstring in docstring_list:
        res = split_docstring_to_comm_and_decl(docstring)
        if res:
            (comm, decl) = res
            decl = parse_decl(decl)
            out_str = export_comm_and_decl(comm, decl)
            result.append(out_str)
        else:
            continue
    return ",\n\n".join(result) + ","
