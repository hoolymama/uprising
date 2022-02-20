
from uprising import progress
import pymel.core as pm
import pymel.core.uitypes as gui

from uprising.pov.session.pov_session import PovSession


class PovPublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.export_frame = self.create_export_frame()
        self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()

        self.populate()
        self.on_ops_change()


    def create_export_frame(self):

        frame = pm.frameLayout(label="Export", bv=True)

        self.stroke_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max strokes/file",
            annotation="Max number of strokes per painting partial program",
            numberOfFields=1,
            value1=500,
        )


        self.options_cb =  pm.checkBoxGrp(
            numberOfCheckBoxes=3,
            label1="Run on Robot",
            label2="Save RDK",
            label3="Save SRC",
            changeCommand=pm.Callback(self.on_ops_change)
        )

        pm.setParent("..")
        return frame

    def create_progress_frame(self):
        frame = pm.frameLayout(
            bv=True, collapse=False, labelVisible=True, label="Progress"
        )
        col = pm.columnLayout(adj=True)
        progress.create(col)
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form
        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, save_but)


        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)


        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, "bottom", 2)
        return go_but

    ##############################################################

    def on_ops_change(self):
        pass
        # save_rdk = pm.checkBoxGrp(self.options_cb, q=True , value2=True)
        # save_src = pm.checkBoxGrp(self.options_cb, q=True , value3=True)

        # pm.intFieldGrp(
        #     self.stroke_chunk_if, edit=True, en=(save_src or save_rdk)
        # )

    def on_go(self):
        self.save()
        stroke_chunk_size = pm.intFieldGrp(
            self.stroke_chunk_if, query=True, value1=True
        )

        run_on_robot = pm.checkBoxGrp(self.options_cb, q=True , value1=True)
        # save_files = pm.checkBoxGrp(self.options_cb, q=True , value2=True)
        save_rdk = pm.checkBoxGrp(self.options_cb, q=True , value2=True)
        save_src = pm.checkBoxGrp(self.options_cb, q=True , value3=True)

        try:
            pov_session = PovSession(stroke_chunk_size, run_on_robot, save_rdk, save_src)
            pov_session.run()
        except ValueError:
            print("PovSession aborted")

    def populate(self):

        var = ("upov_pov_pub_options", 0, 1, 1)
        vals = pm.optionVar.get(var[0], var[1:])
        if len(vals) != (len(var)-1):
            vals =  var[1:]
        pm.checkBoxGrp(
            self.options_cb,
            e=True,
            valueArray3=vals)

        var = ("upov_pov_pub_chunk_ctl", 500)
        pm.intFieldGrp(
            self.stroke_chunk_if,
            e=True,
            value1=pm.optionVar.get(var[0],var[1]))

    def save(self):
        # board

        var = "upov_pov_pub_options"
        pm.optionVar[var] = pm.checkBoxGrp(self.options_cb, q=True, valueArray3=True)

        var = "upov_pov_pub_chunk_ctl"
        pm.optionVar[var] = pm.intFieldGrp(
            self.stroke_chunk_if, q=True, value1=True)


def find_contributing_stroke_nodes():
    pass
    # painting_node = pm.PyNode("mainPaintingShape")
    # all_skels = pm.ls(type="skeletonStroke")

    # result = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    # if result:
    #     return result

    # for node in all_skels:
    #     with isolate_nodes([node], all_skels):
    #         try:
    #             pm.paintingQuery(painting_node, cc=True)
    #         except RuntimeError:
    #             continue
    #     result.append(node)
    # print "{} of {} skeleton nodes contributing".format(
    #     len(result), len(all_skels))
    # return result
