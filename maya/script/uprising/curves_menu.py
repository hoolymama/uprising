
import pymel.core as pm

from uprising import curve_utils as cutl


def create():
    menu = pm.menu(label="Curves", tearOff=True)
    pm.menuItem(
        label='Select strokes from curves',
        ann="Choose curves to select connected curveStrokes",
        command=pm.Callback(on_select_strokes))

    pm.menuItem(
        label='Select curves below',
        command=pm.Callback(on_select_curves_below))

    pm.menuItem(
        label='Duplicate curves with strokes',
        ann="Duplicate selected curves and add to selected painting node",
        command=pm.Callback(duplicate_curves_to_painting))

    pm.menuItem(
        label='Add curves to painting/collector in order',
        ann="Add selected curves to selected painting node or collectStrokes node. If they are already connected, disconnect and add in the order they are selected",
        command=pm.Callback(
            add_curves_to_strokes_att))

    pm.menuItem(
        label='Add new curves as clones',
        ann="Add selected curves to the painting, using another selected curve as stroke node source.",
        command=pm.Callback(
            add_curves_using_source_strokes))

    pm.menuItem(
        label='Reverse order of selected curves',
        command=pm.Callback(reverse_connection_order))

    pm.menuItem(
        label='Remove selected curves from painting',
        ann="Break selected curve connections",
        command=pm.Callback(on_remove_curve_instances))

    pm.menuItem(
        label='Remove curves from selected painting',
        ann="Break selected curve connections",
        command=pm.Callback(on_remove_strokes_from_selected_node))

    pm.menuItem(
        label='Connect curve visibility',
        ann="Connect curve visibility to stroke active",
        command=pm.Callback(on_connect_curve_vis_active))

    pm.menuItem(
        label='Disconnect curve visibility',
        ann="Disconnect curve visibility from stroke active",
        command=pm.Callback(on_disconnect_curve_vis_active))

    pm.menuItem(
        label='Remove unconnected curveStrokes',
        ann="Delete curveStrokes that are connected to selected curves but which have no destination painting",
        command=pm.Callback(
            on_remove_hanging_stroke_curves))

    pm.menuItem(
        label='Cleanup curve plugs',
        ann="Remove unconnected curveStroke plugs on selected painting",
        command=pm.Callback(
            on_cleanup_curve_plugs))

    pm.menuItem(
        label='Contain strokes to mesh',
        ann="Choose a mesh and either some curves or curveStrokes",
        command=pm.Callback(on_contain_strokes_in_mesh))

    return menu


def on_select_strokes():
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True,
        ut=True)
    pm.select(
        pm.listHistory(
            curves,
            future=True,
            levels=1,
            type="curveStroke"))


def on_select_curves_below():
    pm.select(pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True,
        ut=True))


def duplicate_curves_to_painting():

    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True)
    for curve in curves:
        cutl.duplicate_curve_to_painting(curve)


def add_curves_to_strokes_att():
    dest_node = pm.ls(selection=True, dag=True, leaf=True,
                      type=("painting", "collectStrokes"))[0]
    if not dest_node:
        raise IndexError("No painting or collectStrokes node selected")
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True)

    stroke_nodes = pm.listConnections(
        curves, d=True,
        s=False,
        type="curveStroke")

    painting_conns = pm.listConnections(
        stroke_nodes, d=True,
        s=False,
        type=("painting", "collectStrokes"),
        c=True, p=True)
    for conn in painting_conns:
        conn[0] // conn[1]

    for curve in curves:
        cutl.connect_curve_to_strokes_att(
            curve, dest_node, connect_to="next_available")


def add_curves_using_source_strokes():

    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type=("nurbsCurve"),
        ni=True)

    targets = curves[:-1]
    src = cutl.get_stroke_node(curves[-1])
    destination = cutl.get_strokes_destination(curves[-1])

    for curve in targets:

        curve_xf = curve.getParent()
        pm.xform(curve_xf, cpc=True)
        curve = curve_xf.getChildren()[0]
        cutl.connect_curve_using_src(curve, src, destination)

    cutl.do_rename_inputs('crv_%d', 'strk_%d', targets)


def reverse_connection_order():
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type=("nurbsCurve", "curveStroke"),
        ni=True)

    stroke_curves = pm.listConnections(
        curves, d=True,
        s=False,
        type="curveStroke")

    painting_conns = pm.listConnections(
        stroke_curves, d=True,
        s=False,
        type="painting",
        c=True, p=True)

    for conn in painting_conns:
        conn[0] // conn[1]

    c1, c2 = list(zip(*painting_conns))
    painting_conns = list(zip(list(c1), reversed(list(c2))))

    for conn in painting_conns:
        conn[0] >> conn[1]


def on_remove_curve_instances():

    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True)

    cutl.delete_curve_instances(curves)


def on_remove_strokes_from_selected_node():

    nodes = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type=["painting", "collectStrokes"])

    cutl.delete_strokes_from(nodes)


def on_connect_curve_vis_active():
    curves_xfs = pm.listRelatives(pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True), parent=True)
    cutl.curve_vis_active_connection(curves_xfs, True)


def on_disconnect_curve_vis_active():
    curves_xfs = pm.listRelatives(pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True), parent=True)
    cutl.curve_vis_active_connection(curves_xfs, False)


def on_remove_hanging_stroke_curves():
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True,
        ut=True,
        v=True)
    stroke_curves = pm.listConnections(
        curves, d=True, s=False, type="curveStroke")
    for sc in stroke_curves:
        paintings = pm.listConnections(
            sc.attr("output"), d=True, s=False, type="painting")
        if not paintings:
            pm.delete(sc)


def on_cleanup_curve_plugs():
    paintings = pm.ls(selection=True, type="painting")
    for p in paintings:
        cutl.remove_unconnected_curve_plugs(p)


def on_contain_strokes_in_mesh():
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True)

    meshes = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="mesh",
        ni=True)

    cutl.contain_strokes_in_mesh(curves, meshes[0])
