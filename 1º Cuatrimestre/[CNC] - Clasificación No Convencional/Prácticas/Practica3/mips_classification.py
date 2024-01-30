from hiclass import LocalClassifierPerNode
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from hiclass import metrics
import pandas as pd
import arff

# Cargar los datos
try:
    # Cargar el archivo ARFF
    with open('data/mips10trainatt.arff', 'r') as f:
        arff_dict_train = arff.load(f)
    with open('data/mips10testatt.arff', 'r') as f:
        arff_dict_test = arff.load(f)
except FileNotFoundError:
    print("Archivo no encontrado. Por favor, asegúrate de que el nombre del archivo es correcto.")

# Extracción de metadatos y datos
metadatos_train = arff_dict_train.get('attributes', [])
datos_train = arff_dict_train.get('data', [])
clasificacion_train = arff_dict_train.get('classification_hierarchical', [])
print('clasificacion')
print(clasificacion_train)
print(metadatos_train)
# Convertir a DataFrame de Pandas para un manejo más fácil
df_train = pd.DataFrame(datos_train, columns=[attr[0] for attr in metadatos_train])
# print(df_train.head())  # Mostrar los primeros 5 registros

# Extracción de metadatos y datos
metadatos_test = arff_dict_test.get('attributes', [])
datos_test = arff_dict_test.get('data', [])
clasificacion_test = arff_dict_test.get('classification_hierarchical', [])
# Convertir a DataFrame de Pandas
df_test = pd.DataFrame(datos_test, columns=[attr[0] for attr in metadatos_test])
# print(df_test.head())  # Mostrar los primeros 5 registros

# Define datos de entrenamiento y test
# La última columna es la etiqueta de clasificación jerárquica
X_train =df_train[df_train.columns[:-1]]
Y_train =df_train[df_train.columns[-1]]

X_test =df_test[df_train.columns[:-1]]
Y_test =df_test[df_train.columns[-1]]

# y la columna "clasificacion_jerarquica" tiene los valores separados por "/"
# Dividir la columna en columnas separadas
Y_train = pd.DataFrame(Y_train)  # Convierte la Serie en un DataFrame
Y_test = pd.DataFrame(Y_test) 

Y_train[['nivel1', 'nivel2', 'nivel3','nivel4']] = Y_train['classification_hierarchical'].str.split('/', expand=True)
Y_train = Y_train.drop('classification_hierarchical', axis=1)
print('columns y_train',Y_train.columns)
print('Y_TRAIN.HEAD()',Y_train.head())  # Mostrar los primeros 5 registros


Y_test[['nivel1', 'nivel2', 'nivel3','nivel4']] = Y_test['classification_hierarchical'].str.split('/', expand=True)
Y_test = Y_test.drop('classification_hierarchical', axis=1)
print('columns y_test',Y_test.columns)
print('Y_TEST.HEAD()',Y_train.head())  # Mostrar los primeros 5 registros

print('Ejecutando el modelo RandomForestClassifier:LocalClassifierPerNode :::-------------------------------')
# Use random forest classifiers for every node
rf = RandomForestClassifier()
classifier = LocalClassifierPerNode(local_classifier=rf)

# Train local classifier per node
classifier.fit(X_train, Y_train)

# Predict
y_pred = classifier.predict(X_test)
print('Predicciones:-------------------------------')
print(y_pred)

###Calcula metricas por nivel de la jerarquia
y_pred_df=pd.DataFrame(y_pred,columns=['nivel1','nivel2','nivel3','nivel4'])

# Reemplazar 'None' como cadena y None como nulo por -1 en columna1

Y_test['nivel3'] = Y_test['nivel3'].replace({'None': '-1', None: '-1'})
Y_test['nivel4'] = Y_test['nivel4'].replace({'None': '-1', None: '-1'})

# Ensure the 'nivel3' and 'nivel4' columns contain integer values
Y_test['nivel3'] = Y_test['nivel3'].astype(int)
Y_test['nivel4'] = Y_test['nivel4'].astype(int)

# Replace 'None' values with -1 for nivel3 and nivel4 in y_pred_df
y_pred_df['nivel3'] = y_pred_df['nivel3'].replace({'None': -1, None: '-1'})
y_pred_df['nivel4'] = y_pred_df['nivel4'].replace({'None': -1, None: '-1'})

# Ensure the 'nivel3' and 'nivel4' columns contain integer values in y_pred_df
y_pred_df['nivel3'] = y_pred_df['nivel3'].astype(int)
y_pred_df['nivel4'] = y_pred_df['nivel4'].astype(int)

# Debugging code to print unique values in nivel3 and nivel4 columns
unique_values_nivel3 = Y_test['nivel3'].unique()
unique_values_nivel4 = Y_test['nivel4'].unique()
print("Unique values in Y_test['nivel3']:", unique_values_nivel3)
print("Unique values in Y_test['nivel4']:", unique_values_nivel4)

