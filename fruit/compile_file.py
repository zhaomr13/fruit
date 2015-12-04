import os, sys
import subprocess

# default cpp compiler
cpp_compiler = "/usr/bin/g++"

def compile_file(name):
    """
    compile a cplusplus program to accelerate program

    """

    sys.path.append(os.environ["HOME"]+"/.fruit")
    config = __import__("current_project")
    exec_path = config.root_path + "/.execuable"
    if not os.path.isdir(exec_path) :
        os.makedirs(exec_path)

    compile_status = subprocess.call("%s %s.cpp -fPIC -shared -o %s.so"%(cpp_compiler, name, exec_path+"/"+name), shell=True)
    return compile_status

