import os
import pymel.core as pm
import images
import write
import datetime


def create():
    menu = pm.menu(label="Images", tearOff=True)
    pm.menuItem(label="Write image palette",
                command=pm.Callback(write_png_palette_csv))
    pm.menuItem(
        label="Show in monitor",
        command=pm.Callback(show_image_in_monitor))

    pm.menuItem(
        label="Publish snapshot",
        command=pm.Callback(make_snapshot))

    return menu

def make_snapshot():
    res = 1024
    export_dir = os.path.join(pm.workspace.getPath(), 'export')
    entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                             dialogStyle=2, fileMode=3, dir=export_dir)
    if not entries:
        pm.displayWarning('Nothing Selected')
        return
    timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M%S')
    # write.write_ref_image(entries[0], timestamp, res)


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
    attr >> shader.attr("input")
    return shader

def select_cimg_node(attr):
    pm.select(pm.PyNode(attr).node())


def _make_swatch_ui(swatches):
    win = pm.window(title="cImg Monitor", widthHeight=(1536, 800))
    frame = pm.frameLayout(lv=False)
    scroll = pm.scrollLayout() 
    pm.rowColumnLayout(numberOfColumns=3, columnWidth=[(1, 512),(2 ,512),(3, 512)])

    for swatch in swatches:
        # frame = pm.frameLayout(lv=False, bv=True)
        pm.columnLayout(adj=True)
        pm.button(label="Select {}".format(swatch["attr"]),  command=pm.Callback(select_cimg_node, swatch["attr"])  )
        pm.swatchDisplayPort(w=512, h=512, renderSize=512, sn=swatch["shader"])
        pm.setParent("..")
    win.show()


def show_image_in_monitor():
    objects = pm.ls(sl=True)
    swatches = []
    for o in objects:
        for attName in  pm.listAttr(o, r=True):
            try:
                att = o.attr(attName)
                if att.type() == "cImgData":
                    pack = {
                        "attr": att,
                        "shader": None
                    }
                    shaders = pm.listConnections(
                        att, d=True, s=False, type="cImgShader")
                    if shaders:
                        pack["shader"] = shaders[0]
                    else:
                        pack["shader"] = _make_and_connect_shader(att)
                    swatches.append(pack)
            except:
                pass

    _make_swatch_ui(swatches)
