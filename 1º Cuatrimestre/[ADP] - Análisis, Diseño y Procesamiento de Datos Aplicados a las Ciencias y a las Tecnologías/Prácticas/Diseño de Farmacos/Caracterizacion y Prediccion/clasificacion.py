from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, cohen_kappa_score, roc_auc_score
import numpy as np
import pandas as pd
# from pymongo import MongoClient  # Descomentar si estás accediendo a MongoDB

# Preprocesamiento de datos:
# client = MongoClient('mongodb://localhost:27017/')  # Descomentar para conexión MongoDB
# db = client.your_database  # Reemplazar con el nombre de tu base de datos
# collection = db.molecules  # Reemplazar con el nombre de tu colección

# Ejemplo de cómo podrías cargar los datos (esto es solo un marcador de posición)
# data = pd.DataFrame(list(collection.find()))  # Descomentar para cargar datos desde MongoDB

# Para el ejemplo, supondré que `data` ya está en un DataFrame de Pandas.
# Convertir posiciones de bit a fingerprint completo (Vector de 1024 dimensiones)
def expand_fingerprint(bits, size=1024):
    fingerprint = np.zeros(size)
    fingerprint[bits] = 1
    return fingerprint

# Aplicar la función a cada fila (asumiendo que la columna de bits se llama 'bit_positions')
data['fingerprint'] = data['bit_positions'].apply(lambda bits: expand_fingerprint(bits))

# Convertir la lista de fingerprints en un DataFrame y concatenarlo con la clase
fp_df = pd.DataFrame(data['fingerprint'].tolist())
fp_df['class'] = data['class'].map({'Active': 1, 'Inactive': 0})

# Eliminar las columnas donde todos los valores son '0'
fp_df = fp_df.loc[:, (fp_df != 0).any(axis=0)]

# Variables predictoras y variable respuesta
X = fp_df.drop('class', axis=1)
y = fp_df['class']

# Dividir en conjunto de entrenamiento y prueba
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Construcción del modelo
model = RandomForestClassifier(random_state=42)
model.fit(X_train, y_train)

# Evaluación del modelo
y_pred = model.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
kappa = cohen_kappa_score(y_test, y_pred)
# AURoc (necesita probabilidades predichas, no etiquetas)
y_pred_proba = model.predict_proba(X_test)[:, 1]
roc_auc = roc_auc_score(y_test, y_pred_proba)

# Imprimir métricas
print(f'Accuracy: {accuracy:.2f}')
print(f'Kappa: {kappa:.2f}')
print(f'ROC AUC: {roc_auc:.2f}')