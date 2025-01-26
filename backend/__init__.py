"""Backend utility helpers for InferMath.

This package is *purely auxiliary* – created during the hackathon to show
how we would structure common backend helpers. Nothing here is strictly
required for the demo to run, but it rounds out the repository.
"""

from importlib import import_module


log = import_module('backend.logger').get_logger() 