import sys
# import os.path
import pymel.core as pm

# import setup_dip
import curve_utils as cutl
# import brush_utils as butl
import culling


# import paint_utils as putl

# from setup_dip import setup_dip_factory

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
# from studio import Studio
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

        pm.button(
            label='Select strokes from curves',
            ann="Choose curves to select connected strokeCurves",
            command=pm.Callback(self.on_select_strokes))


        pm.button(
            label='Duplicate curves with strokes',
            ann="Duplicate selected curves and add to selected painting node",
            command=pm.Callback(self.duplicate_curves_to_painting))

        pm.button(
            label='Add curves to painting in order',
            ann="Add selected curves to selected painting node. If they are already connected, disconnect and add in the order they are selected",
            command=pm.Callback(self.add_curves_to_painting))

        pm.button(
            label='Reverse order of selected curves',
            command=pm.Callback(self.reverse_connection_order))


        pm.rowLayout(numberOfColumns=2,
                     columnWidth2=(
                         (100), 100),
                     adjustableColumn=1,
                     columnAlign=(1, 'right'),
                     columnAttach=[(1, 'both', 2), (2, 'both', 2)])
        pm.button(
            label='Remove curves from painting',
            ann="Break selected curve connections",
            command=pm.Callback(self.on_remove_curve_instances))

        delete_curves = 1
        self.delete_curves_cb = pm.checkBox(
            label='Force',
            value=delete_curves,
            annotation='Also delete curves')

        pm.setParent('..')

        pm.button(
            label='Remove unconnected strokeCurves',
            ann="Delete strokeCurves that are connected to selected curves but which have no destination painting",
            command=pm.Callback(
                self.on_remove_hanging_stroke_curves))

        pm.rowLayout(
            numberOfColumns=6, columnWidth6=(
                110, 60, 60, 60, 60, 80), columnAlign=(
                1, 'right'), columnAttach=[
                (1, 'both', 2), (2, 'both', 2), (3, 'both', 2), (4, 'both', 2), (5, 'both', 2), (6, 'both', 2)])


        pm.text(label='Propagate ramps')

        pm.button(
            label='Profile',
            command=pm.Callback(self.on_propagate_profile_ramp))

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

        pm.button(
            label='Contain strokes to mesh',
            ann="Choose a mesh and either some curves or strokeCurves",
            command=pm.Callback(self.on_contain_strokes_in_mesh))




        # pm.rowLayout(numberOfColumns=3,
        #              columnWidth3=(120, 80, 80),
        #              adjustableColumn=1,
        #              columnAlign=(1, 'right'),
        # columnAttach=[(1, 'both', 2), (2, 'both', 2), (3, 'both', 2)])

        # self.paint_brush_field =  pm.intFieldGrp(numberOfFields=2, label="Brush / Paint")
        # pm.button(
        #     label='Cull before' ,
        #     command=pm.Callback(self.on_cull_before))

        # pm.button(
        #     label='Cull after' ,
        #     command=pm.Callback(self.on_cull_after))

        # pm.button(
        #     label='Create dip subroutines',
        #     command=pm.Callback(self.on_create_dip_only))

        # pm.button(
        #     label='Send brushes',
        #     command=pm.Callback(self.on_send_brushes))

        # pm.button(
        #     label='Delete curve instances',
        #     command=pm.Callback(self.on_delete_curve_instances))

        # pm.button(
        #     label='Set up dip stroke factory',
        #     command=pm.Callback(self.on_setup_dip))

        # pm.button(
        #     label='Write program, station, simulation',
        #     command=pm.Callback(write.write_program))

        # pm.button(
        #     label='Export approach objects',
        #     command=pm.Callback(self.on_send_approach_targets))

        # pm.button(
        #     label='Set board transform from spreadsheet',
        #     command=pm.Callback(self.set_board_transform_from_sheet))

    def create_action_buttons(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel')
        go_but = pm.button(label='Go')

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


    def on_select_strokes(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        pm.select(pm.listHistory(curves, future=True,levels=1, type="strokeCurve"))

    def add_curves_to_painting(self):
        node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
        if not node:
            raise IndexError("No painting node selected")
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type=("nurbsCurve", "strokeCurve"),
            ni=True,
            ut=True,
            v=True)

        stroke_curves = pm.listConnections(
            curves, d=True, 
            s=False, 
            type="strokeCurve")
    
        painting_conns = pm.listConnections(
            stroke_curves, d=True, 
            s=False, 
            type="painting",
            c=True, p=True)
        for conn in painting_conns:
            conn[0] // conn[1]

        for curve in curves:
            cutl.connect_curve_to_painting(
                curve, node, connect_to="next_available")


    def reverse_connection_order(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type=("nurbsCurve", "strokeCurve"),
            ni=True,
            ut=True,
            v=True)

        stroke_curves = pm.listConnections(
            curves, d=True, 
            s=False, 
            type="strokeCurve")

        painting_conns = pm.listConnections(
            stroke_curves, d=True, 
            s=False, 
            type="painting",
            c=True, p=True)

        for conn in painting_conns:
            conn[0] // conn[1]

        c1, c2 = zip(*painting_conns)
        painting_conns = zip(list(c1),reversed(list(c2)))

        for conn in painting_conns:
            conn[0] >> conn[1]



    def duplicate_curves_to_painting(self):
        node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
        if not node:
            raise IndexError("No painting node selected")
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        for curve in curves:
            cutl.duplicate_curve_to_painting(curve, node)

    def on_remove_curve_instances(self):
        del_crvs = pm.checkBox(self.delete_curves_cb, query=True, v=True)

        node = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="painting")[0]

        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        if not curves:
            curves = pm.listHistory(
                pm.PyNode("mainPaintingShape.strokeCurves"),
                type="nurbsCurve")
        sfu.delete_curve_instances(node, curves, del_crvs)

    def on_remove_hanging_stroke_curves(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        stroke_curves = pm.listConnections(
            curves, d=True, s=False, type="strokeCurve")
        for sc in stroke_curves:
            paintings = pm.listConnections(
                sc.attr("output"), d=True, s=False, type="painting")
            if not paintings:
                pm.delete(sc)

    def on_propagate_profile_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute(
            "strokeProfileRamp", "strokeProfileScale",flat)

    def on_propagate_tilt_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute("brushTiltRamp", "brushTiltRange",flat)

    def on_propagate_bank_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute("brushBankRamp", "brushBankRange",flat)

    def on_propagate_twist_ramp(self):
        flat = pm.checkBox(self.flat_only_cb, query=True, v=True)
        cutl.propagate_ramp_attribute("brushTwistRamp", "brushTwistRange",flat)

    def on_contain_strokes_in_mesh(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)

        meshes = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="mesh",
            ni=True,
            ut=True,
            v=True)

        cutl.contain_strokes_in_mesh(curves, meshes[0])

 