
import pymel.core as pm
import brush_utils as butl
import paint_utils as putl
import board_utils as bdutl
import uprising_util as uutl
import pymel.core.uitypes as gui


class SetupTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_butttons()
        self.create_action_buttons_and_layout()

    def create_butttons(self):
        pm.setParent(self.column)

        with uutl.activatable(state=False):
            self.setup_board_row = pm.rowLayout(
                adjustableColumn=2,
                numberOfColumns=2, columnWidth2=(
                    350, 150), columnAlign=(
                    1, 'right'), columnAttach=[
                    (1, 'both', 2), (2, 'both', 2)])

            self.setup_board_tf = pm.textFieldGrp(
                label='Setup board', text='board_name')

            self.depth_only_cb = pm.checkBox(
                label='Depth only',
                value=0,
                annotation='Only set depth of the board')
            pm.setParent("..")

        with uutl.activatable(state=False):
            self.setup_brushes_tf = pm.textFieldGrp(
                label='Setup brushes', text='pouch_name')

        with uutl.activatable(state=False):
            self.setup_paints_tf = pm.textFieldGrp(
                label='Setup paints', text='palette_name')

        with uutl.activatable(state=False):
            self.setup_rack_tf = pm.textFieldGrp(
                label='Setup rack', text='rack_name')

    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel')
        go_but = pm.button(label='Go', command=pm.Callback(self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, cancel_but)

        self.attachNone(cancel_but, 'top')
        self.attachForm(cancel_but, 'left', 2)
        self.attachPosition(cancel_but, 'right', 2, 50)
        self.attachForm(cancel_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def populate(self):
        val = "default"
        if "up_setup_palette_name" in pm.optionVar:
            val = pm.optionVar["up_setup_palette_name"]
        pm.textFieldButtonGrp(self.setup_paints_tf, e=True, text=val)

    def save(self):
        val = pm.textFieldButtonGrp(self.setup_paints_tf, q=True, text=True)
        pm.optionVar["up_setup_palette_name"] = val

    def on_go(self):
        do_board = pm.rowLayout(self.setup_board_row, q=True, en=True)
        do_brushes = pm.textFieldGrp(self.setup_brushes_tf, q=True, en=True)
        do_paints = pm.textFieldGrp(self.setup_paints_tf, q=True, en=True)
        do_rack = pm.textFieldGrp(self.setup_rack_tf, q=True, en=True)
        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")

        if do_board:
            name = pm.textFieldGrp(self.setup_board_tf, q=True, text=True)
            depth_only = pm.checkBox(self.depth_only_cb, q=True, v=True)
            bdutl.setup_board_from_sheet(painting_node, name, depth_only)

        if do_brushes:
            name = pm.textFieldGrp(self.setup_brushes_tf, q=True, text=True)
            butl.setup_brushes_from_sheet(painting_node, dip_node, name)

        if do_paints:
            name = pm.textFieldGrp(self.setup_paints_tf, q=True, text=True)
            putl.setup_paints_from_sheet(painting_node, dip_node, name)

        if do_rack:
            name = pm.textFieldGrp(self.setup_rack_tf, q=True, text=True)
            putl.setup_rack_from_sheet(dip_node)
