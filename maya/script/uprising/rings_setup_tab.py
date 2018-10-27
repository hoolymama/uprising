import sys
# import os.path
import pymel.core as pm

# import setup_dip
import curve_utils as cutl
# import brush_utils as butl
import uprising_util as uutl
# from contextlib import contextmanager

# from setup_dip import setup_dip_factory

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
from studio import Studio
from robolink import (
    Robolink
)


class RingsSetupTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()

    def create_buttons(self):
        pm.setParent(self.column)

        # pm.button(
        #     label='Randomize selected curve rotations',
        #     command=pm.Callback(self.on_random_rotations))

        # pm.button(
        #     label='Duplicate into gaps',
        #     command=pm.Callback(self.on_duplicate_into_gaps))

        # self.subcurvemin_ctl  = pm.checkBoxGrp(label="Lift over curve", value1=False)
        with uutl.activatable(state=True):
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
            pm.rowLayout(numberOfColumns=2,
                 columnWidth2=(350, 150),
                 columnAlign=(1, 'right'),
                 columnAttach=[(1, 'both', 2), (2, 'both', 2)])
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

        cancel_but = pm.button(label='Cancel')
        go_but = pm.button(
            label='Go', command=pm.Callback(
                self.on_auto_stroke_setup))

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

    def on_auto_stroke_setup(self):
        # subcurve_min_lift = pm.checkBoxGrp(self.subcurve_ctl, q=True, value1=True)

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
                "min_strokes": pm.intSliderGrp( self.min_strokes_ctl, q=True, v=True), 
                "max_length": pm.floatSliderGrp(self.max_length_ctl, q=True, v=True), 
                "overlap": pm.floatSliderGrp(self.overlap_ctl, q=True, v=True)
            }
        if do_lift:
            lift = pm.floatFieldGrp(self.lift_ctl, q=True, v=True)
        if do_twist:
            twist = pm.floatFieldGrp(self.twist_ctl, q=True, v=True)
            flat_only =  pm.checkBox(self.flat_only_cb, q=True, v=True)
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
            dmax= pm.floatFieldGrp(self.density_max_ctl, q=True, v=True)
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
