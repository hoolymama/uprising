import os.path
import pymel.core as pm

import setup_dip
import curve_utils as cutl
import brush_utils as butl
import paint_utils as putl

from setup_dip import setup_dip_factory

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
import painting as pnt

from robolink import (
    Robolink
)
import robodk as rdk
import paint
import brush
import stroke
import cluster
import target
import write
import uprising_util
import const

reload(uprising_util)
reload(paint)
reload(brush)
reload(stroke)
reload(cluster)
reload(target)
reload(write)
reload(setup_dip)
reload(const)

reload(cutl)
reload(butl)
reload(putl)
reload(pnt)
reload(sfu)

RL = Robolink()


class PaintingTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_butttons()
        self.create_action_buttons()

    def create_butttons(self):
        pm.setParent(self.column)

        pm.button(
            label='Create Stroke Factory',
            command=pm.Callback(sfu.create_stroke_factory))
 
        pm.button(
            label='Create painting',
            command=pm.Callback(self.on_create_painting))

        pm.button(
            label='Create dip subroutines',
            command=pm.Callback(self.on_create_dip_only))

        pm.button(
            label='Send brushes',
            command=pm.Callback(self.on_send_brushes))

        pm.button(
            label='Create all',
            command=pm.Callback(self.on_create_all))

        pm.button(
            label='Add curve to factory',
            command=pm.Callback(cutl.add_curve_to_sf))

        pm.button(
            label='Update curve driver connections',
            command=pm.Callback(cutl.update_curve_sf))


        pm.button(
            label='Add brush to factory',
            command=pm.Callback(butl.add_brush_to_sf))

        pm.button(
            label='Add all paint trays to factory',
            command=pm.Callback(putl.add_all_trays_to_sf))

        pm.button(
            label='Send paint trays',
            command=pm.Callback(self.on_send_paint_trays))

        pm.button(
            label='Delete curve instances',
            command=pm.Callback(self.on_delete_curve_instances))

        pm.button(
            label='Set up dip stroke factory',
            command=pm.Callback(self.on_setup_dip))

        pm.button(
            label='Write program, station, simulation',
            command=pm.Callback(write.write_program))

        pm.button(
            label='Randomize paints assignments',
            command=pm.Callback(self.on_random_paints))

        pm.button(
            label='Randomize brush assignments',
            command=pm.Callback(self.on_random_brushes))

        pm.button(
            label='Export approach objects',
            command=pm.Callback(self.on_send_approach_targets))

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


    def on_send_paint_trays(self):
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        putl.send_paints(painting_factory)

    def on_random_paints(self):
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        cutl.assign_random_paints(painting_factory)
        
    def on_random_brushes(self):
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        cutl.assign_random_brushes(painting_factory)

    def on_send_brushes(self):
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        butl.send_brushes(painting_factory)

    def on_send_approach_targets(self):
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        sfu.send_aproach_objects(painting_factory)

    def on_create_all(self):
        RL.setWindowState(-1)
        RL.Render(False)
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        dip_factory = pm.PyNode("dipStrokeFactoryShape")
       
        butl.send_brushes(painting_factory)

        dip = pnt.Painting(dip_factory, True)
        dip.create_dip_subroutines()

        painting = pnt.Painting(painting_factory)
        painting.create_painting_program()
        RL.Render(True)
        RL.setWindowState(2)
 
    def on_create_painting(self):
        RL.setWindowState(-1)
        RL.Render(False)
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(painting_factory)
        painting.create_painting_program()
        RL.Render(True)
        RL.setWindowState(2)

    def on_create_dip_only(self):
        RL.setWindowState(-1)
        RL.Render(False)
        dip_factory = pm.PyNode("dipStrokeFactoryShape")
        dip = pnt.Painting(dip_factory, True)
        dip.create_dip_subroutines()
        RL.Render(True)
        RL.setWindowState(2)
 
    def on_delete_curve_instances(self):
        nodes = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="strokeFactory")
        for node in nodes:
            sfu.delete_curve_instances(node)

    def on_setup_dip(self):
        painting = pm.PyNode("paintingStrokeFactoryShape")
        dip = pm.PyNode("dipStrokeFactoryShape")
        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
        if not curves:
            raise TypeError("Need dip curves")
        setup_dip_factory(painting, dip, curves)

    def on_test_strokes(self):
        node = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.test_strokes()

    def on_show_painting(self):
        node = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.show()
