import sys
import re
import pymel.core as pm

import curve_utils as cutl
import brush_utils as butl
import culling

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui

from robolink import (
    Robolink
)


class PaintingTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()

    def create_buttons(self):
        pm.setParent(self.column)
 
        
        pm.rowLayout(
            numberOfColumns=5, columnWidth5=(
                110, 60, 60, 60, 80), columnAlign=(
                1, 'right'), columnAttach=[
                (1, 'both', 2), (2, 'both', 2), (3, 'both', 2), (4, 'both', 2), (5, 'both', 2) ])

        pm.text(label='Propagate ramps')

 
        pm.button(
            label='Tilt',
            command=pm.Callback(self.on_propagate_tilt_ramp))

        pm.button(
            label='Bank',
            command=pm.Callback(self.on_propagate_bank_ramp))

        pm.button(
            label='Twist',
            command=pm.Callback(self.on_propagate_twist_ramp))

        flat_only = 1
        self.flat_only_cb = pm.checkBox(
            label='Flat only',
            value=flat_only,
            annotation='Only propagate flat brush ramps')

        pm.setParent('..')


        self.rename_inputs_tf = pm.textFieldButtonGrp(
            label='Rename curves',
            text='crv_%d',
            buttonLabel='Go',
            buttonCommand=pm.Callback(
                self.on_rename_inputs))



        # self.lift_length_ff = pm.floatSliderButtonGrp(
        #     label='Tip length to lift length',
        #     field=True,
        #     maxValue=5.0,
        #     step=0.01,
        #     value=1.1,
        #     symbolButtonDisplay=False,
        #     buttonLabel="Go",
        #     buttonCommand=pm.Callback(self.on_tip_length_to_lift_length),
        #     columnWidth=(4, 60)
        # )

        # self.lift_height_ff = pm.floatSliderButtonGrp(
        #     label='Tip length to lift height',
        #     field=True,
        #     maxValue=5.0,
        #     step=0.01,
        #     value=1.1,
        #     symbolButtonDisplay=False,
        #     buttonLabel="Go",
        #     buttonCommand=pm.Callback(self.on_tip_length_to_lift_height),
        #     columnWidth=(4, 60)
        # )

        # self.lift_bias_ff = pm.floatSliderButtonGrp(
        #     label='Tip length to lift bias',
        #     field=True,
        #     maxValue=5.0,
        #     step=0.01,
        #     value=1.1,
        #     symbolButtonDisplay=False,
        #     buttonLabel="Go",
        #     buttonCommand=pm.Callback(self.on_tip_length_to_lift_bias),
        #     columnWidth=(4, 60)
        # )

        # self.profile_height_max_ff = pm.floatSliderButtonGrp(
        #     label='Tip length to profile max',
        #     field=True,
        #     maxValue=5.0,
        #     step=0.01,
        #     value=1.0,
        #     symbolButtonDisplay=False,
        #     buttonLabel="Go",
        #     buttonCommand=pm.Callback(self.on_tip_length_to_profile_max),
        #     columnWidth=(4, 60)
        # )


    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(label='Go')

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

    def populate(self):
        pass
        # val = "default"
        # if "up_setup_palette_name" in pm.optionVar:
        #     val = pm.optionVar["up_setup_palette_name"]
        # print val
        # pm.textFieldGrp(self.setup_paints_tf, e=True, text=val)

    def save(self):
        pass
        # val = pm.textFieldButtonGrp(self.setup_paints_tf, q=True, text=True)
        # pm.optionVar["up_setup_palette_name"] = val

    # def on_propagate_profile_ramp(self):
    #     flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
    #     cutl.propagate_ramp_attribute(
    #         "strokeProfileRamp", "strokeProfileScale", flat)

    def on_propagate_tilt_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute("brushTiltRamp", "brushTiltRange", flat)

    def on_propagate_bank_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute("brushBankRamp", "brushBankRange", flat)

    def on_propagate_twist_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute(
            "brushTwistRamp", "brushTwistRange", flat)
 
    def on_rename_inputs(self):
        templates = [
            x.strip() for x in pm.textFieldButtonGrp(
                self.rename_inputs_tf,
                query=True,
                text=True).split(",")]
        if len(templates) == 2:
            crv_t, sc_t = templates
        elif len(templates) == 1:
            crv_t = "%s_crv" % templates[0]
            sc_t = "%s_stc" % templates[0]
        else:
            pm.error("Invalid templates")

        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True)

        cutl.do_rename_inputs(crv_t, sc_t, curves)
        # make sure templates are okay
    

    def on_connect_curve_vis_active(self):
        curves_xfs = pm.listRelatives(pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True), parent=True)
        print curves_xfs
        cutl.curve_vis_active_connection(curves_xfs, True)

    def on_disconnect_curve_vis_active(self):
        curves_xfs = pm.listRelatives(pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True), parent=True)
        cutl.curve_vis_active_connection(curves_xfs, False)


    def on_tip_length_to_attr(self, attr_name, mult=1, offset=0):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True)

        for curve in curves:
            stroke_curve=cutl.get_stroke_node(curve)
            attr = stroke_curve.attr(attr_name)
            butl.set_stroke_curve_att_from_brush_tip(attr, mult, offset)


    def on_tip_length_to_lift_length(self):
        mult = pm.floatSliderButtonGrp(self.lift_length_ff, query=True, value=True)
        self.on_tip_length_to_attr("liftLength", mult)
        

    def on_tip_length_to_lift_height(self):
        mult = pm.floatSliderButtonGrp(self.lift_height_ff, query=True, value=True)
        self.on_tip_length_to_attr("liftHeight", mult)
        
    def on_tip_length_to_lift_bias(self):
        mult = pm.floatSliderButtonGrp(self.lift_bias_ff, query=True, value=True)
        self.on_tip_length_to_attr("liftBias", mult)
        

    def on_tip_length_to_profile_max(self):
        mult = pm.floatSliderButtonGrp(self.profile_height_max_ff, query=True, value=True)
        self.on_tip_length_to_attr("strokeProfileScaleMax", mult)
        
