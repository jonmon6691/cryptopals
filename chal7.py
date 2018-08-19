import base64
import itertools
import string
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

backend = default_backend()

cipher = Cipher(algorithms.AES(b"YELLOW SUBMARINE"), modes.ECB(), backend)
aes_ecb_dec = cipher.decryptor()

ciphertext = None
with open("7.txt", 'rb') as f:
    ciphertext = base64.standard_b64decode(f.read())

plaintext = aes_ecb_dec.update(ciphertext) + aes_ecb_dec.finalize()

print(plaintext.decode('ASCII'))
