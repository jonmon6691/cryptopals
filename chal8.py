""" Detect AES in ECB mode
    In this file are a bunch of hex-encoded ciphertexts.

    One of them has been encrypted with ECB.

    Detect it.

    Remember that the problem with ECB is that it is stateless and deterministic; the same 16 byte plaintext block will always produce the same 16 byte ciphertext.

"""
import base64
import itertools
import string

ciphertexts = []
with open("8.txt", 'r') as f:
    for line in f:
        ciphertexts.append(bytes.fromhex(line.strip()))

def grouper(n, iterable, fillvalue=None):
    """ grouper(3, 'ABCDEFG', 'x') --> ABC DEF Gxx """
    args = [iter(iterable)] * n
    return itertools.zip_longest(fillvalue=fillvalue, *args)

for i, ct in enumerate(ciphertexts):
    matches = 0
    for j, blk1 in enumerate(grouper(16, ct)):
        for k, blk2 in enumerate(grouper(16, ct)):
            if j != k and blk1 == blk2:
                print ("Row %d: Blocks %d, %d both %s" % (i + 1, j, k, bytes(blk1).hex()))
    