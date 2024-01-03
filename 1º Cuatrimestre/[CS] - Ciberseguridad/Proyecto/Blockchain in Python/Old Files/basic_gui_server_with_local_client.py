from flask import Flask, jsonify, request
from blockchain import Blockchain
from tkinter import Tk, Label, Button, Text, END, Entry
import requests
import threading

app = Flask(__name__)
my_blockchain = Blockchain()

def update_textbox():
    textbox.delete(1.0, END)
    response = requests.get('http://127.0.0.1:5000/')
    try:
        response.raise_for_status()
        blocks = response.json()
        for block in blocks:
            textbox.insert(END, f"{block}\n")
    except requests.exceptions.RequestException as e:
        print(f"Error while updating the textbox: {e}")

def add_block():
    # Add a new block to the server with the given data
    data = entry.get()
    response = requests.post('http://127.0.0.1:5000/', json={'data': data})
    try:
        response.raise_for_status()
        update_textbox()
        entry.delete(0, END)  # Borra el contenido del Entry
    except requests.exceptions.RequestException as e:
        print(f"Error while adding block on the server: {e}")

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

def start_server():
    app.run(port=5000)

def scheduled_update():
    update_textbox()
    root.after(1000, scheduled_update)  # Programa la próxima actualización después de 1000 ms (1 segundo)

# GUI configuration
root = Tk()
root.title("Blockchain Server")

# Label
label = Label(root, text="Blockchain Server")
label.pack()

# Texbox
textbox = Text(root, height=10, width=50)
textbox.pack()

# Entry to add new blocks
entry = Entry(root, width=30)
entry.pack()

# Button to add new blocks
button = Button(root, text="Add Block", command=lambda: add_block())
button.pack()

# Initiate the server
server_thread = threading.Thread(target=start_server)
server_thread.start()

# Initiate the GUI
root.after(1000, scheduled_update)
root.mainloop()
