import string

cypher_text = bytes.fromhex('1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736')

letters = [ord(char) for char in string.ascii_letters+" '"]

biggest_yet = 0
for key in range(0xff):
    plaintext = bytes([byte ^ key for byte in cypher_text])
    score = sum([byte in letters for byte in plaintext])
    if score > biggest_yet:
        biggest_yet = score
        print("%s: %s" % (hex(key), plaintext))
