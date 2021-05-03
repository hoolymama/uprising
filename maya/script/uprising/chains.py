from uprising import utils

import pymel.core as pm


def chunkify_skels(chain_skel_pairs, max_chains):
    reset(chain_skel_pairs)
    num = len(chain_skel_pairs)

    for i, (chain, skel) in enumerate(chain_skel_pairs):
        print "{}/{} - Chunk connections for {}".format(
            (i+1), num, chain.name())

        chain.attr("maxChainsPerOutput").set(max_chains)
        count = chain.attr("outputCount").get()
        for j in range(count):
            try:
                chain.attr("outputs")[j] >> skel.attr(
                    "inputData")[j].attr("chains")
            except:
                pass

        skel.attr("selector").set(-1)
 


def reset(chain_skel_pairs):
    for chain, skel in chain_skel_pairs:
        for src, dest in chain.attr("outputs").connections(d=True, s=False, c=True, p=True):
            if src.index() != 0:
                if dest.node() == skel:
                    src // dest
                pm.removeMultiInstance(src, b=True)
                pm.removeMultiInstance(dest.parent(), b=True)
        chain.attr("maxChainsPerOutput").set(0)
        skel.attr("inputData")[0].attr("splitAngle").set(360)
        skel.attr("selector").set(0)


def get_chain_skel_pairs(*skels):
    result = []
    if not skels:
        skels = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    if not skels:
        skels = [n for n in pm.PyNode("mainPaintingShape").listHistory(
            type="skeletonStroke") if n.attr("nodeState").get() == 0]
    for skel in skels:
        chain = skel.attr("inputData[0].chains").connections(
            s=True, d=False)[0]
        result.append((chain, skel))
    return result


def count_outputs(chain_skel_pairs):
    return sum([chain.attr("outputCount").get() for chain, _ in chain_skel_pairs])
