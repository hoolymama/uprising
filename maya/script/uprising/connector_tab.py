import pymel.core as pm
import pymel.core.uitypes as gui
import re

class ConnectorTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.src_att_field = pm.textField(text="--")
        self.src_offset_field = pm.intField(value=0)

        self.dest_att_field = pm.textField(text="--")
        self.dest_offset_field = pm.intField(value=0)

        self.src_load_but = pm.button(
            label="Load Src", command=pm.Callback(self.load_src))
        self.dest_load_but = pm.button(
            label="Load Dest", command=pm.Callback(self.load_dest))

        self.scroll = pm.scrollLayout(childResizable=True)
        self.scroll_form = pm.formLayout(nd=100)

        self.src_column = pm.columnLayout(adj=True)
        pm.setParent("..")
        self.dest_column = pm.columnLayout(adj=True)

        self.create_action_buttons_and_layout()

        self.src_att_popup = pm.popupMenu(parent=self.src_att_field)
        self.dest_att_popup = pm.popupMenu(parent=self.dest_att_field)

    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        dry_but = pm.button(label="Dry run", command=pm.Callback(self.on_dry))
        go_but = pm.button(label="Connect", command=pm.Callback(self.on_go))

        self.attachForm(self.src_att_field, "left", 2)
        self.attachPosition(self.src_att_field, "right", 2, 40)
        self.attachForm(self.src_att_field, "top", 2)
        self.attachNone(self.src_att_field, "bottom")

        self.attachControl(self.src_offset_field,
                           "left", 2, self.src_att_field)
        self.attachPosition(self.src_offset_field, "right", 2, 50)
        self.attachForm(self.src_offset_field, "top", 2)
        self.attachNone(self.src_offset_field, "bottom")

        self.attachPosition(self.dest_att_field, "left", 2, 50)
        self.attachPosition(self.dest_att_field, "right", 2, 90)
        self.attachForm(self.dest_att_field, "top", 2)
        self.attachNone(self.dest_att_field, "bottom")

        self.attachControl(self.dest_offset_field,
                           "left", 2, self.dest_att_field)
        self.attachForm(self.dest_offset_field, "right", 2)
        self.attachForm(self.dest_offset_field, "top", 2)
        self.attachNone(self.dest_offset_field, "bottom")

        self.attachForm(self.src_load_but, "left", 2)
        self.attachPosition(self.src_load_but, "right", 2, 50)
        self.attachControl(self.src_load_but, "top", 2, self.src_att_field)
        self.attachNone(self.src_load_but, "bottom")

        self.attachPosition(self.dest_load_but, "left", 2, 50)
        self.attachForm(self.dest_load_but, "right", 2)
        self.attachControl(self.dest_load_but, "top", 2, self.dest_att_field)
        self.attachNone(self.dest_load_but, "bottom")

        # /////////////
        self.attachNone(dry_but, "top")
        self.attachForm(dry_but, "left", 2)
        self.attachPosition(dry_but, "right", 2, 50)
        self.attachForm(dry_but, "bottom", 2)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, "left", 2, 50)
        self.attachForm(go_but, "bottom", 2)
        # /////////////

        self.attachForm(self.scroll, "left", 2)
        self.attachForm(self.scroll, "right", 2)
        self.attachControl(self.scroll, "top", 2, self.src_load_but)
        self.attachControl(self.scroll, "bottom", 2, go_but)

        self.scroll_form.attachForm(self.src_column, "left", 2)
        self.scroll_form.attachPosition(self.src_column, "right", 2, 50)
        self.scroll_form.attachForm(self.src_column, "top", 2 )
        self.scroll_form.attachForm(self.src_column, "bottom", 2)

        self.scroll_form.attachPosition(self.dest_column, "left", 2, 50)
        self.scroll_form.attachForm(self.dest_column, "right", 2)
        self.scroll_form.attachForm(self.dest_column, "top", 2)
        self.scroll_form.attachForm(self.dest_column, "bottom", 2)
 


    def _clear_entries(self, which):
        col = self.src_column if which == "src" else self.dest_column
        children = pm.columnLayout(col, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def load_col(self, which):
        self._clear_entries(which)

        col = self.src_column if which == "src" else self.dest_column
        pm.setParent(col)
        objects = sorted(pm.ls(sl=True))
        for o in objects:
            pm.nameField(o=o)

        atts = self.get_attribute_templates(o)

        popup = self.src_att_popup if which == "src" else self.dest_att_popup
        pm.popupMenu(popup, edit=True, deleteAllItems=True)
        pm.setParent(popup, menu=True)
        for att in atts:
            pm.menuItem(label=att,  command=pm.Callback(self.set_attr_template, att, which))

    def set_attr_template(self, att, which):
        field = self.src_att_field if which == "src" else self.dest_att_field
        pm.textField(field, edit=True, text=att)

    def load_src(self):
        self.load_col("src")

    def load_dest(self):
        self.load_col("dest")

    def get_nodes(self, which):
        col = self.src_column if which == "src" else self.dest_column
        return [pm.nameField(c, q=True, o=True) for c in pm.columnLayout(col, q=True, ca=True)]

    def get_connection_pairs(self):
        result = []
        src_template = pm.textField(self.src_att_field, q=True, text=True)
        dest_template = pm.textField(
            self.dest_att_field, q=True, text=True)

        src_offset = pm.intField(self.src_offset_field, q=True, value=True)
        dest_offset = pm.intField(self.dest_offset_field, q=True, value=True)

        src_nodes = self.get_nodes("src")
        dest_nodes = self.get_nodes("dest")
        slen = len(src_nodes)
        dlen = len(dest_nodes)
        if (not (dlen == slen or dlen == 1 or slen == 1)):
            raise ValueError(
                "Wrong number of nodes selected. Must be the same, or one column must have 1 node.")

        maxlen = max(dlen, slen)

        for i in range(maxlen):
            result.append((
                pm.PyNode(src_nodes[i % slen]).attr(
                    src_template.format(i+src_offset)),
                pm.PyNode(dest_nodes[i % dlen]).attr(
                    dest_template.format(i+dest_offset)))
            )
        return result

    def on_go(self):
        for p in self.get_connection_pairs():
            print p[0], ">>", p[1]
            p[0] >> p[1]

    def on_dry(self):
        for p in self.get_connection_pairs():
            print p[0], ">>", p[1]

    @staticmethod
    def get_attribute_templates(node):
        result = []
        attrs = pm.listAttr(node)
        print attrs
        print "-"*20
        for attr in attrs:
            try:
                attname = node.attr(attr).name()
                if node.attr(attr).isArray():
                    attname = "{}[0]".format(attname)
                attname = attname.partition(".")[2]

                if "[-1]" in attname:
                    attname = attname.replace("[-1]", "[{}]")

                result.append(attname)
                template = re.sub(r"\[.*?]", r"[{}]", attname)
                result.append(template)

            except (RuntimeError, pm.MayaAttributeError):
                pass
        return sorted(list(set(result)))
  