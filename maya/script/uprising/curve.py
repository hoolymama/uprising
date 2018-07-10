import robolink
import pymel.core as pm
from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET
import robodk as rdk
import uprising.maya_util as uut

RL = Robolink()

def setup():
    cp = pm.ls(sl=True, dag=True, leaf=True, type="curvePoints")[0]
    name = cp.name()
    points = pm.PyNode(cp).attr("outPointsWorld").get()
    plug_ids = pm.PyNode("curvePointsShape1").attr("curves").getArrayIndices()

    frame = RL.AddFrame("%s ref" % name)
    frame.setPose(rdk.eye())
    frame.setVisible(False, visible_frame=False)

    index = 0
    for plug_id in plug_ids:
        new_points = []
        count = pm.PyNode(cp).attr("curves[%d].numPoints" % plug_id).get()
        for c in range(count):
            index+=1
            p = points[index]
            new_points.append([p.x*10, p.y*10, p.z*10, 0, -1, 0])

        object_curve = RL.AddCurve(new_points, frame, True)
        if plug_id == plug_ids[0]:
            object_curve.setName(name)
            path_settings = RL.AddMillingProject("%s settings" % name)
            prog, status = path_settings.setMillingParameters(part=object_curve)
 