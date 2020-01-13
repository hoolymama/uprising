import os


from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET

CLEAN_FILE = os.path.join(os.environ["UPRISING_PROJECT_PATH"], "robodk", "clean.rdk")


def _link(**kw):

    # mode = kw.get("mode", "api")

    print "kw:", kw

    # if mode == "api":
    #     startup_args = ["-NOUI", "-EXIT_LAST_COM"]
    # elif mode == "hidden":
    startup_args = ["-HIDDEN", "-NOSPLASH", "-NOSHOW"]
    # else:
    #     startup_args = []

    if kw.get("debug"):
        startup_args.append("-DEBUG")

    # print "STARTUP ARGS: ", startup_args
    path = "/Applications/RoboDK/RoboDK.app/Contents/MacOS/RoboDK"
    # print "path: ", path
    result = Robolink(args=startup_args, robodk_path=path)
    print "Created Robo link"

    return result


class Robo:
    """Singleton to keep some common data accessible.

    We store the list of instance types, projects, and the package tree
    here. This data is fetched once and then all the job & submitter
    nodes have access to it. User can force an update, which might be
    handy if they started working when offline, and then need to get
    real before submitting.
    """
    instance = None

    @classmethod
    def clear(cls):
        print "CLEARING INSTANCE"
        cls.instance = None


    class __Robo:
        def __init__(self, **kw):
            # This is called once
            self._api =   kw.get("mode", "api") == "api"
            self._link = _link(**kw)
            self._robot = self._link.Item("", ITEM_TYPE_ROBOT)
            print "Got link and robot"
           
            self._robot.setParam("PostProcessor", "KUKA KRC4_RN")
            self._link.AddFile(CLEAN_FILE)
            print "Loaded Clean scene and set PostProcessor to KUKA KRC4_RN"

        def __str__(self):
            return repr(self)

        def link(self):
            return self._link

        def robot(self):
            return self._robot

        def close(self):
            try:
                if self._api :
                    self._link.Disconnect()
                    print "Disconnected RoboDK"
                else:
                    self._link.CloseRoboDK()
                    print "Closed RoboDK"
            except BaseException:
                pass

        def show(self):
            self._link.ShowRoboDK()


    # Robo __init__
    def __init__(self, **kw):
        """Create a new Robo the first time only.
        """
        if kw.get("force"):
            Robo.clear()

        if not Robo.instance:
            Robo.instance = Robo.__Robo(**kw)

    def __getattr__(self, name):
        """Delegate method calls to the singleton."""
        return getattr(self.instance, name)





# from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET

# CLEAN_FILE = os.path.join(os.environ["UPRISING_PROJECT_PATH"], "robodk", "clean.rdk")

# path = "/Applications/RoboDK/RoboDK.app/Contents/MacOS/RoboDK"
# arglist = ["-NOUI", "-EXIT_LAST_COM", "-DEBUG"]
# link = Robolink(args=arglist, robodk_path=path)
# print "Started"
# for station in link.getOpenStations():
#     station.Delete()
# link.AddFile(CLEAN_FILE)
# print "Loaded CLEAN_FILE"



# list_items = link.ItemList()
# for item in list_items:
#     print(item.Name())

# print "----------------"

# link = Robolink()
# list_items = link.ItemList()
# for item in list_items:
#     print(item.Name())



# link.Disconnect()
# print "Disconnected"
