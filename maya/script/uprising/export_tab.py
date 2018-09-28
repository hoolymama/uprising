import sys
# import os.path
import pymel.core as pm
import write

# import setup_dip
# import curve_utils as cutl
# import brush_utils as butl
# import paint_utils as putl

import setup_dip
# import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
from studio import Studio
from robolink import (
    Robolink
)


class ExportTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_ui()
        self.create_action_buttons()

    def create_ui(self):
        pm.setParent(self.column)

        pm.rowLayout(
            numberOfColumns=4, columnWidth4=(
                100, 80, 80, 80), adjustableColumn=1, columnAlign=(
                1, 'right'), columnAttach=[
                (1, 'both', 2), (2, 'both', 2), (3, 'both', 2), (4, 'both', 2)])

        pm.text(label='Export to RoboDK')

        pm.button(
            label='Painting only',
            command=pm.Callback(self.on_create_painting))

        pm.button(
            label='Dips only',
            command=pm.Callback(self.on_create_dips))

        pm.button(
            label='All',
            command=pm.Callback(self.on_create_all))

        pm.setParent('..')

        self.description_ff = pm.scrollField(
            wordWrap=True, text="Description...")
        pm.button(
            label='Write program package',
            command=pm.Callback(self.write_program_package))

        pm.button(
            label='Write maya package only',
            command=pm.Callback(self.write_maya_package))


        pm.rowLayout(
            numberOfColumns=2, columnWidth2=(
                150, 200), adjustableColumn=1, columnAlign=(
                1, 'right'), columnAttach=[
                (1, 'both', 2), (2, 'both', 2)])

        self.frame_if = pm.intFieldGrp(label="Frames to run", numberOfFields=2, value1=1, value2=1 )
    
        pm.button(
            label='Export and write packages',
            command=pm.Callback(self.on_export_and_write_series))


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

    def write(self, painting_node, dip_node):
        RL = Robolink()
        RL.setWindowState(-1)
        try:
            studio = Studio(painting_node, dip_node)
            studio.write()
        except Exception:
            t, v, tb = sys.exc_info()
            RL.setWindowState(2)
            raise t, v, tb
        RL.setWindowState(2)

    def on_create_painting(self):
        painting_node = pm.PyNode("mainPaintingShape")
        self.write(painting_node, None)

    def on_create_dips(self):
        dip_node = pm.PyNode("dipPaintingShape")
        self.write(None, dip_node)

    def on_create_all(self):
        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")
        self.write(painting_node, dip_node)

    def write_program_package(self):
        desc = pm.scrollField(self.description_ff, q=True, text=True)
        export_dir = write.choose_export_dir()
        if export_dir:
            write.export_package(export_dir, desc)

    def write_maya_package(self):
        desc = pm.scrollField(self.description_ff, q=True, text=True)
        export_dir = write.choose_export_dir()
        if export_dir:
            write.export_maya_package_only(export_dir, desc)

    def on_export_and_write_series(self):
        start_frame =  pm.intFieldGrp(self.frame_if, query=True, value1=True)
        end_frame =  pm.intFieldGrp(self.frame_if, query=True, value2=True)
        print "%d %d" % (start_frame, end_frame)

        export_dir = write.choose_export_dir()
        if export_dir:
            desc = pm.scrollField(self.description_ff, q=True, text=True)
            painting_node = pm.PyNode("mainPaintingShape")
            dip_node = pm.PyNode("dipPaintingShape")
            for f in range(start_frame, end_frame+1):
                pm.currentTime(f)
                setup_dip.doit()
                self.write(painting_node, dip_node)
                write.export_package(export_dir, desc, f)




