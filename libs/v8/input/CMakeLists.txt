add_library(
    v8_input STATIC 
    key_syms_win.cc)

install(TARGETS v8_input ARCHIVE DESTINATION libs)