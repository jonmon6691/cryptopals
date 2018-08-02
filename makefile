CC=cl
CFLAGS=-Iinclude -I"C:\Program Files\OpenSSL\include" -nologo
#DEPS = hellomake.h
OBJ = crypto_tools.obj

all: chal1.exe chal1_ssl.exe chal2.exe chal3.exe chal4.exe

obj/%.obj: src/%.c
	$(CC) -c /Fo: $@ $< $(CFLAGS)

chal1.exe: obj/chal1.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal1_ssl.exe: obj/chal1_ssl.obj obj/crypto_tools.obj 
	$(CC) /Fe: $@ $^ $(CFLAGS)  -link -LIBPATH:"C:\Program Files\OpenSSL\lib" libcrypto.lib libssl.lib 
	cp chal1_ssl.exp obj && rm -f chal1_ssl.exp
	cp chal1_ssl.lib obj && rm -f chal1_ssl.lib

chal2.exe: obj/chal2.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal3.exe: obj/chal3.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal4.exe: obj/chal4.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

clean:
	rm -f obj/*.obj
	rm -f obj/*.lib
	rm -f obj/*.exp
