import sys
import pymel.core as pm
from brush import Brush
import curve_utils as cutl
import uprising_util as uutl
import pymel.core.uitypes as gui
import stroke_factory_utils as sfu


class BrushTestTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()
        
    def create_buttons(self):
        pm.setParent(self.column)

        pm.scrollField(
            wordWrap=True,
            text="""Pick some brushes and a curve and a collectStrokes node.

A clone of the curve will be generated for each brush and will be connected up to the collectStrokes node.

Gap is the offset from one curve to the next. If you want to line strokes up next to each other, turn on Use Widths. It adds in the brush widths so that the gap is the gap between the extent of brush borders.

""",
            editable=False)

        self.offset_ctl = pm.floatFieldGrp(
            height=30,
            label='Gap (xy)',
            numberOfFields=2, value1=0, value2=0.1, pre=3, extraLabel='cm')

        self.add_widths_cb = pm.checkBoxGrp(
            height=30,
            label='Use widths',
            value1=1,
            annotation='Add in the widths of strokes to make even gaps')

        self.brush_width_mult_ff = pm.floatFieldGrp(
            height=30,
            label='Brush width mult',
            numberOfFields=1, value1=1.0, pre=3)


        # self.collector_nf = pm.textFieldButtonGrp(label="Collector", editable=False, buttonLabel="select")

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(
            label='Save',
            en=False)

        go_button = pm.button(
            label='Generate',
            command=pm.Callback(self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, save_but)

        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)

        self.attachNone(go_button, 'top')
        self.attachForm(go_button, 'right', 2)
        self.attachControl(go_button, 'left', 2, save_but)
        self.attachForm(go_button, 'bottom', 2)

    def on_go(self):

        painting = pm.PyNode("mainPaintingShape")

        offset = pm.floatFieldGrp(self.offset_ctl, query=True, value=True)
        use_widths = pm.checkBoxGrp(
            self.add_widths_cb, query=True, value1=True)

        brush_width_mult = pm.floatFieldGrp(self.brush_width_mult_ff, query=True, value1=True)

        if offset[0] == 0 and  offset[1] == 0:
            pm.error("Please provide a gap")

        direction = pm.dt.Vector(offset[0], offset[1], 0).normal()
        offset_length = pm.dt.Vector(offset[0], offset[1], 0).length()

        print offset
        print use_widths

        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True)
        if not curves:
            pm.error("No curves selected")

        brush_nodes = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="brushNode",
            ni=True)
        if not brush_nodes:
            pm.error("No brushNodes selected")

 
        collector = pm.ls( selection=True,type="collectStrokes")
        if not collector:
            pm.error("No collectStrokes selected")
        collector = collector[0]
 
        # half - and / 10 because cm to mm
        width_mult = brush_width_mult * 0.05

        orig_curve_xf = curves[0].getParent()
        pos = orig_curve_xf.attr("t")
        extent = 0
        new_curves = []
        for brush_node in brush_nodes:
            conns = brush_node.connections(d=True, s=False, p=True)
            main_conn = [c for c in conns if c.node() == painting][0]
            index = int(main_conn.split("[")[1].split("]")[0])
            brush = Brush.brush_at_index(painting, index)

            extent += offset_length + (brush.width*width_mult)
            curve_xf = pm.duplicate(orig_curve_xf, rr=True)[0]
            curve_xf.rename("crv_{}".format(brush.node_name))
            curr_pos = direction*extent
            pm.move(curr_pos[0],curr_pos[1], curr_pos[2], curve_xf, relative=True )
            extent +=  (brush.width*width_mult)
            cutl.connect_curve_to_strokes_att(curve_xf.getShapes()[0], collector, connect_to="next_available")

            curve_stroke = cutl.get_stroke_node(curve_xf)

            curve_stroke.attr("brushId").set(index)

            new_curves.append(curve_xf)

        cutl.curve_vis_active_connection(new_curves)




            # brush.width
            # brush.node_name

            # print main_conn, index, brush

            # xf = pm.duplicate(orig_curve_xf, rr=True)[0]
            # index = brushNode.connections(destination=True, source=False, type="painting" )


# conn[0].node()


# conns = brush_node.connections(destination=True, source=False  )

