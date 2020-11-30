import pymel.core as pm
import pymel.core.uitypes as gui


class BrushAssignTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.main_tf = pm.textFieldGrp(
            label="Main Painting", text="--", en=False, cw2=(140, 300)
        )
        self.scroll = pm.scrollLayout()
        self.skels_column = pm.columnLayout(adj=True)
        self.create_action_buttons_and_layout()
        self.reload()

    def _connected_brush_indices(self, node):
        conns = node.attr("brushes").connections(s=True, c=True)
        indices = [str(a[0].logicalIndex()) for a in conns]
        return ",".join(indices)

    def _create_entry(self, node):
        connected_indices = self._connected_brush_indices(node)
        tf = pm.textFieldGrp(
            label=node, text=connected_indices, cw2=(140, 300))
        return tf

    def _clear_entries(self):
        children = pm.columnLayout(self.skels_column, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def reload(self):
        try:
            painting = pm.PyNode("mainPaintingShape")
        except pm.MayaNodeError:
            return
        connected_indices = self._connected_brush_indices(painting)
        pm.textFieldGrp(self.main_tf, edit=True, text=connected_indices)

        self._clear_entries()
        skels = pm.ls(type="skeletonStroke")
        for skel in skels:
            pm.setParent(self.skels_column)
            self._create_entry(skel)

    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        reload_but = pm.button(
            label="Reload", command=pm.Callback(self.reload))
        go_but = pm.button(label="Apply", command=pm.Callback(self.on_go))

        self.attachForm(self.main_tf, "left", 2)
        self.attachForm(self.main_tf, "right", 2)
        self.attachForm(self.main_tf, "top", 2)
        self.attachNone(self.main_tf, "bottom")

        self.attachForm(self.scroll, "left", 2)
        self.attachForm(self.scroll, "right", 2)
        self.attachControl(self.scroll, "top", 2, self.main_tf)
        self.attachControl(self.scroll, "bottom", 2, reload_but)

        self.attachNone(reload_but, "top")
        self.attachForm(reload_but, "left", 2)
        self.attachPosition(reload_but, "right", 2, 50)
        self.attachForm(reload_but, "bottom", 2)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, "left", 2, 50)
        self.attachForm(go_but, "bottom", 2)

    def on_go(self):

        all_brushes = {}
        painting = pm.PyNode("mainPaintingShape")
        conns = painting.attr("brushes").connections(s=True, c=True)
        for conn in painting.attr("brushes").connections(s=True, c=True):
            all_brushes[conn[0].logicalIndex()] = conn[1]

        tfs = pm.columnLayout(self.skels_column, q=True, ca=True)
        for tf in tfs:
            node_name = pm.textFieldGrp(tf, q=True, label=True)
            node = pm.PyNode(node_name)
            val = pm.textFieldGrp(tf, q=True, text=True)
            indices = [int(i) for i in val.split(
                ",") if i is not None and i.isdigit()]
            self._replug(node, indices, all_brushes)

        self.reload()

    def _replug(self, node, indices, all_brushes):
        # get existing connected indices and compare against the new ones.
        existing = self._connected_brush_indices(node)
        proposed = ",".join(str(i) for i in indices)
        if existing == proposed:
            return

        # disconnect and delete all plugs
        to_remove = node.attr("brushes").getArrayIndices()
        for i in to_remove:
            pm.removeMultiInstance(
                node.attr("brushes[{:d}]".format(i)), b=True)

        # connect the new ones:
        for i in indices:
            if i not in all_brushes:
                pm.displayWarning(
                    "Illegal index {}. All indices must be connections on the main painting"
                )
                continue
            all_brushes[i].attr("outPaintBrush") >> node.attr(
                "brushes[{:d}]".format(i))
