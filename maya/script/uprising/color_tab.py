# import datetime
# import os
# import tarfile
# import time
# from contextlib import contextmanager

# import calibration as cal
# import progress
import pymel.core as pm
import pymel.core.uitypes as gui
# import robo
# import stats
# import uprising_util as uutl
# import write
# from studio import Studio



class ColorTab(gui.FormLayout):


    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.color_frame_ui = self.color_frame()
        # self.export_frame = self.create_export_frame()
        # self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()
        # self.on_ops_change()

    def color_frame(self):
        frame = pm.frameLayout(label="Colors", bv=True)

        self.base_color_wg = pm.colorSliderGrp(label="Base Color", rgb=(1,0,0) )

        self.base_pot_id_wg = pm.intSlider(label="Base Pot ID", value1=5, minValue=0, maxValue=9)

        self.passes_wg = pm.checkBoxGrp(numberOfCheckBoxes=3, label="Passes", valueArray3=(0,1,1))

        self.spread_direction_wg = pm.radioButtonGrp(
            height=30, l="Spread direction", sl=3, nrb=3, la3=["Down", "Up", "Both"]
        )

        self.spread_distance_wg = pm.intSlider(label="Spread Distance", value1=2, minValue=0, maxValue=10)


        self.sample_probability_power_wg = pm.floatSlider(label="Sample Probablility Falloff", value=0, minValue=0, maxValue=1)


        self.hsv_deviation_wg = pm.floatSlider(label="Hue Deviation", value=0, minValue=0, maxValue=360)
        self.sat_deviation_wg = pm.floatSlider(label="Sat Deviation", value=0, minValue=0, maxValue=1)
        self.val_deviation_wg = pm.floatSlider(label="Val Deviation", value=0, minValue=0, maxValue=1)

        self.cumulative_dev_wg = pm.checkBoxGrp(numberOfCheckBoxes=1, label="Cumulative Deviation", value1=True)






        # with uutl.activatable(): 
        #     self.max_chains_if = pm.intFieldGrp(
        #         numberOfFields=1, label="Max chains/node", value1=50
        #     )

 
        # self.retries_mode_rb = pm.radioButtonGrp(
        #     height=30,
        #     l="Retries mode",
        #     sl=4,
        #     nrb=4,
        #     la4=["None", "Current first", "All", "Coil"],
        #     ann=ann,
        #     changeCommand=pm.Callback(self.on_ops_change),
        # )

        # self.read_board_calibration_cb = pm.checkBoxGrp(
        #     label="Read Calibration", label1="Board", value1=1
        # )

        # # self.varying_attrib_wg = pm.textFieldGrp(label="Attribute", text="splitAngle")

        # self.coil_delta_ff = pm.floatFieldGrp(
        #     numberOfFields=1, label="Delta", value1=20
        # )


        # self.varying_range_wg = pm.floatFieldGrp(
        #     numberOfFields=2, label="Range", value1=270, value2=0.0
        # )

        # self.num_retries_wg = pm.intFieldGrp(
        #     label="Attempts", numberOfFields=1, value1=12
        # )

        # # self.retries_scope_rb = pm.radioButtonGrp(
        # #     height=30, l="Retries scope", sl=2, nrb=2, la2=["Selected", "All"]
        # # )

        pm.setParent("..")
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachForm(go_but, "left", 2)
        self.attachForm(go_but, "bottom", 2)
        return go_but


    ##############################################################

    def on_go(self):

        base_color =  pm.colorSliderGrp(self.base_color_wg, query=True, rgb=True)
        base_pot_id = pm.intFieldGrp(self.base_pot_id_wg,query=True, value1=True)
        passes = pm.checkBoxGrp(self.passes_wg, query=True, valueArray3=True)
 
        
    
        self.spread_direction_wg = pm.radioButtonGrp(
            height=30, l="Spread direction", sl=3, nrb=3, la3=["Down", "Up", "Both"]
        )

        self.spread_distance_wg = pm.intFieldGrp(label="Spread Distance", value1=2)


        self.sample_probability_power_wg = pm.floatFieldGrp(label="Sample Probablility Falloff", value1=0)


        self.hue_deviation_wg = pm.floatFieldGrp(label="Hue Deviation", value1=0)
        self.sat_deviation_wg = pm.floatFieldGrp(label="Sat Deviation", value1=0)
        self.val_deviation_wg = pm.floatFieldGrp(label="Val Deviation", value1=0)

        self.cumulative_dev_wg = pm.checkBoxGrp(numberOfCheckBoxes=1, label="Cumulative Deviation", value1=True)


        




        print base_color 
        print base_pot_id
        print passes





        # do_painting, do_subprograms = pm.checkBoxGrp(
        #     self.export_ops_cb, query=True, valueArray2=True
        # )
        # retries_mode = pm.radioButtonGrp(self.retries_mode_rb, query=True, sl=True)
        # # retries_scope = pm.radioButtonGrp(self.retries_scope_rb, query=True, sl=True)
        # retries_count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
        # first, last = pm.floatFieldGrp(self.varying_range_wg, query=True, value=True)

        # should_do_retries = (
        #     not (do_subprograms and not do_painting)
        # ) and retries_mode > 1

        # retries_attribute = "splitAngle"
        # delta = pm.floatFieldGrp(self.coil_delta_ff, query=True, value1=True)

        # # pm.textFieldGrp(
        # #     self.varying_attrib_wg, query=True, text=True
        # # )

        # read_board_cal = pm.checkBoxGrp(
        #     self.read_board_calibration_cb, query=True, value1=True
        # )

        # water_wipe_repeats = pm.intSliderGrp(
        #     self.water_wipe_repeats_isg, query=True, value=True
        # )

        # cluster_chunk_size = pm.intFieldGrp(
        #     self.cluster_chunk_if, query=True, value1=True
        # )

        # directory = write.choose_publish_dir()
        # if not directory:
        #     return

        # timestamp = write.get_timestamp()
        # directory = os.path.join(directory, timestamp)

        # uutl.mkdir_p(directory)


        # total_timer_start = time.time()

        # if should_do_retries:
            
        #     if read_board_cal:
        #         cal.read_board_calibration()

        #     nodes = find_contributing_skel_nodes()


        #     step_values = get_step_values(first, last, retries_count)

        #     retries_result = do_retries(
        #         nodes, retries_attribute, retries_mode, step_values, delta
        #     )

        #     uutl.show_in_window(retries_result, title="Retries results")
        #     write.json_report(directory, "retries", retries_result)

        #     status = "SUCCESS" if retries_result["success"] else "FAILURE"
        #     time_str = str(datetime.timedelta(seconds=int(retries_result["timer"])))
        #     progress.update(
        #         major_line="Retries completed in {} : {}".format(time_str, status),
        #         minor_line="",
        #         major_progress=0,
        #         minor_progress=0,
        #     )

        # if do_painting:
        #     write.publish_painting(directory, cluster_chunk_size=cluster_chunk_size)

        # if do_subprograms:
        #     write.publish_pick_place(directory)
        #     write.publish_dips(directory, water_wipe_repeats)

        # if do_painting or do_subprograms:
        #     src_folder = os.path.join(directory, "src")
        #     with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
        #         tar.add(src_folder, arcname=os.path.sep)

        #     progress.update(
        #         header="Export completed",
        #         major_line="",
        #         minor_line="",
        #         major_progress=0,
        #         minor_progress=0,
        #     )

        # delta = int(time.time() - total_timer_start)

        # total_time_str = str(datetime.timedelta(seconds=delta))
        # progress.update(
        #     major_line="Total time: {}".format(total_time_str),
        #     minor_line="",
        #     major_progress=0,
        #     minor_progress=0,
        # )
        # ptg_stats = stats.stats()
        # all_stats = {
        #     "painting_stats":ptg_stats,
        #     "per_brush_stats": stats.stats_per_brush()
        # }
        # ptg_stats = stats.stats()
        # uutl.show_in_window(all_stats, title="Painting stats")
        # write.json_report(directory, "stats", all_stats)
        # write.write_maya_scene(directory, "scene")
        # write.session_entry(directory, timestamp, ptg_stats)


