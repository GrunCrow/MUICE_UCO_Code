from flask import Flask, render_template, request, jsonify
from blockchain import Blockchain
import threading
from tkinter import Tk, Label, Button, Text, END, Entry
import requests
import webbrowser
import time

LOCAL_ADDRESS = "Local Miner"
SERVER_ADDRESS = "Server Miner"

app = Flask(__name__)
my_blockchain = Blockchain()

def add_block(data, miner_address):
    my_blockchain.add_block(data, miner_address)
    return {'message': 'Block added successfully', 'block': my_blockchain.get_latest_block().to_dict()}

last_server_response = None  # Store last server response to avoid updating the textbox when the server responds with the same data

def update_textbox():
    textbox.delete(1.0, END)
    response = requests.get('http://127.0.0.1:5000/?source=tkinter')  # Agrega ?source=tkinter al URL
    try:
        response.raise_for_status()
        blocks = response.json()
        for block in blocks:
            textbox.insert(END, f"Index: {block['index']}\n")
            textbox.insert(END, f"Miner Address: {block['miner_address']}\n")
            textbox.insert(END, f"Previous Hash: {block['previous_hash']}\n")
            textbox.insert(END, f"Timestamp: {block['timestamp']}\n")
            textbox.insert(END, f"Data: {block['data']}\n")
            textbox.insert(END, f"Current Hash: {block['current_hash']}\n")
            textbox.insert(END, "------------------------\n")
    except requests.exceptions.RequestException as e:
        print(f"Error while updating the textbox: {e}")


def add_block_local():
    data = entry.get()
    if data:
        miner_address = my_blockchain.select_miner()
        response = add_block(data, LOCAL_ADDRESS)
        update_textbox()
        entry.delete(0, END)
    else:
        print("Invalid request, data not provided")

@app.route('/', methods=['GET', 'POST'])
def index():
    global last_server_response
    if request.method == 'GET':
        # Verify if the request comes from the browser or from the tkinter client
        if request.args.get('source') == 'tkinter':
            return jsonify(my_blockchain.to_dict())
        else:
            # Render HTML template
            return render_template('index.html', blocks=my_blockchain.to_dict())
    elif request.method == 'POST':
        data = request.form.get('block_data')
        if data != last_server_response:
            response = add_block(data, SERVER_ADDRESS)
            update_textbox()
            last_server_response = data
            return render_template('index.html', blocks=my_blockchain.to_dict(), message=response['message'])
        else:
            update_textbox()
            return render_template('index.html', blocks=my_blockchain.to_dict())
        
@app.route('/validate', methods=['GET'])
def validate_blockchain():
    if my_blockchain.is_valid():
        return {'message': 'Blockchain is valid'}
    else:
        return {'message': 'Blockchain is not valid'}, 500

def start_server():
    app.run(port=5000)

def open_browser():
    webbrowser.open('http://127.0.0.1:5000/')

def scheduled_update():
    update_textbox()
    validate_blockchain()
    root.after(1000, scheduled_update)  # Schedule the next update after 1000 ms (1 second)

if __name__ == "__main__":
    server_thread = threading.Thread(target=start_server)
    server_thread.start()

    # Iniciar la interfaz local de Tkinter
    root = Tk()
    root.title("Blockchain Client")

    # Label
    label = Label(root, text="Blockchain Client")
    label.pack()

    # Texbox
    textbox = Text(root, height=30, width=100)
    textbox.pack()

    # Entry to add new blocks
    entry = Entry(root, width=30)
    entry.pack()

    # Button to add new blocks locally
    button = Button(root, text="Add Block (Local)", command=lambda: add_block_local(), bg="#007BFF", fg="white", relief="raised")
    button.pack()

    # Iniciar la actualización programada
    root.after(1000, scheduled_update)
    
    # Abrir el navegador web
    open_browser()

    # Iniciar la interfaz local de Tkinter
    root.mainloop()