# # import math
# # import pymel.core as pm
# import logging
# logger = logging.getLogger('uprising')


# from robolink import (
#     Robolink,
#     ITEM_TYPE_ROBOT,
#     INSTRUCTION_COMMENT,
#     INSTRUCTION_SHOW_MESSAGE
# )

# # import uprising_util as uutl
# from program import Program, ProgramError
# import uprising_util as uutl
# import const as k
# # import brush_utils as butl
# # import palette_utils as putl
# # from brush import Brush


# class PickPlaceProgram(Program):

#     @classmethod
#     def generate_program_name(cls, brush_id):
#         if cls.__name__ == "PickProgram":
#             return "pick_{:02d}".format(brush_id)
#         return "place_{:02d}".format(brush_id)

#     def __init__(self, brush, pack):
#         name = PickPlaceProgram.generate_program_name(pack["brush_id"])
#         super(PickPlaceProgram, self).__init__(name)

#         self.RL = Robolink()
#         self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
#         self.brush = brush
#         self.pack = pack
#         self.targets = {}

#     def write(self):
#         logger.debug("PickPlaceProgram WRITE")
#         frame_name = "pick_place_{:02d}_frame".format(self.pack["brush_id"])
#         self.frame = uutl.create_frame(frame_name, False)

#         self.brush.write()
#         self.tool = self.RL.Item(self.brush.name)
#         if not self.tool.Valid():
#             raise ProgramError(
#                 "No Gripper Brush. Risk of damage. Can't continue.")

#         self.program.setSpeed(self.pack["lin_speed"], self.pack["ang_speed"])
#         self.program.setRounding(self.pack["rounding"])
#         self.program.setPoseTool(self.tool)

#         self.pin_target = self._create_target_from_pack("pin")
#         self.pin_ap_target = self._create_target_from_pack("pin_ap")
#         self.clear_target = self._create_target_from_pack("clear")
#         self.clear_ap_target = self._create_target_from_pack("clear_ap")


#     def _create_target_from_pack(self, key):
#         return self._create_a_target( 
#             self.pack[key].attr("worldMatrix[0]").get(),
#             "pick_place_{}_{:02d}_base".format( key, self.pack["brush_id"]),
#             k.FACING_RACK_JOINTS,
#             False)

#     def _create_a_target(self, mat, name, facing_joints, force=True):
#         target = self.RL.Item(name)
#         if target.Valid():
#             if force:
#                 target.Delete()
#             else:
#                 return target

#         tool_pose = uutl.maya_to_robodk_mat(mat)
#         flange_pose = tool_pose * self.brush.matrix.invH()
#         target = self.RL.AddTarget(name, self.frame, self.robot)
#         joints = self.robot.SolveIK(flange_pose, facing_joints)
#         target.setPose(tool_pose)
#         target.setJoints(joints)
#         return target

# class PickProgram(PickPlaceProgram):

#     def __init__(self, brush, pack):
#         print "TYPE SELF", type(self)
#         print "self.__class__.__name__",  self.__class__.__name__
#         # print "self.__class__.__name__",  super.__class__.__name__
             
#         super(PickProgram, self).__init__(brush, pack)

#     def write(self, studio):
#         logger.debug("PickProgram WRITE")
#         self.program = uutl.create_program(self.program_name)
#         self.program.ShowInstructions(False)
#         with uutl.minimize_robodk():
#             super(PickProgram, self).write()
     
#             self.program.RunInstruction( "Starting pick", INSTRUCTION_COMMENT)
#             self.program.addMoveJ(self.pin_ap_target)
#             self.program.addMoveL(self.pin_target)
#             self.program.Pause()
#             self.program.RunInstruction( "Gripper closes here",  INSTRUCTION_SHOW_MESSAGE)
#             # close command
#             # wait for closed
#             self.program.addMoveL(self.clear_target)
#             self.program.addMoveL(self.clear_ap_target)
     


# class PlaceProgram(PickPlaceProgram):

#     def __init__(self, gripper, pack):
#         print "TYPE SELF", type(self)
#         super(PlaceProgram, self).__init__(gripper, pack)


#     def write(self, studio):
#         logger.debug("PlaceProgram WRITE")
#         self.program = uutl.create_program(self.program_name)
#         self.program.ShowInstructions(False)
#         with uutl.minimize_robodk():
#             super(PlaceProgram, self).write()
     
#             self.program.RunInstruction( "Starting place", INSTRUCTION_COMMENT)
#             self.program.addMoveJ(self.clear_ap_target)
#             self.program.addMoveL(self.clear_target)
#             self.program.Pause()
#             self.program.addMoveL(self.pin_target)
#             self.program.RunInstruction( "Gripper opens here",  INSTRUCTION_SHOW_MESSAGE)
#             # close command
#             # wait for closed
#             self.program.addMoveL(self.pin_ap_target)
     

