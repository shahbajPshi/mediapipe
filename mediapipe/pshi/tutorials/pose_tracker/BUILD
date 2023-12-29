#  bazel run --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/pshi/tutorials/pose_tracker:pose_tracking_cpu \
#  -- --calculator_graph_config_file=mediapipe/pshi/tutorials/pose_tracker/pose_tracking_cpu.binarypb
load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_binary_graph",
)

package(default_visibility = ["//visibility:public"])

# graph runner
cc_library(
    name = "demo_run_graph_main",
    srcs = ["demo_run_graph_main.cc"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/util:resource_util",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:absl_log",
    ],
)

# pose tracking graph dependency
cc_library(
    name = "pose_tracking_cpu_deps",
    deps = [
        "//mediapipe/calculators/core:constant_side_packet_calculator",
        "//mediapipe/calculators/core:flow_limiter_calculator",
        "//mediapipe/pshi/tutorials/pose_tracker/pose_landmark:pose_landmark_cpu",
        "//mediapipe/pshi/tutorials/pose_tracker/pose_renderer:pose_renderer_cpu",
    ],
)

# run pose tracking graph
cc_binary(
    name = "pose_tracking_cpu",
    args = [
        "--calculator_graph_config_file=mediapipe/pshi/tutorials/pose_tracker/pose_tracking_cpu.pbtxt",
    ],
    data = [
        "pose_tracking_cpu.pbtxt",
        "//mediapipe/pshi/tutorials/pose_tracker/pose_detection:pose_detection.tflite",
        "//mediapipe/pshi/tutorials/pose_tracker/pose_landmark:pose_landmark_full.tflite",
    ],
    deps = [
        ":demo_run_graph_main",
        ":pose_tracking_cpu_deps",  # all binary need in .pbtxt is mention in this
    ],
)
