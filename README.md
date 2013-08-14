CuwoCWS
=======

A test plugin that can be used to identify player on cuwo servers in CWSRestart

# Compilation

There are pre- and post-build actions set up to copy dependencies from CWSRestart to the Dependencies folder. There is also a post-build action set up to copy the plugin to *D:\Program Files (x86)\CuWo* with the required dependencies. You might need to change these directories to match your local setup.

# Usage

Compile everything and add ``'CuwoCWS'`` to your base.py

# Where do I place my files? (precompiled cuwo server)

You should extract the release zip into the root folder of your cuwo setup. The CuwoCWS.dll and .pdb should go in the root folder, right next to the run.bat. The dll folder should be a subfolder of the root folder. The CuwoCWS.py from the scripts folder should go into your scripts folder. Don't forget to add it to your base.py
Optional: Start CWSRestart and copy the preset.xml to CWSRestart/presets to import a few settings.
