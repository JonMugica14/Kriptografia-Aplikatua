#!/usr/bin/env python3

import os
from myDSA import DSA

FILES_PATH='files'
PUBLICKEY_FILENAME='public'
SIGNATURES_FILENAME='signatures'

def main():
    dsa = DSA()
    mezuak = []
    sinadurak = []
    gakoa = 0
    dsa.read_publickey(PUBLICKEY_FILENAME)
    filenames, signatures = read_signatures(SIGNATURES_FILENAME)

    for filename, signature in zip(filenames, signatures):
        with open(os.path.join(FILES_PATH, filename), "rb") as f:
            data = f.read()
            if dsa.verify(data, signature):
                print(f'[{filename}]: Valid signature.')
                mezuak.append(data)
                sinadurak.append(signature)
            else:
                print(f'[{filename}]: Invalid signature.')

    gakoa = dsa.gakoa_lortu(mezuak[1], mezuak[2],sinadurak[1], sinadurak[2])
    print(gakoa)
    dsa.d = gakoa
    signature2 = dsa.sign(mezuak[1])
    if dsa.verify(mezuak[1], signature2):
        print('Test valid signature')
    else:
        print('Test INvalid signature')

def read_signatures(filename):
    filenames = []
    signatures = []
    with open(filename, "rt") as f:
        lines = f.readlines()
    for filename, signature in zip(lines[0::2], lines[1::2]):
        filenames.append(filename.rstrip())
        signatures.append(int(signature.rstrip(), 16))

    return filenames, signatures

if __name__ == '__main__':
    main()
