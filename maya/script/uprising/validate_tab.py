
# import os.path
import pymel.core as pm
import random
from robolink import (
    Robolink,
    ITEM_TYPE_STATION,
    ITEM_TYPE_PROGRAM,
    COLLISION_OFF)
import write
import props
import uprising_util as uutl
import curve_utils as cutl
from studio import Studio

import pymel.core.uitypes as gui


class ValidateTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_ui()
        self.create_action_buttons()

    def on_collapse_change(self):
        state = pm.frameLayout(
            self.retries_frame_wg,
            query=True,
            collapse=True)
        pm.frameLayout(self.simple_frame_wg, edit=True, en=state)

    # def on_step_type_change(self):
    #     step_type = pm.radioButtonGrp(
    #         self.varying_step_type_wg, query=True, sl=True)
    #     pm.floatSliderGrp(
    #         self.varying_step_wg, edit=True, en=(
    #             step_type is not 1))

    def create_ui(self):
        pm.setParent(self.column)

        self.simple_frame_wg = pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            labelVisible=True,
            label="Simple validation"
        )

        self.send_paintings_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            height=30,
            label='Send clusters for',
            labelArray2=['Painting', 'Dips'],
            valueArray2=[True, False],
            columnWidth3=(180, 120, 120)
        )

        self.send_selected_props_cb = pm.checkBoxGrp(
            label='Send selected props',
            value1=0,
            height=30,
            columnWidth2=(
                180,
                120))

        pm.setParent("..")

        self.retries_frame_wg = pm.frameLayout(
            collapsable=True,
            collapse=True,
            en=True,
            labelVisible=True,
            label="Retries",
            ec=pm.Callback(self.on_collapse_change),
            cc=pm.Callback(self.on_collapse_change)
        )

        self.num_retries_wg = pm.intSliderGrp(
            field=True,
            label='Number of attempts',
            minValue=2,
            maxValue=100,
            fieldMinValue=2,
            fieldMaxValue=100,
            value=3)

        pm.columnLayout(adj=True)
        self.varying_node_wg = pm.radioButtonGrp(
            label='Varying node', labelArray3=[
                 "Selected", "Shape", "StrokeCurve"], sl=1, numberOfRadioButtons=3)
        self.varying_attrib_wg = pm.textFieldGrp(label='Attribute', text='rz')
        self.varying_range_wg = pm.floatFieldGrp(
            numberOfFields=2, label='Range', value1=0, value2=1.0)

        self.varying_step_type_wg = pm.radioButtonGrp(
            label='Step type',
            sl=1,
            labelArray2=[
                'Random',
                'Linear'],
            numberOfRadioButtons=2
        )

        pm.setParent("..")
        pm.setParent("..")
        pm.setParent("..")

        self.initialize_ui()

    def initialize_ui(self):
        self.on_collapse_change()
 

    def create_action_buttons(self):
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

    def on_go(self):

        do_simple = pm.frameLayout(self.simple_frame_wg, query=True, en=True)
        if do_simple:
            self.do_simple_validation()
        else:
            self.do_retries_validation()

    def validate_path(self, program):
        RL = Robolink()
        update_result = program.Update(COLLISION_OFF)
        return {
            "instructions": update_result[0],
            "time": update_result[1],
            "distance": update_result[2],
            "completion":  update_result[3],
            "problems": update_result[4],
            "status": "SUCCESS" if (update_result[3] == 1.0) else "FAILURE"
        }

    def format_path_result(self, path_info, metadata):
        text = """
        ------------------------------------------
        instructions  : %d
        time          : %s
        distance      : %s
        completion    : %.3f
        problems      : %s
        status        : %s
        """ % (path_info["instructions"],
               path_info["time"],
               path_info["distance"],
               path_info["completion"],
               path_info["problems"],
               path_info["status"])

        if metadata:
            text += "Iter:%i Attr:%s = %s" % (metadata["iteration"],metadata["attribute"], metadata["value"])
        return text






    def do_simple_validation(self):
        send_painting = pm.checkBoxGrp(
            self.send_paintings_cb, query=True, value1=True)
        send_dips = pm.checkBoxGrp(
            self.send_paintings_cb, query=True, value2=True)
        send_selected_props = pm.checkBoxGrp(
            self.send_selected_props_cb, query=True, value1=True)

        painting_node = pm.PyNode(
            "mainPaintingShape") if send_painting else None
        dip_node = pm.PyNode("dipPaintingShape") if send_dips else None

        if painting_node or dip_node:
            with uutl.minimize_robodk():
                studio = Studio(painting_node, dip_node)
                studio.write()

        if painting_node:
            path_result = self.validate_path(studio.painting_program)
            msg = self.format_path_result(path_result, {})
            pm.confirmDialog(
                title='Path validation result',
                message=msg,
                button=['OK'],
                defaultButton='OK',
                cancelButton='OK',
                dismissString='OK')

        if send_selected_props:
            props.send(pm.ls(selection=True))

    def hide_objects(self, obs):
        for o in obs:
            if o.type() == "strokeCurve":
                xf = cutl.get_curve(o).getParent()
            elif  o.type() == "nurbsCurve":
                xf = o.getParent()
            elif o.type() == "transform":
                xf = o
            else:
                pm.warning("Cant get transform")
                return
            xf.attr("visibility").set(0)


    def show_objects(self, obs):
        for o in obs:
            if o.type() == "strokeCurve":
                xf = cutl.get_curve(o).getParent()
            elif  o.type() == "nurbsCurve":
                xf = o.getParent()
            elif o.type() == "transform":
                xf = o
            else:
                pm.warning("Cant get transform")
                return
            xf.attr("visibility").set(1)


    def do_retries_validation(self):
        """Repetitively retry with different values until success.

        Typically there will be just one thing selected.
        """
        count =  pm.intSliderGrp(self.num_retries_wg, query=True, value=True)
        node_type_idx = pm.radioButtonGrp(self.varying_node_wg, query=True, sl=True)
        node_type = ["dummy", "selected", "shape", "stroke_curve"][node_type_idx]
        attribute = pm.textFieldGrp(self.varying_attrib_wg, query=True, text=True)
        low = pm.floatFieldGrp(
                self.varying_range_wg,
                query=True,
                value1=True)
        high = pm.floatFieldGrp(
                self.varying_range_wg,
                query=True,
                value2=True)
        step_type_idx = pm.radioButtonGrp(self.varying_step_type_wg, query=True, sl=True)
        step_type = ["dummy", "random", "linear"][step_type_idx]

        if node_type is "selected":
            obs = pm.ls(selection=True)
        elif node_type is "shape": 
            obs = pm.ls(selection=True, dag=True,  ni=True,  shapes=True)
        else:  # stroke_curve
            obs =  pm.ls(selection=True, type="strokeCurve")

            curves = pm.ls(
                selection=True, dag=True,
                ni=True,
                shapes=True,
                type="nurbsCurve")
            conns = pm.listConnections(
                curves,
                d=True,
                s=False,
                type="strokeCurve")
            obs += conns

        self.hide_objects(obs)

        results = []
        for o in obs:
            self.show_objects([o])
            attempts = self.do_retries_for_object(o,attribute,  step_type, low, high, count)
            status = "SUCCESS" if (attempts > -1) else "FAILURE"
            results.append({"node": o, "status": status, "attempts": attempts})
            self.hide_objects([o])
        print results
        self.show_objects(obs)

        # if step_type is "random":
        #     vals = [random.uniform(low, high) for x in range(count)]
        # else:
        #     valrange = high - low
        #     gap = valrange / (count-1)
        #     vals = [low+(gap*x) for x in range(count)]

        


        # painting_node = pm.PyNode("mainPaintingShape")
        # msg = ""
        # for i, val in enumerate(vals):
        #     self.setParams(obs, attribute, val)
        #     with uutl.minimize_robodk():
        #         studio = Studio(painting_node, None)
        #         studio.write()

        #     path_result = self.validate_path(studio.painting_program)

        #     metadata = {
        #         "iteration": i,
        #         "attribute": attribute,
        #         "value": val
        #     }
        #     msg += self.format_path_result(path_result, metadata)
        #     if path_result["completion"] == 1.0:
        #         break
        # print msg
            



        # painting_node = pm.PyNode("mainPaintingShape")

        # result text = 

        # curves = pm.ls(
        #     selection=True, dag=True,
        #     ni=True,
        #     shapes=True,
        #     type="nurbsCurve")

        # with uutl.minimize_robodk():
        #     studio = Studio(painting_node, None)
        #     studio.write()
    def do_retries_for_object(self, o, attribute, step_type, low, high, count):
        result = -1
        if step_type is "random":
            vals = [random.uniform(low, high) for x in range(count)]
        else:
            valrange = high - low
            gap = valrange / (count-1)
            vals = [low+(gap*x) for x in range(count)]

        painting_node = pm.PyNode("mainPaintingShape")
        msg = ""
        for i, val in enumerate(vals):
            self.setParams([o], attribute, val)
            with uutl.minimize_robodk():
                studio = Studio(painting_node, None)
                studio.write()

            path_result = self.validate_path(studio.painting_program)

            metadata = {
                "iteration": i,
                "attribute": attribute,
                "value": val
            }
            msg += self.format_path_result(path_result, metadata)
            if path_result["completion"] == 1.0:
                result =i+1
                break
        print msg
        return result
            



    def setParams(self, obs, attribute, val):
        for o in obs:
            try:
                att = o.attr(attribute).set(val)
            except (pm.MayaAttributeError, RuntimeError) as e:
                pm.warning("Error with : %s.%s - Skipping"  % (o , attribute) )


        # print "-" * 22
        # print obs
        # print attribute
        # print val           








