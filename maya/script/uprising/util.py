"""one_line_doc_string."""
import os
import re
import pymel.core as pm
import datetime
import uprising.const as k

TOKEN_REGEX = re.compile(r'[a-zA-Z]+')
PLUGINDEX_REGEX = re.compile(r'.*\[([0-9])+\].*')
AE_TEXT_WIDTH = 145
AE_SINGLE_WIDTH = 70
AE_TOTAL_WIDTH = AE_TEXT_WIDTH + (AE_SINGLE_WIDTH * 3)




def convert_objects():
    sel = pm.ls(sl=True, )


    print("CONVERT")




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
# Also on completion, an email is sent to the artist with a link to a URL that displays the render stats in graphical chart form.
