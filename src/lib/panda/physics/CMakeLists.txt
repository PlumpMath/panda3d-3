panda3d_interrogate(physics MODULE pandaphysics)

remake_add_library(physics *.cxx LINK collide)
remake_add_headers(*.h *.I *.T)