# def do_retries(nodes, attribute, retries_mode, step_values, delta):
#     try_existing = retries_mode == 2
#     coil = retries_mode == 4

#     timer_start = time.time()
#     validate_retries_params(attribute, nodes)

#     plugs = [pm.PyNode(node).attr(attribute) for node in nodes]

#     num_plugs = len(plugs)
#     all_skels = pm.ls(type="skeletonStroke")
#     results = []
#     robo.new()
#     progress.update(
#         major_max=num_plugs, header="Running {:d} Retries".format(num_plugs)
#     )

#     for plug_index, plug in enumerate(plugs):
#         node = plug.node()

#         with isolate_nodes([node], all_skels):
#             if coil:
#                 result = do_coil_retries_for_plug(plug_index, num_plugs, plug, delta)
#             else:
#                 result = do_retries_for_plug(
#                     plug_index, num_plugs, plug, step_values, try_existing
#                 )
#             results.append(result)

#     timer_end = time.time()
#     result_data = {
#         "results": results,
#         "success": all([res["solved"] for res in results]),
#         "timer": timer_end - timer_start,
#     }

#     robo.close()
#     return result_data


# def do_retries_for_plug(plug_index, num_plugs, plug, step_values, try_existing):
#     result = {
#         "plug": str(plug),
#         "attempts": -1,
#         "path_results": [],
#         "solved": False,
#         "frame": plug_index,
#         "timer": 0,
#     }

