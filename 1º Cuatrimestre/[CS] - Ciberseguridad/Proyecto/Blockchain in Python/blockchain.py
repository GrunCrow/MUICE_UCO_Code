from block import Block
import time
import random

'''
This is a chain of blocks, where each block is linked to the previous one via its hash.
'''

class Blockchain:
    def __init__(self):
        self.chain = [self.create_genesis_block()]

    def __str__(self):
        return str(self.chain)
    
    def __repr__(self):
        return str(self)
    
    def __eq__(self, other):
        return self.chain == other.chain
    
    def __ne__(self, other):
        return not self.__eq__(other)
    
    def __hash__(self):
        return hash(str(self))
    
    def __len__(self):
        return len(self.chain)
    
    def __getitem__(self, key):
        return self.chain[key]
    
    def __setitem__(self, key, value):
        self.chain[key] = value

    def create_genesis_block(self):
        # create the first block of the blockchain
        return Block(0, "0", int(time.time()), data="Genesis Block", miner_address="Genesis Miner")

    def get_latest_block(self):
        return self.chain[-1]

    def add_block(self, data, miner_address):
        index = len(self.chain)
        previous_block = self.get_latest_block()
        previous_hash = previous_block.current_hash
        timestamp = int(time.time())
        new_block = Block(index, previous_hash, timestamp, data, miner_address=miner_address)
        self.chain.append(new_block)
    
    def to_dict(self):
        return [block.to_dict() for block in self.chain]
    
    def is_valid(self):
        for i in range(1, len(self.chain)):
            current_block = self.chain[i]
            previous_block = self.chain[i - 1]

            if not current_block.is_valid() or \
                current_block.previous_hash != previous_block.current_hash:
                return False

        return True
    
    def select_miner(self):
        # Simple consensus mechanism: Randomly select a miner from the current participants
        participants = ["Local Miner", "Server Miner"]  # Add more participant addresses if needed
        return random.choice(participants)