import os
import time
from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET


CLEAN_FILE = os.path.join(os.environ["UPRISING_PROJECT_PATH"], "robodk", "clean.rdk")
ROBODK_PATH = "/Applications/RoboDK/RoboDK.app/Contents/MacOS/RoboDK"

class Singleton:
    """
    A non-thread-safe helper class to ease implementing singletons.
    This should be used as a decorator -- not a metaclass -- to the
    class that should be a singleton.

    The decorated class can define one `__init__` function that
    takes only the `self` argument. Also, the decorated class cannot be
    inherited from. Other than that, there are no restrictions that apply
    to the decorated class.

    To get the singleton instance, use the `instance` method. Trying
    to use `__call__` will result in a `TypeError` being raised.

    """

    def __init__(self, decorated):
        self._decorated = decorated

    def instance(self):
        """
        Returns the singleton instance. Upon its first call, it creates a
        new instance of the decorated class and calls its `__init__` method.
        On all subsequent calls, the already created instance is returned.

        """
        try:
            return self._instance
        except AttributeError:
            self._instance = self._decorated()
            return self._instance

    def __call__(self):
        raise TypeError('Singletons must be accessed through `instance()`.')

    def __instancecheck__(self, inst):
        return isinstance(inst, self._decorated)


# @Singleton
class Robo:
    def __init__(self, **kw):
        print 'Robo created'
        self.link = None
        self.robot = None

        self.fresh(**kw)

    def fresh(self, **kw):

        self.close()
        # time.sleep(1)

        startup_args = []
        if kw.get("mode", "show") == "hidden":
            startup_args = ["-HIDDEN", "-NOSPLASH", "-NOSHOW"]
        if kw.get("debug"):
            startup_args.append("-DEBUG")

        print "startup_args", startup_args
        self.link = Robolink(args=startup_args, robodk_path=ROBODK_PATH)
        self.link.AddFile(CLEAN_FILE)
        self.robot = self.link.Item("", ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")

    def empty(self):
        try:
            for station in self.link.getOpenStations():
                station.Delete()
        except:
            print "No stations to empty"


    def close(self):
        self.empty()
        try:
            self.link.Disconnect()
            self.link.CloseRoboDK()
        except BaseException:
            print "Already closed"
        self.link = None
        self.robot = None

    def show(self):
        self.link.ShowRoboDK()

    def hide(self):
        self.link.HideRoboDK()

    def clean(self):
        self.empty()
        self.link.AddFile(CLEAN_FILE)
        print "Added clean file: {}".format(CLEAN_FILE)




# rodk = Robo()
# robot = rodk.robot
# rlink = rodk.link


