load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_file")

# buildifier: disable=unnamed-macro
def external_pshi_files():
    http_file(
        name = "com_pshi_pose_detection_tflite",
        sha256 = "9ba9dd3d42efaaba86b4ff0122b06f29c4122e756b329d89dca1e297fd8f866c",
        urls = ["https://storage.googleapis.com/mediapipe-assets/pose_detection.tflite?generation=1678737489600422"],
    )

    http_file(
        name = "com_pshi_pose_landmark_full_tflite",
        sha256 = "e9a5c5cb17f736fafd4c2ec1da3b3d331d6edbe8a0d32395855aeb2cdfd64b9f",
        urls = ["https://storage.googleapis.com/mediapipe-assets/pose_landmark_full.tflite?generation=1661875894245786"],
    )

    http_file(
        name = "com_pshi_pose_landmark_heavy_tflite",
        sha256 = "59e42d71bcd44cbdbabc419f0ff76686595fd265419566bd4009ef703ea8e1fe",
        urls = ["https://storage.googleapis.com/mediapipe-assets/pose_landmark_heavy.tflite?generation=1661875897944151"],
    )

    http_file(
        name = "com_pshi_pose_landmark_lite_tflite",
        sha256 = "1150dc68a713b80660b90ef46ce4e85c1c781bb88b6e3512cc64e6a685ba5588",
        urls = ["https://storage.googleapis.com/mediapipe-assets/pose_landmark_lite.tflite?generation=1681244252454799"],
    )
