import brush
import brush_utils
import cluster
import const
import curve_utils
import chain_menu
import images
import palette_utils
import pymel.core as pm
import pymel.core.uitypes as gui
import setup_dip
import studio
import uprising_util
import write
from uprising import (
    brush_assign_tab,
    color_tab,
    brush_hang_tab,
    brush_test_tab,
    calibration,
    calibration_menu,
    curves_menu,
    export_menu,
    export_tab,
    images_menu,
    info_menu,
    painting_tab,
    palette_menu,
    phex_tab,
    probes_tab,
    publish_tab,
    robo,
    select_menu,
    chain_menu,
    setup_tab,
    stats,
    tools_menu,
    studio,
)

reload(uprising_util)
reload(robo)
reload(brush)
reload(stats)
reload(cluster)
reload(write)
reload(setup_dip)
reload(const)
reload(curve_utils)
reload(brush_utils)
reload(palette_utils)
reload(chain_menu)

reload(setup_tab)
reload(color_tab)
reload(phex_tab)
reload(brush_hang_tab)
reload(publish_tab)
reload(brush_assign_tab)
reload(brush_test_tab)
reload(probes_tab)
reload(tools_menu)
reload(info_menu)
reload(curves_menu)
reload(images_menu)
reload(select_menu)
reload(images)
reload(calibration)
reload(calibration_menu)
reload(studio)
reload(export_tab)
reload(export_menu)


class RobotWindow(gui.Window):
    def __init__(self):

        if not pm.pluginInfo("Uprising", query=True, loaded=True):
            pm.loadPlugin("Uprising")

        others = pm.lsUI(windows=True)
        for win in others:
            if pm.window(win, q=True, title=True) == "Robot Tools":
                pm.deleteUI(win)

        self.setTitle("Robot Tools")
        self.setIconName("Robot Tools")
        self.setWidthHeight([700, 600])

        self.menuBarLayout = pm.menuBarLayout()

        self.tabs = pm.tabLayout(changeCommand=pm.Callback(self.on_tab_changed))

        pm.setParent(self.tabs)
        self.setup_tab = setup_tab.SetupTab()
        self.tabs.setTabLabel((self.setup_tab, "Setup"))

        pm.setParent(self.tabs)
        self.brush_test_tab = brush_test_tab.BrushTestTab()
        self.tabs.setTabLabel((self.brush_test_tab, "Brush test"))

        pm.setParent(self.tabs)
        self.brush_assign_tab = brush_assign_tab.BrushAssignTab()
        self.tabs.setTabLabel((self.brush_assign_tab, "Assign brushes"))

        pm.setParent(self.tabs)
        self.painting_tab = painting_tab.PaintingTab()
        self.tabs.setTabLabel((self.painting_tab, "Painting"))

        pm.setParent(self.tabs)
        self.export_tab = export_tab.exportTab()
        self.tabs.setTabLabel((self.export_tab, "Export"))

        pm.setParent(self.tabs)
        self.phex_tab = phex_tab.phexTab()
        self.tabs.setTabLabel((self.phex_tab, "PHEX"))

        pm.setParent(self.tabs)
        self.brush_hang_tab = brush_hang_tab.brushHangTab()
        self.tabs.setTabLabel((self.brush_hang_tab, "Brush Hang"))

        pm.setParent(self.tabs)
        self.publish_tab = publish_tab.PublishTab()
        self.tabs.setTabLabel((self.publish_tab, "Publish"))

        pm.setParent(self.tabs)
        self.probes_tab = probes_tab.probesTab()
        self.tabs.setTabLabel((self.probes_tab, "Probes"))

        pm.setParent(self.tabs)
        self.color_tab = color_tab.ColorTab()
        self.tabs.setTabLabel((self.color_tab, "Color"))


        pm.setParent(self.menuBarLayout)
        self.tools_menu = tools_menu.create()

        pm.setParent(self.menuBarLayout)
        self.select_menu = select_menu.create()

        pm.setParent(self.menuBarLayout)
        self.curves_menu = curves_menu.create()

        pm.setParent(self.menuBarLayout)
        self.images_menu = images_menu.create()

        pm.setParent(self.menuBarLayout)
        self.palette_menu = palette_menu.create()

        pm.setParent(self.menuBarLayout)
        self.info_menu = info_menu.create()

        pm.setParent(self.menuBarLayout)
        self.calibration_menu = calibration_menu.create()

        pm.setParent(self.menuBarLayout)
        self.chain_menu = chain_menu.create()
        
        pm.setParent(self.menuBarLayout)
        self.export_menu = export_menu.create()

        self.show()
        self.setResizeToFitChildren()

        self.populate()

    def on_tab_changed(self):
        self.save()

    def populate(self):
        var = ("upov_tab_index", 2)
        self.tabs.setSelectTabIndex(pm.optionVar.get(var[0], var[1]))

    def save(self):
        var = "upov_tab_index"
        pm.optionVar[var] = self.tabs.getSelectTabIndex()
