from robolink import INSTRUCTION_INSERT_CODE


def send_lights_off(program, run_on_robot):
    """Trigger a lights-off instruction.
    """
    _send_light(program, 1, 0.0, run_on_robot, False)
    _send_light(program, 2, 0.0, run_on_robot, False)
    _send_light(program, 3, 0.0, run_on_robot, False)
    _send_light(program, 4, 0.0, run_on_robot, False)


def send_lights( program, run_on_robot,color,  last_color=None):
    if not last_color or last_color.r != color.r:
        _send_light(program, 1, color.r, run_on_robot)

    if not last_color or last_color.g != color.g:
        _send_light(program, 2, color.g, run_on_robot)

    if not last_color or last_color.b != color.b:
        _send_light(program, 3, color.b, run_on_robot)

    if not last_color or last_color.a != color.a:
        _send_light(program, 4, color.a, run_on_robot)


def _send_light(program, channel, value, run_on_robot, trigger=True):
    if run_on_robot:
        if trigger:
            channel = channel+10
        else:
            channel = channel+5
        program.setDO(channel, value)
        return

    if trigger:
        program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[{:d}]={:.3f}".format(channel, value), 
            INSTRUCTION_INSERT_CODE
        )
    else:
        program.RunInstruction(
            "$ANOUT[{:d}]={:.3f}".format(channel, value), 
            INSTRUCTION_INSERT_CODE
        )

def send_shutter(program, run_on_robot):
    """
    Send a shutter instruction.

    Shutter behaves like a toggle. If it's open, it closes, and vice versa. 
    """
    if run_on_robot:
        program.setDO(1, 1.0)
        program.Pause(1000)
        program.setDO(1, 0.0)
        return

    program.setDO(1, "TRUE")
    program.Pause(1000)
    program.setDO(1, "FALSE")

 