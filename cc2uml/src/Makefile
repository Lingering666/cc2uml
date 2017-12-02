CC=gcc
ICC_COMMON_CFLAGS=-Wall -funsigned-char -march=pentium-m -mtune=pentium4 -msse2 -pipe -pthread -funsigned-bitfields -fvisibility=hidden -fdata-sections -ffunction-sections
# NO PATH_MAX ICC_CFLAGS_RELEASE=${ICC_COMMON_CFLAGS} -O2 -std=c99
ICC_CFLAGS_RELEASE=${ICC_COMMON_CFLAGS} -O2 
ICC_CFLAGS_DEBUG=${ICC_COMMON_CFLAGS} -O0 -ggdb3
ICC_CXXFLAGS_RELEASE=${ICC_COMMON_CFLAGS} O2 -std=c++11
ICC_CXXFLAGS_DEBUG=${ICC_COMMON_CFLAGS} O2 -std=c++11 -ggdb3

#-print-search-dirs
# 非ASCII字符用宽字符输出
#COMMON_CFLAGS=-Wall -funsigned-char -march=native -mtune=native -finput-charset=gb18030 -fexec-charset=gb18030 -fwide-exec-charset=utf32 -pipe -pthread -funsigned-bitfields -rdynamic -fvisibility=hidden -fvisibility-inlines-hidden -fdata-sections -ffunction-sections

#COMMON_CFLAGS=-Wall -funsigned-char -march=native -mtune=native -finput-charset=gb18030 -fexec-charset=utf8    -fwide-exec-charset=utf-32 -pipe -pthread -funsigned-bitfields -fvisibility=hidden -fdata-sections -ffunction-sections

#COMMON_CFLAGS=-Wall -funsigned-char -march=native -mtune=native -finput-charset=gb18030 -fexec-charset=utf8 -fwide-exec-charset=utf-32le -pipe -pthread -funsigned-bitfields -fvisibility=hidden -fdata-sections -ffunction-sections

COMMON_CFLAGS=-Wall -funsigned-char -march=native -mtune=native -finput-charset=gb18030 -fexec-charset=utf8 -fwide-exec-charset=utf32 -pipe -pthread -funsigned-bitfields -fvisibility=hidden -fdata-sections -ffunction-sections -fdiagnostics-color=always

#COMMON_CFLAGS=-Wall -funsigned-char -march=native -mtune=native -finput-charset=utf-8 -fexec-charset=utf-8 -fwide-exec-charset=utf32 -pipe -pthread -funsigned-bitfields -fvisibility=hidden -fdata-sections -ffunction-sections


CFLAGS_RELEASE=${COMMON_CFLAGS} -std=gnu11 -O2
CFLAGS_DEBUG=${COMMON_CFLAGS} -std=gnu11 -O0 -ggdb3 -fprofile-arcs -ftest-coverage -coverage 

SO_CFLAGS_RELEASE=${CFLAGS_RELEASE} -fPIC -shared
SO_CFLAGS_DEBUG=${CFLAGS_DEBUG} -fPIC -shared

EXE_CFLAGS_RELEASE=${CFLAGS_RELEASE} -fPIE -static
EXE_CFLAGS_DEBUG=${CFLAGS_DEBUG} -fPIE

CXXFLAGS_RELEASE=${COMMON_FLAGS} -std=gnu++14 -O2
CXXFLAGS_DEBUG=${COMMON_FLAGS} -std=gnu++14 -O0 --ggdb3 -fprofile-arcs -ftest-coverage

SO_CXXFLAGS_RELEASE=${CXXFLAGS_RELEASE} -fPIC -shared
SO_CXXFLAGS_DEBUG=${CXXFLAGS_DEBUG} -fPIC -shared

EXE_CXXFLAGS_RELEASE=${CXXFLAGS_RELEASE} -fPIE
EXE_CXXFLAGS_DEBUG=${CXXFLAGS_DEBUG} -fPIE

CPPFLAGS_RELEASE=
CPPFLAGS_DEBUG=-D__DEBUG__

#不建议在大程序里用 -rdynamic -Wl,--export-dynamic，这个会导出大量的符号，导致加载变慢
#SO_LINKER_FLAGS=-pthread -lpthread -ldl -lrt -Wl,--export-dynamic -rdynamic -Wl,--gc-sections -Wl,--print-gc-sections
#EXE_LINKER_FLAGS=-pthread -lpthread -ldl -lrt -Wl,--export-dynamic -pie -rdynamic -Wl,--gc-sections

#COMMON_LDFLAGS=-coverage -Wl,-rpath,/home/whg/soft/glibc/lib
COMMON_LDFLAGS=-Wl,--gc-sections -Wl,--print-gc-sections -Wl,-rpath,/home/whg/soft/glibc/lib -pthread 
#COMMON_LDFLAGS=-Wl,--gc-sections -Wl,--print-gc-sections -coverage
SO_LINKER_FLAGS_RELEASE=$(COMMON_LDFLAGS)
SO_LINKER_FLAGS_DEBUG=$(COMMON_LDFLAGS) -coverage 

EXE_LINKER_FLAGS_RELEASE=${COMMON_LDFLAGS} -pie -static-libgcc
EXE_LINKER_FLAGS_DEBUG=${COMMON_LDFLAGS} -pie -coverage 

COMMON_LDLIBS=-lpthread -ldl -lrt -lgcov 

CFLAGS=$(EXE_CFLAGS_RELEASE)
CXXFLAGS=$(EXE_CXXFLAGS_RELEASE))
LDFLAGS=$(EXE_LINKER_FLAGS_RELEASE)
LDLIBS=$(COMMON_LDLIBS)
CPPFLAGS=$(CPPFLAGS_RELEASE)

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
DST=cc2uml cc2uml.exe

.PHONY: clean all

all : $(DST)

cc2uml: cc2uml.o

cc2uml.exe: cc2uml.win.o

tolatex_icc: tolatex_icc.o
	icc -O2 -o $@ $<

%_icc.c:%.c
	iconv -f gb18030 -t utf8 $< -o $@

%_icc.o:%_icc.c
	icc $(ICC_CFLAGS_DEBUG) -c -o $@ $<

#%:%.o
#	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.win.o:%.c
	 i686-w64-mingw32-gcc -Wall -funsigned-char -march=native -mtune=native -finput-charset=gb18030 -fexec-charset=utf8 -fwide-exec-charset=utf32 -pipe -pthread -funsigned-bitfields -fvisibility=hidden -fdata-sections -ffunction-sections -std=gnu11 -O2 -c $< -o $@

%.exe:%.win.o
	 i686-w64-mingw32-gcc -o $@ $<


clean:
	rm -f *~ $(DST) $(OBJ) *.gc* tolatex_icc* *.win.o
	rm -f *.aux *.log *.blg *.toc *.out *.lot tex4ht.ps *.4*
	rm -f *.xref* *.lg *.idv *.out *.otc *.tmp tmp.*
	rm -f *.vrb *.nav *.snm core