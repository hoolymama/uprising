
from uprising.session import (

    board_calibration_program,
    brush_hang_program,
    calibration_program,
    holder_calibration_program,
    kr8_track_program,
    manual_probe_program,
    pot_calibration_program,
    handle_calibration_program,
    rack_calibration_program,

    board_calibration_session,
    brush_hang_session,
    calibration_session,
    gamut_session,
    holder_calibration_session,
    kr8_track_session,
    manual_probe_session,
    pot_calibration_session,
    handle_calibration_session,
    rack_calibration_session,
)

from uprising.bot.session import (
    cluster,
    bot_painting,
    bot_program,
    cluster_test_program,
    bot_stroke,
    bot_target,
    retries_session,
    dip_wipe_exercise_program,
    dip_wipe_exercise_session,
    dip_wipe_program,
    pick_place_exercise_program,
    pick_place_exercise_session,
    pick_place_program,
    bot_painting_session,
    configurator
)

from uprising.pov.session import (
    pov_target,
    pov_stroke,
    pov_painting,
    pov_program,
    pov_session,
    pov_configurator,
    pov_lights
)

from uprising.common.session import (
    painting,
    program,
    session,
    stroke,
    target
)

from uprising import (
    brush,
    brush_hang_tab,
    brush_test_tab,
    brush_utils,
    calibration,
    calibration_menu,
    chains,
    chain_menu,
    const,
    curve_utils,
    curves_menu,
    gamut_tab,
    images,
    images_menu,
    maya_util,
    paint,
    painting_tab,
    palette_utils,
    phex_tab,
    segment_tab,
    probes_tab,
    progress,
    props,
    props_menu,
    publish_tab,
    rings_design_tab,
    rings_setup_tab,
    robo,
    select_menu,
    sequence,
    stats,
    stroke_factory_utils,
    tools_menu,
    utils,
    kr8_menu,
    pov_publish_tab,
    online,
    persist_ui
    
)

 
import importlib


####################
importlib.reload(pov_lights)
importlib.reload(configurator)
importlib.reload(pov_configurator)

importlib.reload(brush)
importlib.reload(paint)
importlib.reload(target)
importlib.reload(bot_target)
importlib.reload(pov_target)
importlib.reload(stroke)
importlib.reload(bot_stroke)
importlib.reload(pov_stroke)
importlib.reload(cluster)
importlib.reload(painting)
importlib.reload(bot_painting)
importlib.reload(pov_painting)

importlib.reload(chains)
####################



####################
importlib.reload(program)
importlib.reload(cluster_test_program)
importlib.reload(bot_program)
importlib.reload(pov_program)


importlib.reload(calibration_program)
importlib.reload(board_calibration_program)
importlib.reload(holder_calibration_program)
importlib.reload(pot_calibration_program)
importlib.reload(rack_calibration_program)
importlib.reload(handle_calibration_program)

importlib.reload(kr8_track_program)

importlib.reload(brush_hang_program)
importlib.reload(dip_wipe_exercise_program)
importlib.reload(dip_wipe_program)

importlib.reload(manual_probe_program)
importlib.reload(pick_place_exercise_program)
importlib.reload(pick_place_program)
####################


####################
importlib.reload(session)

importlib.reload(calibration_session)
importlib.reload(holder_calibration_session)
importlib.reload(board_calibration_session)
importlib.reload(pot_calibration_session)
importlib.reload(rack_calibration_session)
importlib.reload(handle_calibration_session)

importlib.reload(kr8_track_session)

importlib.reload(bot_painting_session)
importlib.reload(pov_session)
importlib.reload(dip_wipe_exercise_session)
importlib.reload(pick_place_exercise_session)

importlib.reload(brush_hang_session)
importlib.reload(dip_wipe_exercise_session)
importlib.reload(gamut_session)
importlib.reload(manual_probe_session)
importlib.reload(pick_place_exercise_session)
importlib.reload(retries_session)
####################


####################
# importlib.reload(brush_assign_tab)
importlib.reload(brush_hang_tab)
importlib.reload(brush_test_tab)
importlib.reload(gamut_tab)
importlib.reload(painting_tab)
importlib.reload(phex_tab)
importlib.reload(segment_tab)
importlib.reload(probes_tab)
importlib.reload(publish_tab)
importlib.reload(rings_design_tab)
importlib.reload(rings_setup_tab)
importlib.reload(pov_publish_tab)


####################

####################
importlib.reload(calibration_menu)
importlib.reload(chain_menu)
importlib.reload(curves_menu)
importlib.reload(images_menu)
importlib.reload(select_menu)
importlib.reload(tools_menu)
importlib.reload(props_menu)
importlib.reload(kr8_menu)
####################

####################
importlib.reload(calibration)
importlib.reload(const)
importlib.reload(images)
importlib.reload(progress)
importlib.reload(props)
importlib.reload(robo)
importlib.reload(sequence)
importlib.reload(stats)
####################

####################
importlib.reload(brush_utils)
importlib.reload(curve_utils)
importlib.reload(maya_util)
importlib.reload(palette_utils)
importlib.reload(stroke_factory_utils)
importlib.reload(utils)
importlib.reload(persist_ui)
####################



importlib.reload(online)
