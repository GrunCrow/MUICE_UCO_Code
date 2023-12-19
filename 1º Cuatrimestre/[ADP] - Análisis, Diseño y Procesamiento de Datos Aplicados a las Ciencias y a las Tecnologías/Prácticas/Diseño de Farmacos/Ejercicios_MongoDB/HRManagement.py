#####################################################
# 1- Importar las bibliotecas y realizar la conexión
#####################################################

# importing the required libraries
import pymongo
import json
import warnings
warnings.filterwarnings('ignore')

from NON_SHARABLE import *

#-----------------------------------------


# connect to the mongoclient
client = pymongo.MongoClient(MONGO_LINK)

client = pymongo.MongoClient('mongodb://localhost:27017')



# get the database
database = client['HRManagement']

#####################################################
# 2- Crear las colecciones.
#####################################################

collections = ['Departments', 'Employees']
for collection_name in collections:
    if collection_name not in database.list_collection_names():
        database.create_collection(collection_name)


#####################################################
# 3- Insertar datos en las colecciones.
#####################################################

departments_data = [
    {"DEPTNO": "0", "DNAME": "IT", "LOC": "Cordoba", "MANAGER": "0"},
    {"DEPTNO": "1", "DNAME": "Human Resources", "LOC": "Cordoba", "MANAGER": "1"},
    {"DEPTNO": "2", "DNAME": "Marketing", "LOC": "Sevilla", "MANAGER": "6"},
    {"DEPTNO": "3", "DNAME": "Finance", "LOC": "Huelva", "MANAGER": "2"},
    {"DEPTNO": "4", "DNAME": "Operations", "LOC": "Cadiz", "MANAGER": "3"},
    {"DEPTNO": "5", "DNAME": "Customer Support", "LOC": "Valencia", "MANAGER": "4"},
]

