import logging
import sys
from typing import Optional

_LOGGER: Optional[logging.Logger] = None


def get_logger(name: str = "infermath") -> logging.Logger:
    global _LOGGER
    if _LOGGER is None:
        _LOGGER = logging.getLogger(name)
        _LOGGER.setLevel(logging.DEBUG)

        handler = logging.StreamHandler(sys.stdout)
        handler.setFormatter(logging.Formatter("%(asctime)s | %(levelname)s | %(message)s"))
        _LOGGER.addHandler(handler)

    return _LOGGER 