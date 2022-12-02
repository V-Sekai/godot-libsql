# config.py

def can_build(env, platform):
    return False

def configure(env):
    pass

def get_doc_classes():
    return [
        "SQLite",
        "SQLiteQuery",
    ]

def get_doc_path():
    return "doc_classes"
