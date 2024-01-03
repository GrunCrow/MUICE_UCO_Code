# Blockchain Project
This project is a simple implementation of a blockchain in Python. It includes a Flask server that serves as the interface for interacting with the blockchain, and a Tkinter GUI for a local client.

## Features
- Create new blocks with data
- Validate the integrity of the blockchain
- View the entire blockchain
- Very Basic Proof of Work (PoW) consensus algorithm (based on random selection of Miner)

## Installation
1. Clone this repository
2. Install the required Python packages or use the given environment

```
conda env create -f environment.yml
```

3. Activate the environment

```
conda activate Blockchain
```

## Usage
1. Execute gui_server.py
```
python gui_server.py
```

2. Open Flask server:
Open a web browser and navigate to [http://127.0.0.1:5000/](http://127.0.0.1:5000/) to interact with the blockchain.

3. Use the Tkinter GUI to add new blocks to the blockchain and view its current state or add new blocks from the server.

## API
The Flask server provides the following endpoints:

- `GET /`: Returns the current state of the blockchain.
- `POST /`: Adds a new block to the blockchain with the data provided in the block_data form field.
- `GET /validate`: Validates the integrity of the blockchain and returns a message indicating whether it is valid.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

MIT

## Disclaimer
This project is a simple demonstration of a blockchain and is not suitable for real-world use without further development and testing.