unique_values_nivel3 = y_pred_df['nivel3'].unique()
unique_values_nivel4 = y_pred_df['nivel4'].unique()
print("Unique values in y_pred['nivel3']:", unique_values_nivel3)
print("Unique values in y_pred['nivel4']:", unique_values_nivel4)

# Calculate metrics for nivel1
accuracy_score_level_1 = accuracy_score(Y_test['nivel1'], y_pred_df['nivel1'])
precision_score_level_1 = precision_score(Y_test['nivel1'], y_pred_df['nivel1'], average='weighted')
recall_score_level_1 = recall_score(Y_test['nivel1'], y_pred_df['nivel1'], average='weighted')
f1_score_level_1 = f1_score(Y_test['nivel1'], y_pred_df['nivel1'], average='weighted')

print(f'Metrics for Nivel 1:')
print(f'Accuracy: {accuracy_score_level_1}')
print(f'Precision: {precision_score_level_1}')
print(f'Recall: {recall_score_level_1}')
print(f'F1 Score: {f1_score_level_1}')

# Calculate metrics for nivel2
accuracy_score_level_2 = accuracy_score(Y_test['nivel2'], y_pred_df['nivel2'])
precision_score_level_2 = precision_score(Y_test['nivel2'], y_pred_df['nivel2'], average='weighted')
recall_score_level_2 = recall_score(Y_test['nivel2'], y_pred_df['nivel2'], average='weighted')
f1_score_level_2 = f1_score(Y_test['nivel2'], y_pred_df['nivel2'], average='weighted')

print(f'Metrics for Nivel 2:')
print(f'Accuracy: {accuracy_score_level_2}')
print(f'Precision: {precision_score_level_2}')
print(f'Recall: {recall_score_level_2}')
print(f'F1 Score: {f1_score_level_2}')

# Calculate metrics for nivel3
accuracy_score_level_3 = accuracy_score(Y_test['nivel3'].astype(int), y_pred_df['nivel3'].astype(int))
precision_score_level_3 = precision_score(Y_test['nivel3'].astype(int), y_pred_df['nivel3'].astype(int), average='weighted')
recall_score_level_3 = recall_score(Y_test['nivel3'].astype(int), y_pred_df['nivel3'].astype(int), average='weighted')
f1_score_level_3 = f1_score(Y_test['nivel3'].astype(int), y_pred_df['nivel3'].astype(int), average='weighted')

print(f'Metrics for Nivel 3:')
print(f'Accuracy: {accuracy_score_level_3}')
print(f'Precision: {precision_score_level_3}')
print(f'Recall: {recall_score_level_3}')
print(f'F1 Score: {f1_score_level_3}')

# Calculate metrics for nivel4
accuracy_score_level_4 = accuracy_score(Y_test['nivel4'].astype(int), y_pred_df['nivel4'].astype(int))
precision_score_level_4 = precision_score(Y_test['nivel4'].astype(int), y_pred_df['nivel4'].astype(int), average='weighted')
recall_score_level_4 = recall_score(Y_test['nivel4'].astype(int), y_pred_df['nivel4'].astype(int), average='weighted')
f1_score_level_4 = f1_score(Y_test['nivel4'].astype(int), y_pred_df['nivel4'].astype(int), average='weighted')

print(f'Metrics for Nivel 4:')
print(f'Accuracy: {accuracy_score_level_4}')
print(f'Precision: {precision_score_level_4}')
print(f'Recall: {recall_score_level_4}')
print(f'F1 Score: {f1_score_level_4}')

######Métricas hiclass
##accuracy_hc
precision_hc = metrics.precision(Y_test, y_pred_df)
precision_macro_hc = metrics._precision_macro(Y_test, y_pred_df)
precision_micro_hc = metrics._precision_micro(Y_test, y_pred_df)
print("precision_hc:  ", precision_hc," precision_macro_hc: ",precision_macro_hc,"  precision_micro_hc: ", precision_micro_hc)

recall_hc = metrics.recall(Y_test, y_pred_df)
recall_macro_hc = metrics._recall_macro(Y_test, y_pred_df)
recall_micro_hc = metrics._recall_micro(Y_test, y_pred_df)
print("recall_hc:  ", recall_hc," recall_macro_hc: ",recall_macro_hc,"  recall_micro_hc: ", recall_micro_hc)

f1_score_hc = metrics.f1(Y_test, y_pred_df)
f1_score_macro_hc = metrics._f_score_macro(Y_test, y_pred_df)
f1_score_micro_hc = metrics._f_score_micro(Y_test, y_pred_df)
print("f1_score_hc:  ", f1_score_hc," f1_score_macro_hc: ",f1_score_macro_hc,"  f1_score_micro_hc: ", f1_score_micro_hc)

