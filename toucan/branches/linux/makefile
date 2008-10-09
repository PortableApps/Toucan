ifeq ($(CONFIG),unicoderelease)
WXWIN=/home/steve/wxGTK-2.8.8
TOOLCHAINNAME=gtk2u
WXVERSION:=$(shell echo `$(WXWIN)/GCCBuildReleaseGTK2Unicode/wx-config --version`)
CXX=g++
CC=gcc
RESCOMP=windres.exe
RCFILEOBJ=Test_rc.o
RCFILESRC=Test.rc
RESOURCEOBJECT=
OUTPUTPATH=../../../Output/Exec
OBJECTPATH=../../../Output/Objects
BUILDPATHS=$(OBJECTPATH)
PROGRAM=Test
LIBS=`$(WXWIN)/GCCBuildReleaseGTK2Unicode/wx-config --inplace --exec-prefix="$(WXWIN)/GCCBuildReleaseGTK2Unicode" --libs`
LINKERFLAGS=
WARNINGFLAGS=-Wall
OPTFLAGS=-O
DEBUGFLAGS=
LIBPATH=
CPPINC=`$(WXWIN)/GCCBuildReleaseGTK2Unicode/wx-config --inplace --cxxflags`
GCCFLAGS=
LDFLAGS=$(LIBS) $(LIBPATH) $(LINKERFLAGS)
CPPFLAGS=$(CPPINC) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS)
LIBDIRNAME=$(WXWIN)/GCCBuildReleaseGTK2Unicode/lib
RESPATH=--include-dir "$(WXWIN)/include" --include-dir "$(WXWIN)/contrib/include" --include-dir "$(WXWIN)/GCCBuildReleaseGTK2Unicode/lib/wx/include/gtk2-unicode-release-static-2.8"
MACPACKAGEINFO=
endif


OBJECTS=$(OBJECTPATH)/toucan.o $(OBJECTPATH)/frmmain.o $(OBJECTPATH)/frmprogress.o $(OBJECTPATH)/frmrestore.o $(OBJECTPATH)/frmvariable.o $(OBJECTPATH)/basicfunctions.o $(OBJECTPATH)/rootdata.o $(OBJECTPATH)/variables.o $(OBJECTPATH)/script.o $(OBJECTPATH)/cmdline.o $(OBJECTPATH)/secure.o $(OBJECTPATH)/sync.o $(OBJECTPATH)/dragndrop.o $(OBJECTPATH)/waitthread.o $(OBJECTPATH)/vdtc.o $(OBJECTPATH)/rules.o $(OBJECTPATH)/settings.o $(OBJECTPATH)/securedata.o $(OBJECTPATH)/backupprocess.o $(OBJECTPATH)/backupdata.o $(OBJECTPATH)/syncdata.o $(OBJECTPATH)/$(RCFILEOBJ)

all:	$(BUILDPATHS) $(OUTPUTPATH)/$(PROGRAM)

$(OBJECTPATH):
	mkdir -p $(OBJECTPATH)

clean:
	rm -f $(OBJECTPATH)/*.o
	rm -f $(OBJECTPATH)/*.res
	rm -f $(OUTPUTPATH)/$(PROGRAM)

help:
	@echo "Usage: make -f makefile.gcc CONFIG=[unicoderelease] [all|clean|help]"

$(OUTPUTPATH)/$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LIBPATH) $(LINKERFLAGS) $(LIBS)

$(OBJECTPATH)/$(RCFILEOBJ):	$(RCFILESRC)
	$(RESCOMP) --use-temp-file -i$< -o$@ $(RESPATH) --include-dir . --define NOPCH

$(OBJECTPATH)/toucan.o:	toucan.cpp toucan.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) toucan.cpp

$(OBJECTPATH)/frmmain.o:	frmmain.cpp frmmain.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) frmmain.cpp

$(OBJECTPATH)/frmprogress.o:	frmprogress.cpp frmprogress.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) frmprogress.cpp

$(OBJECTPATH)/frmrestore.o:	frmrestore.cpp frmrestore.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) frmrestore.cpp

$(OBJECTPATH)/frmvariable.o:	frmvariable.cpp frmvariable.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) frmvariable.cpp
	
$(OBJECTPATH)/basicfunctions.o:	basicfunctions.cpp basicfunctions.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) basicfunctions.cpp
		
$(OBJECTPATH)/rootdata.o:	rootdata.cpp rootdata.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) rootdata.cpp
	
$(OBJECTPATH)/cmdline.o:	cmdline.cpp cmdline.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) cmdline.cpp

$(OBJECTPATH)/rules.o:	rules.cpp rules.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) rules.cpp

$(OBJECTPATH)/script.o:	script.cpp script.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) script.cpp
	
$(OBJECTPATH)/variables.o:	variables.cpp variables.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) variables.cpp
	
$(OBJECTPATH)/settings.o:	settings.cpp settings.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) settings.cpp

$(OBJECTPATH)/securedata.o:	securedata.cpp securedata.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) securedata.cpp

$(OBJECTPATH)/backupprocess.o:	backupprocess.cpp backupprocess.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) backupprocess.cpp
	
$(OBJECTPATH)/backupdata.o:	backupdata.cpp backupdata.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) backupdata.cpp
	
$(OBJECTPATH)/syncdata.o:	syncdata.cpp syncdata.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) syncdata.cpp
	
$(OBJECTPATH)/waitthread.o:	waitthread.cpp waitthread.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) waitthread.cpp

$(OBJECTPATH)/dragndrop.o:	dragndrop.cpp dragndrop.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) dragndrop.cpp
	
$(OBJECTPATH)/secure.o:	secure.cpp secure.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) secure.cpp
	
$(OBJECTPATH)/sync.o:	sync.cpp sync.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) sync.cpp

$(OBJECTPATH)/vdtc.o:	virtualdirtreectrl.cpp virtualdirtreectrl.h
	$(CXX) -c -o $@ $(CPPFLAGS) $(GCCFLAGS) $(DEBUGFLAGS) $(OPTFLAGS) $(WARNINGFLAGS) $(CPPINC) virtualdirtreectrl.cpp

.PHONY:	all clean
