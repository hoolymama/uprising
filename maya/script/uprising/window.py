
import pymel.core as pm
import pymel.core.uitypes as gui
from uprising import (
    painting_tab,
    setup_tab,
    validate_tab,
    publish_tab,
    brush_assign_tab,
    
    # rings_design_tab,
    # rings_setup_tab,
    # proposal_tab,
    tools_menu,
    export_menu,
    curves_menu,
    images_menu,
    palette_menu,
    tool_type_menu,
    calibration_menu
)


# import uprising.logger_setup


# import setup_dip
# # from setup_dip import setup_dip_factory

# import stroke_factory_utils

# import studio

# import painting
# import program
# # import calibration_program
# # import pick_place_program

# import paint
# import brush
# import stroke
# import cluster
# import target
# import write
# import uprising_util
# import const
# import sheets
# import curve_utils
# import brush_utils
# import palette_utils
# import board_utils

# import props
# import images

# reload(program)
# reload(studio)

# reload(uprising_util)
# reload(paint)
# reload(brush)
# reload(target)
# reload(stroke)
# reload(cluster)
# reload(write)
# reload(setup_dip)
# reload(const)
# reload(images)


# reload(curve_utils)
# reload(brush_utils)
# reload(palette_utils)
# reload(board_utils)
# reload(painting)

# reload(program)

# reload(stroke_factory_utils)
# reload(sheets)

# reload(painting_tab)
# reload(setup_tab)

# reload(props)
# reload(validate_tab)
reload(publish_tab)
reload(brush_assign_tab)

# reload(tools_menu)
# reload(curves_menu)
# reload(images_menu)
# reload(export_menu)
# reload(palette_menu)
# reload(calibration_menu)

# reload(tool_type_menu)


class RobotWindow(gui.Window):

    def __init__(self):

        if not pm.pluginInfo("Uprising", query=True, loaded=True):
            pm.loadPlugin('Uprising')

        others = pm.lsUI(windows=True)
        for win in others:
            if pm.window(win, q=True, title=True) == "Robot Tools":
                pm.deleteUI(win)

        self.setTitle('Robot Tools')
        self.setIconName('Robot Tools')
        self.setWidthHeight([500, 500])

        self.menuBarLayout = pm.menuBarLayout()

        self.tabs = pm.tabLayout(
            changeCommand=pm.Callback(
                self.on_tab_changed))

        pm.setParent(self.tabs)
        self.setup_tab = setup_tab.SetupTab()
        self.tabs.setTabLabel((self.setup_tab, "Setup"))

        pm.setParent(self.tabs)
        self.brush_assign_tab = brush_assign_tab.BrushAssignTab()
        self.tabs.setTabLabel((self.brush_assign_tab, "Assign brushes"))


        pm.setParent(self.tabs)
        self.painting_tab = painting_tab.PaintingTab()
        self.tabs.setTabLabel((self.painting_tab, "Painting"))

        # pm.setParent(self.tabs)
        # self.rings_design_tab = rings_design_tab.RingsDesignTab()
        # self.tabs.setTabLabel((self.rings_design_tab, "Ring design"))

        # pm.setParent(self.tabs)
        # self.rings_setup_tab = rings_setup_tab.RingsSetupTab()
        # self.tabs.setTabLabel((self.rings_setup_tab, "Ring setup"))

        pm.setParent(self.tabs)
        self.validate_tab = validate_tab.ValidateTab()
        self.tabs.setTabLabel((self.validate_tab, "Validate"))

        pm.setParent(self.tabs)
        self.publish_tab = publish_tab.PublishTab()
        self.tabs.setTabLabel((self.publish_tab, "Publish"))

        pm.setParent(self.menuBarLayout)
        self.tools_menu = tools_menu.create()

        pm.setParent(self.menuBarLayout)
        self.curves_menu = curves_menu.create()

        pm.setParent(self.menuBarLayout)
        self.images_menu = images_menu.create()

        pm.setParent(self.menuBarLayout)
        self.palette_menu = palette_menu.create()

        pm.setParent(self.menuBarLayout)
        self.calibration_menu = calibration_menu.create()

        pm.setParent(self.menuBarLayout)
        self.export_menu = export_menu.create()

        pm.setParent(self.menuBarLayout)
        self.tool_type_menu = tool_type_menu.create()

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
