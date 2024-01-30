from web3 import Web3
from eth_account import Account
import random

# Conéctate a tu nodo Ethereum (puedes usar un nodo local o Infura)
w3 = Web3(Web3.HTTPProvider('http://localhost:7545'))

# Define tu clave privada y la dirección del contrato compilado
private_key = '0x994b848eedfbc4c3c161b6d77576182ff2cbb3c82f0eb24cc88aced74631350c'
contract_address = '0xB9A8c7300573cbdDAd4CaB37e52f45940011AE49'

# Crea una instancia del contrato inteligente
contract_abi = [{"inputs":[],"name":"data","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"stateMutability":"view","type":"function"},{"inputs":[{"internalType":"uint256","name":"_data","type":"uint256"}],"name":"setData","outputs":[],"stateMutability":"nonpayable","type":"function"}]
contract = w3.eth.contract(address=contract_address, abi=contract_abi)

# Establece la cuenta desde la clave privada
account = Account.from_key(private_key)

# Genera un valor de gas aleatorio entre 0 y 100000
gas = random.randint(5000, 100000)

# Transacción: Establece el valor del contrato
tx_hash = contract.functions.setData(42).transact({'from': account.address, 'gas': gas})

# Espera a que la transacción sea minada
transaction_receipt = w3.eth.wait_for_transaction_receipt(tx_hash)

# Lectura: Obtiene el valor del contrato
current_data = contract.functions.data().call()

print(f'Valor actual del contrato: {current_data}')
