//Maya ASCII 2015 scene
//Name: renderDispatcher.ma
//Last modified: Sun, Aug 17, 2014 09:40:43 PM
//Codeset: UTF-8
requires maya "2015";
requires -nodeType "renderDispatcher" "Uprising.py" "2015.dev.999";
requires -dataType "bulletSolverData" -dataType "bulletRigidBodyData" -dataType "bulletSoftBodyData"
		 -dataType "bulletSoftConstraintData" -dataType "BulletRigidBodyConstraintData" -dataType "bulletColliderData"
		 -dataType "bulletRigidInitialStateGeometry" -dataType "bulletMotionsStates" -dataType "bulletCollisionShapes"
		 "bullet" "May 19 2014";
currentUnit -l centimeter -a degree -t pal;
fileInfo "application" "maya";
fileInfo "product" "Maya 2015";
fileInfo "version" "2015";
fileInfo "cutIdentifier" "201405190330-916664";
fileInfo "osv" "Linux 2.6.32-358.23.2.el6.mpc07.x86_64 #1 SMP Tue Oct 22 10:10:30 BST 2013 x86_64";
createNode renderDispatcher -n "renderDispatcher1";
	addAttr -ci true -sn "docMeta_Author" -ln "docMeta_Author" -dt "string";
	addAttr -ci true -sn "docMeta_Email" -ln "docMeta_Email" -dt "string";
	addAttr -ci true -sn "docMeta_Keywords" -ln "docMeta_Keywords" -dt "string";
	addAttr -ci true -sn "docMeta_TestScene" -ln "docMeta_TestScene" -dt "string";
	addAttr -ci true -sn "docMeta_Description" -ln "docMeta_Description" -dt "string";
	addAttr -ci true -sn "docBlock_Synopsis" -ln "docBlock_Synopsis" -dt "string";
	addAttr -ci true -sn "docAttr_FrameRange" -ln "docAttr_FrameRange" -dt "string";
	addAttr -ci true -sn "docAttr_CustomFrameRange" -ln "docAttr_CustomFrameRange" -dt "string";
	addAttr -ci true -sn "docAttr_CustomFrameRange0" -ln "docAttr_CustomFrameRange0" 
		-dt "string";
	addAttr -ci true -sn "docAttr_CustomFrameRange1" -ln "docAttr_CustomFrameRange1" 
		-dt "string";
	addAttr -ci true -sn "docAttr_CustomFrameRange2" -ln "docAttr_CustomFrameRange2" 
		-dt "string";
	addAttr -ci true -sn "docAttr_CustomFrameList" -ln "docAttr_CustomFrameList" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpSize" -ln "docAttr_ClumpSize" -dt "string";
	addAttr -ci true -sn "docAttr_FrameRangePolicy" -ln "docAttr_FrameRangePolicy" -dt "string";
	addAttr -ci true -sn "docAttr_RenderLayers" -ln "docAttr_RenderLayers" -dt "string";
	addAttr -ci true -sn "docAttr_RenderSceneFilename" -ln "docAttr_RenderSceneFilename" 
		-dt "string";
	addAttr -ci true -sn "docAttr_TractorSpoolFilename" -ln "docAttr_TractorSpoolFilename" 
		-dt "string";
	addAttr -ci true -sn "docAttr_TractorExecutable" -ln "docAttr_TractorExecutable" 
		-dt "string";
	addAttr -ci true -sn "docAttr_Priority" -ln "docAttr_Priority" -dt "string";
	addAttr -ci true -sn "docAttr_CustomPriority" -ln "docAttr_CustomPriority" -dt "string";
	addAttr -ci true -sn "docAttr_JobInit" -ln "docAttr_JobInit" -dt "string";
	addAttr -ci true -sn "docAttr_JobInitKey" -ln "docAttr_JobInitKey" -dt "string";
	addAttr -ci true -sn "docAttr_JobInitValue" -ln "docAttr_JobInitValue" -dt "string";
	addAttr -ci true -sn "docAttr_JobMetadata" -ln "docAttr_JobMetadata" -dt "string";
	addAttr -ci true -sn "docAttr_JobMetadataKey" -ln "docAttr_JobMetadataKey" -dt "string";
	addAttr -ci true -sn "docAttr_JobMetadataValue" -ln "docAttr_JobMetadataValue" -dt "string";
	addAttr -ci true -sn "docAttr_JobTitle" -ln "docAttr_JobTitle" -dt "string";
	addAttr -ci true -sn "docAttr_JobAfter" -ln "docAttr_JobAfter" -dt "string";
	addAttr -ci true -sn "docAttr_JobAtleast" -ln "docAttr_JobAtleast" -dt "string";
	addAttr -ci true -sn "docAttr_JobAtmost" -ln "docAttr_JobAtmost" -dt "string";
	addAttr -ci true -sn "docAttr_JobTags" -ln "docAttr_JobTags" -dt "string";
	addAttr -ci true -sn "docAttr_JobService" -ln "docAttr_JobService" -dt "string";
	addAttr -ci true -sn "docAttr_JobEnvkey" -ln "docAttr_JobEnvkey" -dt "string";
	addAttr -ci true -sn "docAttr_JobEtalevel" -ln "docAttr_JobEtalevel" -dt "string";
	addAttr -ci true -sn "docAttr_JobComment" -ln "docAttr_JobComment" -dt "string";
	addAttr -ci true -sn "docAttr_JobEditpolicy" -ln "docAttr_JobEditpolicy" -dt "string";
	addAttr -ci true -sn "docAttr_JobCleanup" -ln "docAttr_JobCleanup" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskTitle" -ln "docAttr_ClumpTaskTitle" -dt "string";
	addAttr -ci true -sn "docAttr_JobPostscript" -ln "docAttr_JobPostscript" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskChaser" -ln "docAttr_ClumpTaskChaser" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskPreview" -ln "docAttr_ClumpTaskPreview" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskService" -ln "docAttr_ClumpTaskService" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskId" -ln "docAttr_ClumpTaskId" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskSerialsubtasks" -ln "docAttr_ClumpTaskSerialsubtasks" 
		-dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskCmd" -ln "docAttr_ClumpTaskCmd" -dt "string";
	addAttr -ci true -sn "docAttr_ClumpTaskCleanup" -ln "docAttr_ClumpTaskCleanup" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskTitle" -ln "docAttr_FrameTaskTitle" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskChaser" -ln "docAttr_FrameTaskChaser" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskPreview" -ln "docAttr_FrameTaskPreview" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskService" -ln "docAttr_FrameTaskService" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskId" -ln "docAttr_FrameTaskId" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskSerialsubtasks" -ln "docAttr_FrameTaskSerialsubtasks" 
		-dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskCmd" -ln "docAttr_FrameTaskCmd" -dt "string";
	addAttr -ci true -sn "docAttr_FrameTaskCleanup" -ln "docAttr_FrameTaskCleanup" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskTitle" -ln "docAttr_PrepTaskTitle" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskChaser" -ln "docAttr_PrepTaskChaser" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskPreview" -ln "docAttr_PrepTaskPreview" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskService" -ln "docAttr_PrepTaskService" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskId" -ln "docAttr_PrepTaskId" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskSerialsubtasks" -ln "docAttr_PrepTaskSerialsubtasks" 
		-dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskCmd" -ln "docAttr_PrepTaskCmd" -dt "string";
	addAttr -ci true -sn "docAttr_PrepTaskCleanup" -ln "docAttr_PrepTaskCleanup" -dt "string";
	setAttr ".fr" 0;
	setAttr ".cfr" -type "long3" 1 10 1 ;
	setAttr ".cpri" 10;
	setAttr ".jtt" -type "string" "<layer>_<shortname>_<framespec>_${USER}";
	setAttr ".frttl" -type "string" "task_<framefour>";
	setAttr ".docMeta_Author" -type "string" "Julian Mann";
	setAttr ".docMeta_Email" -type "string" "julian-m@moving-picture.com";
	setAttr ".docMeta_Keywords" -type "string" "";
	setAttr ".docMeta_TestScene" -type "string" "";
	setAttr ".docMeta_Description" -type "string" "Dispatch renders from Maya to Pixar's Tractor";
	setAttr ".docBlock_Synopsis" -type "string" "RenderDispatcher is a Maya dependency graph node that submits jobs to tractor for processing on the render farm. It is designed to be flexible for TDs and intuitive for artists. ";
	setAttr ".docAttr_FrameRange" -type "string" "Choose the Frame Range Policy. Current Frame, TimeSlider, Custom Range or Custom List.";
	setAttr ".docAttr_CustomFrameRange" -type "string" "Frame Range to be used if frameRange is set to customRange.";
	setAttr ".docAttr_CustomFrameRange0" -type "string" "Start frame of custom range.";
	setAttr ".docAttr_CustomFrameRange1" -type "string" "End frame of custom range.";
	setAttr ".docAttr_CustomFrameRange2" -type "string" "Step frame of custom range.";
	setAttr ".docAttr_CustomFrameList" -type "string" "Frames to be used if frameRange is set to customList.\nThis attribute accepts a frame specification to render an irregular set of frames. The specification is a comma separated list of ranges of the form @<first>-<last>x<step>@. If the @<step>@ parameter is missing, a step of 1 will be assumed. If both the @<last>@ and @<step>@ parameter are missing, a single frame will be assumed. \nSome examples:\n@1,4,15@\n@4,11-56x3@\n@20,10-12@\n@1-3@";
	setAttr ".docAttr_ClumpSize" -type "string" "How many frames in a clump. The number of clumps will be approximately the number of frames divided by clump size.";
	setAttr ".docAttr_FrameRangePolicy" -type "string" "If rendering a regular range of frames, you can specify whether to arrange the frames linearly over the clumps, or to cycle the frame numbers between clumps. When set to cycle, each clump will contain a set of frames spread through the entire range. If your frame range is 1 to 200 for example and you set clump size to 10, clump 1 will export frames 1, 21, 41 … 181. As the render order is dependent on the clump generation, In this way, you get to view early on a sampling of images throughout the range.";
	setAttr ".docAttr_RenderLayers" -type "string" "Set whether to create a job for all active render layers or just the current layer.";
	setAttr ".docAttr_RenderSceneFilename" -type "string" "The path where the Maya scene will be saved.";
	setAttr ".docAttr_TractorSpoolFilename" -type "string" "The path where the alf file will be saved.";
	setAttr ".docAttr_TractorExecutable" -type "string" "Path to the tractor executable that will spool the job in Tractor.";
	setAttr ".docAttr_Priority" -type "string" "Use this field to set the pbias value according to a predefined set of priority levels.";
	setAttr ".docAttr_CustomPriority" -type "string" "Use this field to set the pbias value explicitly.";
	setAttr ".docAttr_JobInit" -type "string" "Global state initializations. See \"Tractor Assign Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#assign";
	setAttr ".docAttr_JobInitKey" -type "string" "Global Initialization Key. See JobInit.";
	setAttr ".docAttr_JobInitValue" -type "string" "Global Initialization Value. See JobInit.";
	setAttr ".docAttr_JobMetadata" -type "string" "A set of Key Value pairs that will resolves to a string that is stored with the job. The metadata contents are studio dependent.";
	setAttr ".docAttr_JobMetadataKey" -type "string" "Key for metadata";
	setAttr ".docAttr_JobMetadataValue" -type "string" "Value for Metadata";
	setAttr ".docAttr_JobTitle" -type "string" "Title of the Tractor Job";
	setAttr ".docAttr_JobAfter" -type "string" "delays the start of job processing until the given time; the job is spooled and placed on the queue, but no processing occurs until after the date specified; dates are of the form {month day hour:minute} or for times later on the same day {hour:minute} (up until midnight). Hours should be given as for a 24-hour clock. For example to delay a job until June 23 at 1:45PM, use: Job -after {6 23 13:45} ...";
	setAttr ".docAttr_JobAtleast" -type "string" "sets the default value for Cmds without an explicit -atleast option.";
	setAttr ".docAttr_JobAtmost" -type "string" "sets the default value for Cmds without an explicit -atmost option.";
	setAttr ".docAttr_JobTags" -type "string" "limit-tags to be added to each Cmd's existing tag list.";
	setAttr ".docAttr_JobService" -type "string" "additional service key expressions to be ANDed with each Cmd's existing service expression.";
	setAttr ".docAttr_JobEnvkey" -type "string" "Specifies an arbitrary key (a name or number) that will be passed to the remote alfserver. This Job option is applied as the default RemoteCmd -envkey option, which may be overridden by individual RemoteCmds. There is a procedure defined in alfserver.ini called AlfEnvConfig that examines the envkey associated with each launch request. Based on the envkey keyword and other properties like user name, AlfEnvConfig can customize environment variables and command parameters prior to launch.";
	setAttr ".docAttr_JobEtalevel" -type "string" "specifies that level n of the Task tree should be used to compute the job's estimated time-to-completion value. The outermost level of Tasks in the job script are level zero, their subtasks are level one, etc. The default estimate is based on the number of remaining level 0 nodes multiplied by the average time required by each completed level 0 node (i.e. all its subtasks and its own Cmds).";
	setAttr ".docAttr_JobComment" -type "string" "specifies an arbitrary string that is stored with the job, it is primarily used to include script-generator version information.";
	setAttr ".docAttr_JobEditpolicy" -type "string" "specifies the name of one of the JobEditAccessPolicies defined in crews.config, the policy named \"defaultPolicy\" is used by default. Note that, in a concession to the realities of production wrangling, if a job specifies a policy name here that has not yet been defined in crews.config, then a fallback to \"defaultPolicy\" occurs for that job, until the policy has been defined and crews.config is reloaded by the engine. In all cases, users in the \"Administrators\" crew have full job edit access.";
	setAttr ".docAttr_JobCleanup" -type "string" "a list of Cmds to execute on job termination. See \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_ClumpTaskTitle" -type "string" "Clump label text in the user-interface.";
	setAttr ".docAttr_JobPostscript" -type "string" "The optional \"-postscript\" clause can be added to the main \"Job\" keyword in a job script. This clause adds Cmds or RemoteCmds that will be executed after those in the main job proper, in the following situations: (1) after the main job tasks have all completed successfully; and (2) after the main job stalled due to errors, when all remaining tasks are blocked by errors in subtasks. These commands are similar in spirit to the \"finally\" block in a python exception handler. This clause is structured just like the \"-cmds\" block in task: See \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_ClumpTaskChaser" -type "string" "an external application launched by users from the UI, typically to view a rendered frame; the UI activates the launching button only after the regular commands for this task have completed successfully. When a web browser is connected, there is special handling of chaser commands. Specifically, the named \"imagefile\" will be made available for downloading to the browser. See the site configuration file for details on enabling this feature (httpImgRoot).";
	setAttr ".docAttr_ClumpTaskPreview" -type "string" "launches an external application in the same manner as -chaser, however this version activates the launching button before the commands have completed.";
	setAttr ".docAttr_ClumpTaskService" -type "string" "under certain circumstances it is useful to check-out a remote server for the duration of an entire Task (its subtasks and commands); a service specification as a Task option accomplishes this; the server is checked back in when the Task completes or when there are no more references to the Task's associated id.";
	setAttr ".docAttr_ClumpTaskId" -type "string" "an identifier for a task; used by Cmds for runtime substitution of task-level (shared) remote server names; the id is valid in the scope of this Task (i.e. its subtasks and commands).";
	setAttr ".docAttr_ClumpTaskSerialsubtasks" -type "string" (
		"This option alters the parallel execution strategy applied to this task's dependent child tasks, i.e. those specified within this task's \"-subtasks {...}\" block. Under normal circumstances it is desirable to allow tasks that are siblings to execute in parallel; that is: a task and all of its subtasks are presumed to be independent of its siblings, and thus the sub-trees rooted in the siblings can be dispatched in parallel. The \"-serialsubtasks\" option allows this scheme to be changed such that the top-level sibling tasks in a \"-subtasks {...}\" block are treated in a chain-like sense: each task waits for its prior sibling to complete before evaluating its own deeper subtasks and commands. This scheduling variant can simplify the scripting of jobs that have distinct sequential \"chapters\" or phases of actions; such as \"prepare,\" \"render,\" and \"clean-up\" phases, each of which consists of several subtasks. This sort of thing can also be accomplished with elaborate Instance node references, or sometimes with deep linear nesting of single \"phase\" tasks with lots of Cmds. The serialsubtasks serialization is only applied to the immediate child tasks in this task's \"-subtasks {...}\" block; nested child tasks below them are scheduled in parallel as usual, unless another \"-serialsubtasks\" is specified farther down the hierarchy.");
	setAttr ".docAttr_ClumpTaskCmd" -type "string" "Cmd operators associated with this Clump Task; they are launched in sequence (as resources become available) after all subtasks are completed successfully; if a Task is simply a grouping node it won't need to define any commands.\nSee \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_ClumpTaskCleanup" -type "string" "clean-up Cmd operators for clumps; these commands are executed after those in the -cmds section (if any), or during job deletion if this node was on an active path (some child had dispatched a command); they also run during task-restart if this task or one of its parents had a Task-level (shared) server checked out.\nSee \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_FrameTaskTitle" -type "string" "Frame label text in the user-interface.";
	setAttr ".docAttr_FrameTaskChaser" -type "string" "an external application launched by users from the UI, typically to view a rendered frame; the UI activates the launching button only after the regular commands for this task have completed successfully. When a web browser is connected, there is special handling of chaser commands. Specifically, the named \"imagefile\" will be made available for downloading to the browser. See the site configuration file for details on enabling this feature (httpImgRoot).";
	setAttr ".docAttr_FrameTaskPreview" -type "string" "launches an external application in the same manner as -chaser, however this version activates the launching button before the commands have completed.";
	setAttr ".docAttr_FrameTaskService" -type "string" "under certain circumstances it is useful to check-out a remote server for the duration of an entire Task (its subtasks and commands); a service specification as a Task option accomplishes this; the server is checked back in when the Task completes or when there are no more references to the Task's associated id.";
	setAttr ".docAttr_FrameTaskId" -type "string" "an identifier for a task; used by Cmds for runtime substitution of task-level (shared) remote server names; the id is valid in the scope of this Task (i.e. its subtasks and commands).";
	setAttr ".docAttr_FrameTaskSerialsubtasks" -type "string" (
		"This option alters the parallel execution strategy applied to this task's dependent child tasks, i.e. those specified within this task's \"-subtasks {...}\" block. Under normal circumstances it is desirable to allow tasks that are siblings to execute in parallel; that is: a task and all of its subtasks are presumed to be independent of its siblings, and thus the sub-trees rooted in the siblings can be dispatched in parallel. The \"-serialsubtasks\" option allows this scheme to be changed such that the top-level sibling tasks in a \"-subtasks {...}\" block are treated in a chain-like sense: each task waits for its prior sibling to complete before evaluating its own deeper subtasks and commands. This scheduling variant can simplify the scripting of jobs that have distinct sequential \"chapters\" or phases of actions; such as \"prepare,\" \"render,\" and \"clean-up\" phases, each of which consists of several subtasks. This sort of thing can also be accomplished with elaborate Instance node references, or sometimes with deep linear nesting of single \"phase\" tasks with lots of Cmds. The serialsubtasks serialization is only applied to the immediate child tasks in this task's \"-subtasks {...}\" block; nested child tasks below them are scheduled in parallel as usual, unless another \"-serialsubtasks\" is specified farther down the hierarchy.");
	setAttr ".docAttr_FrameTaskCmd" -type "string" "Cmd operators associated with this Frame Task; they are launched in sequence (as resources become available) after all subtasks are completed successfully; if a Task is simply a grouping node it won't need to define any commands.\nSee \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_FrameTaskCleanup" -type "string" "clean-up Cmd operators for frames; these commands are executed after those in the -cmds section (if any), or during job deletion if this node was on an active path (some child had dispatched a command); they also run during task-restart if this task or one of its parents had a Task-level (shared) server checked out.\nSee \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_PrepTaskTitle" -type "string" "Prep label text in the user-interface.";
	setAttr ".docAttr_PrepTaskChaser" -type "string" "an external application launched by users from the UI, typically to view a rendered frame; the UI activates the launching button only after the regular commands for this task have completed successfully. When a web browser is connected, there is special handling of chaser commands. Specifically, the named \"imagefile\" will be made available for downloading to the browser. See the site configuration file for details on enabling this feature (httpImgRoot).";
	setAttr ".docAttr_PrepTaskPreview" -type "string" "launches an external application in the same manner as -chaser, however this version activates the launching button before the commands have completed.";
	setAttr ".docAttr_PrepTaskService" -type "string" "under certain circumstances it is useful to check-out a remote server for the duration of an entire Task (its subtasks and commands); a service specification as a Task option accomplishes this; the server is checked back in when the Task completes or when there are no more references to the Task's associated id.";
	setAttr ".docAttr_PrepTaskId" -type "string" "an identifier for a task; used by Cmds for runtime substitution of task-level (shared) remote server names; the id is valid in the scope of this Task (i.e. its subtasks and commands).";
	setAttr ".docAttr_PrepTaskSerialsubtasks" -type "string" (
		"This option alters the parallel execution strategy applied to this task's dependent child tasks, i.e. those specified within this task's \"-subtasks {...}\" block. Under normal circumstances it is desirable to allow tasks that are siblings to execute in parallel; that is: a task and all of its subtasks are presumed to be independent of its siblings, and thus the sub-trees rooted in the siblings can be dispatched in parallel. The \"-serialsubtasks\" option allows this scheme to be changed such that the top-level sibling tasks in a \"-subtasks {...}\" block are treated in a chain-like sense: each task waits for its prior sibling to complete before evaluating its own deeper subtasks and commands. This scheduling variant can simplify the scripting of jobs that have distinct sequential \"chapters\" or phases of actions; such as \"prepare,\" \"render,\" and \"clean-up\" phases, each of which consists of several subtasks. This sort of thing can also be accomplished with elaborate Instance node references, or sometimes with deep linear nesting of single \"phase\" tasks with lots of Cmds. The serialsubtasks serialization is only applied to the immediate child tasks in this task's \"-subtasks {...}\" block; nested child tasks below them are scheduled in parallel as usual, unless another \"-serialsubtasks\" is specified farther down the hierarchy.");
	setAttr ".docAttr_PrepTaskCmd" -type "string" "Cmd operators associated with this Prep Task; they are launched in sequence (as resources become available) after all subtasks are completed successfully; if a Task is simply a grouping node it won't need to define any commands.\nSee \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
	setAttr ".docAttr_PrepTaskCleanup" -type "string" "clean-up Cmd operators for frames; these commands are executed after those in the -cmds section (if any), or during job deletion if this node was on an active path (some child had dispatched a command); they also run during task-restart if this task or one of its parents had a Task-level (shared) server checked out.\nSee \"Tractor Command Documentation\":https://renderman.pixar.com/resources/current/Tractor_1/scriptingOperators.html#cmd for command options.";
