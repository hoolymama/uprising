"""one_line_doc_string."""
import os
import pymel.core as pm
import json



def save_stl(node, filename):
    p = node.getParent()
    grp = pm.group(em=True)
    pm.parent(node, grp, relative=True)

    grp.attr("s").set(10, 10, 10)
    pm.select(grp, r=True)
    pm.exportSelected(filename, force=True, type="STL_ATF Export")
    pm.parent(node, p, relative=True)
    pm.delete(grp)


def get_robodk_mat(node, space="object"):
    """Get transposed mat with translate in mm."""
    if space == "object":
        att = "matrix"
    else:
        att = "worldMatrix"
    mat = node.attr(att).get()
    mat = mat.transpose()
    mat = [list(row) for row in mat]
    mat[0][3] = mat[0][3] * 10.0
    mat[1][3] = mat[1][3] * 10.0
    mat[2][3] = mat[2][3] * 10.0
    return mat


def shape_color(shape):
    try:
        shaderGroup = pm.listConnections(
            shape.attr("instObjGroups"), d=True, s=False)[0]
        surfAttr = pm.PyNode(shaderGroup).attr("surfaceShader")
        shader = pm.listConnections(surfAttr, d=False, s=True)[0]
        color = shader.attr("color").get()
    except BaseException:
        color = tuple((1.0, 1.0, 1.0))
    return color



def obj_color(node):
    try:
        shape = node.getShapes()[0]
        shaderGroup = pm.listConnections(
            shape.attr("instObjGroups"), d=True, s=False)[0]
        surfAttr = pm.PyNode(shaderGroup).attr("surfaceShader")
        shader = pm.listConnections(surfAttr, d=False, s=True)[0]
        color = shader.attr("color").get()
    except BaseException:
        color = tuple((1.0, 1.0, 1.0))
    return color


def convert_object(node, directory):
    name = node.name()
    shapes = node.getShapes()
    if shapes:
        # Its either mesh or locator

        if isinstance(shapes[0], pm.nodetypes.Locator):
            return {
                "name": name,
                "type": "locator",
                "matrix": get_robodk_mat(node),
                "worldMatrix": get_robodk_mat(node, "world")
            }

        filename = os.path.join(directory, "%s.stl" % name)
        save_stl(node, filename)
        return {
            "name": name,
            "type": "stl",
            "filename": filename,
            "color": obj_color(node)
        }
    else:
        return {
            "name": name,
            "type": "grp",
            "matrix": get_robodk_mat(node),
            "children": [
                convert_object(
                    child, directory) for child in node.getChildren()]}


def do_convert_objects(json_filename):
    transforms = pm.ls(sl=True, transforms=True)
    for tf in transforms:
        pm.makeIdentity(tf, apply=True, t=False, r=False, s=True, pn=True)

    directory = os.path.dirname(json_filename)
    hierarchy = [convert_object(tf, directory) for tf in transforms]
    with open(json_filename, 'w') as outfile:
        json.dump(hierarchy, outfile, sort_keys=True, indent=4)
    pm.select(transforms)


def convert_objects():

    d = os.path.join(pm.workspace(q=True, rd=True), "data")
    cap = 'Save to STL Hierarchy'
    ok = 'Save'
    entries = pm.fileDialog2(
        caption=cap,
        okCaption=ok,
        fileFilter='*.json',
        dialogStyle=2,
        fileMode=0,
        dir=d)
    if not entries:
        pm.displayWarning('Nothing Selected')
        return
    json_fn = entries[0]
    if not json_fn.endswith(".json"):
        json_fn = "%s.json" % json_fn

    do_convert_objects(json_fn)


def insert_identity():
    transforms = pm.ls(sl=True)
    shapes = pm.ls(
        sl=True,
        dag=True,
        shapes=True,
        v=True,
        ut=True,
        io=False,
        type="mesh")
    for shape in shapes:
        p = shape.getParent()
        grp = pm.group(em=True, n="x_%s" % p.name())
        pm.parent(shape, grp, relative=True, shape=True)
        pm.parent(grp, p, relative=True)

    for tf in transforms:
        pm.makeIdentity(tf, apply=True, t=False, r=False, s=True, pn=True)

    pm.select(transforms)


