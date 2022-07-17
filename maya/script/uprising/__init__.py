import logging
from imp import reload # python 2.x don't need to import reload, use it directly
reload(logging)
logging.basicConfig(level=logging.WARNING,  format='[%(name)s:%(levelname)-5s:Line:%(lineno)-4d] %(message)s')
logger = logging.getLogger(__name__)

logger.warning("INITIALIZED LOGGER")