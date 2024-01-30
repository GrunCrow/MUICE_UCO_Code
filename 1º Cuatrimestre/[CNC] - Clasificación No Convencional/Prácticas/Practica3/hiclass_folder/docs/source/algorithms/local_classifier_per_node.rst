.. _local-classifier-per-node-overview:

Local Classifier Per Node
=========================

One of the most popular approaches in the literature, the local classifier per node consists of training one binary classifier for each node of the class taxonomy, except for the root node. A visual representation of the local classifier per node is shown in the image below.

.. figure:: local_classifier_per_node.svg
   :align: center

   Visual representation of the local classifier per node approach.

.. toctree::
    :hidden:

    training_policies

Each binary classifier is trained in parallel using the library `Ray <https://www.ray.io/>`_. In order to avoid inconsistencies, prediction is performed in a top-down manner. For example, given a hypothetical test example, the local classifier per node firstly queries the binary classifiers at nodes "Reptile" and "Mammal". Let's suppose that in this case the probability of the test example belonging to class "Reptile" is 0.8, while the probability of belonging to class "Mammal" is 0.5, then class "Reptile" is picked. At the next level, only the classifiers at nodes "Snake" and "Lizard" are queried, and again the one with the highest probability is selected.
