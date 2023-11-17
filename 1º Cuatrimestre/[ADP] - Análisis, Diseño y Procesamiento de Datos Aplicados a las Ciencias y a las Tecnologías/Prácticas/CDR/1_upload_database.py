#####################################################
# 1- Importar las bibliotecas y realizar la conexión
#####################################################
import os

import numpy as np
import json

import pymongo
import json
import warnings
warnings.filterwarnings('ignore')

from NON_SHARABLE import *

FILES_PATH = "Data/JSONs/"

# connect to the mongoclient
client = pymongo.MongoClient(MONGO_LINK)

client = pymongo.MongoClient('mongodb://localhost:27017')

# get the database
database = client['Milan_CDR_db'];

#####################################################
# 2- Crear las colecciones.
#####################################################

collist = database.list_collection_names()

if "Milan_CDR_c" in collist:
    print("The collection Milan_CDR_c exists.");
    Milan_CDR_c = database.get_collection("Milan_CDR_c");
else:
    # create collection
    database.create_collection("Milan_CDR_c")
    
    #####################################################
    # 3- Insertar datos en las colecciones.
    #####################################################
    # get collection
    Milan_CDR_c = database.get_collection("Milan_CDR_c");

    for file in os.listdir(FILES_PATH):
        with open(FILES_PATH + file) as f:
            file_data = json.load(f);
            # insert the data into the collection
            Milan_CDR_c.insert_many(file_data);