select -ne :time1;
	setAttr ".ihi" 0;
	setAttr ".o" 22;
	setAttr ".unw" 22;
select -ne :renderPartition;
	setAttr -s 13 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 17 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 3 ".u";
select -ne :defaultRenderingList1;
	setAttr -s 2 ".r";
select -ne :lightList1;
	setAttr -s 2 ".l";
select -ne :defaultTextureList1;
	setAttr -s 2 ".tx";
select -ne :initialShadingGroup;
	setAttr -s 66 ".dsm";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultRenderGlobals;
	setAttr ".ren" -type "string" "arnold";
	setAttr ".outf" 51;
	setAttr ".imfkey" -type "string" "exr";
	setAttr ".an" yes;
	setAttr ".pff" yes;
	setAttr ".ifp" -type "string" "<Scene>/<RenderLayer>/<RenderPass>/<Scene>_<RenderLayer>.<RenderPass>";
select -ne :defaultResolution;
	setAttr ".pa" 1;
	setAttr ".al" yes;
	setAttr ".dar" 1.7769999504089355;
select -ne :defaultLightSet;
	setAttr -s 2 ".dsm";
select -ne :defaultViewColorManager;
	setAttr ".ip" 4;
	setAttr ".dp" 5;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".aoam" 0.72307693958282471;
	setAttr ".aora" 26;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
select -ne :ikSystem;
	setAttr -s 4 ".sol";
connectAttr ":time1.o" ":defaultClapper.ct";
// End of renderDispatcher.ma
