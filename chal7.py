import base64
import itertools
import string

cyphertext = None
with open("7.txt", 'rb') as f:
    cyphertext = base64.standard_b64decode(f.read())

def hamming_dist(a, b):
    diff = [ab^bb for (ab,bb) in zip(a, b)]
    dist = 0
    for b in diff:
        for i in range(8):
            dist += 0x1 & (b >> i)
    return dist

def grouper(n, iterable, fillvalue=None):
    """ grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx """
    args = [iter(iterable)] * n
    return itertools.zip_longest(fillvalue=fillvalue, *args)

block_ham_dists = []
for key_size in range(1,40):
    n = 0
    hd_sum = 0.0
    for a, b in grouper(2, grouper(key_size, cyphertext, 0), (0,)*key_size):
        n += 1
        hd_sum += hamming_dist(a,b)/key_size
    block_ham_dists.append((key_size, hd_sum/n,n))

key_size = sorted(block_ham_dists, key=lambda x:x[1])[0][0]
print("key size: %d" % key_size)

def sc_xor(cypher_text):
    letters = [ord(char) for char in string.ascii_letters+" "]

    best_score = 0
    best_plaintext = None
    best_key = None
    for key in range(0xff):
        plaintext = bytes([byte ^ key for byte in cypher_text])
        score = sum([byte in letters for byte in plaintext])/ len(plaintext)
        if score > best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext

key = []
for block in zip(*grouper(key_size, cyphertext, fillvalue=0)): # transpose
    key_part, pt = sc_xor(bytes(block))
    # print("%02x: %s" % (key_part, pt))
    key.append(key_part)

print(bytes(key), len(key))
plaintext = bytes([ab ^ bb for (ab, bb) in zip(cyphertext,itertools.cycle(key))])

print(plaintext.decode('ASCII'))
