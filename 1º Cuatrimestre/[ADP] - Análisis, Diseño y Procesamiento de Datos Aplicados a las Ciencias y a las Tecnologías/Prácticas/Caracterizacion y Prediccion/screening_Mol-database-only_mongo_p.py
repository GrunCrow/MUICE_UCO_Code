#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Nov  1 21:48:38 2021

@author: gcerruela
"""

# importing the required libraries
import pymongo
import sys
import warnings
warnings.filterwarnings('ignore')
from time import time
from NON_SHARABLE import *

#################################################################################################################################
#Detalles conexion
connection_details  = MONGO_LINK
#connection_details  = 'mongodb://localhost:27017'


#Detalles_2 base de datos molecular
#db_name='CDS16';
db_name='CDS29';



qmol= 'Nc1nc2c(ncn2C2OC(CO)C(O)C2O)c(=O)[nH]1';
qfp=[71,75,80,90,128,147,152,194,210,222,233,314,334,345,356,378,428,433,454,463,492,508,519,577,610,633,650,656,672,697,700,721,740,750,774,807,820,831,849,897,932,935,1019]


threshold=0.4;
#################################################################################################################################



# connect to the mongoclient
client = pymongo.MongoClient(connection_details);

# the list_database_names() method returns a list of strings
database_names = client.list_database_names();

if db_name in database_names:
  print('The database ' +  db_name + ' exists');
  db=client.get_database(db_name);
else:
    print('&&&&&& The database ' +  db_name + ' must be loaded &&&&&.');
    print()
    sys.exit()
    

# El objetivo básico de las tareas de búsqueda de similitudes moleculares (screening) es encontrar moléculas en una base de datos que sean más similares a una molécula de consulta determinada



begin_time = time();


qn = len(qfp)
qmin = int(qn * threshold)
qmax = int(qn / threshold)
reqbits = [count['_id'] for count in db.mfp_counts.find({'_id': {'$in': qfp}}).sort('count', 1).limit(qn - qmin + 1)]
aggregate = [
    {'$match': {'mfp.count': {'$gte': qmin, '$lte': qmax}, 'mfp.bits': {'$in': reqbits}}},
    {'$project': {
        'tanimoto': {'$let': {
            'vars': {'common': {'$size': {'$setIntersection': ['$mfp.bits', qfp]}}},
            'in': {'$divide': ['$$common', {'$subtract': [{'$add': [qn, '$mfp.count']}, '$$common']}]}
        }},
        'smiles': 1,
        'chembl_id': 1
    }},
    {'$match':  {'tanimoto': {'$gte': threshold}}}
]
response = db.molecules.aggregate(aggregate);


for result in response:
   print ('%s : %s : %s' % (result['tanimoto'], result['_id'], result['smiles']))        
end_time = time(); 

elapse_time= end_time-begin_time;
print('Elapse Time: ' + str(round(elapse_time,2)) + ' segundos');

client.close();







