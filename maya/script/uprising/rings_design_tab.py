import sys
# import os.path
import pymel.core as pm

# import setup_dip
import curve_utils as cutl
# import brush_utils as butl
# import paint_utils as putl

# from setup_dip import setup_dip_factory
import uprising_util as uutl
import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# from uprising.sequence import Sequence
# import painting as pnt
from studio import Studio
from robolink import (
    Robolink
)


class RingsDesignTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()

    def get_random_resource_values(self, row):
        if not pm.rowLayout(row, q=True, en=True):
            return None

        spec_ctl, steps_ctl, pow_ctl = pm.rowLayout(
            row, q=True, childArray=True)

        return(
            pm.textFieldGrp(spec_ctl, q=True, text=True),
            pm.intFieldGrp(steps_ctl, q=True, value1=True),
            pm.floatFieldGrp(pow_ctl, q=True, value1=True)
        )

    def random_res_control(self, label, default_text):

        with uutl.activatable():
            row = pm.rowLayout(numberOfColumns=3,
                               columnWidth3=(250, 90, 90),
                               columnAlign=(1, 'right'))
            #, columnAttach=[(1, 'both', 2), (2, 'both', 2), (3, 'both', 2)])
            text_ctrl = pm.textFieldGrp(
                label=label,
                text=default_text
            )
            steps_ctrl = pm.intFieldGrp(
                columnWidth2=(30, 60,),
                numberOfFields=1, value1=-1, label='steps'
            )
            power_ctrl = pm.floatFieldGrp(
                columnWidth2=(30, 60,),
                numberOfFields=1, value1=0, label='pow'
            )
            pm.setParent("..")
        return row

    def create_buttons(self):
        pm.setParent(self.column)



        with uutl.activatable():
            self.spacing_row =  pm.rowLayout(numberOfColumns=2,
             columnWidth2=(350, 150),
             columnAlign=(1, 'right'),
             columnAttach=[(1, 'both', 2), (2, 'both', 2)])
            self.spacing_type_ctl = pm.radioButtonGrp(
                label='Spacing',
                sl=1,
                labelArray2=[
                    'Spines',
                    'Gaps'],
                numberOfRadioButtons=2)

            self.spacing_ctl = pm.floatFieldGrp(
                numberOfFields=1, value1=1, pre=2, extraLabel='cm')
            pm.setParent("..")

        with uutl.activatable():
            self.max_extent_ctrl = pm.floatFieldGrp(
                label='Max extent',
                value1=50
            )

 

        self.random_paints_row = self.random_res_control(
            "Random paints", "0-32")
        self.random_brushes_row = self.random_res_control(
            "Random brushes", "0-16")

        # with uutl.activatable():
        #     self.random_brushes_ctrl = pm.textFieldGrp(
        #         label='Rando brushes',
        #         text='0-16'
        #     )

    def create_action_buttons(self):
        pm.setParent(self)  # form

        set_button = pm.button(
            label='Set value',
            command=pm.Callback(
                self.on_apply,
                False))
        key_button = pm.button(
            label='Keyframe range',
            command=pm.Callback(
                self.on_apply,
                True))
        # key_button = pm.button(
        #     label='Keyframe range',
        #     command=pm.Callback(
        #         self.on_apply,
        #         True))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, set_button)

        self.attachNone(set_button, 'top')
        self.attachForm(set_button, 'left', 2)
        self.attachPosition(set_button, 'right', 2, 50)
        self.attachForm(set_button, 'bottom', 2)

        self.attachNone(key_button, 'top')
        self.attachForm(key_button, 'right', 2)
        self.attachPosition(key_button, 'left', 2, 50)
        self.attachForm(key_button, 'bottom', 2)

    def on_apply(self, do_keys):

        random_paint_params = self.get_random_resource_values(
            self.random_paints_row)
        random_brush_params = self.get_random_resource_values(
            self.random_brushes_row)

        do_max = pm.floatFieldGrp(self.max_extent_ctrl, query=True, en=True)

        spacing_type = "none"
        if pm.rowLayout(
                self.spacing_row,
                query=True, en=True):
            spacing_type = "spine" if (
                pm.radioButtonGrp(
                    self.spacing_type_ctl,
                    query=True,
                    sl=True) == 1) else "gap"

        spacing = pm.floatFieldGrp(self.spacing_ctl, query=True, value1=True)
        max_extent = pm.floatFieldGrp(
            self.max_extent_ctrl,
            query=True,
            value1=True) if do_max else None

        # print "max_extent : %s" % max_extent
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True)
        if not curves:
            pm.error("No curves selected")
            return

        paint_ids = []
        brush_ids = []
        if random_paint_params and random_paint_params[0] == "":
            for curve in curves:
                sc = cutl.get_stroke_curve(curve)
                paint_ids.append(sc.attr("paintId").get())

        if random_brush_params and random_brush_params[0] == "":
            for curve in curves:
                sc = cutl.get_stroke_curve(curve)
                paint_ids.append(sc.attr("brushId").get())

        curves_packs = map(None, curves, brush_ids, paint_ids)

        if do_keys:
            start_frame = int(pm.playbackOptions(q=True, min=True))
            end_frame = int(pm.playbackOptions(q=True, max=True))
            for f in range(start_frame, end_frame + 1):
                pm.currentTime(f)
                cutl._randomize(
                    curves_packs,
                    random_paint_params,
                    random_brush_params,
                    max_extent,
                    spacing,
                    spacing_type,
                    paint_ids,
                    brush_ids,
                    True)
        else:
            cutl._randomize(
                curves_packs,
                random_paint_params,
                random_brush_params,
                max_extent,
                spacing,
                spacing_type,
                paint_ids,
                brush_ids,
                False)

    # def on_duplicate_into_gaps(self):
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True)
    #     cutl.duplicate_into_gaps(curves)

    # def on_arrange_rings_gap(self):
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True)

    #     # painting_node = pm.PyNode("mainPaintingShape")
    #     gap = pm.floatSliderButtonGrp(self.gap_ff, query=True, v=True)
    #     cutl.arrange_rings_gap(curves, gap)

    # def on_arrange_rings_spine(self):
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True)
    #     # painting_node = pm.PyNode("mainPaintingShape")
    #     dist = pm.floatSliderButtonGrp(self.spine_ff, query=True, v=True)
    #     cutl.arrange_rings_spine(curves, dist)

    # def on_random_paints(self):
    #     anim = pm.checkBox(self.random_paints_anim_cb, q=True, value=True)
    #     spec = pm.textFieldButtonGrp(
    #         self.random_paints_tf, query=True, text=True)
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True)
    #     if (anim):
    #         min_frame = int(pm.playbackOptions(min=True, query=True))
    #         max_frame = int(pm.playbackOptions(max=True, query=True))
    #         for f in range(min_frame, max_frame + 1):
    #             pm.currentTime(f)
    #             cutl.assign_random_paints(curves, spec, True)
    #     else:
    #         cutl.assign_random_paints(curves, spec, False)

    # def on_random_brushes(self):
    #     anim = pm.checkBox(self.random_brushes_tf, q=True, value=True)
    #     spec = pm.textFieldButtonGrp(
    #         self.random_brushes_tf, query=True, text=True)
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True,
    #         v=True)
    #     if (anim):
    #         min_frame = int(pm.playbackOptions(min=True, query=True))
    #         max_frame = int(pm.playbackOptions(max=True, query=True))
    #         for f in range(min_frame, max_frame + 1):
    #             pm.currentTime(f)
    #             cutl.assign_random_brushes(curves, spec, True)
    #     else:
    #         cutl.assign_random_brushes(curves, spec, False)

    # def on_random_sequence(self):
    #     paint_spec = pm.textFieldButtonGrp(
    #         self.random_paints_tf, query=True, text=True)
    #     brush_spec = pm.textFieldButtonGrp(
    #         self.random_brushes_tf, query=True, text=True)
    #     start_frame = pm.currentTime(query=True)
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     min_frame = int(pm.playbackOptions(min=True, query=True))
    #     max_frame = int(pm.playbackOptions(max=True, query=True))
    #     gap = pm.floatSliderButtonGrp(self.gap_ff, query=True, v=True)
    #     for f in range(min_frame, max_frame + 1):
    #         print "Randomizing frame: %s" % f
    #         pm.currentTime(f)
    #         cutl.assign_random_paints(painting_node, paint_spec, True)
    #         cutl.assign_random_brushes(painting_node, brush_spec, True)
    #         cutl.arrange_rings(painting_node, gap, True)
    #     pm.currentTime(start_frame)

    # def on_random_rotations(self):
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True,
    #         v=True)
    #     cutl.randomize_ring_rotation(curves)

    # def on_auto_stroke_length(self):
    #     min_strokes = pm.intSliderGrp(
    #         self.min_strokes_ctl, q=True, v=True)
    #     max_length = pm.floatSliderGrp(
    #         self.max_length_ctl, q=True, v=True)
    #     overlap = pm.floatSliderGrp(
    #         self.overlap_ctl, q=True, v=True)
    #     twist_angle = pm.floatSliderGrp(
    #         self.twist_angle_ctl, q=True, v=True)
    #     twist_distance = pm.floatSliderGrp(
    #         self.twist_distance_ctl, q=True, v=True)

    #     # lift_length = pm.floatSliderGrppm.floatSliderGrp(
    #     #     self.lift_length_ctl, q=True, v=True)
    #     curves = pm.ls(
    #         selection=True,
    #         dag=True,
    #         leaf=True,
    #         type="nurbsCurve",
    #         ni=True,
    #         ut=True,
    #         v=True)
    #     if not curves:
    #         raise pm.MayaNodeError("Must select curves")
    #     cutl.auto_set_rings(
    #         curves,
    #         min_strokes,
    #         max_length,
    #         overlap,
    #         twist_angle,
    #         twist_distance)
