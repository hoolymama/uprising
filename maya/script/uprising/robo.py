import os

from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET

CLEAN_FILE = os.path.join(os.environ["UPRISING_PROJECT_PATH"], "robodk", "clean.rdk")


def create_link(**kw):
    startup_args = []
    if kw.get("mode", "show") == "hidden":
        startup_args = ["-HIDDEN", "-NOSPLASH", "-NOSHOW"]
    if kw.get("debug"):
        startup_args.append("-DEBUG")

    print "startup_args", startup_args
    path = "/Applications/RoboDK/RoboDK.app/Contents/MacOS/RoboDK"

    result = Robolink(args=startup_args, robodk_path=path)

    print "Created Robo link"
    return result


class Robo:
    """Singleton to keep some common data accessible."""
    instance = None

    @classmethod
    def clear(cls):
        cls.instance = None


    class __Robo:
        def __init__(self, **kw):
            # This is called once
            # self.mode =   kw.get("mode", "hidden") == "api"

            self._link = create_link(**kw)
            self._link.AddFile(CLEAN_FILE)
            self._robot = self._link.Item("", ITEM_TYPE_ROBOT)
            self._robot.setParam("PostProcessor", "KUKA KRC4_RN")

            print "Got link and robot"
            print "Loaded Clean scene and set PostProcessor to KUKA KRC4_RN"
    

    def __init__(self, **kw):
        """Create a new Robo the first time only.
        """
        if kw.get("force"):
            self.close()

        if not Robo.instance:
            Robo.instance = Robo.__Robo(**kw)

    @property
    def link(self):
        return self.instance and self.instance._link

    @property
    def robot(self):
        return self.instance._robot


    def close(self):
        if self.instance and self.instance._link:
            try:
                for station in self.link.getOpenStations():
                    station.Delete()
                self.link.CloseRoboDK()
            except:
                print "RoboDK not open"
        Robo.clear()

    def show(self):
        self.link.ShowRoboDK()

    def hide(self):
        self.link.HideRoboDK()
    
    def clean(self):
        for station in self.link.getOpenStations():
            station.Delete()
        self.link.AddFile(CLEAN_FILE)

    def __str__(self):
        return  repr(self.instance)


