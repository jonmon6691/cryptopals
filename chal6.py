import base64
import itertools

cyphertext = None
with open("6.txt", 'rb') as f:
    filedata = f.read()
    cyphertext = base64.standard_b64decode(filedata)

def hamming_dist(a, b):
    diff = [ab^bb for (ab,bb) in zip(a, b)]
    dist = 0
    for b in diff:
        for i in range(8):
            dist += 0x1 & (b >> i)
    return dist

def grouper(n, iterable, fillvalue=None):
    "grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx"
    args = [iter(iterable)] * n
    return itertools.zip_longest(fillvalue=fillvalue, *args)

block_ham_dists = []
for key_size in range(1,40):
    n = 0
    hd_sum = 0
    for a, b in grouper(2, grouper(key_size, cyphertext, 0), (0,)*key_size):
        n += 1
        hd_sum += hamming_dist(a,b)/key_size
    if n == 0: continue
    block_ham_dists.append((key_size, hd_sum/n,n))

key_size = sorted(block_ham_dists, key=lambda x:x[1])[0]
print(key_size)

key = []
for block in zip(grouper(key_size, cyphertext)): # transpose
    key.append(sc_xor(block))

plaintext = bytes([ab ^ bb for (ab, bb) in zip(plaintext,itertools.cycle(key))])