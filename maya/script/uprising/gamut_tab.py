
import pymel.core as pm
import pymel.core.uitypes as gui

from uprising.session.gamut_session import GamutSession


class GamutTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.build()
        # self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()
        self.on_ops_change()

    def build(self):

        # frame = pm.frameLayout(label="", bv=True)
        self.minbb_ff = pm.floatFieldGrp(
            numberOfFields=3, label="Min BB", value1=-200, value2=-200, value3=10
        )
        self.maxbb_ff = pm.floatFieldGrp(
            numberOfFields=3, label="Max BB", value1=200, value2=200, value3=200
        )

        self.gap_ff = pm.floatFieldGrp(
            numberOfFields=1, label="Gap", value1=50
        )

        self.vector_type_rb = pm.radioButtonGrp(
            label='Z Axis', sl=2,
            labelArray2=[
                'Camera',
                'Custom'],
            numberOfRadioButtons=2,
            changeCommand=pm.Callback(self.on_ops_change))

        self.z_axis_ff = pm.floatFieldGrp(
            numberOfFields=3, label="Custom front vector", value1=1, value2=0, value3=0
        )

        self.x_axis_ff = pm.floatFieldGrp(
            numberOfFields=3, label="X Axis", value1=0, value2=0, value3=-1
        )

        brushes = pm.ls(type="brushNode")

        self.brush_menu = pm.optionMenuGrp(label="Brush")
        for brush in brushes:
            pm.menuItem(label=brush)

        self.create_cb = pm.checkBoxGrp(
            ncb=2,
            labelArray2=["Maya Locator", "RoboDK Target"],
            label="Create",
            value1=0, value2=1
        )

        # brush_atts = ["outPaintBrush", "outDipBrush", "outWipeBrush"]
        # for brush_node in brushNodes:
        #     for brush_att in brush_atts:
        #         plug = brush_node.attr(brush_att)
        #         Brush(0, plug).send()

    def on_ops_change(self):
        use_custom_vector = pm.radioButtonGrp(
            self.vector_type_rb, query=True, sl=True) == 2
        pm.floatFieldGrp(self.z_axis_ff,
                         edit=True, en=use_custom_vector)

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachForm(go_but, "left", 2)
        self.attachForm(go_but, "bottom", 2)
        return go_but

    ##############################################################

    def on_go(self):

        minbb = pm.floatFieldGrp(self.minbb_ff, q=True, value=True)
        maxbb = pm.floatFieldGrp(self.maxbb_ff, q=True, value=True)

        gap = pm.floatFieldGrp(self.gap_ff, q=True, value1=True)

        x_axis = pm.floatFieldGrp(self.x_axis_ff, q=True, value=True)

        z_axis = pm.floatFieldGrp(
            self.z_axis_ff, q=True, value=True)

        use_custom_vector = pm.radioButtonGrp(
            self.vector_type_rb, query=True, sl=True) == 2

        if not use_custom_vector:
            z_axis = "camera"

        brush_name = pm.optionMenuGrp(self.brush_menu, query=True, value=True)

        create_locator, create_target = pm.checkBoxGrp(
            self.create_cb, q=True, valueArray2=True)

        gamut_session = GamutSession(
            minbb, maxbb, gap, z_axis, x_axis,  brush_name, create_locator, create_target)
        gamut_session.run()
