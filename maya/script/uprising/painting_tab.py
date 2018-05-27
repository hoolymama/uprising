import os.path
import pymel.core as pm
from bugTools import beetleColor
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


reload(pnt)

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
            label='Create permutations',
            command=pm.Callback(self.on_create_slosh))

        pm.button(
            label='Create painting',
            command=pm.Callback(self.on_create_painting))
        
        pm.button(
            label='Show painting',
            command=pm.Callback(self.on_show_painting))

        pm.button(
            label='Show slosh',
            command=pm.Callback(self.on_show_slosh))


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

    
    def on_create_slosh(self):
        node =pm.PyNode("sloshStrokeFactoryShape")
        slosh = pnt.Painting(node)
        slosh.create_program_permutations()



    def on_create_painting(self):
        node =pm.PyNode("paintingStrokeFactoryShape")

        # main_program_name = "painting_program"
        # main_program = RL.AddProgram(main_program_name)

        painting = pnt.Painting(node)
        painting.build_painting_program()
        # slosh = pnt.Painting(slosh_node)

        # slosh_programs = slosh.create_program_permutations()



     

    # def on_send(self):

    #     painting_node, slosh_node = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="strokeFactory")

    #     main_program_name = self.name
    #     main_program = RL.AddProgram(main_program_name)


    #     # painting = pnt.Painting(painting_node)
    #     slosh = pnt.Painting(slosh_node)

    #     slosh_programs = slosh.create_program_permutations()




        # painting.send(slosh)

    def on_show_painting(self):
        node =pm.PyNode("paintingStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.show()

    def on_show_slosh(self):
        node =pm.PyNode("sloshStrokeFactoryShape")
        painting = pnt.Painting(node)
        painting.show()

