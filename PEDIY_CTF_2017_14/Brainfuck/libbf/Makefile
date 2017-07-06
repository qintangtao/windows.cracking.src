#CFLAGS=-g -pg -Wall
#CFLAGS=-g -Wall -fprofile-arcs -ftest-coverage
#CFLAGS=-pipe -O2 -g -Wall -DNO_INTEPRETER
CFLAGS=-pipe -O2 -g -Wall -DCOMPILER_FOR_ALL_TARGET

LIB_DEPS=libbf.c \
         libbf_config.h \
         libbf.h \
         libbf_internal.h \
         libbf_interpreter_simple.c \
         libbf_interpreter_optimizer.c \
         libbf_interpreter_dynalloc.c \
         libbf_interpreter_dynalloc.h \
         libbf_posix_emulation.c \
         libbf_posix_emulation.h \
         libbf_compiler_dynalloc.c \
         libbf_compiler_dynalloc.h \
         libbf_compiler.c \
         libbf_compiler.h \
         libbf_compiler_arm.c \
         libbf_compiler_ppc.c \
         libbf_compiler_i386_x86_64.c \
         libbf_compiler_sparc.c \
         libbf_compiler_mips.c \
         libbf_c_to_bf.c \
         libbf_list.h \
         libbf_list.c \
         libbf_hash.h \
         libbf_hash.c

BIN_DEPS=main.c ${LIB_DEPS}

LIB_SOURCES=libbf.c libbf_interpreter_dynalloc.c libbf_compiler_dynalloc.c libbf_posix_emulation.c libbf_compiler.c libbf_list.c libbf_hash.c libbf_c_to_bf.c

BIN_SOURCES=main.c ${LIB_SOURCES}

all: libbf libbf32 libbf64 libbf.exe libbf_ppc.bin libbf_arm.bin libbf_mips.bin libbf_mipsel.bin libbf_sparc.bin libbf.so main

