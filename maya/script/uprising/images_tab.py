
import os
import pymel.core as pm
import images
import curve_utils as cutl
import brush_utils as butl
import paint_utils as putl


import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
 
class ImagesTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_butttons()
        self.create_action_buttons_and_layout()

    def create_butttons(self):
        pm.setParent(self.column)

        pm.button(
            label='Write palette CSV',
            ann="Write CSV file containing palette for a PNG file",
            command=pm.Callback(self.write_png_palette_csv))


    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel', manage=False)
        go_but = pm.button(label='Go', manage=False)

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

    def write_png_palette_csv(self):
        nodes = pm.ls(selection=True, type="cImgPngFile")
        if len(nodes):
            fn = pm.PyNode("cImgPngFile1").attr("imageFilename").get()
        else:
            images_dir = os.path.join(pm.workspace.getPath(), 'sourceimages')
            entries = pm.fileDialog2(caption="Choose PNG file", okCaption="Open",
                            fileFilter="*.png",  dialogStyle=2, fileMode=1, dir=images_dir)
            if not entries:
                pm.displayWarning('Nothing Selected')
                return
            else:
                fn = entries[0]
        print fn
        images.png_palette_for_sheets(fn)




    #     painting_node = pm.PyNode("mainPaintingShape")
    #     sfu.set_board_from_sheet(painting_node)

    # def create_brushes_from_sheet(self):
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     dip_node = pm.PyNode("dipPaintingShape")
    #     butl.create_brush_geo_from_sheet(painting_node, dip_node)

    # def setup_paints_from_sheet(self):
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     dip_node = pm.PyNode("dipPaintingShape")
    #     putl.setup_paints_from_sheet(painting_node, dip_node)

    # def setup_rack_from_sheet(self):
    #     dip_node = pm.PyNode("dipPaintingShape")
    #     putl.setup_rack_from_sheet(dip_node)


    # def setup_all_from_spreadsheet(self):
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     dip_node = pm.PyNode("dipPaintingShape")
    #     butl.create_brush_geo_from_sheet(painting_node, dip_node)
    #     putl.setup_paints_from_sheet(painting_node, dip_node)
    #     putl.setup_rack_from_sheet(dip_node)
        
    #     sfu.set_board_from_sheet(painting_node)
   
    # def add_curves_to_painting(self):
    #     node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
    #     if not node:
    #         raise IndexError("No painting node selected")
    #     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True)
    #     for curve in curves:
    #         cutl.connect_curve_to_painting(curve, node, connect_to="next_available")

    # def on_generate_brush_dip_curves(self):
    #     lift = pm.floatSliderButtonGrp(self.gen_dip_curves_ff, query=True, value=True)
    #     force = pm.checkBox(self.force_gen_brush_curves_cb, query=True, v=True)
    #     cutl.generate_brush_dip_curves(lift, force )

    # def visualize_brush_dips(self):
    #     painting_dip = pm.PyNode("mainPaintingShape")
    #     dip_dip = pm.PyNode("dipPaintingShape")
        
