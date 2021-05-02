
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
    skels = pm.PyNode("mainPaintingShape").listHistory(type="skeletonStroke")
    chains.reset(chains.get_chain_skel_pairs(*skels))
