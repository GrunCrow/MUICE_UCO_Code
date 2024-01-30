from hiclass import LocalClassifierPerNode
from sklearn.ensemble import RandomForestClassifier
from hiclass import LocalClassifierPerLevel

# Define data
X_train = [[1], [2], [3], [4]]
X_test = [[4], [3], [2], [1]]
Y_train = [
        ['Electrónica', 'Smartphones', 'iphone'],
        ['Electrónica', 'Smartphones', 'androide'],
        ['Electrónica', 'Laptops', 'mac'],
        ['Electrónica', 'Laptops', 'ibm'],
    ]




# Use random forest classifiers for every node
rf = RandomForestClassifier()
classifier = LocalClassifierPerLevel(local_classifier=rf)

# Train local classifier per node
classifier.fit(X_train, Y_train)

# Predict
predictions = classifier.predict(X_test)
print(predictions)