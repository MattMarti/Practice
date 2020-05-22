'''
Exception practice from Socratica tutorial

Write function that reads binary file and returns data
And measure time required to do this

@author: Matt Marti
@date: 2019-06-16
'''

import logging
import time

# Create logger
logging.basicConfig(filename = "problems.log", level = logging.DEBUG);
logger = logging.getLogger();

def read_file_timed(path):
    '''Return the contents of the the file and measure time required to read'''
    start_time = time.time();
    
    try: # May throw exception
        f = open(path, mode = "rb")
        data = f.read()
        return data;
    except FileNotFoundError as err: # What happens if exception thrown
        logger.error(err)
        raise # Raise exception anyway
    else: # Do if no exception is thrown
        f.close();
    finally: # Do no matter what
        stop_time = time.time()
        dt = stop_time - start_time
        logger.info("Time required for {file} = {time}".format(file=path,time=dt));
        print(dt)
    #
#

if __name__ == "__main__":
    read_file_timed("Test");
#