import os
import time
from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET


CLEAN_FILE = os.path.join(os.environ["UPRISING_PROJECT_PATH"], "robodk", "clean.rdk")
ROBODK_PATH = "/Applications/RoboDK/RoboDK.app/Contents/MacOS/RoboDK"

robot=None
link=None
mode=None
debug=None

def empty():
    global link
    try:
        for station in link.getOpenStations():
            station.Delete()
    except:
        print "No stations to empty"

def close():
    global link
    global robot
    empty()
    try:
       link.Disconnect()
    except BaseException:
        print "Not connected"
    link = None
    robot = None

def new(**kw):
    global link
    global robot
    global mode
    global debug
    close()
 
    args = []
    mode = kw.get("mode", mode)
    debug = kw.get("debug", debug)

    if mode == "hidden":
        args = ["-HIDDEN", "-NOSPLASH", "-NOSHOW"]
    elif mode == "api":
        args = ["-NEWINSTANCE", "-NOUI", "-EXIT_LAST_COM"]
    if debug:
        args.append("-DEBUG")

    link = Robolink(args=args, robodk_path=ROBODK_PATH)
    link.AddFile(CLEAN_FILE)
    robot = link.Item("", ITEM_TYPE_ROBOT)
    robot.setParam("PostProcessor", "KUKA KRC4_RN")


def show():
    global link
    link.ShowRoboDK()

def hide():
    global link
    link.HideRoboDK()

def clean():
    global link
    empty()
    link.AddFile(CLEAN_FILE)
    print "Added clean file: {}".format(CLEAN_FILE)



# sys.path.append(os.path.abspath("/Users/julian/tools/darwin64/uprising/2019.dev.999/maya/scripts/uprising"))
# import robo

# class Singleton:
#     """
#     A non-thread-safe helper class to ease implementing singletons.
#     This should be used as a decorator -- not a metaclass -- to the
#     class that should be a singleton.

#     The decorated class can define one `__init__` function that
#     takes only the `self` argument. Also, the decorated class cannot be
#     inherited from. Other than that, there are no restrictions that apply
#     to the decorated class.

#     To get the singleton instance, use the `instance` method. Trying
#     to use `__call__` will result in a `TypeError` being raised.

#     """

#     def __init__(self, decorated):
#         self._decorated = decorated

#     def instance(self):
#         """
#         Returns the singleton instance. Upon its first call, it creates a
#         new instance of the decorated class and calls its `__init__` method.
#         On all subsequent calls, the already created instance is returned.

#         """
#         try:
#             return self._instance
#         except AttributeError:
#             self._instance = self._decorated()
#             return self._instance

#     def __call__(self):
#         raise TypeError('Singletons must be accessed through `instance()`.')

#     def __instancecheck__(self, inst):
#         return isinstance(inst, self._decorated)




# # @Singleton
# class Robo:
#     def __init__(self, **kw):
#         print 'Robo created'
#         self.link = None
#         self.robot = None

#         self._fresh(**kw)


#     def fresh(self):
#         self._fresh(mode=self.mode, debug=self.debug)

#     def _fresh(self, **kw):

#         self.close()
#         # time.sleep(1)

#         startup_args = []
#         self.mode = kw.get("mode", "show")
#         self.debug=kw.get("debug")

#         if self.mode == "hidden":
#             startup_args = ["-HIDDEN", "-NOSPLASH", "-NOSHOW"]
#         elif self.mode == "api":
#             startup_args = ["-NEWINSTANCE", "-NOUI", "-EXIT_LAST_COM"]
#         if kw.get("debug"):
#             startup_args.append("-DEBUG")

#         # print "startup_args", startup_args
#         self.link = Robolink(args=startup_args, robodk_path=ROBODK_PATH)
#         self.link.AddFile(CLEAN_FILE)
#         self.robot = self.link.Item("", ITEM_TYPE_ROBOT)
#         self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
 

#     def empty(self):
#         try:
#             for station in self.link.getOpenStations():
#                 station.Delete()
#         except:
#             print "No stations to empty"


#     def close(self):
#         self.empty()
#         try:
            
#             self.link.Disconnect()
#             # self.link.CloseRoboDK()
#         except BaseException:
#             print "Already closed"
#         self.link = None
#         self.robot = None

#     def show(self):
#         self.link.ShowRoboDK()

#     def hide(self):
#         self.link.HideRoboDK()

#     def clean(self):
#         self.empty()
#         self.link.AddFile(CLEAN_FILE)
#         print "Added clean file: {}".format(CLEAN_FILE)




# class Robo:
#     """Singleton to keep some common data accessible."""
#     instance = None

#     @classmethod
#     def clear(cls):
#         cls.instance = None

#     class __Robo:
#         def __init__(self, **kw):
#             # This is called once
#             # self.mode =   kw.get("mode", "hidden") == "api"
#             print 'Robo created'
#             self.link = None
#             self.robot = None

#             self._fresh(**kw)



#             self._link = create_link(**kw)
#             self._link.AddFile(CLEAN_FILE)
#             self._robot = self._link.Item("", ITEM_TYPE_ROBOT)
#             self._robot.setParam("PostProcessor", "KUKA KRC4_RN")
#             print "Got link and robot"
#             print "Loaded Clean scene and set PostProcessor to KUKA KRC4_RN"


#         def __init__(self, **kw):
#             """Create a new Robo the first time only.
#             """
#             if kw.get("force"):
#                 self.close()
#             # print "INIT "
#             if not Robo.instance:
#                 # print "instance IS NONE - INSTANTIATING"
#                 Robo.instance = Robo.__Robo(**kw)
#             # else:
#             #     print "instance WAS NOT NONE - DOINT NOTHING"
#             # print "instance is now", Robo.instance
#             # print "LINK AND ROBOT:"
#             # print Robo.instance._link
#             # print Robo.instance._robot
        




# rodk = Robo()
# robot = rodk.robot
# rlink = rodk.link


# r=Robo(mode="api")

# r = Robo(mode="api")
# for i in range(10):
#     print r.robot
#     r.fresh()
# r.close()

# r = Robo(mode="api")
# for i in range(10):
#     print r.robot
#     r.fresh(mode="api")
# r.close()