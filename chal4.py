import string

def sc_xor(cypher_text):
    cypher_text = bytes.fromhex(cypher_text.strip())
    letters = [ord(char) for char in string.ascii_letters+" '"]

    best_score = 0
    best_plaintext = None
    best_key = None
    best_len = 0
    for key in range(0xff):
        plaintext = bytes([byte ^ key for byte in cypher_text])
        score = sum([byte in letters for byte in plaintext])/ len(plaintext)
        if score > best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext
            best_len = len(plaintext)

    return (best_score, best_key, best_plaintext, best_len)

best_score = 0
with open("4.txt", 'r') as file:
    for n, line in enumerate(file):
        score, key, pt, best_len = sc_xor(line)
        if score > best_score:
            best_score = score
            print("Line %d, Key %s, Score %f: %s" % (n, hex(key), score, pt))