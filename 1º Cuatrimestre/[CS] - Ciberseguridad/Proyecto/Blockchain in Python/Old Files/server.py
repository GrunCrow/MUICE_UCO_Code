from flask import Flask, jsonify, request
from blockchain import Blockchain

'''
This is a server that uses the blockchain. It can add new blocks and provides an API for interacting with the blockchain.
'''

app = Flask(__name__)
my_blockchain = Blockchain()

@app.route('/', methods=['GET', 'POST'])
def handle_request():
    if request.method == 'GET':
        return jsonify(my_blockchain.to_dict()), 200
    elif request.method == 'POST':
        data = request.get_json()
        if 'data' in data:
            my_blockchain.add_block(data['data'])
            response = {'message': 'Block added successfully', 'block': my_blockchain.get_latest_block().to_dict()}
            return jsonify(response), 201
        else:
            return jsonify({'message': 'Invalid request, data not provided'}), 400

if __name__ == '__main__':
    app.run(port=5000)