
import pymel.core as pm
import brush_utils as butl
import palette_utils as putl
# import board_utils as bdutl
import uprising_util as uutl
import pymel.core.uitypes as gui
import setup_dip


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
            self.setup_brushes_tf = pm.textFieldGrp(
                label='Setup brushes', text='pouch_name')

        with uutl.activatable(state=False):
            self.setup_paints_tf = pm.textFieldGrp(
                label='Setup paints', text='palette_name')

        pm.separator(style="in", height=20)

        # with uutl.activatable(state=False):
        #     self.setup_board_row = pm.rowLayout(
        #         adjustableColumn=2,
        #         numberOfColumns=2, columnWidth2=(
        #             350, 150), columnAlign=(
        #             1, 'right'), columnAttach=[
        #             (1, 'both', 2), (2, 'both', 2)])

        #     self.setup_board_tf = pm.textFieldGrp(
        #         label='Setup board', text='board_name')

        #     self.depth_only_cb = pm.checkBox(
        #         label='Depth only',
        #         value=0,
        #         annotation='Only set depth of the board')
        #     pm.setParent("..")
 
        # with uutl.activatable(state=False):
        #     self.probes_ctl = pm.floatFieldGrp(
        #         numberOfFields=2,
        #         label='Set Probes off/app',
        #         value1=1.0, value2=10.0)

        # with uutl.activatable(state=False):
        #     self.read_calibration_tf = pm.textFieldGrp(
        #         label='Read calibration', text='test_calibration')


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

        do_brushes = pm.textFieldGrp(self.setup_brushes_tf, q=True, en=True)
        do_paints = pm.textFieldGrp(self.setup_paints_tf, q=True, en=True)
  

        if do_brushes:
            name = pm.textFieldGrp(self.setup_brushes_tf, q=True, text=True)
            butl.setup_brushes_from_sheet(name)

        if do_paints:
            name = pm.textFieldGrp(self.setup_paints_tf, q=True, text=True)
            putl.setup_paints_from_sheet(name)



        if do_brushes or do_paints:
            setup_dip.doit()



    def populate(self):

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

         
        # # probes
        # var = ("upov_probes_ctl_en", True)
        # pm.floatFieldGrp(self.probes_ctl, e=True, en=pm.optionVar.get(var[0],var[1]))
        # uutl.conform_activatable_checkbox(self.probes_ctl)

        # var = ("upov_probes_ctl", 1, 10)
        # vals = pm.optionVar.get(var[0], var[1:])
        # pm.floatFieldGrp(self.probes_ctl, e=True, v1=vals[0], v2=vals[1])
 
        # # calibration
        # var = ("upov_read_calibration_tf_en", True)
        # pm.textFieldGrp(self.read_calibration_tf, e=True, en=pm.optionVar.get(var[0],var[1]))
        # uutl.conform_activatable_checkbox(self.read_calibration_tf)

        # var = ("upov_read_calibration_tf", "default")
        # pm.textFieldGrp(self.read_calibration_tf, e=True, text=pm.optionVar.get(var[0],var[1]))


    def save(self):
        # board
        # var = "upov_setup_board_row_en"
        # pm.optionVar[var] = pm.rowLayout(self.setup_board_row, q=True, en=True)

        # var ="upov_setup_board_tf"
        # pm.optionVar[var] = pm.textFieldGrp(self.setup_board_tf, q=True, text=True)

        # var = "upov_depth_only_cb"
        # pm.optionVar[var] = pm.checkBox(self.depth_only_cb, q=True, value=True)

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

        
        # # probes
        # var = "upov_probes_ctl_en"
        # pm.optionVar[var] = pm.floatFieldGrp(self.probes_ctl, q=True, en=True)

        # var = "upov_probes_ctl" 
        # pm.optionVar[var] =  pm.floatFieldGrp(self.probes_ctl, q=True, v=True)
 
        #  # calibration
        # var = "upov_read_calibration_tf_en"
        # pm.optionVar[var] = pm.textFieldGrp(self.read_calibration_tf, q=True, en=True)

        # var ="upov_read_calibration_tf"
        # pm.optionVar[var] = pm.textFieldGrp(self.read_calibration_tf, q=True, text=True)



