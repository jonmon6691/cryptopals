a = bytes.fromhex('1c0111001f010100061a024b53535009181c')
b = bytes.fromhex('686974207468652062756c6c277320657965')
print(bytes([ab ^ bb for (ab, bb) in zip(a,b)]))