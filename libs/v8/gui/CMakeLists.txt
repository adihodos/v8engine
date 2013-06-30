add_library(
    v8_gui STATIC 
    basic_window.cc)

install(TARGETS v8_gui ARCHIVE DESTINATION libs)