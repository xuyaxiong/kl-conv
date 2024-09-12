Dll链接库函数声明转换为JS函数声明
===================
## 1. 打包
```bash
# 1. 创建虚拟环境
python -m venv venv
# 2. 激活虚拟环境
venv\Scripts\activate
# 3. 安装依赖
pip install -r requirements.txt
# 4. 执行打包指令
python setup.py
```

## 2. 测试
```bash
pytest
```