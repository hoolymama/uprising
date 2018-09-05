import sys
# import os.path
import pymel.core as pm

# import setup_dip
import curve_utils as cutl
import brush_utils as butl
# import paint_utils as putl

# from setup_dip import setup_dip_factory

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
from studio import Studio
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
            label='Add curves to painting',
            ann="Add selected curves to selected painting node",
            command=pm.Callback(self.add_curves_to_painting))


        pm.button(
            label='Create painting',
            command=pm.Callback(self.on_create_painting))

        # pm.button(
        #     label='Create dip subroutines',
        #     command=pm.Callback(self.on_create_dip_only))

        # pm.button(
        #     label='Send brushes',
        #     command=pm.Callback(self.on_send_brushes))

        # pm.button(
        #     label='Create all',
        #     command=pm.Callback(self.on_create_all))
        # pm.button(
        #     label='Setup from spreadsheet',
        #     ann="Read and connect paints and brushes and board from the spreadsheet",
        #     command=pm.Callback(self.setup_from_spreadsheet))

     

        # pm.rowLayout(numberOfColumns=2,
        #              columnWidth2=(
        #                  (100), 100),
        #              adjustableColumn=1,
        #              columnAlign=(1, 'right'),
        #              columnAttach=[(1, 'both', 2), (2, 'both', 2)])
        # pm.button(
        #     label='Generate brush dip curves from default curves',
        #     ann="Add selected curves to selected painting node",
        #     command=pm.Callback(self.on_generate_brush_dip_curves))

        # force_gen_bc  = 1
        # self.force_gen_brush_curves_cb = pm.checkBox(
        #     label='Force',
        #     value=force_gen_bc,
        #     annotation='Force generate brush curves')
    


        # pm.button(
        #     label='Update curve driver connections',
        #     command=pm.Callback(cutl.update_curve_sf))


        # pm.button(
        #     label='Add brush to factory',
        #     command=pm.Callback(butl.add_brush_to_painting))

        # pm.button(
        #     label='Add all paint trays to factory',
        #     command=pm.Callback(putl.add_all_trays_to_sf))

        # pm.button(
        #     label='Send paint trays',
        #     command=pm.Callback(self.on_send_paint_trays))

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
        #     label='Randomize paints assignments',
        #     command=pm.Callback(self.on_random_paints))

        # pm.button(
        #     label='Randomize brush assignments',
        #     command=pm.Callback(self.on_random_brushes))

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

 

 
    def add_curves_to_painting(self):
        node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
        if not node:
            raise IndexError("No painting node selected")
        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
        for curve in curves:
            cutl.connect_curve_to_painting(curve, node, connect_to="next_available")



    def on_generate_brush_dip_curves(self):

        force = pm.checkBox(self.force_gen_brush_curves_cb, query=True, v=True)

        dip_painting_node = pm.PyNode("dipPaintingShape")
        if not dip_painting_node:
            raise IndexError("Can't find dipPaintingShape")
        dip_brushes = pm.ls("brushes|dipBrushes|bx*", selection=True, dag=True, leaf=True)
        if not  dip_brushes:
            dip_brushes = pm.ls("brushes|dipBrushes|bx*", dag=True, leaf=True)
 
        cutl.generate_brush_dip_curves(dip_painting_node, dip_brushes, force )



    # def on_setup_dip(self):
    #     painting = pm.PyNode("mainPaintingShape")
    #     dip = pm.PyNode("dipPaintingShape")
    #     dip_curves_grp = pm.PyNode("brushes|dipCurves")
    #     setup_dip_factory(painting, dip, dip_curves_grp)



    # def on_send_paint_trays(self):
    #     painting_factory = pm.PyNode("dipPaintingShape")
    #     putl.send_paints(painting_factory)

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
        RL = Robolink()
        RL.setWindowState(-1)
        RL.Render(False)
        
        # pnt.Dip(pm.PyNode("dipPaintingShape")).write()
        # pnt.Painting(pm.PyNode("mainPaintingShape")).write()

        RL.Render(True)
        RL.setWindowState(2)
 
    def on_create_painting(self):
        RL = Robolink()
        RL.setWindowState(-1)
        try:
            painting_node = pm.PyNode("mainPaintingShape")
            dip_node = pm.PyNode("dipPaintingShape")
            studio = Studio(painting_node, dip_node)
            studio.write()
        except Exception:
            t, v, tb = sys.exc_info()
            RL.setWindowState(2)
            raise t, v, tb

        RL.setWindowState(2)


        # painting = pnt.Painting(painting_node)
        # painting.create_painting_program()
        # RL.Render(True)

    # def on_create_dip_only(self):
    #     RL = Robolink()
    #     RL.setWindowState(-1)
    #     RL.Render(False)
    #     dip_factory = pm.PyNode("dipStrokeFactoryShape")
    #     dip = pnt.Painting(dip_factory, True)
    #     dip.create_dip_subroutines()
    #     RL.Render(True)
    #     RL.setWindowState(2)
 
    def on_delete_curve_instances(self):
        nodes = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="strokeFactory")
        for node in nodes:
            sfu.delete_curve_instances(node)



    # def on_test_strokes(self):
    #     node = pm.PyNode("paintingStrokeFactoryShape")
    #     painting = pnt.Painting(node)
    #     painting.test_strokes()

    # def on_show_painting(self):
    #     node = pm.PyNode("paintingStrokeFactoryShape")
    #     painting = pnt.Painting(node)
    #     painting.show()
