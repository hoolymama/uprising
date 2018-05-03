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


def obj_color(node):
    try:
        shape = node.getShapes()[0]
        shaderGroup = pm.listConnections(
            shape.attr("instObjGroups"), d=True, s=False)[0]
        surfAttr = pm.PyNode(shaderGroup).attr("surfaceShader")
        shader = pm.listConnections(surfAttr, d=False, s=True)[0]
        color = shader.attr("color").get()
    except BaseException:
        color = tuple(1.0, 1.0, 1.0)
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


# def setup():
#     """one_line_doc_string."""
#     pm.createNode('renderDispatcher')


# def layer_name(layer):
#     """one_line_doc_string."""
#     if layer == 'defaultRenderLayer':
#         return 'masterLayer'
#     return layer


# def maya_attr_name(prefix, opt):
#     """one_line_doc_string."""
#     return '%s%s%s' % (prefix, opt[0].upper(), opt[1:])


# def get_filename_template():
#     """one_line_doc_string."""
#     fn_template = pm.PyNode('defaultRenderGlobals').imageFilePrefix.get()
#     default_template = (
#         "<Scene>/<RenderLayer>/<RenderPass>/<Scene>_<RenderLayer>.<RenderPass>"
#         )
#     return fn_template or default_template

# def set_user_render_filerules():
#     """add user directory to renders paths"""
#     user = os.getenv('USER')
#     for rule in ["images","renders","image","renderScenes"]:
#         pm.workspace.fileRules[rule] = "renders/%s" % user

# def template_has_token(token):
#     """one_line_doc_string."""
#     return get_filename_template().find(token) != -1


# def renderer():
#     """one_line_doc_string."""
#     return pm.PyNode('defaultRenderGlobals').currentRenderer.get()


# def make_directory_for(filename):
#     """Ensure directory exists for given filename."""
#     directory = os.path.dirname(filename)
#     if not os.path.exists(directory):
#         os.makedirs(directory)


# def save_dispatcher_node(node):
#     """
#     Export a dispatcher node as a preset.

#     In order to have only the dispatcher in the
#     resulting file and no other crap, the node is
#     first exported to a temp location and then
#     filtered to extract the dispatcher before
#     saving in the preset directory.
#     """
#     start_dir = pm.internalVar(userPresetsDir=True)
#     single_filter = "Maya ASCII (*.ma)"
#     filename = pm.fileDialog2(
#         fileFilter=single_filter,
#         dialogStyle=2, fileMode=0, dir=start_dir)
#     try:
#         filename = filename[0]
#         pm.select(node)

#         tmp_dir = pm.internalVar(userTmpDir=True)
#         tmp_name = datetime.datetime.now().strftime('%Y_%m_%d_%H_%M_%S')
#         tmp_name = os.path.join(
#             tmp_dir, "renderDispatcherPreset%s.ma" % tmp_name)

#         pm.exportSelected(
#             tmp_name, force=True, constructionHistory=False,
#             channels=False, constraints=False, expressions=True,
#             shader=False, preserveReferences=False, type="mayaAscii")

#         out = open(filename, "w")

#         shortname = os.path.split(filename)[-1]

#         out.write('//Maya ASCII 2015 scene;\n')
#         out.write('//Name: %s;\n' % shortname)
#         out.write('//Codeset: UTF-8;\n')
#         out.write('requires maya "2012";\n')
#         out.write('requires "Uprising.py" "Unknown";\n')
#         out.write('fileInfo "application" "maya";;\n')

#         with open(tmp_name) as in_file:
#             found_dispatcher = False
#             for line in in_file:
#                 stripped_line = line.strip()
#                 if stripped_line.startswith("createNode renderDispatcher"):
#                     found_dispatcher = True
#                 elif found_dispatcher == True and ( not line.startswith("\t") ):
#                     break
#                 if found_dispatcher is True:
#                     out.write(line)
#             out.write("//End of %s\n" % shortname)
#             out.close()
#         print "Saved: %s" % filename
#     except:
#         print "Couldn't Write File"


# def import_dispatcher_node(central=False):
#     """Import a saved dispatcher."""
#     start_dir = pm.internalVar(userPresetsDir=True)
#     if central:
#         start_dir = os.path.dirname(
#             os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
#         start_dir = os.path.join(start_dir, "presets")
#     single_filter = "Maya ASCII (*.ma)"
#     filename = pm.fileDialog2(
#         fileFilter=single_filter, dialogStyle=2, fileMode=1, dir=start_dir)
#     try:
#         filename = filename[0]
#         new_nodes = pm.importFile(filename, returnNewNodes=True)
#         dispatcher_node = None
#         for node in new_nodes:
#             if pm.nodeType(node) == "renderDispatcher":
#                 dispatcher_node = node
#                 break
#         if dispatcher_node:
#             pm.select(dispatcher_node)
#             pm.mel.openAEWindow()
#             print "Loaded: %s" % filename
#         else:
#             print "Couldn't find renderDispatcher node in %s" % filename
#     except:
#         print "No File Chosen"


# def add_uprising_token_attribute(node):

#     result = pm.promptDialog(
#         title='Token Name',
#         message='Enter Name:',
#         button=['OK', 'Cancel'],
#         defaultButton='OK',
#         cancelButton='Cancel',
#         dismissString='Cancel')

#     if result == 'OK':
#         text = pm.promptDialog(query=True, text=True)

#         if text in k.K_TOKEN_NAMES:
#             pm.error("Invalid Token Name: %s is builtin")
#         if re.match(TOKEN_REGEX, text):
#             attr_name = "utoken_%s" % text
#             pm.addAttr(node, ln=attr_name, dt="string")
#         else:
#             pm.error("Invalid Token Name: %s doesn't match regex: %s" % (text, TOKEN_REGEX))

# def flatten_unicode_keys(d):
#     for k in d.keys():
#         if isinstance(k, unicode):
#             v = d[k]
#             del d[k]
#             d[str(k)] = v


# Dispatch arnold assExport and render tasks.
# Render logs are saved alongside the ass files during drendering and compiled into a single json log when the render is complete.
# Also on completion, an email is sent to the artist with a link to a URL
# that displays the render stats in graphical chart form.
