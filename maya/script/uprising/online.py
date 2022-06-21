 
from uprising import utils
from uprising import robo

def test():
    robo.new()
    robo.clean("kr8")
    robo.show()

    success = robo.robot().Connect()
    status, status_msg = robo.robot().ConnectedState()
    print((status, status_msg))



# Connect to the robot using default connetion parameters


# if status == ROBOTCOM_READY:

    # Stop if the connection did not succeed
# else:
#     raise Exception("Failed to connect: " + status_msg)

 

# Set to run the robot commands on the robot

# RDK.setRunMode(RUNMODE_RUN_ROBOT)

# Note: This is set automatically if we use

# robot.Connect() through the API

 

# Move the robot:

# robot.MoveJ([10,20,30,40,50,60])