employees_data = [
    {"EMPNO": "0", "ENAME": "Alba Marquez Rodriguez", "JOB": "Computing Engineer", "HIREDATE": "2023-12-01",
    "SAL": 25000, "COMM": 1000, "DEPTNO": "0", "ISBOSS": True, "BOSS": None},
    {"EMPNO": "1", "ENAME": "Maria Isabel Hernandez Gonzalez", "JOB": "Human Resources", "HIREDATE": "2023-11-01",
    "SAL": 13000, "COMM": 1000, "DEPTNO": "1", "ISBOSS": True, "BOSS": None},
    {"EMPNO": "2", "ENAME": "Juana de la Cruz Santos", "JOB": "IT Technician", "HIREDATE": "2023-04-04",
    "SAL": 20000, "COMM": 1000, "DEPTNO": "0", "ISBOSS": False, "BOSS": 0},
    {"EMPNO": "3", "ENAME": "Jorge Perez Munoz", "JOB": "IT Technician", "HIREDATE": "2023-15-01",
    "SAL": 15000, "COMM": 1000, "DEPTNO": "1", "ISBOSS": False, "BOSS": 0},
    {"EMPNO": "4", "ENAME": "Lola Martinez Caraballo", "JOB": "IT Technician", "HIREDATE": "2023-14-02",
    "SAL": 13000, "COMM": 1000, "DEPTNO": "1", "ISBOSS": False, "BOSS": 0},
    {"EMPNO": "5", "ENAME": "Luis Moreno Diaz", "JOB": "Psicologist", "HIREDATE": "2023-18-02",
    "SAL": 12000, "COMM": 1000, "DEPTNO": "1", "ISBOSS": False, "BOSS": 1},
    {"EMPNO": "6", "ENAME": "Amanda Villa Lopez", "JOB": "Marketer", "HIREDATE": "2023-18-02",
    "SAL": 23000, "COMM": 1000, "DEPTNO": "2", "ISBOSS": True, "BOSS": 0},
    {"EMPNO": "7", "ENAME": "Carlos Ruiz Gomez", "JOB": "Financial Analyst", "HIREDATE": "2023-05-10",
    "SAL": 18000, "COMM": 800, "DEPTNO": "3", "ISBOSS": True, "BOSS": None},
    {"EMPNO": "8", "ENAME": "Ana Jimenez Garcia", "JOB": "Operations Manager", "HIREDATE": "2023-06-15",
    "SAL": 22000, "COMM": 1200, "DEPTNO": "4", "ISBOSS": True, "BOSS": None},
    {"EMPNO": "9", "ENAME": "Miguel Serrano Navarro", "JOB": "Customer Support Specialist", "HIREDATE": "2023-04-20",
    "SAL": 16000, "COMM": 700, "DEPTNO": "5", "ISBOSS": True, "BOSS": None},
    {"EMPNO": "10", "ENAME": "Elena Rodriguez Fernandez", "JOB": "IT Analyst", "HIREDATE": "2023-05-01",
    "SAL": 19000, "COMM": 900, "DEPTNO": "0", "ISBOSS": False, "BOSS": 0},
    {"EMPNO": "11", "ENAME": "Pedro Sanchez Gomez", "JOB": "Marketing Specialist", "HIREDATE": "2023-06-10",
    "SAL": 21000, "COMM": 1000, "DEPTNO": "2", "ISBOSS": False, "BOSS": 6},
    {"EMPNO": "12", "ENAME": "Sara Diaz Hernandez", "JOB": "Customer Support Representative", "HIREDATE": "2023-07-01",
    "SAL": 17000, "COMM": 800, "DEPTNO": "5", "ISBOSS": False, "BOSS": 9},
    {"EMPNO": "13", "ENAME": "Antonio Lopez Ruiz", "JOB": "Finance Assistant", "HIREDATE": "2023-08-15",
    "SAL": 15000, "COMM": 700, "DEPTNO": "3", "ISBOSS": False, "BOSS": 7},
    {"EMPNO": "14", "ENAME": "Laura Martin Alonso", "JOB": "Operations Specialist", "HIREDATE": "2023-09-20",
    "SAL": 20000, "COMM": 1000, "DEPTNO": "4", "ISBOSS": False, "BOSS": 8},
    {"EMPNO": "15", "ENAME": "Pablo Garcia Rodriguez", "JOB": "IT Specialist", "HIREDATE": "2023-10-01",
    "SAL": 18000, "COMM": 900, "DEPTNO": "0", "ISBOSS": False, "BOSS": 10},
    {"EMPNO": "16", "ENAME": "Isabel Ramos Martinez", "JOB": "Customer Support Representative", "HIREDATE": "2023-11-10",
    "SAL": 16000, "COMM": 800, "DEPTNO": "5", "ISBOSS": False, "BOSS": 9},
    {"EMPNO": "17", "ENAME": "Manuel Torres Rodriguez", "JOB": "IT Analyst", "HIREDATE": "2023-12-01",
    "SAL": 18000, "COMM": 900, "DEPTNO": "0", "ISBOSS": False, "BOSS": 10},
    {"EMPNO": "18", "ENAME": "Silvia Fernandez Navarro", "JOB": "Marketing Coordinator", "HIREDATE": "2023-01-15",
    "SAL": 19000, "COMM": 1000, "DEPTNO": "2", "ISBOSS": False, "BOSS": 6},
    {"EMPNO": "19", "ENAME": "Rafael Lopez Diaz", "JOB": "Finance Analyst", "HIREDATE": "2023-02-20",
    "SAL": 17000, "COMM": 800, "DEPTNO": "3", "ISBOSS": False, "BOSS": 7},
    {"EMPNO": "20", "ENAME": "Carmen Serrano Ruiz", "JOB": "Operations Specialist", "HIREDATE": "2023-03-01",
    "SAL": 20000, "COMM": 1000, "DEPTNO": "4", "ISBOSS": False, "BOSS": 8},
    {"EMPNO": "21", "ENAME": "Diego Garcia Herrera", "JOB": "IT Specialist", "HIREDATE": "2023-04-10",
    "SAL": 19000, "COMM": 900, "DEPTNO": "0", "ISBOSS": False, "BOSS": 10},
    {"EMPNO": "22", "ENAME": "Natalia Diaz Martin", "JOB": "Customer Support Representative", "HIREDATE": "2023-05-15",
    "SAL": 16000, "COMM": 800, "DEPTNO": "5", "ISBOSS": False, "BOSS": 9},
    {"EMPNO": "23", "ENAME": "Juan Perez Gomez", "JOB": "Marketing Specialist", "HIREDATE": "2023-06-01",
    "SAL": 21000, "COMM": 1000, "DEPTNO": "2", "ISBOSS": False, "BOSS": 6},
    {"EMPNO": "24", "ENAME": "Sofia Martin Torres", "JOB": "Finance Assistant", "HIREDATE": "2023-07-10",
    "SAL": 15000, "COMM": 700, "DEPTNO": "3", "ISBOSS": False, "BOSS": 7},
    {"EMPNO": "25", "ENAME": "Alejandro Ruiz Garcia", "JOB": "Operations Manager", "HIREDATE": "2023-08-15",
    "SAL": 22000, "COMM": 1200, "DEPTNO": "4", "ISBOSS": False, "BOSS": 8},
]

#####################################################
# 3- Insertar datos en las colecciones.
#####################################################      

# Insertar datos en la colección "Departments"
database['Departments'].insert_many(departments_data)

# Insertar datos en la colección "Employees"
database['Employees'].insert_many(employees_data)


#####################################################
# 4- consultas pagina 25
#####################################################
'''result_1 = weekly_demand_collection.find({
    "center_id" : { "$eq" : 55},
    "homepage_featured" : { "$ne" : 0}
})

#for i in result_1:
#    print(i)'''