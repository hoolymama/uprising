
import pymel.core as pm

import setup_dip
import curve_utils as cutl
import brush_utils as butl
import paint_utils as putl


import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
 
class SetupTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_butttons()
        self.create_action_buttons_and_layout()

    def create_butttons(self):
        pm.setParent(self.column)

        pm.frameLayout(collapsable=False, labelVisible=True, label="Set up from Sheets")
        pm.columnLayout(adj=True, rowSpacing=2 )
        pm.button(
            label='Setup board from spreadsheet',
            ann="Read board coordinates from spreadsheet and set the rig",
            command=pm.Callback(self.set_board_transform_from_sheet))

        pm.button(
            label='Setup brushes from spreadsheet',
            ann="Read brushes parameters from spreadsheet and generate dip and painting geometry",
            command=pm.Callback(self.create_brushes_from_sheet))


        pm.button(
            label='Setup paints from spreadsheet',
            ann="Read paint and rack parameters from spreadsheet. Create new shaders and positions",
            command=pm.Callback(self.setup_paints_from_sheet))


        pm.button(
            label='Setup all from spreadsheet',
            ann="Read and connect paints and brushes and board from the spreadsheet",
            command=pm.Callback(self.setup_all_from_spreadsheet))

        pm.setParent('..')
        pm.setParent('..')

        pm.frameLayout(collapsable=False, labelVisible=True, label="Dip curves")
        pm.columnLayout(adj=True)


        pm.rowLayout(numberOfColumns=2,
                     columnWidth2=(
                         (100), 100),
                     adjustableColumn=1,
                     columnAlign=(1, 'right'),
                     columnAttach=[(1, 'both', 2), (2, 'both', 2)])
        pm.button(
            label='Generate brush dip curves from default curves',
            ann="Add selected curves to selected painting node",
            command=pm.Callback(self.on_generate_brush_dip_curves))

        force_gen_bc  = 1
        self.force_gen_brush_curves_cb = pm.checkBox(
            label='Force',
            value=force_gen_bc,
            annotation='Force generate brush curves')
    
        pm.setParent('..')

        pm.button(
            label='Set up dip combination curves',
            ann="Create dip curves for all brush and paint combinations used in the painting. Typically do this after setting up brush dip curve shapes",
            command=pm.Callback(setup_dip.doit))

        pm.button(
            label='Visualize brush dip motions',
            ann="See how brushes will move when dipping in the trays",
            command=pm.Callback(self.visualize_brush_dips))

        pm.setParent('..')   
        pm.setParent('..')

    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel', enable=False)
        go_but = pm.button(label='Go', enable=False)

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

    def set_board_transform_from_sheet(self):
        painting_node = pm.PyNode("mainPaintingShape")
        sfu.set_board_from_sheet(painting_node)

    def create_brushes_from_sheet(self):
        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")
        butl.create_brush_geo_from_sheet(painting_node, dip_node)

    def setup_paints_from_sheet(self):
        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")
        putl.setup_paints_from_sheet(painting_node, dip_node)

    def setup_all_from_spreadsheet(self):
        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")
        butl.create_brush_geo_from_sheet(painting_node, dip_node)
        putl.setup_paints_from_sheet(painting_node, dip_node)
        sfu.set_board_from_sheet(painting_node)
   
    def add_curves_to_painting(self):
        node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
        if not node:
            raise IndexError("No painting node selected")
        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)
        for curve in curves:
            cutl.connect_curve_to_painting(curve, node, connect_to="next_available")

    def on_generate_brush_dip_curves(self):
        force = pm.checkBox(self.force_gen_brush_curves_cb, query=True, v=True)
        cutl.generate_brush_dip_curves(force )

    def visualize_brush_dips(self):
        painting_dip = pm.PyNode("mainPaintingShape")
        dip_dip = pm.PyNode("dipPaintingShape")
        
