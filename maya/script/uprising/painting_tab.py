import os.path
import pymel.core as pm
import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
import painting as pnt

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    ITEM_TYPE_TARGET,
    ITEM_TYPE_PROGRAM,
    ITEM_TYPE_TOOL,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_INSERT_CODE,
    INSTRUCTION_START_THREAD,
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)
import robodk as rdk

import paint
import brush
import stroke
import cluster
reload(paint)
reload(brush)
reload(stroke)
reload(cluster)


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

        # pm.button(
        #     label='Create permutations',
        #     command=pm.Callback(self.on_create_dip))

        pm.button(
            label='Create painting',
            command=pm.Callback(self.on_create_painting))

        pm.button(
            label='Create dip subroutines',
            command=pm.Callback(self.on_create_dip_only))

        pm.button(
            label='Show painting',
            command=pm.Callback(self.on_show_painting))

        pm.button(
            label='Test strokes',
            command=pm.Callback(self.on_test_strokes))

        # pm.button(
        #     label='Show dip',
        # command=pm.Callback(self.on_show_dip))

        pm.button(
            label='Add curve to factory',
            command=pm.Callback(sfu.add_curve_to_sf))

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

    # def on_create_dip(self):
    #     node = pm.PyNode("dipStrokeFactoryShape")
    #     dip = pnt.Painting(node)
    #     dip.create_program_permutations()

    def on_create_painting(self):
        RL.Render(False)
        painting_factory = pm.PyNode("paintingStrokeFactoryShape")
        dip_factory = pm.PyNode("dipStrokeFactoryShape")

        # main_program_name = "painting_program"
        # main_program = RL.AddProgram(main_program_name)

        painting = pnt.Painting(painting_factory)
        dip = pnt.Painting(dip_factory, True)
        
        painting.create_painting_program()

 

    def on_create_dip_only(self):
 
        dip_factory = pm.PyNode("dipStrokeFactoryShape")
        dip = pnt.Painting(dip_factory, True)
        dip.create_dip_subroutines()

 




        # dip = pnt.Painting(dip_node)

        # dip_programs = dip.create_program_permutations()
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
        sfu.setup_dip_factory(painting, dip)

    def on_test_strokes(self):
        node = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.test_strokes()

        # painting.send(dip)

    def on_show_painting(self):
        node = pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.show()

    # def on_setup_dip():
    #     node =pm.PyNode("paintingStrokeFactoryShape")

    # def on_show_dip(self):
    #     node =pm.PyNode("dipStrokeFactoryShape")
    #     painting = pnt.Painting(node)
    #     painting.show()
