import sys
# import os.path
import pymel.core as pm

# import setup_dip
import curve_utils as cutl
# import brush_utils as butl
# import paint_utils as putl

# from setup_dip import setup_dip_factory

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
from studio import Studio
from robolink import (
    Robolink
)


class RingsTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()

    def create_buttons(self):
        pm.setParent(self.column)

        # pm.rowLayout(numberOfColumns=2,
        #              columnWidth2=(
        #                  (100), 100),
        #              adjustableColumn=1,
        #              columnAlign=(1, 'right'),
        #              columnAttach=[(1, 'both', 2), (2, 'both', 2)])
        # pm.button(
        #     label='Arrange rings',
        #     ann="Arrange the rings with constant gap",
        #     command=pm.Callback(self.on_arrange_rings))


        self.gap_ff = pm.floatSliderButtonGrp(
            label='Arrange const gap',
            field=True,
            minValue=-5.0,
            maxValue=5.0,
            # fieldMinValue=0,
            # fieldMaxValue=5.0,
            value=0,
            symbolButtonDisplay=False ,
            buttonLabel="Go",
            buttonCommand=pm.Callback(self.on_arrange_rings_gap),
            columnWidth=(4, 60)
            )

        self.spine_ff = pm.floatSliderButtonGrp(
            label='Arrange const spines',
            field=True,
            minValue=0.0,
            maxValue=10.0,
            # fieldMinValue=0,
            # fieldMaxValue=5.0,
            value=2,
            symbolButtonDisplay=False ,
            buttonLabel="Go",
            buttonCommand=pm.Callback(self.on_arrange_rings_spine),
            columnWidth=(4, 60)
            )

        # self.gap_ff = pm.floatSliderGrp(
        #     label='Gap (cm)',
        #     field=True,
        #     minValue=0,
        #     maxValue=20.0,
        #     fieldMinValue=0,
        #     fieldMaxValue=5.0,
        #     value=0,
        #     columnWidth=(1, 50),
        #     width=200)

        # pm.setParent('..')

        pm.button(
            label='Randomize paints assignments',
            command=pm.Callback(self.on_random_paints))

        pm.button(
            label='Randomize brush assignments',
            command=pm.Callback(self.on_random_brushes))


        pm.button(
            label='Randomize and arrange sequence',
            command=pm.Callback(self.on_random_sequence))

        pm.button(
            label='Randomize selected curve rotatioons',
            command=pm.Callback(self.on_random_rotations))



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

    def on_arrange_rings_gap(self):
        painting_node = pm.PyNode("mainPaintingShape")
        gap = pm.floatSliderButtonGrp(self.gap_ff, query=True, v=True)
        cutl.arrange_rings_gap(painting_node, gap)
 

    def on_arrange_rings_spine(self):
        painting_node = pm.PyNode("mainPaintingShape")
        dist = pm.floatSliderButtonGrp(self.spine_ff, query=True, v=True)
        cutl.arrange_rings_spine(painting_node, dist)
 
    def on_random_paints(self):
        painting_node = pm.PyNode("mainPaintingShape")
        cutl.assign_random_paints(painting_node)

    def on_random_brushes(self):
        painting_node = pm.PyNode("mainPaintingShape")
        cutl.assign_random_brushes(painting_node)

    def on_random_sequence(self):
        start_frame = pm.currentTime(query=True)
        painting_node = pm.PyNode("mainPaintingShape")
        min_frame =  int(pm.playbackOptions(min=True, query=True))
        max_frame =  int(pm.playbackOptions(max=True, query=True))
        gap = pm.floatSliderButtonGrp(self.gap_ff, query=True, v=True)
        for f in range(min_frame, max_frame+1):
            print "Randomizing frame: %s" % f
            pm.currentTime(f)
            cutl.assign_random_paints(painting_node, True)
            cutl.assign_random_brushes(painting_node, True)
            cutl.arrange_rings(painting_node, gap, True)
        pm.currentTime(start_frame)


    def on_random_rotations(self):
        curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)
        cutl.randomize_ring_rotation(curves)


