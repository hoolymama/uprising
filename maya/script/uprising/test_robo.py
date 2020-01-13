
# import random
# import math

from robolink import Robolink
import cProfile, pstats, StringIO

pr = cProfile.Profile()
pr.enable()

RL = Robolink()
list_items = RL.ItemList() 
for item in list_items:
    print item.Name()

pr.disable()

s = StringIO.StringIO()
sortby = 'cumulative'
ps = pstats.Stats(pr, stream=s).sort_stats(sortby)
ps.print_stats()
print s.getvalue()

# import os
# import pymel.core as pm

# import uprising.uprising_util as uutl

 # turn off auto rendering (faster)
# RL.Render(False)

# # Automatically delete previously generated items (Auto tag)
# list_items = RL.ItemList()  # list all names
# print list_items
# for item in list_items:
#     if item.Name().startswith('Auto'):


# def  get_flange_mode():
#     service = sheets._get_service()
#     result = service.spreadsheets().values().get(
#         spreadsheetId=sheets.SHEETS["Measurements"],
#         range='Brushes!A2:A2').execute()

#     print    result['values'][0][0]
    

# get_flange_mode()

    # data = result.get('values', [])

    # for row in data:
    #     loc_name = row[0]
    #     try:
    #         node = pm.PyNode(loc_name)
    #     except pm.MayaNodeError:
    #         node = pm.spaceLocator()
    #         node.rename(loc_name)

    #     vals = [uutl.numeric(x) * 0.1 for x in row[1:4]]
    #     node.attr("translate").set(*vals)




# arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]

# num = len(arr)
# step = 3

# spans = int(math.ceil((num - 1) / step))
# print("spans:%s" % spans)

# nverts = spans + 1
# print("nverts:%s" % nverts)


# for i in range(nverts):
#     fraction = i / float(spans)
#     param = (num - 1) * fraction
#     # print("param:%s" % param)
#     leftid = int(math.floor(param))
#     rightid = int(math.ceil(param))
#     weight = math.ceil(param) - param if not leftid == rightid else 1
#     print("frac:%s\t\tparam:%s\t\tL:%s R:%s\t\tweight:%s" %
#           (fraction, param, leftid, rightid, weight))
    # print("L:%s R:%s" % (leftid, rightid))

# RL = Robolink()

# print RL.TIMEOUT
# Change the default timeout here, in seconds:
# RL.TIMEOUT = 120 # in seconds
# RL.COM.settimeout(RL.TIMEOUT)
 
# frame =  RL.Item('frm')
# frame = RL.AddFrame('frm')


# robot = RL.Item('', ITEM_TYPE_ROBOT)

# RL.AddTarget("tgt", frame,  robot)
# RL.AddTarget("tgt", frame,  robot)

# target = RL.Item("tgt")
# print "T", type(target)

# prg = RL.AddProgram("prg")

# prg.addMoveL(target)

# target1 =  RL.Item("foobar")
# print target.Valid()

# # # # # # # # # # # # # # # # # #
# class WeightedRandomizer:
#     def __init__ (self, weights):
#         self.__max = .0
#         self.__weights = []
#         for value, weight in weights.items ():
#             self.__max += weight
#             self.__weights.append ( (self.__max, value) )

#     def random (self):
#         r = random.random () * self.__max
#         for ceil, value in self.__weights:
#             if ceil > r: return value


# max_steps = 4
# power = 1

# sentinel = max_steps+1
# w = {}
# results = {}
# for i in range(sentinel):
#     j=1.0-(i / float(sentinel))
#     w[i] =   pow(j, power)
#     w[-i] = w[i]
#     results[i] = 0
#     results[-i] = 0


# print w

# arr = []
# for key, value in w.iteritems():
#     temp = [key,value]
#     arr.append(temp)

# arr =  sorted(arr, key=lambda tup: tup[0])
# arr = [x[1] for x in arr]

# tot = sum(arr)
# arr = ["%.1f%%" %  ((a / tot) *100) for a in arr ]
# print "\t".join(arr)
# # # # # # # # # # # # # # # # # # # # #

# print "%d" % ((float(a[1]) / float(siz))  )

# w = {'A': 1.0, 'B': 1.0, 'C': 18.0}
# or w = {'A': 5, 'B': 5, 'C': 90}
# or w = {'A': 1.0/18, 'B': 1.0/18, 'C': 1.0}
# or or or

# wr = WeightedRandomizer (w)

