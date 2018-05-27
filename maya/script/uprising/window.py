import os.path
import pymel.core as pm
import pymel.core.uitypes as gui
from  uprising import painting_tab , props_tab

reload(painting_tab)
reload(props_tab)

class RobotWindow(gui.Window):

    def __init__(self):
        others = pm.lsUI(windows=True)
        for win in others:
            if pm.window(win, q=True, title=True) == "Robot Tools":
                pm.deleteUI(win)
            
        self.setTitle('Robot Tools')
        self.setIconName('Robot Tools')
        self.setWidthHeight([500, 500])

        self.tabs = pm.tabLayout()

        pm.setParent(self.tabs)
        self.painting_tab = painting_tab.PaintingTab()
        self.tabs.setTabLabel((self.painting_tab, "Painting"))
        pm.setParent(self.tabs)
        self.props_tab = props_tab.PropsTab()
        self.tabs.setTabLabel((self.props_tab, "Props"))

        self.show()
        self.setResizeToFitChildren()
        self.tabs.setSelectTabIndex(1)

    def onTabChanged(self):
        print ""
 