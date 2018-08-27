
import pymel.core as pm
import pymel.core.uitypes as gui
from  uprising import (painting_tab , setup_tab, props_tab)

import setup_dip
# from setup_dip import setup_dip_factory

import stroke_factory_utils

import painting 
import paint
import brush
import stroke
import cluster
import target
import write
import uprising_util
import const
import sheets
import curve_utils
import brush_utils
import paint_utils


reload(uprising_util)
reload(paint)
reload(brush)
reload(stroke)
reload(cluster)
reload(target)
reload(write)
reload(setup_dip)
reload(const)

reload(curve_utils)
reload(brush_utils)
reload(paint_utils)
reload(painting)
reload(stroke_factory_utils)
reload(sheets)



reload(painting_tab)
reload(setup_tab)
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
        self.setup_tab = setup_tab.SetupTab()
        self.tabs.setTabLabel((self.setup_tab, "Setup"))
         
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
 