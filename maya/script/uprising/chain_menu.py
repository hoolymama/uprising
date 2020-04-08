
import pymel.core as pm

def create():
    menu = pm.menu(label="Chains", tearOff=True)

    pm.menuItem(label="Split outputs",
                command=pm.Callback(on_split_outputs))
    pm.menuItem(
        label="Reset outputs",
        command=pm.Callback(on_reset_outputs))

    pm.menuItem(
        label="Convert old scene",
        command=pm.Callback(on_convert_old_scene))
 

    return menu


def on_split_outputs():
    chains = get_chain_nodes()

    pm.promptDialog(
        title='Max chains',
        message='Enter Max chains per output:',
        button=['OK'],
        defaultButton='OK')

    max_chains = int(pm.promptDialog(query=True, text=True))
    fan_chain_nodes(chains, max_chains)


def on_reset_outputs():
    chains = get_chain_nodes()
    for chain in chains:
        reset(chain)

def on_convert_old_scene():
    for node in pm.ls(type="skChainNode"):
        dests = node.attr("output").connections(d=True, s=False,p=True)
        for dest in dests:
            if dest.attrName(longName=True) == "chains":
                node.attr("outputs")[0] >> dest

def get_chain_nodes():
    sel = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    if not sel:
        sel = pm.ls(type="skeletonStroke")
    return list(set(pm.listHistory(sel, lv=1, type="skChainNode")))


def fan_chain_nodes(chain_nodes, max_chains=0):
    num = len(chain_nodes)

    for i, node in enumerate(chain_nodes):
        print "{}/{} - Fanning outputs for {}".format((i+1), num, node.name())
        reset(node)
  

        skel_zero = node.attr("outputs")[0].connections(d=True, s=False)[0]
        skel_zero.attr("splitAngle").set(360)

        collector = skel_zero.attr("output").connections(d=True, s=False, type="collectStrokes")[0]
        strokes_attr = collector.attr("strokes")

        node.attr("maxChainsPerOutput").set(max_chains)
        count = node.attr("outputCount").get()
        for j in range(1, count):
            skel_n = pm.duplicate(skel_zero, rr=True, ic=True)[0]
            skel_n.rename("{}_{:02d}".format(skel_zero, j))
            strokes_plug = next_plug(strokes_attr) 
            skel_n.attr("output") >> strokes_plug 
            # node.attr("outputs")[0] // skel_n.attr("chains")[0]
            node.attr("outputs")[j] >> skel_n.attr("chains")[0]


# def delete_extras(node):
#     for src, dest in node.attr("outputs").connections(d=True, s=False, c=True, p=True):
#         if src.index() is not 0:
#             pm.delete(dest.node())

#     for att in node.attr("outputs"):
#         if att.index() is not 0:
#             pm.removeMultiInstance(att, b=True)


def reset(node):
    node.attr("maxChainsPerOutput").set(0)
    for src, dest in node.attr("outputs").connections(d=True, s=False, c=True, p=True):
        if src.index() is not 0:
            pm.delete(dest.node())

    for att in node.attr("outputs"):
        if att.index() is not 0:
            pm.removeMultiInstance(att, b=True)


def next_plug(attr):
    indices = attr.getArrayIndices()
    next_available = indices[-1] + 1 if indices else 0
    return attr[next_available]