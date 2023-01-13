# config.py

import subprocess
import platform as python_platform

def can_build(env, platform):
    if platform == "web":
        return False
    try:
        subprocess.check_call(["cargo", "--version"])
        subprocess.check_call(["/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig"])
        subprocess.check_call(["/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig --libs --static libcrypto"])
    except:
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
