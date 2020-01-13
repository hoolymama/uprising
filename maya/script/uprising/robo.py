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
    print result
    return result


class Robo:
    """Singleton to keep some common data accessible."""
    instance = None

    @classmethod
    def clear(cls):
        cls.instance = None

# INIT 
# ROBO IS NONE
# instance WAS NOT NONE - DOINT NOTHING
# instance is now <uprising.robo.__Robo instance at 0x15ac33ab8>
# LINK AND ROBOT:
# <robolink.Robolink instance at 0x15ac33b00>
# RoboDK item (4562391056) of type 2
# <robolink.Robolink instance at 0x15ac33b00>


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
        # print "INIT "
        if not Robo.instance:
            # print "instance IS NONE - INSTANTIATING"
            Robo.instance = Robo.__Robo(**kw)
        # else:
        #     print "instance WAS NOT NONE - DOINT NOTHING"
        # print "instance is now", Robo.instance
        # print "LINK AND ROBOT:"
        # print Robo.instance._link
        # print Robo.instance._robot
        
            

    @property
    def link(self):
        return Robo.instance and Robo.instance._link

    @property
    def robot(self):
        if Robo.instance  and Robo.instance._link:
            return Robo.instance._robot


    def close(self):
        if Robo.instance and Robo.instance._link:
            try:
                for station in Robo.instance._link.getOpenStations():
                    station.Delete()
                Robo.instance._link.CloseRoboDK()
            except:
                print "RoboDK not open"
        Robo.clear()

    def show(self):
        if Robo.instance and Robo.instance._link:
             Robo.instance._link.ShowRoboDK()

    def hide(self):
        if Robo.instance and Robo.instance._link:
            Robo.instance._link.HideRoboDK()
    
    def clean(self):
        if Robo.instance and Robo.instance._link:
            for station in Robo.instance._link.getOpenStations():
                print "Deleting existing station"
                station.Delete()
        Robo.instance._link.AddFile(CLEAN_FILE)
        print "Added clean file: {}".format(CLEAN_FILE)

    def __str__(self):
        return  repr(Robo.instance)


# rodk = Robo()
# robot = rodk.robot
# rlink = rodk.link