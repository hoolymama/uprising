
# import os.path
import pymel.core as pm
import write
import props
import uprising_util as uutl
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

    def create_ui(self):
        pm.setParent(self.column)

        self.send_paintings_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            height=30,
            label='Send Clusters',
            labelArray2=['Painting', 'Dips'],
            valueArray2=[True, True],
            columnWidth3=(180, 120, 120))

        # self.send_board_cb = pm.checkBoxGrp(
        #     label='Send Board',
        #     value1=1,
        #     height=30,
        #     columnWidth2=(
        #         180,
        #         120))

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
        send_painting = pm.checkBoxGrp(
            self.send_paintings_cb, query=True, value1=True)
        send_dips = pm.checkBoxGrp(
            self.send_paintings_cb, query=True, value2=True)
        # send_board = pm.checkBoxGrp(
        #     self.send_board_cb, query=True, value1=True)

        painting_node = pm.PyNode(
            "mainPaintingShape") if send_painting else None
        dip_node = pm.PyNode("dipPaintingShape") if send_dips else None

        with uutl.minimize_robodk():
            studio = Studio(painting_node, dip_node)
            studio.write()
 