#     # print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
#     painting_node = pm.PyNode("mainPaintingShape")
#     try:
#         pm.paintingQuery(painting_node, cc=True)
#     except RuntimeError as ex:
#         pm.displayInfo(ex.message)
#         result["solved"] = True
#         return result

#     if try_existing:
#         existing = plug.get()
#         values = [existing] + [v for v in step_values if v < existing]
#     else:
#         values = step_values

#     plug_start = time.time()

#     progress.update(major_progress=plug_index)

#     num_values = len(values)
#     for i, value in enumerate(values):
#         plug.set(value)
#         pm.refresh()

#         progress.update(
#             major_line="Retrying plug: {} - {:d}/{:d}, Iteration:{:d}/{:d} Value:{:f}".format(
#                 plug, (plug_index + 1), num_plugs, i + 1, num_values, value
#             )
#         )

#         iter_start = time.time()

#         robo.new()

#         program = Studio(do_painting=True).painting_program

#         if not program and program.painting and program.painting.clusters:
#             continue

#         program.write()

#         path_result = program.validate_path() or {"status": "SUCCESS"}

#         path_result.update(
#             {"iteration": i, "value": value, "timer": time.time() - iter_start}
#         )

#         result["path_results"].append(path_result)

#         if path_result["status"] == "SUCCESS":
#             result["attempts"] = i + 1
#             result["solved"] = True
#             break

#     result["timer"] = time.time() - plug_start
#     return result


# def do_coil_retries_for_plug(plug_index, num_plugs, plug, delta):
#     result = {
#         "plug": str(plug),
#         "attempts": -1,
#         "path_results": [],
#         "solved": False,
#         "frame": plug_index,
#         "timer": 0,
#     }

#     # print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
#     painting_node = pm.PyNode("mainPaintingShape")
#     try:
#         pm.paintingQuery(painting_node, cc=True)
#     except RuntimeError as ex:
#         pm.displayInfo(ex.message)
#         result["solved"] = True
#         return result

#     plug_start = time.time()

#     progress.update(major_progress=plug_index)

#     i = 0
#     values = []
#     plug.set(360)
#     while True:
#         value = plug.get()
#         values.append(value)

#         pm.refresh()

#         progress.update(
#             major_line="Plug: {} - {:d}/{:d}, Attempt:{:d} Vals:{}".format(
#                 plug,
#                 (plug_index + 1),
#                 num_plugs,
#                 i + 1,
#                 (",").join(["{:.1f}".format(v) for v in values]),
#             )
#         )

#         iter_start = time.time()

#         robo.new()

#         program = Studio(do_painting=True).painting_program
#         if program and program.painting and program.painting.clusters:

#             program.write()

#             path_result = program.validate_path() or {"status": "SUCCESS"}

#             path_result.update(
#                 {"iteration": i, "value": value, "timer": time.time() - iter_start}
#             )

#             result["path_results"].append(path_result)

#             if path_result["status"] == "SUCCESS":
#                 result["attempts"] = i + 1
#                 result["solved"] = True
#                 break

#             # Here if we need to try again
#             i += 1
#             coil = pm.PyNode(plug).node().attr("outCoil").get()
#             next_val = min(coil, value) - delta
#             print "Coil: {}, Value: {}, Next: {}".format(coil, value, next_val)
#             if next_val < 0:
#                 break
#             plug.set(next_val)

#     result["timer"] = time.time() - plug_start
#     return result


# def validate_retries_params(attribute, nodes):
#     if len(nodes) == 0:
#         pm.error("No nodes selected. Aborting!")
#     if any([n.type() != "skeletonStroke" for n in nodes]):
#         confirm = pm.confirmDialog(
#             title="Confirm",
#             message="Selected nodes are not skeletonStrokes. Continue? ",
#             button=["Yes", "No"],
#             defaultButton="Yes",
#             cancelButton="No",
#             dismissString="No",
#         )
#         if confirm == "No":
#             pm.error("User aborted")

