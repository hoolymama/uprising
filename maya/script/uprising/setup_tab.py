
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
        self.populate()

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

        with uutl.activatable(state=False):
            self.probes_ctl = pm.floatFieldGrp(
                numberOfFields=2,
                label='Set Probes off/app',
                value1=1.0, value2=10.0)

        with uutl.activatable(state=False):
            self.read_calibration_tf = pm.textFieldGrp(
                label='Read calibration', text='test_calibration')


        # with uutl.activatable(state=True):
        #     self.generate_probes_ctl = pm.text(
        #         label='Generate probes', align="left" )

    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(label='Go', command=pm.Callback(self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, save_but)

        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def on_go(self):
        self.save()

        do_board = pm.rowLayout(self.setup_board_row, q=True, en=True)
        do_brushes = pm.textFieldGrp(self.setup_brushes_tf, q=True, en=True)
        do_paints = pm.textFieldGrp(self.setup_paints_tf, q=True, en=True)
        do_rack = pm.textFieldGrp(self.setup_rack_tf, q=True, en=True)
        do_calibration = pm.textFieldGrp(self.read_calibration_tf, q=True, en=True)


        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")
        wipe_node = pm.PyNode("wipePaintingShape")
        rack = pm.PyNode("rack1")


        if do_board:
            name = pm.textFieldGrp(self.setup_board_tf, q=True, text=True)
            depth_only = pm.checkBox(self.depth_only_cb, q=True, v=True)
            bdutl.setup_board_from_sheet(painting_node, name, depth_only)
            # bdutl.generate_probes(painting_node)


        if do_brushes:
            name = pm.textFieldGrp(self.setup_brushes_tf, q=True, text=True)
            butl.setup_brushes_from_sheet(painting_node, rack, name)

        if do_paints:
            name = pm.textFieldGrp(self.setup_paints_tf, q=True, text=True)
            putl.setup_paints_from_sheet(painting_node, rack, name)

        if do_rack:
            name = pm.textFieldGrp(self.setup_rack_tf, q=True, text=True)
            putl.setup_rack_from_sheet(dip_node)

        gen_probes = pm.floatFieldGrp(self.probes_ctl, q=True, en=True)
        if gen_probes:
            offset, approach_dist = pm.floatFieldGrp(
                self.probes_ctl, q=True, v=True)
            bdutl.generate_probes(painting_node, offset, approach_dist)

        if do_calibration:
            name = pm.textFieldGrp(self.read_calibration_tf, q=True, text=True)
            bdutl.set_board_vertices_from_sheets(painting_node, name)


    def populate(self):

        # board
        var = ("upov_setup_board_row_en", True)
        pm.rowLayout(self.setup_board_row, e=True, en=pm.optionVar.get(var[0],var[1]))
        uutl.conform_activatable_checkbox(self.setup_board_row)

        var = ("upov_setup_board_tf", "default")
        pm.textFieldGrp(self.setup_board_tf, e=True, text=pm.optionVar.get(var[0],var[1]))

        var = ("upov_depth_only_cb", False)
        pm.checkBox(self.depth_only_cb, e=True, value=pm.optionVar.get(var[0],var[1]))

        # brushes
        var = ("upov_setup_brushes_tf_en", True)
        pm.textFieldGrp(self.setup_brushes_tf, e=True, en=pm.optionVar.get(var[0],var[1]))
        uutl.conform_activatable_checkbox(self.setup_brushes_tf)

        var = ("upov_setup_brushes_tf", "default")
        pm.textFieldGrp(self.setup_brushes_tf, e=True, text=pm.optionVar.get(var[0],var[1]))

        # paints
        var = ("upov_setup_paints_tf_en", True)
        pm.textFieldGrp(self.setup_paints_tf, e=True, en=pm.optionVar.get(var[0],var[1]))
        uutl.conform_activatable_checkbox(self.setup_paints_tf)

        var = ("upov_setup_paints_tf", "default")
        pm.textFieldGrp(self.setup_paints_tf, e=True, text=pm.optionVar.get(var[0],var[1]))

        # rack
        var = ("upov_setup_rack_tf_en", True)
        pm.textFieldGrp(self.setup_rack_tf, e=True, en=pm.optionVar.get(var[0],var[1]))
        uutl.conform_activatable_checkbox(self.setup_rack_tf)

        var = ("upov_setup_rack_tf", "default")
        pm.textFieldGrp(self.setup_rack_tf, e=True, text=pm.optionVar.get(var[0],var[1]))

        # probes
        var = ("upov_probes_ctl_en", True)
        pm.floatFieldGrp(self.probes_ctl, e=True, en=pm.optionVar.get(var[0],var[1]))
        uutl.conform_activatable_checkbox(self.probes_ctl)

        var = ("upov_probes_ctl", 1, 10)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(self.probes_ctl, e=True, v1=vals[0], v2=vals[1])
 
        # calibration
        var = ("upov_read_calibration_tf_en", True)
        pm.textFieldGrp(self.read_calibration_tf, e=True, en=pm.optionVar.get(var[0],var[1]))
        uutl.conform_activatable_checkbox(self.read_calibration_tf)

        var = ("upov_read_calibration_tf", "default")
        pm.textFieldGrp(self.read_calibration_tf, e=True, text=pm.optionVar.get(var[0],var[1]))


    def save(self):
        # board
        var = "upov_setup_board_row_en"
        pm.optionVar[var] = pm.rowLayout(self.setup_board_row, q=True, en=True)

        var ="upov_setup_board_tf"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_board_tf, q=True, text=True)

        var = "upov_depth_only_cb"
        pm.optionVar[var] = pm.checkBox(self.depth_only_cb, q=True, value=True)

        # brushes
        var = "upov_setup_brushes_tf_en"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_brushes_tf, q=True, en=True)

        var ="upov_setup_brushes_tf"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_brushes_tf, q=True, text=True)

        # paints
        var = "upov_setup_paints_tf_en"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_paints_tf, q=True, en=True)

        var ="upov_setup_paints_tf"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_paints_tf, q=True, text=True)

        # rack
        var = "upov_setup_rack_tf_en"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_rack_tf, q=True, en=True)

        var ="upov_setup_rack_tf"
        pm.optionVar[var] = pm.textFieldGrp(self.setup_rack_tf, q=True, text=True)

        # probes
        var = "upov_probes_ctl_en"
        pm.optionVar[var] = pm.floatFieldGrp(self.probes_ctl, q=True, en=True)

        var = "upov_probes_ctl" 
        pm.optionVar[var] =  pm.floatFieldGrp(self.probes_ctl, q=True, v=True)
 
         # calibration
        var = "upov_read_calibration_tf_en"
        pm.optionVar[var] = pm.textFieldGrp(self.read_calibration_tf, q=True, en=True)

        var ="upov_read_calibration_tf"
        pm.optionVar[var] = pm.textFieldGrp(self.read_calibration_tf, q=True, text=True)



