# config.py

import subprocess
import platform as python_platform

def can_build(env, platform):
    if env.msvc:
        return False
    try:
        subprocess.check_call(["cargo", "--version"])
    except:
        return False
    if python_platform.system() == 'Windows' and env["use_mingw"] and env["use_llvm"]:
        return False
    return True

def configure(env):
    pass

def get_doc_classes():
    return [
        "MVSQLite",
        "MVSQLiteQuery",
    ]

def get_doc_path():
    return "doc_classes"
