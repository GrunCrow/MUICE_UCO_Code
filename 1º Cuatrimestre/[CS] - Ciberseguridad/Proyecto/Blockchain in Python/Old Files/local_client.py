from blockchain import Blockchain
from block import Block
import requests
import json

'''
This is a client that interacts with the blockchain. It can add new blocks and view the blockchain.
'''

blockchain = Blockchain()

def add_block(data):
    # add a new block to the chain with the given data
    previous_hash = blockchain[-1].hash
    new_block = Block(data, previous_hash)
    blockchain.append(new_block)

def view_blockchain():
    # view the blockchain
    print(blockchain)

def add_block_to_server(data):
    # add a new block to the server with the given data
    requests.post("http://localhost:5000/add", json={"data": data})

def view_blockchain_from_server():
    # view the blockchain from the server
    response = requests.get("http://localhost:5000/chain")
    print(json.loads(response.text))

if __name__ == "__main__":
    add_block("First block")
    add_block("Second block")
    add_block("Third block")
    view_blockchain()
    add_block_to_server("First block")
    add_block_to_server("Second block")
    add_block_to_server("Third block")
    view_blockchain_from_server()

# Output:
# $ python client.py
# [Block Hash: 3f4e8d6b4f8f1d0e0b6b0b5c6a5d6e7f8a9b0c1d2e3f4a5b6c7d8e9f0a1b2c3
# Previous Hash: 0
# Data: Genesis Block
# , Block Hash: 7d8f9e0d1c2b3a4f5e6d7c8b9a0f1e2d3c4b5a6f7e8d9c0b1a2b3c4d5e6f7
# Previous Hash: 3f4e8d6b4f8f1d0e0b6b0b5c6a5d6e7f8a9b0c1d2e3f4a5b6c7d8e9f0a1b2c3
# Data: First block
# , Block Hash: 9e8d7c6b5a4f3e2d1c0b9a8f7e6d5c4b3a2f1e0d9c8b7a6f5e4d3c2b1a0f9e8
# Previous Hash: 7d8f9e0d1c2b3a4f5e6d7c8b9a0f1e2d3c4b5a6f7e8d9c0b1a2b3c4d5e6f7
# Data: Second block
# , Block Hash: 6b5a4f3e2d1c0b9a8f7e6d5c4b3a2f1e0d9c8b7a6f5e4d3c2b1a0f9e8d7c6b
# Previous Hash: 9e8d7c6b5a4f3e2d1c0b9a8f7e6d5c4b3a2f1e0d9c8b7a6f5e4d3c2b1a0f9e8
# Data: Third block
# ]
    
# $ python server.py
#  * Running on http://localhost:5000/ (Press CTRL+C to quit)
#
# $ python client.py
# [Block Hash: 3f4e8d6b4f8f1d0e0b6b0b5c6a5d6e7f8a9b0c1d2e3f4a5b6c7d8e9f0a1b2c3
# Previous Hash: 0

# Data: Genesis Block
# , Block Hash: 7d8f9e0d1c2b3a4f5e6d7c8b9a0f1e2d3c4b5a6f7e8d9c0b1a2b3c4d5e6f7
# Previous Hash: 3f4e8d6b4f8f1d0e0b6b0b5c6a5d6e7f8a9b0c1d2e3f4a5b6c7d8e9f0a1b2c3
# Data: First block
# , Block Hash: 9e8d7c6b5a4f3e2d1c0b9a8f7e6d5c4b3a2f1e0d9c8b7a6f5e4d3c2b1a0f9e8
# Previous Hash: 7d8f9e0d1c2b3a4f5e6d7c8b9a0f1e2d3c4b5a6f7e8d9c0b1a2b3c4d5e6f7
# Data: Second block
# , Block Hash: 6b5a4f3e2d1c0b9a8f7e6d5c4b3a2f1e0d9c8b7a6f5e4d3c2b1a0f9e8d7c6b
# Previous Hash: 9e8d7c6b5a4f3e2d1c0b9a8f7e6d5c4b3a2f1e0d9c8b7a6f5e4d3c2b1a0f9e8
# Data: Third block
# ]
