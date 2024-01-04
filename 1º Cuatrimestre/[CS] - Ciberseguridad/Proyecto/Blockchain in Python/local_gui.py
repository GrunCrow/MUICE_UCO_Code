from tkinter import Tk, Label, Button, Text, END, Entry
import requests

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
    # Agrega un nuevo bloque al servidor con los datos proporcionados
    data = entry.get()
    response = requests.post('http://127.0.0.1:5000/', data={'block_data': data})
    try:
        response.raise_for_status()
        update_textbox()
        entry.delete(0, END)  # Borra el contenido del Entry
    except requests.exceptions.RequestException as e:
        print(f"Error while adding block on the server: {e}")

# Configuración de la GUI local
root = Tk()
root.title("Local Blockchain Client")

# Etiqueta
label = Label(root, text="Local Blockchain Client")
label.pack()

# Caja de texto
textbox = Text(root, height=10, width=50)
textbox.pack()

# Entry para agregar nuevos bloques
entry = Entry(root, width=30)
entry.pack()

# Botón para agregar nuevos bloques
button = Button(root, text="Add Block", command=add_block)
button.pack()

# Inicia la actualización programada de la caja de texto
root.after(1000, update_textbox)

# Inicia la GUI local
root.mainloop()
