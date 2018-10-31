import pymel.core as pm
import setup_dip
import pymel.core.uitypes as gui
import curve_utils as cutl


class Menu(gui.Menu):

    def __init__(self):
        # pm.setParent(self)
        self.setLabel("Tools")
        self.create_ui()

    def create_ui(self):
        pm.menuItem( label="Bake mapped paintIds", command=pm.Callback(self.on_bake_paint_ids) )
        pm.menuItem( label="Make dip combinations", command=pm.Callback(setup_dip.doit) )


    def _bake_first_paint_id(self, painting, curve):
        print "Baking curve"
        cc = pm.paintingQuery(painting, cc=True)
        if cc:
            sc =  pm.paintingQuery(painting, ci=0, sc=True)
            if sc:
                paint_id =  pm.paintingQuery(painting, ci=0, si=0, clusterPaintId=True)
                print "paint id is %d" % paint_id
                stroke_curve = cutl.get_stroke_curve(curve)
                stroke_curve.attr("paintId").set(paint_id)

    def on_bake_paint_ids(self):
        painting = pm.PyNode("mainPaintingShape")
        curves = cutl.get_curves_from_painting(painting)
        visible_curves = [curve for curve in curves if curve.getParent().attr("visibility").get()]
        print "on_bake_paint_ids"

        for curve in curves:
            cutl.hide_objects( curves)
            cutl.show_objects([curve])

            self._bake_first_paint_id(painting, curve)
        cutl.show_objects(visible_curves)
