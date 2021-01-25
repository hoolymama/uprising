
import pymel.core as pm
import uprising.const as k
from uprising.session.program import Program, ProgramError
from uprising import robo

import uprising.maya_util as mutil

import robodk as rdk
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)
LINEAR_SPEED = 10 # mm
ANGULAR_SPEED = 20 # deg/sec
ROUNDING_DISTANCE = 20 #mm
HOME_JOINTS = [0, -90, 90, 0, 0, 0]


class Kr8TrackProgram(Program):
    def __init__(self, name):
        super(Kr8TrackProgram, self).__init__(name)
        self.tool = None
        self.locators = pm.ls("CAMERA_TRACKING|targets|*")
        self.tool_transform = pm.PyNode("CAMERA_TRACKING|plate")
        self.tool_matrix = robo.maya_to_robodk_mat(pm.dt.Matrix(
            pm.xform(self.tool_transform, q=True, ws=True,   m=True)))

        self.tool_mesh = pm.PyNode("CAMERA_TRACKING|plate|plateShape")
        self.tool_color = mutil.shape_color(self.tool_mesh)
        self.tool_name = "fbx_0_plate"

    def send(self):
 
        super(Kr8TrackProgram, self).send()

        self.tool = self.send_tool()
        if not self.tool.Valid():
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")

        self.program.setSpeed(LINEAR_SPEED, ANGULAR_SPEED)
        self.program.setRounding(ROUNDING_DISTANCE)
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction(
            "Starting track move", INSTRUCTION_COMMENT)

        self.send_locators()

        self.program.addMoveJ(robo.home_approach)

    def send_tool(self):

        link = robo.link()
        robot = robo.robot()

        tool_item = robot.AddTool(self.tool_matrix, self.tool_name)

        points = self.tool_mesh.getPoints(space="world")

        _, vids = self.tool_mesh.getTriangles()
        triangles = []
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10]
            )

        shape = link.AddShape(triangles)
        shape.setColor(list(self.tool_color))
        tool_item.AddGeometry(shape, rdk.eye())
        shape.Delete()
        return tool_item

    def send_locators(self):
        for i, locator in enumerate(self.locators):
            name = locator.name()

            self.program.RunInstruction(
                "Moving to locator {} {}".format(i, name),
                INSTRUCTION_COMMENT
            )

            target = self._create_a_target(
                locator.attr("worldMatrix[0]").get(),
                "track_{}".format(name)
            )
            self.program.addMoveJ(target)

    def _create_a_target(self, mat, name):
        robot = robo.robot()
        link = robo.link()
        tool_pose = robo.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.tool_matrix.invH()
        print "ADDING JOINT TARGET: ", name
        target = link.AddTarget(name, robo.calibration_frame, robot)
        joints = robot.SolveIK(flange_pose, HOME_JOINTS)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target
 