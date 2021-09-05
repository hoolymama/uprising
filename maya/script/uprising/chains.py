from uprising import utils

import pymel.core as pm
from cioseq.sequence import Sequence
from uprising import utils

def chunkify_skels(chain_skel_pairs, max_chains):
    utils.reset_skels(chain_skel_pairs)
    num = len(chain_skel_pairs)

    for i, (chain, skel) in enumerate(chain_skel_pairs):
        print "{}/{} - Chunk connections for {}".format((i + 1), num, chain.name())

        chain.attr("maxChainsPerOutput").set(max_chains)
        count = chain.attr("outputCount").get()
        for j in range(count):
            try:
                chain.attr("outputs")[j] >> skel.attr("inputData")[j].attr("chains")
            except:
                pass

        skel.attr("selector").set(-1)


# def reset(chain_skel_pairs):
#     for chain, skel in chain_skel_pairs:
#         for dest, src in skel.attr("inputData").connections(s=True, d=False, c=True, p=True):
#             if src.index() != 0:
#                 try:
#                     src // dest
#                 except RuntimeError:
#                     pass
#         # remove all inactive muklti instances
#         for plug in skel.attr("inputData"):
#             if not plug.chains.isConnected():
#                 pm.removeMultiInstance(plug, b=True)

#         for plug in chain.attr("outputs"):
#             if not plug.isConnected():
#                 pm.removeMultiInstance(plug, b=True)

#         chain.attr("maxChainsPerOutput").set(0)
#         skel.attr("inputData")[0].attr("splitAngle").set(360)
#         skel.attr("inputData")[0].attr("active").set(True)
#         skel.attr("selector").set(0)


# def get_chain_skel_pairs(*skels):
#     result = []
#     if not skels:
#         skels = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
#     if not skels:
#         skels = [
#             n
#             for n in pm.PyNode("mainPaintingShape").listHistory(type="skeletonStroke")
#             if n.attr("nodeState").get() == 0
#         ]
#     for skel in skels:
#         chain = skel.attr("inputData[0].chains").connections(s=True, d=False)[0]
#         result.append((chain, skel))
#     return result


def count_outputs(chain_skel_pairs):
    return sum([chain.attr("outputCount").get() for chain, _ in chain_skel_pairs])


def on_print_stats():
    chain_skel_pairs = utils.get_chain_skel_pairs()
  
    result = []
    for chain, skel in chain_skel_pairs:
        active_indices = []
        inactive_indices = []

        # chain.attr("outputs").getArrayIndices()
        # indices = chain.attr("outputs").get(multiIndices=True)
        indices = range(chain.attr("outputs").getNumElements())
        for i in indices:
            if skel.attr("inputData")[i].attr("active").get():
                active_indices.append(i)
            else:
                inactive_indices.append(i)

        active_seq = None
        inactive_seq = None
        if active_indices:
            active_seq = Sequence.create(active_indices)
        if inactive_indices:
            inactive_seq = Sequence.create(inactive_indices)

        result.append(
            {
                "src": str(chain),
                "dest": str(skel),
                "active": str(active_seq),
                "inactive": str(inactive_seq),
                "total": len(indices),
                "num_active": len(active_indices),
                "num_inactive": len(inactive_indices),
                "ratio": "{:02f} / {:02f}".format(
                    len(active_indices) / float(len(indices)),
                    len(inactive_indices) / float(len(indices)),
                ),
                "max_chains_per_output": chain.attr("maxChainsPerOutput").get(),
                "selector": skel.attr("selector").get(),
            }
        )
    utils.show_in_window(result, title="Chain stats")