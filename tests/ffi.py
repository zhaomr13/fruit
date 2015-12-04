from cffi import FFI
ffi = FFI()
ffi.cdef("""
    void hello();
""")
lib = ffi.dlopen("/home/zhaomr/workdir/fruit/tests/.execuable/hello.so")
lib.hello()
