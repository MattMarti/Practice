import logging

# Create and configure logger
log_format = "%(levelname)s %(asctime)s - %(message)s"
logging.basicConfig(filename = "Logfile.log", 
    level = logging.DEBUG, 
    format = log_format,
    filemode = 'w');
logger = logging.getLogger() # working with one logger without a name: The Root Logger

# Test logger
logger.debug("This is a debug message")
logger.info("Useful info for the user")
logger.warning("I can't do that, Dave")
logger.error("Did you just try to divide by zero?")
logger.critical("The entire internet is down!")

