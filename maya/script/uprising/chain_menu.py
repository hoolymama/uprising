
import pymel.core as pm
from uprising import chains


def create():

    menu = pm.menu(label="Chains", tearOff=True)

    pm.menuItem(label="Split outputs",
                command=pm.Callback(on_split_outputs))
    pm.menuItem(
        label="Reset",
        command=pm.Callback(on_reset))
    return menu


def on_split_outputs():

    choice = pm.promptDialog(
        title='Max chains',
        message='Enter Max chains per output:',
        button=["Cancel", "OK"],
        defaultButton='OK',
        cancelButton="Cancel",
        dismissString="Cancel"
    )

    if choice == 'OK':
        max_chains = int(pm.promptDialog(query=True, text=True))
        chain_skel_pairs = chains.get_chain_skel_pairs()
        chains.chunkify_skels(chain_skel_pairs, max_chains)
        return
    print "Operation cancelled."


def on_reset():
    chains.reset(chains.get_chain_skel_pairs())

# def chunkify_skels(chain_skel_pairs, max_chains=0):
#     num = len(chain_skel_pairs)

#     for i, (chain, skel) in enumerate(chain_skel_pairs):
#         print "{}/{} - Fanning outputs for {}".format((i+1), num, chain.name())

#         chain.attr("maxChainsPerOutput").set(max_chains)
#         count = chain.attr("outputCount").get()
#         for j in range(1, count):
#             chain.attr("outputs")[j] >> skel.attr("inputData")[j].attr("chains")
#             skel.attr("inputData")[j].attr("splitAngle").set(360)


# def reset(chain_skel_pairs):
#     for chain, skel in chain_skel_pairs:
#         for src, dest in chain.attr("outputs").connections(d=True, s=False, c=True, p=True):
#             if src.index() != 0:
#                 if dest.node() == skel:
#                     src // dest
#                 pm.removeMultiInstance(src, b=True)
#                 pm.removeMultiInstance(dest.parent(), b=True)
#     chain.attr("maxChainsPerOutput").set(0)

# def get_chain_skel_pairs():
#     result = []
#     skels = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
#     if not skels:
#         skels = pm.ls(type="skeletonStroke")
#     for skel in skels:
#         chain = skel.attr("inputData[0].chains").connections(s=True, d=False)[0]
#         result.append((chain, skel))
#     return result


# def on_convert_old_scene():
#     for node in pm.ls(type="skChainNode"):
#         dests = node.attr("output").connections(d=True, s=False, p=True)
#         for dest in dests:
#             if dest.attrName(longName=True) == "chains":
#                 node.attr("outputs")[0] >> dest

# def get_chain_nodes():
#     sel = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
#     if not sel:
#         sel = pm.ls(type="skeletonStroke")
#     return list(set(pm.listHistory(sel, lv=1, type="skChainNode")))


# def next_plug(attr):
#     indices = attr.getArrayIndices()
#     next_available = indices[-1] + 1 if indices else 0
#     return attr[next_available]
