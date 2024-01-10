load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_binary_graph",
)

mediapipe_binary_graph(
    name = "graph",
    graph = "graph.pbtxt",
    output_name = "graph.binarypb",
    deps = [
        "//mediapipe/calculators/core:pass_through_calculator",
    ],
)

cc_binary(
    name = "2_1",
    srcs = ["main.cpp"],
    deps = [
        #        "//mediapipe/calculators/core:pass_through_calculator",
        #        "//mediapipe/framework/port:map_util",
        #        "//mediapipe/framework/port:ret_check",
        #        "//mediapipe/framework/port:statusor",
        #        "@com_google_absl//absl/strings",
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
