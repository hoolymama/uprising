
import pymel.core as pm


import pymel.core.uitypes as gui


def _deactivate_all_nodes(nodes):
    for node in nodes:
        node.attr("active").set(0)


def _activate_all_nodes(nodes):
    for node in nodes:
        node.attr("active").set(1)


class KeysTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_ui()
        self.create_action_buttons()

    def create_ui(self):
        pm.setParent(self.column)

        min_frame = pm.currentTime(q=True)
        self.frame_if = pm.intFieldGrp(
            label="Start frame",
            numberOfFields=1,
            value1=min_frame)

        self.parent_if = pm.intFieldGrp(
            label="First parent Id",
            numberOfFields=1,
            value1=0)

        self.send_dry_run_cb = pm.checkBoxGrp(
            label='Dry run',
            value1=0,
            height=30)

        # self.initialize_ui()

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(label='Go', command=pm.Callback(self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, save_but)

        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def populate(self):
        pass
        # val = "default"
        # if "up_setup_palette_name" in pm.optionVar:
        #     val = pm.optionVar["up_setup_palette_name"]
        # print val
        # pm.textFieldGrp(self.setup_paints_tf, e=True, text=val)

    def save(self):
        pass
        # val = pm.textFieldButtonGrp(self.setup_paints_tf, q=True, text=True)
        # pm.optionVar["up_setup_palette_name"] = val

    def on_go(self):

        dry = pm.checkBoxGrp(self.send_dry_run_cb, q=True, value1=True)

        first_frame = pm.intFieldGrp(
            self.frame_if,
            query=True,
            value1=True)

        first_parent = pm.intFieldGrp(
            self.parent_if,
            query=True,
            value1=True)

        painting_node = pm.PyNode("mainPaintingShape")
        painting_node.attr("applyFilters").set(0)

        vals = []
        frame = first_frame
        conns = painting_node.attr("strokes").connections(
            s=True, d=False,  c=True)
        nodes = [c[1] for c in conns]
        for plug, input_node, in conns:
            _deactivate_all_nodes(nodes)
            index = int(plug.split("[")[1].split("]")[0])
            if index < first_parent:
                continue
            input_node.attr("active").set(1)
            combos = pm.paintingQuery(painting_node, toolCombinations=True)
            brush_ids_reversed = sorted(set(combos[::3]))[::-1]
            for bid in brush_ids_reversed:
                vals.append([frame, index, bid])
                frame += 1

        parent_id_attr = painting_node.attr("stfl[0].stfod")
        brush_id_attr = painting_node.attr("stfl[1].stfod")

  
        if not dry:
            for t, parent_id, brush_id in vals:
                pm.currentTime(t)
                parent_id_attr.setKey(value=parent_id)
                brush_id_attr.setKey(value=brush_id)

        _activate_all_nodes(nodes)

        for val in vals:
            print("frame:%d parent:%d brush:%d" % tuple(val))
