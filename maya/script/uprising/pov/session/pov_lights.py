from robolink import INSTRUCTION_INSERT_CODE


def send_lights_off(program, dmx, run_on_robot):
    """Trigger a lights-off instruction."""
    _send_light(program, dmx, 1, 0.0, run_on_robot, False)
    _send_light(program, dmx, 2, 0.0, run_on_robot, False)
    _send_light(program, dmx, 3, 0.0, run_on_robot, False)
    _send_light(program, dmx, 4, 0.0, run_on_robot, False)


def send_lights(program, dmx, run_on_robot, color, last_color=None):
    """Send RGBA color to a DMX box.

    Args:
        program: RoboDK program
        dmx: ID of the DMX box (0-5)
        run_on_robot: Whether to run through the robot driver, or write an offline program
        color: RGBA color
        last_color: Previous color - no need to set color for a bulb that doesn't change. Defaults to None.
    """
    if not last_color or last_color.r != color.r:
        _send_light(program, dmx, 1, color.r, run_on_robot)

    if not last_color or last_color.g != color.g:
        _send_light(program, dmx, 2, color.g, run_on_robot)

    if not last_color or last_color.b != color.b:
        _send_light(program, dmx, 3, color.b, run_on_robot)

    if not last_color or last_color.a != color.a:
        _send_light(program, dmx, 4, color.a, run_on_robot)


def _send_light(program, dmx, channel, value, run_on_robot, trigger=True):
    # NOTE: NEED TO REWRITE FOR RUN ON ROBOT!!!
    
    channel = channel + (dmx * 4)
    if run_on_robot:
        trigger_offset = 24 if trigger else 0
        channel = channel + trigger_offset
        program.setDO(channel, value)
        return
    
    # Not running on robot
    if trigger:

        program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[{:d}]={:.3f}".format(channel, value),
            INSTRUCTION_INSERT_CODE,
        )
    else:
        program.RunInstruction(
            "$ANOUT[{:d}]={:.3f}".format(channel, value), INSTRUCTION_INSERT_CODE
        )


def send_shutter(program, run_on_robot):
    """
    Send a shutter instruction.

    Shutter behaves like a toggle. If it's open, it closes, and vice versa.
    """
    if run_on_robot:
        program.setDO(49, 1.0)
        program.Pause(1000)
        program.setDO(49, 0.0)
        return

    program.setDO(1, "TRUE")
    program.Pause(1000)
    program.setDO(1, "FALSE")
