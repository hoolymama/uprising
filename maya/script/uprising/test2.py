
from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT
)

RL = Robolink()
# Move a robot along a line given a start and end point by steps
# This macro shows different ways of programming a robot using a Python
# script and the RoboDK API

# Default parameters:
# Start point with respect to the robot base frame
P_START = [1755, -500, 2155]
P_END = [1755, 600, 2155]    # End point with respect to the robot base frame
NUM_POINTS = 10                # Number of points to interpolate

# Function definition to create a list of points (line)


def MakePoints(xStart, xEnd, numPoints):
    """Generates a list of points."""
    if len(xStart) != 3 or len(xEnd) != 3:
        raise Exception("Start and end point must be 3-dimensional vectors")
    if numPoints < 2:
        raise Exception("At least two points are required")

    # Starting Points
    pt_list = []
    x = xStart[0]
    y = xStart[1]
    z = xStart[2]

    # How much we add/subtract between each interpolated point
    x_steps = (xEnd[0] - xStart[0]) / (numPoints - 1)
    y_steps = (xEnd[1] - xStart[1]) / (numPoints - 1)
    z_steps = (xEnd[2] - xStart[2]) / (numPoints - 1)

    # Incrementally add to each point until the end point is reached
    for i in range(numPoints):
        point_i = [x, y, z]  # create a point
        # append the point to the list
        pt_list.append(point_i)
        x = x + x_steps
        y = y + y_steps
        z = z + z_steps
    return pt_list


#---------------------------------------------------
#--------------- PROGRAM START ---------------------
# API to communicate with RoboDK for simulation and offline/online programming
from robolink import *
from robodk import *      # Robotics toolbox for industrial robots

# Generate the points curve path
POINTS = MakePoints(P_START, P_END, NUM_POINTS)

# Initialize the RoboDK API


# turn off auto rendering (faster)
RL.Render(False)

# Automatically delete previously generated items (Auto tag)
list_items = RL.ItemList()  # list all names
print list_items
for item in list_items:
    if item.Name().startswith('Auto'):
        # RL.Item(item_name).Delete()
        item.Delete()

# Promt the user to select a robot (if only one robot is available it will
# select that robot automatically)
robot = RL.ItemUserPick('Select a robot', ITEM_TYPE_ROBOT)

# Turn rendering ON before starting the simulation
RL.Render(True)

# Abort if the user hits Cancel
if not robot.Valid():
    quit()

# Retrieve the robot reference frame
reference = robot.Parent()

# Use the robot base frame as the active reference
robot.setPoseFrame(reference)

# get the current orientation of the robot (with respect to the active
# reference frame and tool frame)
pose_ref = robot.Pose()
print(Pose_2_TxyzRxyz(pose_ref))
# a pose can also be defined as xyzwpr / xyzABC
#pose_ref = TxyzRxyz_2_Pose([100,200,300,0,0,pi])


###################
# Turn off rendering (faster)

RL.Render(False)
prog = RL.AddProgram('AutoProgram')

# Hide program instructions (optional, but faster)
prog.ShowInstructions(False)

# Retrieve the current robot position:
pose_ref = robot.Pose()

# Iterate through a number of points
for i in range(len(POINTS)):
    # add a new target
    ti = RL.AddTarget('Auto Target %i' % (i + 1))

    # use the reference pose and update the XYZ position
    pose_ref.setPos(POINTS[i])
    ti.setPose(pose_ref)

    # force to use the target as a Cartesian target (default)
    ti.setAsCartesianTarget()

    # Add the target as a Linear/Joint move in the new program

    print ti
    prog.addMoveL(ti)

# Hide the target items from the tree: it each movement still keeps its own target.
# Right click the movement instruction and select "Select Target" to see
# the target in the tree
# prog.ShowTargets(False)

# Turn rendering ON before starting the simulation (automatic if we are done)
RL.Render(True)

# #--------------------------------------
# # Update the program path to display the yellow path in RoboDK.
# # Set collision checking ON or OFF
# check_collisions = COLLISION_OFF
# # Update the path (can take some time if collision checking is active)
# update_result = program.Update(check_collisions)
# # Retrieve the result
# n_insok = update_result[0]
# time = update_result[1]
# distance = update_result[2]
# percent_ok = update_result[3] * 100
# str_problems = update_result[4]
# if percent_ok < 100.0:
#     msg_str = "WARNING! Problems with <strong>%s</strong> (%.1f):<br>%s" % (
#         program_name, percent_ok, str_problems)
# else:
#     msg_str = "No problems found for program %s" % program_name

# # Notify the user:
# print(msg_str)
# RL.ShowMessage(msg_str)
