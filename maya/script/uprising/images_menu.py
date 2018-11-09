import os
import pymel.core as pm
import images


def create():
    menu = pm.menu(label="Images", tearOff=True)
    pm.menuItem(label="Write image palette",
                command=pm.Callback(write_png_palette_csv))
    pm.menuItem(
        label="Show in monitor",
        command=pm.Callback(show_image_in_monitor))

    return menu


def write_png_palette_csv():
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


def _make_and_connect_shader(attr):
    shader = pm.shadingNode("cImgShader", asTexture=True)
    tex = pm.shadingNode("place2dTexture", asUtility=True)
    tex.attr("outUV") >> shader.attr("uv")
    attr >> shader.attr("imageData")
    return shader

def _make_swatch_ui(swatches):
    win = pm.window(title="cImg Monitor")
    tabs = pm.tabLayout()
    for swatch in swatches:
        tab = pm.frameLayout(label=swatch["attr"])
        pm.tabLayout(tabs, e=True, tabLabel=(tab, swatch["attr"]) )
        pm.swatchDisplayPort(w=256, h=256, renderSize=256, sn=swatch["shader"])
        pm.setParent("..")
    win.show()
    win.setResizeToFitChildren()

def show_image_in_monitor():
    objects = pm.ls(sl=True)
    swatches = []
    for o in objects:
        attributes = ["output", "outIndexImage", "outColorImage", "outPalette"]
        for att in attributes:
            try:
                attr = o.attr(att)
                if attr.type() == "cImgData":
                    pack = {
                        "attr": attr,
                        "shader": None
                    }
                    shaders = pm.listConnections(
                        attr, d=True, s=False, type="cImgShader")
                    if shaders:
                        pack["shader"] = shaders[0]
                    else:
                        pack["shader"] = _make_and_connect_shader(attr)
                    swatches.append(pack)
            except (pm.MayaNodeError, pm.MayaAttributeError):
                pass
    _make_swatch_ui(swatches)
