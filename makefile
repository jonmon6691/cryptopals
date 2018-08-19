CC=cl
CFLAGS=-Iinclude -I"C:\Program Files\OpenSSL\include" -nologo
#DEPS = hellomake.h
OBJ = crypto_tools.obj

all: chal1.exe chal1_ssl.exe chal2.exe chal3.exe chal4.exe chal5.exe chal6.exe chal7.exe chal8.exe chal9.exe chal10.exe

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

chal5.exe: obj/chal5.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal6.exe: obj/chal6.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)  -link -LIBPATH:"C:\Program Files\OpenSSL\lib" libcrypto.lib libssl.lib 
	cp chal6.exp obj && rm -f chal6.exp
	cp chal6.lib obj && rm -f chal6.lib

chal7.exe: obj/chal7.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)  -link -LIBPATH:"C:\Program Files\OpenSSL\lib" libcrypto.lib libssl.lib 
	cp chal7.exp obj && rm -f chal7.exp
	cp chal7.lib obj && rm -f chal7.lib

chal8.exe: obj/chal8.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal9.exe: obj/chal9.obj obj/crypto_tools.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

chal10.exe: obj/chal9.obj obj/crypto_tools.obj obj/aes.obj
	$(CC) /Fe: $@ $^ $(CFLAGS)

clean:
	rm -f obj/*.obj
	rm -f obj/*.lib
	rm -f obj/*.exp
