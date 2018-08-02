CC=cl
CFLAGS=-Iinclude -I"C:\Program Files\OpenSSL\include"
#DEPS = hellomake.h
OBJ = crypto_tools.obj

all: chal1.exe chal1_ssl.exe chal2.exe

obj/%.obj: src/%.c
	$(CC) -c /Fo: $@ $< $(CFLAGS)

chal1.exe: obj/chal1.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal1_ssl.exe: obj/chal1_ssl.obj obj/crypto_tools.obj 
	$(CC) /Fe: $@ $^ $(CFLAGS)  libcrypto.lib libssl.lib -link -LIBPATH:"C:\Program Files\OpenSSL\lib" 

chal2.exe: obj/chal2.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

clean:
	rm obj/*.obj
