import pymel.core.uitypes as gui
import pymel.core as pm

from uprising.pov.ui import (
    painting_tab,
    publish_tab,
    tools_menu
)

from uprising.pov import reloader
reload(reloader)


class PovWindow(gui.Window):
    def __init__(self):

        if not pm.pluginInfo("Uprising", query=True, loaded=True):
            pm.loadPlugin("Uprising")

        others = pm.lsUI(windows=True)
        for win in others:
            if pm.window(win, q=True, title=True) == "Pov Tools":
                pm.deleteUI(win)

        self.setTitle("Pov Tools")
        self.setIconName("Pov Tools")
        self.setWidthHeight([700, 600])

        self.menuBarLayout = pm.menuBarLayout()

        self.tabs = pm.tabLayout(
            changeCommand=pm.Callback(self.on_tab_changed))

        pm.setParent(self.tabs)
        self.painting_tab = painting_tab.PovPaintingTab()
        self.tabs.setTabLabel((self.painting_tab, "Painting"))

        pm.setParent(self.tabs)
        self.publish_tab = publish_tab.PovPublishTab()
        self.tabs.setTabLabel((self.publish_tab, "Publish"))

        pm.setParent(self.menuBarLayout)
        self.tools_menu = tools_menu.create()

        self.show()
        self.setResizeToFitChildren()

        self.populate()

    def on_tab_changed(self):
        self.save()

    def populate(self):
        var = ("pov_ov_tab_index", 2)
        self.tabs.setSelectTabIndex(pm.optionVar.get(var[0], var[1]))

    def save(self):
        var = "pov_ov_tab_index"
        pm.optionVar[var] = self.tabs.getSelectTabIndex()
