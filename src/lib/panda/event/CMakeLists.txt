panda3d_interrogate(event MODULE panda)

remake_add_library(event *.cxx LINK putil express pbase pstatclient linmath)
remake_add_headers(*.h *.I *.T)
