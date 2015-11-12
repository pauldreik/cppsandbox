#this is a forwarding makefile, so eclipse can delegate building to cmake
#and ninja.

here:=$(shell pwd)
projectname:=$(notdir $(here))
wheretobuild:=../build-$(projectname)
ninjafile:=$(wheretobuild)/build.ninja

.DEFAULT_GOAL:=all
showinfo:
	echo $(here) $(projectname) $(wheretobuild) $(ninjafile)
.PHONY: all
all: $(ninjafile) 
	cd $(wheretobuild);ninja
$(ninjafile): $(wheretobuild)
	cd $(wheretobuild); cmake -GNinja $(here)
$(wheretobuild):
	mkdir -p $(wheretobuild)
