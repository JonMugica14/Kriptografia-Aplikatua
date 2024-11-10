#!/usr/bin/env python3

import secrets, hashlib

class DSA:
    '''
    Reads and writes DSA keys from/to files (just 1024 bits long).
    The hash of the messages is computed with SHA-1.
    '''
    @classmethod
    def int2hex(cls, num, length=None):
        '''
        Returns a string representing the hexadecimal code of num.
        If length is set, leading zero bytes are added as needed.
        '''
        if length:
            length *= 2
            return '{:0{length}x}'.format(num, length=length)
        else:
            return format(num, 'x')
    
    def __init__(self, p=None, q=None, alpha=None, beta=None, d=None):
        '''
        Sets the initial parameters. The variable names follow the
        nomenclature in the book "Understanding Cryptograhy".
        p and q are the group and subgroup cardinalities.
        alpha is the generator.
        beta is the public/verification key.
        d is the private/signing key.
        '''
        self.p = p
        self.q = q
        self.alpha = alpha
        self.beta = beta
        self.d = d
    
    def read_publickey(self, filename):
        '''
        Read the system parameters and the public/verification key from a
        file.
        '''
        with open(filename, "rb") as f:
            line = f.readline()
            self.p = int(line, 16)
            line = f.readline()
            self.q = int(line, 16)
            line = f.readline()
            self.alpha = int(line, 16)
            line = f.readline()
            self.beta = int(line, 16)

    def read_privatekey(self, filename):
        '''
        Read the private/signing key from a file.
        '''
        with open(filename, "rb") as f:
            line = f.readline()
            self.d = int(line, 16)

    def write_publickey(self, filename):
        '''
        Write the system parameters and the public/verification key to a
        file.
        '''
        if not self.p:
            raise Exception('Cannot write public key. Not set.')
        with open(filename, "wt") as f:
            f.write(self.int2hex(self.p) + '\n')
            f.write(self.int2hex(self.q) + '\n')
            f.write(self.int2hex(self.alpha) + '\n')
            f.write(self.int2hex(self.beta) + '\n')

    def write_privatekey(self, filename):
        '''
        Write the private/signing key to a file.
        '''
        if not self.d:
            raise Exception('Cannot write private key. Not set.')
        with open(filename, "wt") as f:
            f.write(self.int2hex(self.d) + '\n')

    def sign(self, m):
        '''
        Sign a message, m.
        '''
        if not self.d:
            raise Exception('Cannot sign. Private key not set.')
        digest = hashlib.sha1(m).digest()
        h = int.from_bytes(digest, 'big')
        ke = secrets.randbelow(self.q - 1) + 1 # 0 is not a valid key
        r = pow(self.alpha, ke, self.p) % self.q
        s = ((h + self.d*r) * pow(ke, -1, self.q)) % self.q
        
        return (r << 160) + s
    
    def verify(self, m, signature):
        '''
        Verify a signature.
        '''
        if not self.p:
            raise Exception('Cannot verify. Public key not set.')
        
        # Split the signature into r and s
        r = signature >> 160
        s = signature & ((1 << 160) - 1)

        if not (0 < r < self.q) or not (0 < s < self.q):
          return False  # Signature is out of bounds

        digest = hashlib.sha1(m).digest()
        h = int.from_bytes(digest, 'big')

        # Calculate w, u1, and u2
        w = pow(s, -1, self.q)
        u1 = (h * w) % self.q
        u2 = (r * w) % self.q

      # Calculate v
        v1 = (pow(self.alpha, u1, self.p) * pow(self.beta, u2, self.p)) % self.p
        v = v1 % self.q
        # Compute v and r
                
        return v == r

    def gakoa_lortu(self, m1, m2, signature1, signature2): 
        h_x1 = int.from_bytes(hashlib.sha1(m1).digest(), 'big')
        h_x2 = int.from_bytes(hashlib.sha1(m2).digest(), 'big')

        r = signature1 >> 160

        s1 = signature1 & ((1 << 160) - 1) 
        s2 = signature2 & ((1 << 160) - 1)
     
        k = ((h_x1 - h_x2) * pow(s1 - s2, -1, self.q)) % self.q
       
        d = ((s1 * k - h_x1) * pow(r, -1, self.q)) % self.q

        self.d = d
        return d
        #return k, d
