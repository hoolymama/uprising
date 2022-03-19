 
from uprising import brush_utils as butl
from uprising import curve_utils as cutl
import pymel.core as pm
import pymel.core.uitypes as gui


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

        pm.button(
            label='Contact',
            command=pm.Callback(self.on_propagate_contact_ramp))

        pm.setParent('..')


        self.rename_inputs_tf = pm.textFieldButtonGrp(
            label='Rename curves',
            text='crv_%d',
            buttonLabel='Go',
            buttonCommand=pm.Callback(
                self.on_rename_inputs))

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save',en=False, command=pm.Callback(self.save))
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


    def save(self):
        pass


    def on_propagate_tilt_ramp(self):
        cutl.propagate_ramp_attribute("brushTiltRamp", "brushTiltRange")

    def on_propagate_bank_ramp(self):
        cutl.propagate_ramp_attribute("brushBankRamp", "brushBankRange")

    def on_propagate_twist_ramp(self):
        cutl.propagate_ramp_attribute(
            "brushTwistRamp", "brushTwistRange")

    def on_propagate_contact_ramp(self):
        cutl.propagate_ramp_attribute(
            "contactRamp", "contactRampResolution")
 
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
