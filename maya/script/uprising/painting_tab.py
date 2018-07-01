import os.path
import pymel.core as pm

import setup_dip
import curve_utils as cutl
import brush_utils as butl
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
import uprising_util

reload(uprising_util)
reload(paint)
reload(brush)
reload(stroke)
reload(cluster)
reload(target)
reload(setup_dip)

reload(cutl)
reload(butl)
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
            label='Delete curve instances',
            command=pm.Callback(self.on_delete_curve_instances))

        pm.button(
            label='Set up dip stroke factory',
            command=pm.Callback(self.on_setup_dip))

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

 


    def on_send_brushes(self):
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        butl.send_brushes(painting_factory)

    def on_create_all(self):
        RL.Render(False)
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        dip_factory = pm.PyNode("dipStrokeFactoryShape")
       
        butl.send_brushes(painting_factory)

        dip = pnt.Painting(dip_factory, True)
        dip.create_dip_subroutines()

        painting = pnt.Painting(painting_factory)
        painting.create_painting_program()

        RL.Render(True)
 
    def on_create_painting(self):
        RL.Render(False)
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(painting_factory)
        painting.create_painting_program()
        RL.Render(True)

    def on_create_dip_only(self):
        RL.Render(False)
        dip_factory = pm.PyNode("dipStrokeFactoryShape")
        dip = pnt.Painting(dip_factory, True)
        dip.create_dip_subroutines()
        RL.Render(True)
 
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
        setup_dip_factory(painting, dip)

    def on_test_strokes(self):
        node = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.test_strokes()

    def on_show_painting(self):
        node = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.show()
