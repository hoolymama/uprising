import pymel.core as pm
from uprising import chains
from uprising import utils
from uprising import const as k


def create():

    menu = pm.menu(label="Chains", tearOff=True)

    pm.menuItem(label="Split outputs", command=pm.Callback(on_split_outputs))
    pm.menuItem(label="Reset", command=pm.Callback(on_reset))
    pm.menuItem(label="Create SK Graph Nodes", command=pm.Callback(create_skgraphs))

    pm.menuItem(label="Print connection stats", command=pm.Callback(chains.on_print_stats))

    return menu


def on_split_outputs():

    choice = pm.promptDialog(
        title="Max chains",
        message="Enter Max chains per output:",
        button=["Cancel", "OK"],
        defaultButton="OK",
        cancelButton="Cancel",
        dismissString="Cancel",
    )

    if choice == "OK":
        max_chains = int(pm.promptDialog(query=True, text=True))
        chain_skel_pairs = utils.get_chain_skel_pairs()
        chains.chunkify_skels(chain_skel_pairs, max_chains)
        return
    print("Operation cancelled.")


def on_reset():
    skels = pm.PyNode(k.PAINTING_NAME).listHistory(type="skeletonStroke")
    utils.reset_skels(utils.get_chain_skel_pairs(*skels))

def create_skgraphs():
    chains = pm.ls(sl=True, type="skChainNode")
    for chain in chains:
        result = []
        graphs = pm.listHistory(chain, future=True, type="skGraph")
        suffix=str(chain).partition("_")[2]
        if not graphs:
            graph = pm.createNode("skGraph")
        else:
            graph = graphs[0]
        chain.attr("outputs")[0] >> graph.attr("chains")
        graph.attr("drawColor1") >> graph.attr("drawColor2")
        graph.attr("drawColor1").set(1,0,0)
        graph.attr("drawCircles").set(False)
        tf = graph.getParent()
        pm.rename(tf,"skGraph_{}".format(suffix))
        pm.rename(graph,"skGraph_{}Shape".format(suffix))
        if not tf.getParent():
            pm.parent(tf, "skGraphs", r=True)
        result.append(graph)
        
    pm.select(result)
          