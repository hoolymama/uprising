import pymel.core as pm

import brush_utils as butl

def up_foo_func(*args, **kw):

    print "hello foo"
    for arg in args:
        print arg

    for k in kw:
        print "%s : %s" % (k, kw[k])

def load_numbered_brush_pouch(*args, **kw):
    name = args[0]
    frame = int(kw["frame"])  

    painting_node = kw["painting_node"]
    dip_node = kw["dip_node"]
    

    pouch_name = "%s_%d" %(name, frame)
    print "-" * 20
    print  frame 
    print painting_node 
    print dip_node 
    print pouch_name 
    print "-" * 20

    butl.setup_brushes_from_sheet(painting_node, dip_node, pouch_name)
