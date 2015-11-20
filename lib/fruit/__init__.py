import os, sys
import utils

def import_file(name):
    """ Import a module.

    The name should be a string.
    If the name begin with '.', the relative path called
    If the name begin with nothing, the path relative to the
    project's root is called

    """
    if name.startswith('.'):
        name = name[1:]
        print 1
    else:
        print 2
        sys.path.append(os.environ["HOME"]+"/.fruit")
        config = __import__("current_project")
        sys.path.append(config.root_path + utils._prefix(name) )
    #name = utils._suffix(name)
    __import__(name)
    return sys.modules[name]


