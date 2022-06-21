import logging
import os
import sys

# set up logging to file
logging.basicConfig(
     stream=sys.stdout,
     level=logging.DEBUG, 
     format= '[%(asctime)s][%(name)s:%(lineno)d][%(levelname)s] - %(message)s',
     datefmt='%H:%M:%S'
 )

logger = logging.getLogger(__name__)  

logger.info('Logging has been set up')