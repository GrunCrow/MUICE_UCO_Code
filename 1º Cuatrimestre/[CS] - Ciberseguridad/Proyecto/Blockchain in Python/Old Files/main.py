from blockchain import Blockchain
import requests
import time

def local_cmd_run():
    # Create a blockchain
    my_blockchain = Blockchain()

    # Add example blocks
    my_blockchain.add_block("Datos del Bloque 1")
    my_blockchain.add_block("Datos del Bloque 2")
    my_blockchain.add_block("Datos del Bloque 3")

    # Print blockchain
    for block in my_blockchain.chain:
        print(f"Index: {block.index}")
        print(f"Previous Hash: {block.previous_hash}")
        print(f"Timestamp: {block.timestamp}")
        print(f"Data: {block.data}")
        print(f"Current Hash: {block.current_hash}")
        print("\n")

def poll_server():
    while True:
        try:
            response = requests.get('http://127.0.0.1:5000/')
            response.raise_for_status()
            print(response.json())
        except requests.exceptions.RequestException as e:
            print(f"Error while obtaining the blockchain from the server: {e}")
        time.sleep(5)  # Perform the request every 5 seconds (adjust as needed)


if __name__ == '__main__':
    # Create example blocks
    data = ["Block 1 Data", "Block 2 Data", "Block 3 Data"]

    # Send blocks to the server
    for d in data:
        response = requests.post('http://127.0.0.1:5000/', json={'data': d})
        try:
            response.raise_for_status()
            print(response.json())
        except requests.exceptions.RequestException as e:
            print(f"Error while adding block on the server: {e}")

    # Start the polling function in a separate thread
    import threading
    polling_thread = threading.Thread(target=poll_server)
    polling_thread.start()

    # Obtain and show example blocks from the server
    try:
        response = requests.get('http://127.0.0.1:5000/')
        response.raise_for_status()  # If fails, it raises an exception
        print(response.json())
    except requests.exceptions.RequestException as e:
        print("Error while trying to obtain the blockchain from the server", e)
