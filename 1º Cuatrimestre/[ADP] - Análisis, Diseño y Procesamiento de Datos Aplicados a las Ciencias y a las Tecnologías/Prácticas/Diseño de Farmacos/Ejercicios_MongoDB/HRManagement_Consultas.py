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




# Consulta para obtener todos los departamentos
all_departments = database['Departments'].find()
for department in all_departments:
    print(department)

# Consulta para obtener todos los empleados
all_employees = database['Employees'].find()
for employee in all_employees:
    print(employee)

# Consulta para obtener los empleados de un departamento específico (ejemplo con DEPTNO = '0')
it_department_employees = database['Employees'].find({"DEPTNO": "0"})
for employee in it_department_employees:
    print(employee)

# Consulta para obtener el nombre del jefe de un departamento (ejemplo con DEPTNO = '2')
marketing_department_boss = database['Employees'].find_one({"DEPTNO": "2", "ISBOSS": True})
print(marketing_department_boss['ENAME'])

# Consulta para obtener la cantidad total de empleados
total_employees_count = database['Employees'].count_documents({})
print(f"Total de empleados: {total_employees_count}")

# Consulta para obtener la cantidad total de empleados del departamento de IT (ejemplo con DEPTNO = '0')
it_department_employees_count = database['Employees'].count_documents({"DEPTNO": "0"})
print(f"Total de empleados del departamento de IT: {it_department_employees_count}")

# Consulta para obtener los empleados cuyo salario sea mayor a 20000
high_salary_employees = database['Employees'].find({"SAL": {"$gt": 20000}})
for employee in high_salary_employees:
    print(employee)

# Consulta para obtener los departamentos en una ubicación específica (ejemplo con LOC = 'Cordoba')
cordoba_departments = database['Departments'].find({"LOC": "Cordoba"})
for department in cordoba_departments:
    print(department)

# Consulta para obtener los empleados que fueron contratados después de cierta fecha (ejemplo con HIREDATE después de '2023-06-01')
recently_hired_employees = database['Employees'].find({"HIREDATE": {"$gt": "2023-06-01"}})
for employee in recently_hired_employees:
    print(employee)

# Consulta para obtener los empleados que son jefes (ISBOSS=True)
boss_employees = database['Employees'].find({"ISBOSS": True})
for boss in boss_employees:
    print(boss)
