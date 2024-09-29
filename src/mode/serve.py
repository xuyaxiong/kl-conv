from flask import Flask, jsonify
import threading

# 创建 Flask 应用
app = Flask(__name__)


# 示例 Flask 路由
@app.route("/status")
def status():
    return jsonify({"status": "GUI is running", "message": "Hello from Flask!"})


# 启动 Flask 服务器的函数
def run_flask():
    app.run(port=5000, debug=False, use_reloader=False)


def run_serve_mode():
    # 启动 Flask 服务器的线程
    flask_thread = threading.Thread(target=run_flask)
    flask_thread.daemon = (
        True  # 设置为守护线程，这样当 Tkinter 窗口关闭时 Flask 线程也会关闭
    )
    flask_thread.start()
