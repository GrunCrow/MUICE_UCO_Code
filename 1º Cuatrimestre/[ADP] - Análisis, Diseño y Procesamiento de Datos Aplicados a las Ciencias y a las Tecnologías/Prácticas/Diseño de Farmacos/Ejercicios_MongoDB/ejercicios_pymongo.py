# -*- coding: utf-8 -*-
"""
Editor de Spyder

Este es un archivo temporal.
"""


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
database = client['sample_db'];

#####################################################
# 2- Crear las colecciones.
#####################################################

collist = database.list_collection_names()

if "weekly_demand" in collist:
  print("The collection weekly_demand exists.");
  weekly_demand_collection = database.get_collection("weekly_demand");
else:
      # create weekly demand collection
      database.create_collection("weekly_demand")
      
       #####################################################
       # 3- Insertar datos en las colecciones.
       #####################################################
      # get collection weekly_demand
      weekly_demand_collection = database.get_collection("weekly_demand");
      # open the weekly_demand json file
      with open("weekly_demand.json") as f:
        file_data = json.load(f);
        # insert the data into the collection
        weekly_demand_collection.insert_many(file_data);
        # get the count of total data points
        #print("Collection weekly_demand created: " + str(weekly_demand_collection.find().count()) + 'registros');
        # >> 456548


if "meal_info" in collist:
  print("The collection meal_info exists.");
  meal_info_collection = database.get_collection("meal_info");
else:
      # create weekly demand collection
      database.create_collection("meal_info");
      
      #####################################################
      # 3- Insertar datos en las colecciones.
      #####################################################      
      # get collection meal_info 
      meal_info_collection = database.get_collection("meal_info");
      # open the meal_info json file
      with open("meal_info.json") as f:
          file_data = json.load(f);
          # insert the data into the collection
          meal_info_collection.insert_many(file_data);
          # get the count of the total data points
          # print("Collection meal_info created: " +  str(meal_info_collection.find().count()) + 'registros');
          # >> 51


#####################################################
# 4- consultas pagina 24
#####################################################
print('-----------------------------------------------')
print(weekly_demand_collection.find_one( {}, { "week": 1, "checkout_price" : 1}))
print('-----------------------------------------------')
print(weekly_demand_collection.find_one( {}, {"num_orders" : 0, "meal_id" : 0}))
print('-----------------------------------------------')
print(weekly_demand_collection.find_one( {"center_id" : 55, "meal_id" : 1885}, {"_id" : 0, "week" : 0} ))
print('-----------------------------------------------')

#####################################################
# 4- consultas pagina 25
#####################################################
result_1 = weekly_demand_collection.find({
    "center_id" : { "$eq" : 55},
    "homepage_featured" : { "$ne" : 0}
})

#for i in result_1:
#    print(i)
    
#####################################################
# 5- consultas pagina 26
#####################################################
result_2 = weekly_demand_collection.find({
    "center_id" : { "$in" : [ 24, 11] }
})

#for i in result_2:
#    print(i)

result_3 = weekly_demand_collection.find({
    "center_id" : { "$nin" : [ 24, 11] }
})

#for i in result_3:
#    print(i)    
    
#####################################################
# 4- consultas pagina 27
#####################################################
result_4 = weekly_demand_collection.find({
    "center_id" : 55,
    "checkout_price" : { "$lt" : 200, "$gt" : 100}
})

# for i in result_4:
#    print(i)

    
#####################################################
# 4- consultas pagina 28
#####################################################
result_5 = weekly_demand_collection.find({
    "$and" : [{
                 "center_id" : { "$eq" : 11}
              },
              {
                   "meal_id" : { "$ne" : 1778}
              }]
})

#for i in result_5:
#    print(i)
    
#####################################################
# 4- consultas pagina 29
#####################################################
result_6 = weekly_demand_collection.find({
    "$or" : [{
                 "center_id" : { "$eq" : 11}
              },
              {
                   "meal_id" : { "$in" : [1207, 2707]}
              }]
})

#for i in result_6:
#    print(i)    
    
#####################################################
# 4- consultas pagina 30
#####################################################
result_7 = meal_info_collection.find({
    "cuisine" : { "$regex" : "^C" }
})

#for i in result_7:
#    print(i)
    
result_8 = meal_info_collection.find({
    "$and" : [
        { 
            "category" : {
            "$regex" : "^S"
        }},
        {
            "cuisine" : {
                "$regex" : "ian$"
        }}
    ]
})

#for i in result_8:
#    print(i)

#####################################################
# 4- consultas pagina 31
#####################################################

result_9 = weekly_demand_collection.aggregate([
    ## stage 1
    {
        "$match" : 
                 {"center_id" : {"$eq" : 11 } }
    },
    ## stage 2
    {
        "$count" : "total_rows"
    }
])

#for i in result_9:
#    print(i)
    
#####################################################
# 4- consultas pagina 32
#####################################################
    
result_10 = weekly_demand_collection.aggregate([
    ## stage 1
    {
        "$match" : 
                 {"center_id" : {"$eq" : 11 } }
    },
    ## stage 2
    {
        "$group" : { "_id" : 0 ,
                     "average_num_orders": { "$avg" : "$num_orders"},
                     "unique_meal_id" : {"$addToSet" : "$meal_id"}} 
    }
])

#for i in result_10:
#    print(i)






