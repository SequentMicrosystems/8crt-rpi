Welcome to SM8crt's documentation!
=====================================

Install
=======

.. code-block:: bash

    sudo pip install SM8crt

or

.. code-block:: bash

    sudo pip3 install SM8crt

Update
======

.. code-block:: bash

    sudo pip install SM8crt -U

or

.. code-block:: bash

    sudo pip3 install SM8crt -U


Initiate class
==============

.. code-block:: console

   $ python
   Python 3.11.8 (main, Feb 12 2024, 14:50:05) [GCC 13.2.1 20230801] on linux
   Type "help", "copyright", "credits" or "license" for more information.
   >>> import SM8crt
   >>> sm = SM8crt.SM8crt()
   >>> sm.set_led(2, 1) # Set LED 2 to ON
   >>>


Documentation
=============

.. toctree::
   :maxdepth: 2
   :caption: Contents:

.. automodule:: SM8crt
   :members:
   :undoc-members:
   :show-inheritance:

.. vi:se ts=4 sw=4 et:
