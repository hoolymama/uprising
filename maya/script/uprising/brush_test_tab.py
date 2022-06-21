import pymel.core as pm
from .brush import Brush
from uprising import curve_utils as cutl
import pymel.core.uitypes as gui
from uprising import const as k



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

        painting = pm.PyNode(k.PAINTING_NAME)

        offset = pm.floatFieldGrp(self.offset_ctl, query=True, value=True)
        use_widths = pm.checkBoxGrp(
            self.add_widths_cb, query=True, value1=True)

        brush_width_mult = pm.floatFieldGrp(
            self.brush_width_mult_ff, query=True, value1=True)

        if offset[0] == 0 and offset[1] == 0:
            pm.error("Please provide a gap")

        direction = pm.dt.Vector(offset[0], offset[1], 0).normal()
        offset_length = pm.dt.Vector(offset[0], offset[1], 0).length()


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

        collector = pm.ls(selection=True, type="collectStrokes")
        if not collector:
            pm.error("No collectStrokes selected")
        collector = collector[0]

        # half - and / 10 because cm to mm
        width_mult = brush_width_mult * 0.05

        orig_curve_xf = curves[0].getParent()
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
            pm.move(curr_pos[0], curr_pos[1],
                    curr_pos[2], curve_xf, relative=True)
            extent += (brush.width*width_mult)
            cutl.connect_curve_to_strokes_att(
                curve_xf.getShapes()[0], collector, connect_to="next_available")

            curve_stroke = cutl.get_stroke_node(curve_xf)

            curve_stroke.attr("brushId").set(index)

            new_curves.append(curve_xf)

        cutl.curve_vis_active_connection(new_curves)
