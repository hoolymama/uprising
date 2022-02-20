import os
import pymel.core as pm
import images


def create():
    pm.menu(label="Images", tearOff=True)
    pm.menuItem(label="Write image palette",
                command=pm.Callback(write_png_palette_csv))
    pm.menuItem(
        label="Show in monitor",
        command=pm.Callback(show_image_in_monitor))


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
    print( fn)
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

    menuBarLayout = pm.menuBarLayout()

    frame = pm.frameLayout(lv=False)
    pm.scrollLayout()
    gridlayout = pm.gridLayout(numberOfColumns=2, cellWidth=512,cellHeight=512)
    _add_to_swatch_ui(gridlayout, swatches)

    # for swatch in swatches:
    #     pm.columnLayout(adj=True)
    #     pm.button(label="Select {}".format(swatch["attr"]),  command=pm.Callback(
    #         select_cimg_node, swatch["attr"]))
    #     pm.swatchDisplayPort(w=512, h=512, renderSize=512, sn=swatch["shader"])
    #     pm.setParent("..")


    pm.setParent(menuBarLayout)
    edit_menu  = pm.menu(label="Edit")
    pm.menuItem(label="Swatch resolution",subMenu=True)
    pm.menuItem( label="512", command=pm.Callback( on_edit_swatch_res, gridlayout, 512))
    pm.menuItem( label="384", command=pm.Callback( on_edit_swatch_res, gridlayout, 384))
    pm.menuItem( label="256", command=pm.Callback( on_edit_swatch_res, gridlayout, 256))
    pm.setParent("..", menu=True)

    pm.menuItem(label="Number of Columns",subMenu=True)
    # pm.menuItem( label="1", command=pm.Callback( on_edit_num_columns, gridlayout, 1))
    pm.menuItem( label="2", command=pm.Callback( on_edit_num_columns, gridlayout, 2))
    pm.menuItem( label="3", command=pm.Callback( on_edit_num_columns, gridlayout, 3))
    pm.menuItem( label="4", command=pm.Callback( on_edit_num_columns, gridlayout, 4))
    pm.menuItem( label="5", command=pm.Callback( on_edit_num_columns, gridlayout, 5))
    pm.menuItem( label="6", command=pm.Callback( on_edit_num_columns, gridlayout, 6))
    pm.setParent("..", menu=True)

    pm.menuItem(label="Add Selected", command=pm.Callback( on_add_selected, gridlayout))

    win.show()

def _add_to_swatch_ui(gridlayout, swatches):
    pm.setParent(gridlayout)
    for swatch in swatches:
        col = pm.columnLayout(adj=True)
        pm.rowLayout( nc=2, adjustableColumn=1, cw2=(150, 50))
        pm.button(label="Select {}".format(swatch["attr"]),  command=pm.Callback(
            select_cimg_node, swatch["attr"]))
        pm.button(label="Remove" ,command=pm.Callback( on_remove_swatch, col))
        pm.setParent(col)
        pm.swatchDisplayPort(w=512, h=512, renderSize=512, sn=swatch["shader"])
        pm.setParent("..")

def on_remove_swatch(col):
    pm.deleteUI(col)

def on_add_selected(gridlayout):
    swatches = get_swatches_for_selected()
    _add_to_swatch_ui(gridlayout, swatches)

def on_edit_swatch_res( gridlayout, res):
    imgcols = pm.gridLayout(gridlayout, query=True, childArray=True)
    for col in imgcols:
        _,displayPort = pm.columnLayout(col, query=True, childArray=True)
        pm.swatchDisplayPort(displayPort, edit=True, w=res, h=res, renderSize=res)
    pm.gridLayout(gridlayout, edit=True , cellWidth=res, cellHeight=res)

def on_edit_num_columns( gridlayout, num):
    pm.gridLayout(gridlayout, edit=True , numberOfColumns=num )


def show_image_in_monitor():
    swatches = get_swatches_for_selected()
    _make_swatch_ui(swatches)


def get_swatches_for_selected():

    objects = pm.ls(sl=True)
    swatches = []
    for o in objects:
        for attName in pm.listAttr(o, r=True):
            atts = [o.attr(attName)]
            try:
                atts += atts[0].iterDescendants(None, True)
            except BaseException:
                pass

            for d in atts:
                if d.type() == "cImgData":
                    pack = {
                        "attr": d,
                        "shader": None
                    }
                    shaders = pm.listConnections(
                        d, d=True, s=False, type="cImgShader")
                    if shaders:
                        pack["shader"] = shaders[0]
                    else:
                        pack["shader"] = _make_and_connect_shader(d)
                    swatches.append(pack)
    return swatches
