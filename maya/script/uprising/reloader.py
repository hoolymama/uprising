
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
    pov_configurator
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
    brush_assign_tab,
    brush_hang_tab,
    connector_tab,
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
    pov_particles_menu,
    pov_publish_tab
)


####################
reload(configurator)
reload(brush)
reload(paint)
reload(target)
reload(bot_target)
reload(pov_target)
reload(stroke)
reload(bot_stroke)
reload(pov_stroke)
reload(cluster)
reload(painting)
reload(bot_painting)
reload(pov_painting)

reload(chains)
####################



####################
reload(program)
reload(cluster_test_program)
reload(bot_program)
reload(pov_program)


reload(calibration_program)
reload(board_calibration_program)
reload(holder_calibration_program)
reload(pot_calibration_program)
reload(rack_calibration_program)
reload(handle_calibration_program)

reload(kr8_track_program)

reload(brush_hang_program)
reload(dip_wipe_exercise_program)
reload(dip_wipe_program)

reload(manual_probe_program)
reload(pick_place_exercise_program)
reload(pick_place_program)
####################


####################
reload(session)

reload(calibration_session)
reload(holder_calibration_session)
reload(board_calibration_session)
reload(pot_calibration_session)
reload(rack_calibration_session)
reload(handle_calibration_session)

reload(kr8_track_session)

reload(bot_painting_session)
reload(pov_session)
reload(dip_wipe_exercise_session)
reload(pick_place_exercise_session)

reload(brush_hang_session)
reload(dip_wipe_exercise_session)
reload(gamut_session)
reload(manual_probe_session)
reload(pick_place_exercise_session)
reload(retries_session)
####################


####################
reload(brush_assign_tab)
reload(brush_hang_tab)
reload(connector_tab)
reload(brush_test_tab)
reload(gamut_tab)
reload(painting_tab)
reload(phex_tab)
reload(probes_tab)
reload(publish_tab)
reload(rings_design_tab)
reload(rings_setup_tab)
reload(pov_publish_tab)


####################

####################
reload(calibration_menu)
reload(chain_menu)
reload(curves_menu)
reload(images_menu)
reload(select_menu)
reload(tools_menu)
reload(props_menu)
reload(pov_particles_menu)
####################

####################
reload(calibration)
reload(const)
reload(images)
reload(progress)
reload(props)
reload(robo)
reload(sequence)
reload(stats)
####################

####################
reload(brush_utils)
reload(curve_utils)
reload(maya_util)
reload(palette_utils)
reload(stroke_factory_utils)
reload(utils)
####################
