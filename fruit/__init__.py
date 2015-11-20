import os, sys
from fruit.utils.pathformat import prefix
from fruit.utils.pathformat import suffix

def import_file(name):
    """ Import a module.

    The name should be a string.
    If the name begin with '.', the relative path called
    If the name begin with nothing, the path relative to the
    project's root is called

    """

    path0 = sys.path[0]
    if name.startswith('.'):
        name = name[1:]
    else:
        sys.path.append(os.environ["HOME"]+"/.fruit")
        config = __import__("current_project")
        sys.path[0] = config.root_path + "/" + prefix(name)
        name = suffix(name)
    __import__(name)
    sys.path[0] = path0
    return sys.modules[name]