#     for node in nodes:
#         plug = pm.PyNode(node).attr(attribute)
#         if plug.get(lock=True):
#             pm.error("{} is locked. Can't adjust.".format(plug))
#         if plug.inputs():
#             pm.error("{} has input connections. Can't adjust.".format(plug))


# def get_step_values(low, high, count):
#     if count < 2:
#         return [high]
#     vals = []
#     valrange = high - low
#     gap = valrange / (count - 1)
#     vals = [low + (gap * x) for x in range(count)]
#     return vals

#     def populate(self):
#         pass

#     def save(self):
#         pass

# def find_contributing_skel_nodes():


#     painting_node = pm.PyNode("mainPaintingShape")
#     all_skels = pm.ls(type="skeletonStroke")
 
#     result = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"] 
#     if result:
#         return result

#     for node in all_skels:
#         with isolate_nodes([node], all_skels):
#             try:
#                 pm.paintingQuery(painting_node, cc=True)
#             except RuntimeError:
#                 continue
#         result.append(node)
#     print "{} of {} skeleton nodes contributing".format(len(result), len(all_skels))
#     return result




    # def create_export_frame(self):

    #     frame = pm.frameLayout(label="Export", bv=True)

    #     try:
    #         pm.Attribute("mainPaintingShape.subject")
    #     except BaseException:
    #         try:
    #             pm.PyNode("mainPaintingShape").addAttr("subject", dataType="string")
    #         except BaseException:
    #             pass

    #     try:
    #         self.subject_field = pm.attrControlGrp(
    #             attribute="mainPaintingShape.subject"
    #         )
    #     except BaseException:
    #         pass

    #     self.export_ops_cb = pm.checkBoxGrp(
    #         numberOfCheckBoxes=2,
    #         label="Export",
    #         labelArray2=("Painting", "Subprograms"),
    #         valueArray2=(1, 1),
    #         changeCommand=pm.Callback(self.on_ops_change),
    #     )

    #     self.cluster_chunk_if = pm.intFieldGrp(
    #         height=30,
    #         label="Max clusters/file",
    #         annotation="Max number of clusters per painting partial program",
    #         numberOfFields=1,
    #         value1=100,
    #     )

    #     self.water_wipe_repeats_isg = pm.intSliderGrp(
    #         label="Water wipe repeats",
    #         height=30,
    #         field=True,
    #         minValue=1,
    #         maxValue=5,
    #         fieldMinValue=1,
    #         fieldMaxValue=5,
    #         value=2,
    #         annotation="How many times to repeat the whole wipe pattern after a water dip.",
    #     )
    #     pm.setParent("..")
    #     return frame

    # def create_progress_frame(self):
    #     frame = pm.frameLayout(
    #         bv=True, collapse=False, labelVisible=True, label="Progress"
    #     )
    #     col = pm.columnLayout(adj=True)
    #     progress.create(col)
    #     return frame

    # def on_ops_change(self):
    #     do_painting, do_subprograms = pm.checkBoxGrp(
    #         self.export_ops_cb, query=True, valueArray2=True
    #     )
    #     can_do_retries = not (do_subprograms and not do_painting)
    #     pm.frameLayout(self.retries_frame, edit=True, en=can_do_retries)
    #     pm.intFieldGrp(self.cluster_chunk_if, edit=True, en=do_painting)
    #     pm.intSliderGrp(self.water_wipe_repeats_isg, edit=True, en=do_subprograms)
    #     retries_mode = pm.radioButtonGrp(self.retries_mode_rb, query=True, sl=True)


    #     coil = (retries_mode==4)
 
    #     do_varying_range = (not coil) and can_do_retries
 

    #     pm.floatFieldGrp(self.varying_range_wg, edit=True, en=do_varying_range)
    #     pm.intFieldGrp(self.num_retries_wg, edit=True, en=do_varying_range)
        
    #     pm.floatFieldGrp(self.coil_delta_ff, edit=True, en=(coil and can_do_retries))
    #     # pm.radioButtonGrp(self.retries_scope_rb, edit=True, en=(can_do_retries))
    #     pm.checkBoxGrp(self.read_board_calibration_cb, edit=True, en=(can_do_retries))

    #     can_go = (
    #         (can_do_retries and (retries_mode>1)) or do_painting or do_subprograms
    #     )
    #     pm.button(self.go_but, edit=True, en=can_go)
