
import pymel.core as pm
import pymel.core.uitypes as gui
from uprising import (
    painting_tab,
    setup_tab,
    validate_tab,
    publish_tab,
    rings_design_tab,
    rings_setup_tab,
    images_tab)


# import uprising.logger_setup


import setup_dip
# from setup_dip import setup_dip_factory

import stroke_factory_utils

import studio

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
import culling
import props
import images


reload(studio)
reload(uprising_util)
reload(paint)
reload(brush)
reload(stroke)
reload(cluster)
reload(target)
reload(write)
reload(setup_dip)
reload(const)
reload(images)

reload(curve_utils)
reload(brush_utils)
reload(paint_utils)
reload(painting)
reload(stroke_factory_utils)
reload(sheets)

reload(painting_tab)
reload(setup_tab)
reload(props)
reload(validate_tab)
reload(publish_tab)
reload(images_tab)

reload(rings_design_tab)
reload(rings_setup_tab)
reload(culling)


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
        self.images_tab = images_tab.ImagesTab()
        self.tabs.setTabLabel((self.images_tab, "Images"))

        pm.setParent(self.tabs)
        self.setup_tab = setup_tab.SetupTab()
        self.tabs.setTabLabel((self.setup_tab, "Setup"))

        pm.setParent(self.tabs)
        self.painting_tab = painting_tab.PaintingTab()
        self.tabs.setTabLabel((self.painting_tab, "Painting"))
 
        pm.setParent(self.tabs)
        self.rings_design_tab = rings_design_tab.RingsDesignTab()
        self.tabs.setTabLabel((self.rings_design_tab, "Ring design"))

        pm.setParent(self.tabs)
        self.rings_setup_tab = rings_setup_tab.RingsSetupTab()
        self.tabs.setTabLabel((self.rings_setup_tab, "Ring setup"))

        pm.setParent(self.tabs)
        self.validate_tab = validate_tab.ValidateTab()
        self.tabs.setTabLabel((self.validate_tab, "Validate"))

        pm.setParent(self.tabs)
        self.publish_tab = publish_tab.PublishTab()
        self.tabs.setTabLabel((self.publish_tab, "Publish"))

        self.show()
        self.setResizeToFitChildren()
        self.tabs.setSelectTabIndex(5)

    def onTabChanged(self):
        print ""
