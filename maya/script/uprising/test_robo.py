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
    INSTRUCTION_SHOW_MESSAGE,
    RUNMODE_MAKE_ROBOTPROG,
    RUNMODE_SIMULATE
)
import robodk as rdk

RL = Robolink()

robot = RL.Item('', ITEM_TYPE_ROBOT)
tools = RL.ItemList(filter = ITEM_TYPE_TOOL)

print [t.Name() for t in tools]


# WINDOWSTATE_HIDDEN      = -1        # Hidden
# WINDOWSTATE_SHOW        = 0         # Visible
# WINDOWSTATE_MINIMIZED   = 1         # Minimize window
# WINDOWSTATE_NORMAL      = 2         # Show normal window (last known state)
# WINDOWSTATE_MAXIMIZED   = 3         # Show maximized window
# WINDOWSTATE_FULLSCREEN  = 4         # Show fulscreen window
# WINDOWSTATE_CINEMA      = 5         # Show maximized window without the toolbar and without the menu
# WINDOWSTATE_FULLSCREEN_CINEMA= 6    # Show fulscreen window without the
# toolbar and without the menu


# RL.Render(False)
# RL.setWindowState(-1)

# RL.setWindowState(2)

# robot.Childs()
# for c in robot.Childs():
#     print c.Name()
#     c.setVisible(False)


def draw_seg(verts, color, name):
    # print "color %s %s %s" % color
    # print "name %s" % name
    RL = Robolink()
    triangles = [
        list(verts[0]), list(verts[1]), list(verts[2]),
        list(verts[1]), list(verts[3]), list(verts[2])
    ]
    geo = RL.Item(name)
    if not geo.Valid():
        geo = RL.AddShape(triangles)
        geo.setName(name)
        # geo.setColor(list(color))
    else:
        shape = RL.AddShape(triangles)
        geo.AddGeometry(shape, rdk.eye())
        shape.Delete()
    geo.setColor(list(color))
    # shape.Delete()
# robot = RL.Item('', ITEM_TYPE_ROBOT)


# RL.Item("geox_0000").Delete()

# draw_seg([(155.29114253597766,
#            1765.774396773453,
#            1521.7159430626668),
#           (155.32392910181824,
#            1765.8563631880545,
#            1501.7161378999165),
#           (130.4024290692281,
#            1764.9430504232703,
#            1519.8304555151044),
#           (134.21949217830098,
#            1765.151090370014,
#            1500.1991858515826)],
#          (1.0,
#           0.3822000026702881,
#           0.0),
#          "geox_0000")

# draw_seg([(130.4024290692281,
#            1764.9430504232703,
#            1519.8304555151044),
#           (134.21949217830098,
#            1765.151090370014,
#            1500.1991858515826),
#           (106.11409766970165,
#            1764.1465010657394,
#            1514.3440649659312),
#           (113.4702483894046,
#            1764.4696507293734,
#            1495.748833833087)],
#          (1.0,
#           0.3822000026702881,
#           0.0),
#          "geox_0000")


# def construct_command(m1, m2, w, c):
#     t1 = rdk.Pose_2_TxyzRxyz(m1)
#     t2 = rdk.Pose_2_TxyzRxyz(m1)
#     return 'draw_segment(%s , %s , %s , %s)' % (t1, t2, w, c)


# robot = RL.Item('', ITEM_TYPE_ROBOT)

# mat1 = robot.Pose()
# mat2 = mat1
# width = 2
# color = (1, 0, 0.5)

# cmd = construct_command(mat1, mat2, width, color)


# print cmd
# print mat1.isHomogeneous()

# mat2 = rdk.Mat([[0.313, 0.000, 0.950, 1479.004], [-0.000, 1.000, -0.000, 498.947],
#                 [-0.950, -0.000, 0.313, 1457.620], [0.000, 0.000, 0.000, 1.000]])
# print mat2
# print mat2.isHomogeneous()

# mat2 = mat1
# width = 2
# color = (1, 0, 0.5)


# cmd = 'draw_segment(%s , %s , %s , %s)' % (mat1, mat2, width, color)


# # print str(cmd)


# print mat1[0]
# print mat1[1]
# print mat1[2]
# print mat1[3]

# RL.Render(False)
# prog = RL.AddProgram('AutoProgram')
# RL.setRunMode(RUNMODE_MAKE_ROBOTPROG)
# folder = "/Users/julian/projects/robot/export"
# # program = RL.Item("px")
# # print program.Valid()
# RL.ProgramStart("px_file", folder, "KUKA_KRC4_RN", robot)
# robot.RunInstruction('AutoProgram', INSTRUCTION_CALL_PROGRAM)
# RL.Render(True)
# RL.Finish()

# prog_filename = "PX_%s" % timestamp
# station = os.path.join(ts_dir, "%s.rdk" % timestamp)
# RL.Save(station)

# RL.ProgramStart(prog_filename, ts_dir, "KUKA_KRC4_RN", robot)
# print "ProgramStart %s" % ts_dir
# RL.Render(False)
# print "Render False - now run"
# program = RL.Item("px")
# RL.RunProgram("px", False)
# print "Render True"
# RL.Render(True)
# print "DONE"
# RL.Finish()


# target1 = RL.Item("px_s_0_t_16")
# target2 = RL.Item("px_s_1_t_0")

# print target1.Valid()
# print target2.Valid()

# # -55.068085, -52.875797, 82.444529, 43.678911, -57.296768, 107.162231

# # -51.849064, -4.286521, -36.710349, -35.656859, 46.651584, 81.140209

# robot = RL.Item('', ITEM_TYPE_ROBOT)

# joints = [-51.849064, -4.286521, -36.710349, -35.656859, 46.651584,
# 81.140209]

# print robot.JointsConfig(joints)

# mat = rdk.Mat([[]])

# print mat.size()[1]


# print mat


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
