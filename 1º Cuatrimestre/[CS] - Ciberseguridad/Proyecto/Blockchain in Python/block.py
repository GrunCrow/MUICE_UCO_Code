import hashlib
import time

'''
This is the basic unit of a blockchain. Each block contains data, the hash of the block, and the hash of the previous block.
'''

class Block:
    def __init__(self, index, previous_hash, timestamp, data, current_hash=None, miner_address=None):
        self.index = index
        self.previous_hash = previous_hash
        self.timestamp = timestamp
        self.data = data
        self.current_hash = current_hash or self.calculate_hash()
        # Proof Of Stake
        self.miner_address = miner_address

    def __str__(self):
        return f"Block Hash: {self.current_hash}\nPrevious Hash: {self.previous_hash}\nData: {self.data}"

    def __repr__(self):
        return str(self)

    def __eq__(self, other):
        return self.current_hash == other.current_hash and \
            self.previous_hash == other.previous_hash and \
            self.data == other.data

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(self.current_hash + self.previous_hash + self.data)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, key):
        return self.data[key]

    def __setitem__(self, key, value):
        self.data[key] = value

    def calculate_hash(self):
        data_string = str(self.index) + str(self.previous_hash) + str(self.timestamp) + str(self.data)
        return hashlib.sha256(data_string.encode()).hexdigest()

    def to_dict(self):
        block_dict = {
            "index": self.index,
            "previous_hash": self.previous_hash,
            "timestamp": self.timestamp,
            "data": self.data,
            "current_hash": self.current_hash,
            "miner_address": self.miner_address
        }
        return block_dict
    
    def is_valid(self):
        return self.current_hash == self.calculate_hash()
