
import pymel.core as pm
import curve_utils as cutl
import utils as uutl


import pymel.core.uitypes as gui


class RingsSetupTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()
        self.populate()

    def create_buttons(self):
        pm.setParent(self.column)

        with uutl.activatable(state=False):
            self.distribution_frame = pm.frameLayout(
                borderVisible=True,
                collapsable=False,
                labelVisible=False,
                label="Stroke distribution")
            self.min_strokes_ctl = pm.intSliderGrp(
                field=True,
                label='Minimum strokes',
                minValue=1,
                maxValue=6,
                fieldMinValue=1,
                fieldMaxValue=50,
                value=1)

            self.max_length_ctl = pm.floatSliderGrp(
                field=True,
                label='Max stroke length',
                minValue=1.0,
                maxValue=40.0,
                fieldMinValue=1.0,
                fieldMaxValue=200.0,
                value=30.0)

            self.overlap_ctl = pm.floatSliderGrp(
                field=True,
                label='Overlap length',
                minValue=0.0,
                maxValue=10.0,
                fieldMinValue=0.0,
                fieldMaxValue=20.0,
                value=4.0)
            pm.setParent("..")

        with uutl.activatable(state=False):
            self.subcurve_ctl = pm.floatFieldGrp(
                numberOfFields=1,
                label='Lift above curve',
                ann="Set subcurve min so that entry lift starts on the curve",
                value1=1.0)

        with uutl.activatable(state=False):
            self.lift_ctl = pm.floatFieldGrp(
                numberOfFields=3,
                label='Tip relative lift l/h/b',
                ann="Set lift parameters relative to brush tip length",
                value1=1.3,
                value2=1.3,
                value3=0)

        with uutl.activatable(state=False):
            self.twist_row = pm.rowLayout(
                numberOfColumns=2, columnWidth2=(
                    350, 150), columnAlign=(
                    1, 'right'), columnAttach=[
                    (1, 'both', 2), (2, 'both', 2)])
            self.twist_ctl = pm.floatFieldGrp(
                numberOfFields=2,
                label='Twist angle/dist',
                value1=0, value2=1)
            self.flat_only_cb = pm.checkBox(
                label='Flat only',
                value=1,
                annotation='Only set flat brush ramps')

            pm.setParent("..")

        with uutl.activatable(state=False):
            self.profile_ctl = pm.floatFieldGrp(
                numberOfFields=2,
                label='Profile h-ratio/dist',
                value1=1.0, value2=1)

        with uutl.activatable(state=False):
            self.rand_rotation_ctl = pm.floatFieldGrp(
                numberOfFields=2,
                label='Random rotation min/max',
                value1=0.0, value2=360)

        with uutl.activatable(state=False):
            self.density_frame = pm.frameLayout(
                borderVisible=True,
                collapsable=False,
                labelVisible=False,
                label="Foo")
            self.density_min_ctl = pm.floatFieldGrp(
                numberOfFields=2,
                label='Min radius/density',
                value1=5, value2=4)
            self.density_max_ctl = pm.floatFieldGrp(
                numberOfFields=2,
                label='Max radius/density',
                value1=60, value2=0.4)
            self.density_pow_ctl = pm.floatFieldGrp(
                numberOfFields=1,
                label='Power',
                value1=1)
            pm.setParent("..")

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(
            label='Go', command=pm.Callback(
                self.on_go))

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

    def on_random_rotations(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        cutl.randomize_ring_rotation(curves)

    def on_go(self):

        do_twist = pm.floatFieldGrp(self.twist_ctl, q=True, en=True)
        do_profile = pm.floatFieldGrp(self.profile_ctl, q=True, en=True)
        do_subcurve = pm.floatFieldGrp(self.subcurve_ctl, q=True, en=True)
        do_rand_rotate = pm.floatFieldGrp(
            self.rand_rotation_ctl, q=True, en=True)
        do_distribution = pm.frameLayout(
            self.distribution_frame, q=True, en=True)
        do_lift = pm.floatFieldGrp(self.lift_ctl, q=True, en=True)

        do_density = pm.frameLayout(self.density_frame, q=True, en=True)

        density = None
        distribution = None
        twist = None
        profile = None
        subcurve_factor = None
        rand_rotate = None
        lift = None

        if do_distribution:
            distribution = {
                "min_strokes": pm.intSliderGrp(
                    self.min_strokes_ctl, q=True, v=True), "max_length": pm.floatSliderGrp(
                    self.max_length_ctl, q=True, v=True), "overlap": pm.floatSliderGrp(
                    self.overlap_ctl, q=True, v=True)}
        if do_lift:
            lift = pm.floatFieldGrp(self.lift_ctl, q=True, v=True)
        if do_twist:
            twist = pm.floatFieldGrp(self.twist_ctl, q=True, v=True)
            flat_only = pm.checkBox(self.flat_only_cb, q=True, v=True)
            twist.append(flat_only)
        if do_profile:
            profile = pm.floatFieldGrp(self.profile_ctl, q=True, v=True)
        if do_subcurve:
            subcurve_factor = pm.floatFieldGrp(
                self.subcurve_ctl, q=True, v1=True)
        if do_rand_rotate:
            rand_rotate = pm.floatFieldGrp(
                self.rand_rotation_ctl, q=True, v=True)

        if do_density:
            dmin = pm.floatFieldGrp(self.density_min_ctl, q=True, v=True)
            dmax = pm.floatFieldGrp(self.density_max_ctl, q=True, v=True)
            power = pm.floatFieldGrp(self.density_pow_ctl, q=True, v1=True)
            density = [dmin, dmax, power]

        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        if not curves:
            raise pm.MayaNodeError("Must select curves")
        cutl.auto_set_rings(
            curves,
            distribution,
            lift,
            twist,
            profile,
            subcurve_factor,
            rand_rotate,
            density)

    def populate(self):

        var = ("upov_distribution_frame_en", True)
        pm.frameLayout(
            self.distribution_frame,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.distribution_frame)

        var = ("upov_min_strokes_ctl", 1)
        pm.intSliderGrp(
            self.min_strokes_ctl,
            e=True,
            value=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_max_length_ctl", 1)
        pm.floatSliderGrp(
            self.max_length_ctl,
            e=True,
            value=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_overlap_ctl", 1)
        pm.floatSliderGrp(
            self.overlap_ctl,
            e=True,
            value=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_subcurve_ctl_en", True)
        pm.floatFieldGrp(
            self.subcurve_ctl,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.subcurve_ctl)

        var = ("upov_subcurve_ctl", 1)
        pm.floatFieldGrp(
            self.subcurve_ctl,
            e=True,
            value1=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_lift_ctl_en", True)
        pm.floatFieldGrp(
            self.lift_ctl,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.lift_ctl)

        var = ("upov_lift_ctl", 1.3, 1.3, 0.0)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(self.lift_ctl,
                         e=True,
                         value1=vals[0],
                         value2=vals[1],
                         value3=vals[2]
                         )

        var = ("upov_twist_row_en", True)
        pm.rowLayout(
            self.twist_row,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.twist_row)

        var = ("upov_twist_ctl", 0, 1)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(
            self.twist_ctl,
            e=True,
            value1=vals[0],
            value2=vals[1])

        var = ("upov_flat_only_cb", False)
        pm.checkBox(
            self.flat_only_cb,
            e=True,
            value=pm.optionVar.get(
                var[0],
                var[1]))

        var = ("upov_profile_ctl_en", True)
        pm.floatFieldGrp(
            self.profile_ctl,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.profile_ctl)

        var = ("upov_profile_ctl", 0, 1)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(
            self.profile_ctl,
            e=True,
            value1=vals[0],
            value2=vals[1])

        var = ("upov_rand_rotation_ctl_en", True)
        pm.floatFieldGrp(
            self.rand_rotation_ctl,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.rand_rotation_ctl)

        var = ("upov_rand_rotation_ctl", 0, 1)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(
            self.rand_rotation_ctl,
            e=True,
            value1=vals[0],
            value2=vals[1])

        var = ("upov_density_frame_en", True)
        pm.frameLayout(
            self.density_frame,
            e=True,
            en=pm.optionVar.get(
                var[0],
                var[1]))
        uutl.conform_activatable_checkbox(self.density_frame)

        var = ("upov_density_min_ctl", 1, 2)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(
            self.density_min_ctl,
            e=True,
            value1=vals[0],
            value2=vals[1])

        var = ("upov_density_max_ctl", 1, 2)
        vals = pm.optionVar.get(var[0], var[1:])
        pm.floatFieldGrp(
            self.density_max_ctl,
            e=True,
            value1=vals[0],
            value2=vals[1])

        var = ("upov_density_pow_ctl", 1)
        pm.floatFieldGrp(
            self.density_pow_ctl,
            e=True,
            value1=pm.optionVar.get(var[0], var[1]))

    def save(self):
        # board
        var = "upov_distribution_frame_en"
        pm.optionVar[var] = pm.frameLayout(
            self.distribution_frame, q=True, en=True)

        var = "upov_min_strokes_ctl"
        pm.optionVar[var] = pm.intSliderGrp(
            self.min_strokes_ctl, q=True, value=True)

        var = "upov_max_length_ctl"
        pm.optionVar[var] = pm.floatSliderGrp(
            self.max_length_ctl, q=True, value=True)

        var = "upov_overlap_ctl"
        pm.optionVar[var] = pm.floatSliderGrp(
            self.overlap_ctl, q=True, value=True)

        var = "upov_subcurve_ctl_en"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.subcurve_ctl, q=True, en=True)

        var = "upov_subcurve_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.subcurve_ctl, q=True, value1=True)

        var = "upov_lift_ctl_en"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.lift_ctl, q=True, en=True)

        var = "upov_lift_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.lift_ctl, q=True, value=True)

        var = "upov_twist_row_en"
        pm.optionVar[var] = pm.rowLayout(self.twist_row, q=True, en=True)

        var = "upov_twist_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.twist_ctl, q=True, value=True)

        var = "upov_flat_only_cb"
        pm.optionVar[var] = pm.checkBox(self.flat_only_cb, q=True, value=True)

        var = "upov_profile_ctl_en"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.profile_ctl, q=True, en=True)

        var = "upov_profile_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.profile_ctl, q=True, value=True)

        var = "upov_rand_rotation_ctl_en"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.rand_rotation_ctl, q=True, en=True)

        var = "upov_rand_rotation_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.rand_rotation_ctl, q=True, value=True)

        var = "upov_density_frame_en"
        pm.optionVar[var] = pm.frameLayout(
            self.density_frame, q=True, en=True)

        var = "upov_density_min_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.density_min_ctl, q=True, value=True)

        var = "upov_density_max_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.density_max_ctl, q=True, value=True)

        var = "upov_density_pow_ctl"
        pm.optionVar[var] = pm.floatFieldGrp(
            self.density_pow_ctl, q=True, value1=True)