# # results = {'A': 0, 'B': 0, 'C': 0}
# for i in range (10000):
#     results [wr.random () ] += 1

# print ('After 10000 rounds the distribution is:')
# print (results)


# from robolink import (
#     Robolink,
#     ITEM_TYPE_ROBOT,
#     ITEM_TYPE_TARGET,
#     ITEM_TYPE_STATION,
#     ITEM_TYPE_PROGRAM,
#     ITEM_TYPE_TOOL,
#     INSTRUCTION_CALL_PROGRAM,
#     INSTRUCTION_INSERT_CODE,
#     INSTRUCTION_START_THREAD,
#     INSTRUCTION_COMMENT,
#     COLLISION_OFF,
#     INSTRUCTION_SHOW_MESSAGE,
#     RUNMODE_MAKE_ROBOTPROG,
#     RUNMODE_SIMULATE
# )
# import robodk as rdk
# import datetime
# import sheets

# from contextlib import contextmanager


# @contextmanager
# def smth():
#     print "start"
#     yield
#     print midd
#     print "end"


# with smth():
#     midd =  "middle"


# # RL = Robolink()


# program = RL.Item('px', ITEM_TYPE_PROGRAM)

# Update the path (can take some time if collision checking is active)
# update_result = program.Update(COLLISION_OFF)
# # Retrieve the result
# n_insok = update_result[0]
# time = update_result[1]
# distance = update_result[2]
# percent_ok = update_result[3]*100
# str_problems = update_result[4]
# if percent_ok < 100.0:
#     msg_str = "WARNING! Problems with <strong>%s</strong> (%.1f):<br>%s" % (program.Name(), percent_ok, str_problems)
# else:
#     msg_str = "No problems found for program %s" % program.Name()

# print msg_str


# def get_measurements_values(cell_range, service, dimension="ROWS"):
#     result = service.spreadsheets().values().get(
#         spreadsheetId=sheets.SHEETS["Measurements"],
#         range=cell_range, majorDimension=dimension).execute()
#     return result.get('values', [])


# def get_palette_header(search_str, service):
#     batch_size = 100
#     batches = 10
#     total_rows = batch_size * batches
#     for r, x in [("Paints!A%d:A%d" % (x + 1, x + batch_size), x)
#                  for x in xrange(0, total_rows, batch_size)]:
#         values = get_measurements_values(r, service, "COLUMNS")
#         if values:
#             for i, v in enumerate(values[0]):
#                 if v == search_str:
#                     row = (x + i + 1)
#                     cell_range = "Paints!A%d:B%d" % (row, row)
#                     header_values = get_measurements_values(
#                         cell_range, service)[0]
#                     header_values.append(row)
#                     return tuple(header_values)


# def get_palette_by_name(name):
#     service = sheets._get_service()
#     # name, medium, row = get_palette_header(name, service)
#     header = get_palette_header(name, service)
#     if header:
#         name, medium, row = header
#         cell_range = "Paints!A%d:E%d" % (row + 1, row + 64)
#         palette = get_measurements_values(cell_range, service)
#         new_palette = []
#         for entry in palette:
#             if len(entry) == 0:
#                 break
#             new_palette.append(entry)

#         # new_palette = [entry for entry in palette if len(entry) >= 5]
#         return tuple([name, medium, new_palette])


# print get_palette_by_name("ben_test28palette")


# timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M%S')
# print timestamp

# robot = RL.Item('', ITEM_TYPE_ROBOT)
# stat = RL.Item("", ITEM_TYPE_STATION)
# stat.setName("678987")
# print  [prog.Name() for prog in progs]
# stat = RL.Item("", ITEM_TYPE_STATION)
# print stat.Name()
# RL.CloseStation(stat)


# print [s.Name() for s in RL.getOpenStations()]
# for  station in RL.getOpenStations():
#     station.Delete()

# RL.AddFile("/Users/julian/projects/robot/stations/clean.rdk")


# stat = RL.Item("", ITEM_TYPE_STATION)
# stat.Delete()


# print [t.Name() for t in tools]

# WINDOWSTATE_HIDDEN      = -1        # Hidden
# WINDOWSTATE_SHOW        = 0         # Visible
# WINDOWSTATE_MINIMIZED   = 1         # Minimize window
# WINDOWSTATE_NORMAL      = 2         # Show normal window (last known state)
# WINDOWSTATE_MAXIMIZED   = 3         # Show maximized window
# WINDOWSTATE_FULLSCREEN  = 4         # Show fulscreen window
# WINDOWSTATE_CINEMA      = 5         # Show maximized window without the toolbar and without the menu
# WINDOWSTATE_FULLSCREEN_CINEMA= 6    # Show fulscreen window without the
# toolbar and without the menu


