Local Hierarchical Classifier
=============================

A :literal:`local hierarchical classifier` is a supervised machine learning model, where the output of the classification algorithm is defined over a pre-established hierarchical class taxonomy. In HiClass, there are 3 main approaches for local hierarchical classification, i.e., the most common design patterns for local hierarchical classification identified in the literature [1]_, which are the :ref:`local-classifier-per-node-overview`, :ref:`local-classifier-per-parent-node-overview` and :ref:`local-classifier-per-level-overview`.

In this example, we will be using the :literal:`LocalClassifierPerNode` along with the :literal:`RandomForestClassifier` from scikit-learn, but you can click on the other tabs to see how the code changes for the :literal:`LocalClassifierPerParentNode` and :literal:`LocalClassifierPerLevel`:

.. tabs::

    .. code-tab:: python
        :caption: LocalClassifierPerNode

        from hiclass import LocalClassifierPerNode
        from sklearn.ensemble import RandomForestClassifier

    .. code-tab:: python
        :caption: LocalClassifierPerParentNode

        from hiclass import LocalClassifierPerParentNode
        from sklearn.ensemble import RandomForestClassifier

    .. code-tab:: python
        :caption: LocalClassifierPerLevel

        from hiclass import LocalClassifierPerLevel
        from sklearn.ensemble import RandomForestClassifier


We will be using a :literal:`RandomForestClassifier` for each node in the :literal:`LocalClassifierPerNode`, except for the root node. This :literal:`LocalClassifierPerNode` model will have the same structure pre-defined in the hierarchical data used to train the model. This is how we create both objects:

.. tabs::

    .. code-tab:: python
        :caption: LocalClassifierPerNode

        rf = RandomForestClassifier()
        classifier = LocalClassifierPerNode(local_classifier=rf)

    .. code-tab:: python
        :caption: LocalClassifierPerParentNode

        rf = RandomForestClassifier()
        classifier = LocalClassifierPerParentNode(local_classifier=rf)

    .. code-tab:: python
        :caption: LocalClassifierPerLevel

        rf = RandomForestClassifier()
        classifier = LocalClassifierPerLevel(local_classifier=rf)

.. note::

   The :literal:`LocalClassifierPerParentNode` has a :literal:`RandomForestClassifier` for each parent node existing in the hierarchy, while the :literal:`LocalClassifierPerLevel` contains a :literal:`RandomForestClassifier` for each level in the training labels. More information on the nuances of the hierarchical classifiers is available at the section :ref:`Algorithms Overview`.

.. [1] Silla, C. N., & Freitas, A. A. (2011). A survey of hierarchical classification across different application domains. Data Mining and Knowledge Discovery, 22(1), 31-72.