# conns[0].node()

        # attach colllectStrokes to painting

        # random_paint_params = self.get_random_resource_values(
        #     self.random_paints_row)
        # random_brush_params = self.get_random_resource_values(
        #     self.random_brushes_row)

        # do_max = pm.floatFieldGrp(self.max_extent_ctrl, query=True, en=True)

        # spacing_type = "none"
        # if pm.rowLayout(
        #         self.spacing_row,
        #         query=True, en=True):
        #     spacing_type = "spine" if (
        #         pm.radioButtonGrp(
        #             self.spacing_type_ctl,
        #             query=True,
        #             sl=True) == 1) else "gap"

        # spacing = pm.floatFieldGrp(self.spacing_ctl, query=True, value1=True)
        # max_extent = pm.floatFieldGrp(
        #     self.max_extent_ctrl,
        #     query=True,
        #     value1=True) if do_max else None

        # # print "max_extent : %s" % max_extent
        # curves = pm.ls(
        #     selection=True,
        #     dag=True,
        #     leaf=True,
        #     type="nurbsCurve",
        #     ni=True)
        # if not curves:
        #     pm.error("No curves selected")
        #     return

        # paint_ids = []
        # brush_ids = []
        # if random_paint_params and random_paint_params[0] == "":
        #     for curve in curves:
        #         sc = cutl.get_stroke_node(curve)
        #         paint_ids.append(sc.attr("paintId").get())

        # if random_brush_params and random_brush_params[0] == "":
        #     for curve in curves:
        #         sc = cutl.get_stroke_node(curve)
        #         paint_ids.append(sc.attr("brushId").get())

        # curves_packs = map(None, curves, brush_ids, paint_ids)

        # if do_keys:
        #     start_frame = int(pm.playbackOptions(q=True, min=True))
        #     end_frame = int(pm.playbackOptions(q=True, max=True))
        #     for f in range(start_frame, end_frame + 1):
        #         pm.currentTime(f)
        #         cutl._randomize(
        #             curves_packs,
        #             random_paint_params,
        #             random_brush_params,
        #             max_extent,
        #             spacing,
        #             spacing_type,
        #             paint_ids,
        #             brush_ids,
        #             True)
        # else:
        #     cutl._randomize(
        #         curves_packs,
        #         random_paint_params,
        #         random_brush_params,
        #         max_extent,
        #         spacing,
        #         spacing_type,
        #         paint_ids,
        #         brush_ids,
        #         False)

    # def populate(self):

    #     var = ("upov_spacing_row_en", True)
    #     pm.rowLayout(
    #         self.spacing_row,
    #         e=True,
    #         en=pm.optionVar.get(
    #             var[0],
    #             var[1]))
    #     uutl.conform_activatable_checkbox(self.spacing_row)

    #     var = ("upov_spacing_type_ctl", 2)
    #     pm.radioButtonGrp(
    #         self.spacing_type_ctl,
    #         e=True,
    #         sl=pm.optionVar.get(
    #             var[0],
    #             var[1]))

    #     var = ("upov_spacing_ctl", 0.3)
    #     pm.floatFieldGrp(
    #         self.spacing_ctl,
    #         e=True,
    #         value1=pm.optionVar.get(
    #             var[0],
    #             var[1]))

    #     var = ("upov_max_extent_ctrl_en", True)
    #     pm.floatFieldGrp(
    #         self.max_extent_ctrl,
    #         e=True,
    #         en=pm.optionVar.get(
    #             var[0],
    #             var[1]))
    #     uutl.conform_activatable_checkbox(self.max_extent_ctrl)

    #     var = ("upov_max_extent_ctrl", 57)
    #     pm.floatFieldGrp(
    #         self.max_extent_ctrl,
    #         e=True,
    #         v1=pm.optionVar.get(
    #             var[0],
    #             var[1]))

    #     self.populate_random_res("random_paints_row")
    #     self.populate_random_res("random_brushes_row")

    # def save(self):
    #     # board
    #     var = "upov_spacing_row_en"
    #     pm.optionVar[var] = pm.rowLayout(self.spacing_row, q=True, en=True)

    #     var = "upov_spacing_type_ctl"
    #     pm.optionVar[var] = pm.radioButtonGrp(
    #         self.spacing_type_ctl, q=True, sl=True)

    #     var = "upov_spacing_ctl"
    #     pm.optionVar[var] = pm.floatFieldGrp(
    #         self.spacing_ctl, q=True, value1=True)

    #     var = "upov_max_extent_ctrl_en"
    #     pm.optionVar[var] = pm.floatFieldGrp(
    #         self.max_extent_ctrl, q=True, en=True)

    #     var = "upov_max_extent_ctrl"
    #     pm.optionVar[var] = pm.floatFieldGrp(
    #         self.max_extent_ctrl, q=True, v1=True)

    #     self.save_random_res("random_paints_row")
    #     self.save_random_res("random_brushes_row")

    # def populate_random_res(self, row_attribute):
    #     row = getattr(self, row_attribute)
    #     var = ("upov_%s_en" % row_attribute, True)
    #     pm.rowLayout(row, e=True, en=pm.optionVar.get(var[0], var[1]))

    #     text_ctrl, steps_ctrl, power_ctrl = row.getChildArray()

    #     var = ("upov_%s_spec" % row_attribute, "0-15")
    #     pm.textFieldGrp(
    #         text_ctrl,
    #         e=True,
    #         text=pm.optionVar.get(
    #             var[0],
    #             var[1]))

    #     var = ("upov_%s_steps" % row_attribute, -1)
    #     pm.intFieldGrp(
    #         steps_ctrl,
    #         e=True,
    #         value1=pm.optionVar.get(
    #             var[0],
    #             var[1]))

    #     var = ("upov_%s_power" % row_attribute, 0.0)
    #     pm.floatFieldGrp(
    #         power_ctrl,
    #         e=True,
    #         value1=pm.optionVar.get(
    #             var[0],
    #             var[1]))

    # def save_random_res(self, row_attribute):

    #     row = getattr(self, row_attribute)
    #     opvar = "upov_%s_en" % row_attribute
    #     pm.optionVar[opvar] = pm.rowLayout(row, q=True, en=True)

    #     text_ctrl, steps_ctrl, power_ctrl = row.getChildArray()

    #     var = "upov_%s_spec" % row_attribute
    #     pm.optionVar[var] = pm.textFieldGrp(text_ctrl, q=True, text=True)

    #     var = "upov_%s_steps" % row_attribute
    #     pm.optionVar[var] = pm.intFieldGrp(steps_ctrl, q=True, value1=True)

    #     var = "upov_%s_power" % row_attribute
    #     pm.optionVar[var] = pm.floatFieldGrp(power_ctrl, q=True, value1=True)
