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
            label='Add curves to painting',
            ann="Add selected curves to selected painting node",
            command=pm.Callback(self.add_curves_to_painting))


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

        delete_curves  = 1
        self.delete_curves_cb = pm.checkBox(
            label='Force',
            value=delete_curves,
            annotation='Also delete curves')
    
        pm.setParent('..')





        pm.button(
            label='Remove unconnected strokeCurves' ,
            ann="Delete strokeCurves that are connected to selected curves but which have no destination painting",
            command=pm.Callback(self.on_remove_hanging_stroke_curves))

 

        pm.rowLayout(numberOfColumns=5,
                     columnWidth5=(100, 80, 80, 80, 80),
                     adjustableColumn=1,
                     columnAlign=(1, 'right'),
                     columnAttach=[(1, 'both', 2), (2, 'both', 2), (3, 'both', 2), (4, 'both', 2), (5, 'both', 2)])
        
        pm.text( label='Propagate ramps' )

        pm.button(
            label='Profile' ,
            command=pm.Callback(self.on_propagate_profile_ramp))
    
        pm.button(
            label='Tilt' ,
            command=pm.Callback(self.on_propagate_tilt_ramp))
    
        pm.button(
            label='Bank' ,
            command=pm.Callback(self.on_propagate_bank_ramp))
    
        pm.button(
            label='Twist' ,
            command=pm.Callback(self.on_propagate_twist_ramp))
    

        pm.setParent('..')


        pm.rowLayout(numberOfColumns=3,
                     columnWidth3=(120, 80, 80),
                     adjustableColumn=1,
                     columnAlign=(1, 'right'),
                     columnAttach=[(1, 'both', 2), (2, 'both', 2), (3, 'both', 2)])

        self.paint_brush_field =  pm.intFieldGrp(numberOfFields=2, label="Brush / Paint")
        pm.button(
            label='Cull before' ,
            command=pm.Callback(self.on_cull_before))
    
        pm.button(
            label='Cull after' ,
            command=pm.Callback(self.on_cull_after))

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

    def add_curves_to_painting(self):
        node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
        if not node:
            raise IndexError("No painting node selected")
        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True )
        for curve in curves:
            cutl.connect_curve_to_painting(
                curve, node, connect_to="next_available")








    def on_remove_curve_instances(self):
        del_crvs = pm.checkBox(self.delete_curves_cb, query=True, v=True)


        node = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="painting")[0]

        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)
        if not curves:
            curves = pm.listHistory(pm.PyNode("mainPaintingShape.strokeCurves"), type="nurbsCurve", ni=True, ut=True, v=True)
        sfu.delete_curve_instances(node, curves, del_crvs)


    def on_remove_hanging_stroke_curves(self):
        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)
        stroke_curves = pm.listConnections(curves, d=True, s=False, type="strokeCurve")
        for sc in stroke_curves:
            paintings =  pm.listConnections(sc.attr("output"), d=True, s=False, type="painting")
            if not paintings:
                pm.delete(sc)

    

    def on_propagate_profile_ramp(self):
        cutl.propagate_ramp_attribute("strokeProfileRamp","strokeProfileScale")

    def on_propagate_tilt_ramp(self):
        cutl.propagate_ramp_attribute("brushTiltRamp","brushTiltRange")

    def on_propagate_bank_ramp(self):
        cutl.propagate_ramp_attribute("brushBankRamp","brushBankRange")

    def on_propagate_twist_ramp(self):
        cutl.propagate_ramp_attribute("brushTwistRamp","brushTwistRange")



    def on_cull(self, after=False):
        node = pm.ls(selection=True, dag=True, leaf=True, type="painting")[0]
        brush_id = pm.intFieldGrp(self.paint_brush_field, q=True, value1=True) 
        paint_id = pm.intFieldGrp(self.paint_brush_field, q=True, value2=True) 
        culling.cull(node, brush_id, paint_id, after)

    def on_cull_before(self):
        self.on_cull(False)

    def on_cull_after(self):
        self.on_cull(True)

    # def on_generate_brush_dip_curves(self):

    #     force = pm.checkBox(self.force_gen_brush_curves_cb, query=True, v=True)

    #     dip_painting_node = pm.PyNode("dipPaintingShape")
    #     if not dip_painting_node:
    #         raise IndexError("Can't find dipPaintingShape")
    #     dip_brushes = pm.ls("brushes|dipBrushes|bx*", selection=True, dag=True, leaf=True)
    #     if not  dip_brushes:
    #         dip_brushes = pm.ls("brushes|dipBrushes|bx*", dag=True, leaf=True)

    #     cutl.generate_brush_dip_curves(dip_painting_node, dip_brushes, force )

    # def on_setup_dip(self):
    #     painting = pm.PyNode("mainPaintingShape")
    #     dip = pm.PyNode("dipPaintingShape")
    #     dip_curves_grp = pm.PyNode("brushes|dipCurves")
    #     setup_dip_factory(painting, dip, dip_curves_grp)

    # def on_send_paint_trays(self):
    #     painting_factory = pm.PyNode("dipPaintingShape")
    #     putl.send_paints(painting_factory)

    # def on_send_brushes(self):
    #     painting_factory = pm.PyNode("paintingStrokeFactoryShape")
    #     butl.send_brushes(painting_factory)

    # def on_send_approach_targets(self):
    #     painting_factory = pm.PyNode("paintingStrokeFactoryShape")
    #     sfu.send_aproach_objects(painting_factory)

    # def on_create_all(self):
    #     RL = Robolink()
    #     RL.setWindowState(-1)
    #     RL.Render(False)

    #     # pnt.Dip(pm.PyNode("dipPaintingShape")).write()
    #     # pnt.Painting(pm.PyNode("mainPaintingShape")).write()

    #     RL.Render(True)
    #     RL.setWindowState(2)

    # def on_create_painting(self):
    #     RL = Robolink()
    #     RL.setWindowState(-1)
    #     try:
    #         painting_node = pm.PyNode("mainPaintingShape")
    #         dip_node = pm.PyNode("dipPaintingShape")
    #         studio = Studio(painting_node, dip_node)
    #         studio.write()
    #     except Exception:
    #         t, v, tb = sys.exc_info()
    #         RL.setWindowState(2)
    #         raise t, v, tb

    #     RL.setWindowState(2)

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

    # def on_test_strokes(self):
    #     node = pm.PyNode("paintingStrokeFactoryShape")
    #     painting = pnt.Painting(node)
    #     painting.test_strokes()

    # def on_show_painting(self):
    #     node = pm.PyNode("paintingStrokeFactoryShape")
    #     painting = pnt.Painting(node)
    #     painting.show()
