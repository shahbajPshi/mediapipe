load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_simple_subgraph",
)

package(default_visibility = ["//visibility:public"])

mediapipe_simple_subgraph(
    name = "pose_renderer_cpu",
    graph = "pose_renderer_cpu.pbtxt",
    register_as = "PoseRendererCpu",
    deps = [
        ":pose_landmarks_to_render_data",
        "//mediapipe/calculators/image:image_properties_calculator",
        "//mediapipe/calculators/image:recolor_calculator",
        "//mediapipe/calculators/util:annotation_overlay_calculator",
        "//mediapipe/calculators/util:detections_to_render_data_calculator",
        "//mediapipe/calculators/util:rect_to_render_data_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "pose_landmarks_to_render_data",
    graph = "pose_landmarks_to_render_data.pbtxt",
    register_as = "PoseLandmarksToRenderData",
    deps = [
        "//mediapipe/calculators/core:concatenate_vector_calculator",
        "//mediapipe/calculators/core:split_proto_list_calculator",
        "//mediapipe/calculators/util:landmarks_to_render_data_calculator",
        "//mediapipe/calculators/util:rect_to_render_scale_calculator",
    ],
)
