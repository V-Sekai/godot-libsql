# config.py

import subprocess
import platform as python_platform


def can_build(env, platform):
    if platform == "web":
        return False
    try:
        subprocess.check_call(["cargo", "--version"])
    except:
        return False
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "Libsql",
        "LibsqlQuery",
    ]


def get_doc_path():
    return "doc_classes"
