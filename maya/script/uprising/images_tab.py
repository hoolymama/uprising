
import os
import pymel.core as pm
import images
import pymel.core.uitypes as gui


class ImagesTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_butttons()
        self.create_action_buttons_and_layout()

    def create_butttons(self):
        pm.setParent(self.column)

        pm.button(
            label='Write palette CSV',
            ann="Write CSV file containing palette for a PNG file",
            command=pm.Callback(self.write_png_palette_csv))

    def create_action_buttons_and_layout(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel', manage=False)
        go_but = pm.button(label='Go', manage=False)

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

    def write_png_palette_csv(self):
        nodes = pm.ls(selection=True, type="cImgPngFile")
        if len(nodes):
            fn = pm.PyNode("cImgPngFile1").attr("imageFilename").get()
        else:
            images_dir = os.path.join(pm.workspace.getPath(), 'sourceimages')
            entries = pm.fileDialog2(
                caption="Choose PNG file",
                okCaption="Open",
                fileFilter="*.png",
                dialogStyle=2,
                fileMode=1,
                dir=images_dir)
            if not entries:
                pm.displayWarning('Nothing Selected')
                return
            else:
                fn = entries[0]
        print fn
        images.png_palette_for_sheets(fn)