# def draw_seg(verts, color, name):
#     # print "color %s %s %s" % color
#     # print "name %s" % name
#     RL = Robolink()
#     triangles = [
#         list(verts[0]), list(verts[1]), list(verts[2]),
#         list(verts[1]), list(verts[3]), list(verts[2])
#     ]
#     geo = RL.Item(name)
#     if not geo.Valid():
#         geo = RL.AddShape(triangles)
#         geo.setName(name)
#         # geo.setColor(list(color))
#     else:
#         shape = RL.AddShape(triangles)
#         geo.AddGeometry(shape, rdk.eye())
#         shape.Delete()
#     geo.setColor(list(color))


# def save_prog():
#     robot = RL.Item('', ITEM_TYPE_ROBOT)
#     RL.setRunMode(RUNMODE_MAKE_ROBOTPROG)
#     timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
#     ddir = "/Users/julian/projects/robot/export/"
#     prog_filename = "PX_%s" % timestamp

#     RL.ProgramStart(prog_filename, ddir, "KUKA_KRC4_RN_RN", robot)
#     # RL.RunProgram('px', True)
#     robot.RunInstruction("px", INSTRUCTION_CALL_PROGRAM)
#     # RL.RunMessage("tgt")
#     RL.Finish()


# RL.ProgramStart("px_file", folder, "KUKA_KRC4_RN_RN", robot)
# robot.RunInstruction('AutoProgram', INSTRUCTION_CALL_PROGRAM)
# RL.Render(True)
# RL.Finish()

# prog_filename = "PX_%s" % timestamp
# station = os.path.join(ts_dir, "%s.rdk" % timestamp)
# RL.Save(station)

# RL.ProgramStart(prog_filename, ts_dir, "KUKA_KRC4_RN_RN", robot)
# print "ProgramStart %s" % ts_dir
# RL.Render(False)
# print "Render False - now run"
# program = RL.Item("px")
# RL.RunProgram("px", False)
# print "Render True"
# RL.Render(True)
# print "DONE"
# RL.Finish()

    # shape.Delete()
# robot = RL.Item('', ITEM_TYPE_ROBOT)


# import pymel.core as pm

# n_clusters = pm.paintingQuery("mainPaintingShape", cc=True)
# for j in range(n_clusters):
#     n_strokes = pm.paintingQuery("mainPaintingShape", ci=j, sc=True)
#     for k in range(n_strokes):
#         rots = pm.paintingQuery(
#             "mainPaintingShape",
#             clusterIndex=j,
#             strokeIndex=k,
#             strokeStopRotations=True,
#             rotateOrder="zyx",
#             rotateUnit="rad"
#         )
#         pos = pm.paintingQuery(
#             "mainPaintingShape",
#             clusterIndex=j,
#             strokeIndex=k,
#             strokeStopPositions=True
#         )
#         print "C=%s, S=%s" % (j, k)
#         if pos and rots:
#             print pos
#             print rots


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
# RL.ProgramStart("px_file", folder, "KUKA_KRC4_RN_RN", robot)
# robot.RunInstruction('AutoProgram', INSTRUCTION_CALL_PROGRAM)
# RL.Render(True)
# RL.Finish()

# prog_filename = "PX_%s" % timestamp
# station = os.path.join(ts_dir, "%s.rdk" % timestamp)
# RL.Save(station)

# RL.ProgramStart(prog_filename, ts_dir, "KUKA_KRC4_RN_RN", robot)
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

# tool_item = RL.Item("tgt")
# if tool_item.Valid():
#     tool_item.Delete()

# shape = RL.Item("Shape")
# if shape.Valid():
#     shape.Delete()

# robot = RL.Item('', ITEM_TYPE_ROBOT)
# tool_item = robot.AddTool(rdk.Mat(mat), "tgt")
# shape = RL.AddShape(triangles)


# tool_item.AddGeometry(shape, rdk.eye())


# shape.setPoseTool(tool_item)
# item = RL.Item("Shape")
# robot = RL.Item('', ITEM_TYPE_ROBOT)

# tool = RL.Item('Shape', ITEM_TYPE_TOOL)
# robot.setPoseTool()

# print tool.Name()
# # tool.AttachClosest()