targz:
	tar cvzf libbf.tar.gz ${LIB_DEPS} main.c Makefile LICENSE_LGPL_EN.txt README test/*.b test/*.bf test/test.c test/*result*.txt dump_ref.txt test/test_gen_ref.c

libbf_mips.bin: ${BIN_DEPS}
	/usr/mips/bin/mips-linux-gcc $(CFLAGS) ${BIN_SOURCES} -o $@

libbf_mipsel.bin: ${BIN_DEPS}
	/usr/mipsel/bin/mipsel-linux-gcc $(CFLAGS) ${BIN_SOURCES} -o $@

libbf_arm.bin: ${BIN_DEPS}
	/usr/arm/bin/arm-linux-gcc $(CFLAGS) ${BIN_SOURCES} -o $@

libbf_ppc.bin: ${BIN_DEPS}
	/usr/powerpc/bin/powerpc-linux-gcc $(CFLAGS) ${BIN_SOURCES} -o $@

libbf_sparc.bin: ${BIN_DEPS}
	/usr/sparc/bin/sparc-linux-gcc $(CFLAGS) ${BIN_SOURCES} -o $@

libbf.exe: ${BIN_DEPS}
	i586-mingw32msvc-gcc $(CFLAGS) ${BIN_SOURCES} -o $@

libbf: ${BIN_DEPS}
	$(CC) $(CFLAGS) ${BIN_SOURCES} -o $@

libbf64: ${BIN_DEPS}
	$(CC) -m64 $(CFLAGS) ${BIN_SOURCES} -o $@

libbf32: ${BIN_DEPS}
	$(CC) -m32 $(CFLAGS) ${BIN_SOURCES} -o $@

main: main.c libbf.so
	$(CC) $(CFLAGS) main.c -o $@ -lbf -L.

libbf.so: ${LIB_DEPS}
	$(CC) $(CFLAGS) -fPIC ${LIB_SOURCES} -shared -o $@

clean:
	$(RM) *.o libbf libbf32 libbf.exe libbf_*.bin main libbf.so

EXEC=./libbf
EXEC_I386=./libbf32
EXEC_X86_64=./libbf64
EXEC_WINE=~/wine-0.0.20050725-winehq/wine libbf.exe
EXEC_PPC=NO_DYNALLOC=yes qemu-ppc -L /usr/powerpc libbf_ppc.bin
EXEC_ARM=qemu-arm -L /usr/arm libbf_arm.bin
EXEC_SPARC=qemu-sparc -L /usr/sparc libbf_sparc.bin
EXEC_MIPS=qemu-mips -L /usr/mips libbf_mips.bin
EXEC_MIPSEL=qemu-mipsel -L /usr/mipsel libbf_mipsel.bin

test-c-to-bf:
	$(EXEC) --c-to-bf test/test.c | $(EXEC) --interpreter-optimizer-cdump-int - > test/test_gen.c
	diff test/test_gen.c test/test_gen_ref.c

test-optimizer:
	echo "" > dump.txt
	printf "\n\nfactor.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/factor.b >> dump.txt
	printf "\n\nfib.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/fib.b >> dump.txt
	printf "\n\nmandelbrot.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/mandelbrot.b >> dump.txt
	printf "\n\nyapi.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/yapi.b >> dump.txt
	printf "\n\nnumwarp.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/numwarp.b >> dump.txt
	printf "\n\nprime.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/prime.b >> dump.txt
	printf "\n\nselfportrait.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/selfportrait.b >> dump.txt
	printf "\n\nsquares.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/squares.b >> dump.txt
	printf "\n\nmandelbrot.b\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/mandelbrot.b >> dump.txt
	printf "\n\ntriangle.bf\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/triangle.bf >> dump.txt
	printf "\n\nryan-beer.bf\n\n" >> dump.txt
	${EXEC} --interpreter-optimizer-cdump-int test/ryan-beer.bf >> dump.txt
	diff dump.txt dump_ref.txt

test-compiler:
	${EXEC} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt
	${EXEC} --compile-execute-int-8 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC} --compile-execute-int-16 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC} --compile-execute-int-32 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC} --compile-execute-int-64 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC} --compile-execute-int-128 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC} --compile-execute-int-8 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC} --compile-execute-int-16 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC} --compile-execute-int-32 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC} --compile-execute-int-64 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC} --compile-execute-int-128 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC} --compile-execute-int-8 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC} --compile-execute-int-16 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC} --compile-execute-int-32 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC} --compile-execute-int-64 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC} --compile-execute-int-128 test/triangle.bf | diff - test/triangle_result.txt
	echo "255" | ${EXEC} --compile-execute-int-8 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC} --compile-execute-int-16 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC} --compile-execute-int-32 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC} --compile-execute-int-64 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC} --compile-execute-int-128 test/prime.b | diff - test/prime_result.txt
	${EXEC} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
	${EXEC} --compile-execute-int-16 test/pi16.b | diff - test/pi16_result.txt
	${EXEC} --compile-execute-int-32 test/pi16.b | diff - test/pi16_result.txt
	${EXEC} --compile-execute-int-64 test/pi16.b | diff - test/pi16_result.txt
	${EXEC} --compile-execute-int-128 test/pi16.b | diff - test/pi16_result.txt


test-compiler-sparc:
	${EXEC_SPARC} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-8 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_SPARC} --compile-execute-int-8 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_SPARC} --compile-execute-int-8 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/triangle.bf | diff - test/triangle_result.txt
	echo "255" | ${EXEC_SPARC} --compile-execute-int-8 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_SPARC} --compile-execute-int-16 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_SPARC} --compile-execute-int-32 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_SPARC} --compile-execute-int-64 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_SPARC} --compile-execute-int-128 test/prime.b | diff - test/prime_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/pi16.b | diff - test/pi16_result.txt

test-compiler-arm:
	${EXEC_ARM} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-8 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_ARM} --compile-execute-int-8 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_ARM} --compile-execute-int-8 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/triangle.bf | diff - test/triangle_result.txt
	echo "255" | ${EXEC_ARM} --compile-execute-int-8 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_ARM} --compile-execute-int-16 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_ARM} --compile-execute-int-32 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_ARM} --compile-execute-int-64 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_ARM} --compile-execute-int-128 test/prime.b | diff - test/prime_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/pi16.b | diff - test/pi16_result.txt

test-compiler-ppc:
	${EXEC_PPC} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-8 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_PPC} --compile-execute-int-8 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_PPC} --compile-execute-int-8 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/triangle.bf | diff - test/triangle_result.txt
	echo "255" | ${EXEC_PPC} --compile-execute-int-8 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_PPC} --compile-execute-int-16 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_PPC} --compile-execute-int-32 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_PPC} --compile-execute-int-64 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_PPC} --compile-execute-int-128 test/prime.b | diff - test/prime_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/pi16.b | diff - test/pi16_result.txt

test-compiler-mips:
	${EXEC_MIPS} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-8 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/ryan-beer.bf | diff - test/ryan-beer_result.txt
	${EXEC_MIPS} --compile-execute-int-8 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/mandelbrot.b | diff - test/mandelbrot_result.txt
	${EXEC_MIPS} --compile-execute-int-8 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/triangle.bf | diff - test/triangle_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/triangle.bf | diff - test/triangle_result.txt
	echo "255" | ${EXEC_MIPS} --compile-execute-int-8 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_MIPS} --compile-execute-int-16 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_MIPS} --compile-execute-int-32 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_MIPS} --compile-execute-int-64 test/prime.b | diff - test/prime_result.txt
	echo "255" | ${EXEC_MIPS} --compile-execute-int-128 test/prime.b | diff - test/prime_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/pi16.b | diff - test/pi16_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/pi16.b | diff - test/pi16_result.txt

test-compiler-comp-yapi-all-arch:
	time ${EXEC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	time ${EXEC_I386} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt	
	time ${EXEC_X86_64} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	time ${EXEC_MIPS} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	time ${EXEC_ARM} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	time ${EXEC_PPC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	time ${EXEC_SPARC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt	

test-compiler-all-arch: test-compiler-squares-all-arch test-compiler-yapi-all-arch

test-compiler-squares-all-arch: test-compiler-squares-i386 test-compiler-squares-wine test-compiler-squares-x86-64 test-compiler-squares-ppc test-compiler-squares-arm test-compiler-squares-sparc test-compiler-squares-mips test-compiler-squares-mipsel

test-compiler-squares-i386:
	${EXEC_I386} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_I386} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_I386} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_I386} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_I386} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt

test-compiler-squares-wine:
	${EXEC_WINE} --compile-execute-int-8 test/squares.b | diff - test/squares_result_wine.txt
	${EXEC_WINE} --compile-execute-int-16 test/squares.b | diff - test/squares_result_wine.txt
	${EXEC_WINE} --compile-execute-int-32 test/squares.b | diff - test/squares_result_wine.txt
	${EXEC_WINE} --compile-execute-int-64 test/squares.b | diff - test/squares_result_wine.txt
	${EXEC_WINE} --compile-execute-int-128 test/squares.b | diff - test/squares_result_wine.txt

test-compiler-squares-x86-64:
	${EXEC_X86_64} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_X86_64} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_X86_64} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_X86_64} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_X86_64} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt
	${EXEC_X86_64} --compile-execute-int-256 test/squares.b | diff - test/squares_result.txt

test-compiler-squares-ppc:
	${EXEC_PPC} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt

test-compiler-squares-arm:
	${EXEC_ARM} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt

test-compiler-squares-sparc:
	${EXEC_SPARC} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt

test-compiler-squares-mips:
	${EXEC_MIPS} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt

test-compiler-squares-mipsel:
	${EXEC_MIPSEL} --compile-execute-int-8 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPSEL} --compile-execute-int-16 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPSEL} --compile-execute-int-32 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPSEL} --compile-execute-int-64 test/squares.b | diff - test/squares_result.txt
	${EXEC_MIPSEL} --compile-execute-int-128 test/squares.b | diff - test/squares_result.txt

test-compiler-yapi-all-arch: test-compiler-yapi-i386 test-compiler-yapi-wine test-compiler-yapi-x86-64 test-compiler-yapi-ppc test-compiler-yapi-arm test-compiler-yapi-sparc test-compiler-yapi-mips test-compiler-yapi-mipsel

test-compiler-yapi-i386:
	${EXEC_I386} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_I386} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_I386} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_I386} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt

test-compiler-yapi-wine:
	${EXEC_WINE} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result_wine.txt
	${EXEC_WINE} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result_wine.txt
	${EXEC_WINE} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result_wine.txt
	${EXEC_WINE} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result_wine.txt
	
test-compiler-yapi-x86-64:
	${EXEC_X86_64} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_X86_64} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_X86_64} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_X86_64} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_X86_64} --compile-execute-int-256 test/yapi.b | diff - test/yapi_result.txt

test-compiler-yapi-ppc:
	${EXEC_PPC} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_PPC} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt

test-compiler-yapi-arm:
	${EXEC_ARM} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_ARM} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt

test-compiler-yapi-sparc:
	${EXEC_SPARC} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_SPARC} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt

test-compiler-yapi-mips:
	${EXEC_MIPS} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPS} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt

test-compiler-yapi-mipsel:
	${EXEC_MIPSEL} --compile-execute-int-16 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPSEL} --compile-execute-int-32 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPSEL} --compile-execute-int-64 test/yapi.b | diff - test/yapi_result.txt
	${EXEC_MIPSEL} --compile-execute-int-128 test/yapi.b | diff - test/yapi_result.txt
