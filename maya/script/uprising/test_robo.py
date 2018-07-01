from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    ITEM_TYPE_TARGET,
    ITEM_TYPE_PROGRAM,
    ITEM_TYPE_TOOL,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_INSERT_CODE,
    INSTRUCTION_START_THREAD,
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)
import robodk as rdk

RL = Robolink()


mat = rdk.Mat([[]])

print mat.size()[1]


print mat


# triangles = [
#     [-100, -100, 0],
#     [-100, 100, 0],
#     [0, 0, 50],

#     [100, 100, 0],
#     [-100, 100, 0],
#     [0, 0, 50],


#     [100, 100, 0],
#     [100, -100, 0],
#     [0, 0, 50],

#     [100, -100, 0],
#     [-100, -100, 0],
#     [0, 0, 50]
# ]


# mat = [
#     [1, 0, 0, 0],
#     [0, 1, 0, 0],
#     [0, 0, 1, 5],
#     [0, 0, 0, 1]
# ]

# tool_item = RL.Item("foo")
# if tool_item.Valid():
#     tool_item.Delete()

# shape = RL.Item("Shape")
# if shape.Valid():
#     shape.Delete()

# robot = RL.Item('', ITEM_TYPE_ROBOT)
# tool_item = robot.AddTool(rdk.Mat(mat), "foo")
# shape = RL.AddShape(triangles)


# tool_item.AddGeometry(shape, rdk.eye())


# shape.setPoseTool(tool_item)
# item = RL.Item("Shape")
# robot = RL.Item('', ITEM_TYPE_ROBOT)

# tool = RL.Item('Shape', ITEM_TYPE_TOOL)
# robot.setPoseTool()

# print tool.Name()
# # tool.AttachClosest()
