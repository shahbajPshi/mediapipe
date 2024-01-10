load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_simple_subgraph",
)
load(
    "//mediapipe/pshi/tutorials/pose_tracker:link_pshi_files.bzl",
    "link_pshi_files",
)

package(default_visibility = ["//visibility:public"])

link_pshi_files(
    srcs = [
        "pose_detection.tflite",
    ],
)

mediapipe_simple_subgraph(
    name = "pose_detection_cpu",
    graph = "pose_detection_cpu.pbtxt",
    register_as = "PoseDetectionCpu",
    deps = [
        "//mediapipe/calculators/tensor:image_to_tensor_calculator",
        "//mediapipe/calculators/tensor:inference_calculator",
        "//mediapipe/calculators/tensor:tensors_to_detections_calculator",
        "//mediapipe/calculators/tflite:ssd_anchors_calculator",
        "//mediapipe/calculators/util:detection_letterbox_removal_calculator",
        "//mediapipe/calculators/util:non_max_suppression_calculator",
    ],
)
