
import pymel.core as pm
from uprising import curve_utils as cutl
from uprising import utils as uutl
import pymel.core.uitypes as gui


class RingsDesignTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()
        self.populate()

    def get_random_resource_values(self, row):
        if not pm.rowLayout(row, q=True, en=True):
            return None

        spec_ctl, steps_ctl, pow_ctl = pm.rowLayout(
            row, q=True, childArray=True)

        return(
            pm.textFieldGrp(spec_ctl, q=True, text=True),
            pm.intFieldGrp(steps_ctl, q=True, value1=True),
            pm.floatFieldGrp(pow_ctl, q=True, value1=True)
        )

    def random_res_control(self, label, default_text):

        with uutl.activatable():
            row = pm.rowLayout(numberOfColumns=3,
                               columnWidth3=(250, 90, 90),
                               columnAlign=(1, 'right'))

            pm.textFieldGrp(
                label=label,
                text=default_text
            )
            pm.intFieldGrp(
                columnWidth2=(30, 60,),
                numberOfFields=1, value1=-1, label='steps'
            )
            pm.floatFieldGrp(
                columnWidth2=(30, 60,),
                numberOfFields=1, value1=0, label='pow'
            )
            pm.setParent("..")
        return row

    def create_buttons(self):
        pm.setParent(self.column)

        with uutl.activatable():
            self.spacing_row = pm.rowLayout(
                numberOfColumns=2, columnWidth2=(
                    350, 150), columnAlign=(
                    1, 'right'), columnAttach=[
                    (1, 'both', 2), (2, 'both', 2)])

            self.spacing_type_ctl = pm.radioButtonGrp(
                label='Spacing',
                sl=1,
                labelArray2=[
                    'Spines',
                    'Gaps'],
                numberOfRadioButtons=2)

            self.spacing_ctl = pm.floatFieldGrp(
                numberOfFields=1, value1=1, pre=2, extraLabel='cm')
            pm.setParent("..")

        with uutl.activatable():
            self.max_extent_ctrl = pm.floatFieldGrp(
                label='Max extent',
                value1=50
            )

        self.random_paints_row = self.random_res_control(
            "Random paints", "0-32")
        self.random_brushes_row = self.random_res_control(
            "Random brushes", "0-16")

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(
            label='Save',
            command=pm.Callback(self.save))

        set_button = pm.button(
            label='Set value',
            command=pm.Callback(
                self.on_go,
                False))
        key_button = pm.button(
            label='Keyframe range',
            command=pm.Callback(
                self.on_go,
                True))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, set_button)

        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 33)
        self.attachForm(save_but, 'bottom', 2)

        self.attachNone(set_button, 'top')
        self.attachControl(set_button, 'left', 2, save_but)
        self.attachPosition(set_button, 'right', 2, 66)
        self.attachForm(set_button, 'bottom', 2)

        self.attachNone(key_button, 'top')
        self.attachForm(key_button, 'right', 2)
        self.attachControl(key_button, 'left', 2, set_button)
        self.attachForm(key_button, 'bottom', 2)

    def on_go(self, do_keys):

        random_paint_params = self.get_random_resource_values(
            self.random_paints_row)
        random_brush_params = self.get_random_resource_values(
            self.random_brushes_row)

        do_max = pm.floatFieldGrp(self.max_extent_ctrl, query=True, en=True)

        spacing_type = "none"
        if pm.rowLayout(
                self.spacing_row,
                query=True, en=True):
            spacing_type = "spine" if (
                pm.radioButtonGrp(
                    self.spacing_type_ctl,
                    query=True,
                    sl=True) == 1) else "gap"

        spacing = pm.floatFieldGrp(self.spacing_ctl, query=True, value1=True)
        max_extent = pm.floatFieldGrp(
            self.max_extent_ctrl,
            query=True,
            value1=True) if do_max else None
 
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True)
        if not curves:
            pm.error("No curves selected")
            return

        paint_ids = []
        brush_ids = []
        if random_paint_params and random_paint_params[0] == "":
            for curve in curves:
                sc = cutl.get_stroke_node(curve)
                paint_ids.append(sc.attr("paintId").get())

        if random_brush_params and random_brush_params[0] == "":
            for curve in curves:
                sc = cutl.get_stroke_node(curve)
                paint_ids.append(sc.attr("brushId").get())

        curves_packs = map(None, curves, brush_ids, paint_ids)

        if do_keys:
            start_frame = int(pm.playbackOptions(q=True, min=True))
            end_frame = int(pm.playbackOptions(q=True, max=True))
            for f in range(start_frame, end_frame + 1):
                pm.currentTime(f)
                cutl._randomize(
                    curves_packs,
                    random_paint_params,
                    random_brush_params,
                    max_extent,
                    spacing,
                    spacing_type,
                    paint_ids,
                    brush_ids,
                    True)
        else:
            cutl._randomize(
                curves_packs,
                random_paint_params,
                random_brush_params,
                max_extent,
                spacing,
                spacing_type,
                paint_ids,
                brush_ids,
                False)

    def populate(self):

        var = ("upov_spacing_row_en", True)
        pm.rowLayout(
            self.spacing_row,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.spacing_row)

        var = ("upov_spacing_type_ctl", 2)
        pm.radioButtonGrp(
            self.spacing_type_ctl,
            e=True,
            sl=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_spacing_ctl", 0.3)
        pm.floatFieldGrp(
            self.spacing_ctl,
            e=True,
            value1=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_max_extent_ctrl_en", True)
        pm.floatFieldGrp(
            self.max_extent_ctrl,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.max_extent_ctrl)

        var = ("upov_max_extent_ctrl", 57)
        pm.floatFieldGrp(
            self.max_extent_ctrl,
            e=True,
            v1=pm.optionVar.get(
                var[0],
                var[1]))

        self.populate_random_res("random_paints_row")
        self.populate_random_res("random_brushes_row")

    def save(self):
        # board
        var = "upov_spacing_row_en"
        pm.optionVar[var] = pm.rowLayout(self.spacing_row, q=True, en=True)

        var = "upov_spacing_type_ctl"
        pm.optionVar[var] = pm.radioButtonGrp(
            self.spacing_type_ctl, q=True, sl=True)

        var = "upov_spacing_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.spacing_ctl, q=True, value1=True)

        var = "upov_max_extent_ctrl_en"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.max_extent_ctrl, q=True, en=True)

        var = "upov_max_extent_ctrl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.max_extent_ctrl, q=True, v1=True)

        self.save_random_res("random_paints_row")
        self.save_random_res("random_brushes_row")

    def populate_random_res(self, row_attribute):
        row = getattr(self, row_attribute)
        var = ("upov_%s_en" % row_attribute, True)
        pm.rowLayout(row, e=True, en=pm.optionVar.get(var[0], var[1]))

        text_ctrl, steps_ctrl, power_ctrl = row.getChildArray()

        var = ("upov_%s_spec" % row_attribute, "0-15")
        pm.textFieldGrp(
            text_ctrl,
            e=True,
            text=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_%s_steps" % row_attribute, -1)
        pm.intFieldGrp(
            steps_ctrl,
            e=True,
            value1=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_%s_power" % row_attribute, 0.0)
        pm.floatFieldGrp(
            power_ctrl,
            e=True,
            value1=pm.optionVar.get(
                var[0],
                var[1]))

    def save_random_res(self, row_attribute):

        row = getattr(self, row_attribute)
        opvar = "upov_%s_en" % row_attribute
        pm.optionVar[opvar] = pm.rowLayout(row, q=True, en=True)

        text_ctrl, steps_ctrl, power_ctrl = row.getChildArray()

        var = "upov_%s_spec" % row_attribute
        pm.optionVar[var] = pm.textFieldGrp(text_ctrl, q=True, text=True)

        var = "upov_%s_steps" % row_attribute
        pm.optionVar[var] = pm.intFieldGrp(steps_ctrl, q=True, value1=True)

        var = "upov_%s_power" % row_attribute
        pm.optionVar[var] = pm.floatFieldGrp(power_ctrl, q=True, value1=True)
