#this is a forwarding makefile, so eclipse can delegate building to cmake
#and ninja.

.PHONY: all

wheretobuild:=../build
ninjafile:=$(wheretobuild)/build.ninja
all: $(ninjafile) 
	cd $(wheretobuild);ninja
$(ninjafile): $(wheretobuild)
	cd $(wheretobuild); cmake -GNinja ../compiletimelogarithm
$(wheretobuild):
	mkdir -p $(wheretobuild) && cd $(wheretobuild)
