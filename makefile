CC=cl
CFLAGS=-Iinclude
#DEPS = hellomake.h
OBJ = crypto_tools.obj

all: chal1.exe chal2.exe

obj/%.obj: src/%.c
	$(CC) -c /Fo: $@ $< $(CFLAGS)

chal1.exe: obj/chal1.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal2.exe: obj/chal2.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

clean:
	rm obj/*.obj
