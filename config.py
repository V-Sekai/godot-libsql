# config.py

import subprocess

def can_build(env, platform):
    try:
        subprocess.check_call(["cargo", "--version"])
    except:
        return False
    return True and not env.msvc

def configure(env):
    pass

def get_doc_classes():
    return [
        "SQLite",
        "SQLiteQuery",
    ]

def get_doc_path():
    return "doc_classes"
