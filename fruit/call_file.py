import os, sys
from cffi import FFI

def call(name, *argv) :
    argc = len(argv)
    ffi = FFI()
    sys.path.append(os.environ["HOME"]+"/.fruit")
    #config = __import__("current_project")
    #lib = ffi.dlopen(config.root_path+"/.execuable/%s.so"%name)
    cpp_file = open("%s.cpp"%name);
    function = cpp_file.readline()[2:]
    ffi.cdef(function)
    lib = ffi.dlopen(".execuable/%s.so"%name)
    command =  "result = lib.%s("%name
    for i in xrange(argc-1) :
        command += str(argv[i]) + ","
    if argc > 0 : command += str(argv[argc-1])
    command += ")"
    exec(command)
    #print result
    #return result


