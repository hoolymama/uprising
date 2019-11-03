import sys
import json
import pymel.core as pm
import uprising_util as uutl
import curve_utils as cutl
import pymel.core.uitypes as gui


class probesTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.info_field = pm.scrollField(
            wordWrap=True,
            text="Press a button below to start recording probes", editable=False)

        board_but = pm.button(
            label="Start canvas probes", command=pm.Callback(self.on_start_canvas_probes)
        )

        holders_but = pm.button(
            label="Start holder probes", en=False, command=pm.Callback(self.on_start_holder_probes)
        )

        self.frame = pm.frameLayout(bv=True, lv=False)

        self.attachForm(self.info_field, "left", 2)
        self.attachForm(self.info_field, "right", 2)
        self.attachNone(self.info_field, "bottom")
        self.attachForm(self.info_field, "top", 2)

        self.attachControl(board_but, "top", 2, self.info_field)
        self.attachForm(board_but, "left", 2)
        self.attachPosition(board_but, "right", 2, 50)
        self.attachNone(board_but, "bottom")

        self.attachControl(holders_but, "top", 2, self.info_field)
        self.attachForm(holders_but, "right", 2)
        self.attachPosition(holders_but, "left", 2, 50)
        self.attachNone(holders_but, "bottom")


        self.attachForm(self.frame, "left", 2)
        self.attachForm(self.frame, "right", 2)
        self.attachForm(self.frame, "bottom", 2)
        self.attachControl(self.frame, "top", 2, holders_but)


    def _clear_entries(self):
        children = pm.frameLayout(self.frame, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def on_start_canvas_probes(self):
        self._clear_entries()
   
        pm.setParent(self.frame)
        layout = pm.rowColumnLayout(
            numberOfColumns=3, columnWidth=[(1, 150), (2, 150), (3, 150)]
        )
        fields = [
            {"slot":9, "loc":"top / left"},
            {"slot":10, "loc":"top / center"},
            {"slot":11, "loc":"top / right"},
            {"slot":6, "loc":"3nd row up / left"},
            {"slot":7, "loc":"3nd row up / center"},
            {"slot":8, "loc":"3nd row up / right"},
            {"slot":3, "loc":"2nd row up / left"},
            {"slot":4, "loc":"2nd row up / center"},
            {"slot":5, "loc":"2nd row up / right"},
            {"slot":0, "loc":"bottom / left"},
            {"slot":1, "loc":"bottom / center"},
            {"slot":2, "loc":"bottom / right"}]

        for i in range(12):
            fields[i]["field"]=pm.textField()

        fields = sorted(fields, key=lambda k: k['slot']) 

        info =  "Enter value for probe at location: {}".format(fields[0]["loc"])
        pm.scrollField(self.info_field, e=True, text="Start entering probe values for the canvas")

        for i, f in enumerate(fields) :
            pm.textField(f["field"], edit=True, changeCommand=pm.Callback(self.on_canvas_enter,i, fields ) )

        pm.setFocus(fields[0]["field"])

        pm.setParent("..")
        return layout


    def on_canvas_enter(self,index, fields):
        next_index = index+1
        if (next_index < len(fields)):
            pm.setFocus(fields[next_index]["field"])
            info =  "Enter value for probe at location: {}".format(fields[next_index]["loc"])
            pm.scrollField(self.info_field, e=True, text=info)
            return

        # we are complete
        values = [float(pm.textField(f["field"],q=True,text=True)) for f in  fields]
        
        verts = [item for sublist in pm.sets(
            "probePointsSet", q=True) for item in sublist]

        if not (len(values) == len(verts)):
            raise IndexError(
                "Sheet data and number of verts are different lengths")

        vert_positions = []
        for val, vtx in zip(values, verts) :
            pos = vtx.getPosition(space="world")
            pos.z = (uutl.numeric(val) * 0.1) - 1.0
            vtx.setPosition(pos, space="world")
            vert_positions.append(pos.z)

        info =  "Reading complete.\nBoard vertex Z positions have been set to:\n[{}]".format(", ".join(str(v) for v in vert_positions) )
        pm.scrollField(self.info_field, e=True, text=info)

    def on_start_holder_probes(self):
        pass

    def create_action_buttons(self):
        pm.setParent(self)  # form

        self.spare_but = pm.button(label="Preview", command=pm.Callback(self.on_spare))
        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.ui, "left", 2)
        self.attachForm(self.ui, "right", 2)
        self.attachForm(self.ui, "top", 2)
        self.attachControl(self.ui, "bottom", 2, self.spare_but)

        self.attachNone(self.spare_but, "top")
        self.attachForm(self.spare_but, "left", 2)
        self.attachPosition(self.spare_but, "right", 2, 50)
        self.attachForm(self.spare_but, "bottom", 2)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, "left", 2, 50)
        self.attachForm(go_but, "bottom", 2)



    def on_focus(self, which, index):
        print "on_focus", which, index


    def on_spare(self):
        pass

    def on_go(self):
        pass
