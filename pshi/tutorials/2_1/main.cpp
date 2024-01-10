// Desktop: A main function to run a MediaPipe graph.
// bazel run --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/pshi/tutorials/2_1:2_1 --  --calculator_graph_config_file=//mediapipe/pshi/tutorials/2_1/graph.pbtxt
//
// bazel build --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/pshi/tutorials/2_1:2_1
// bazel-bin/mediapipe/pshi/tutorials/2_1/2_1 '--calculator_graph_config_file=mediapipe/pshi/tutorials/2_1/graph.pbtxt'
#include <cstdlib>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/absl_log.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/resource_util.h"

using namespace mediapipe;

constexpr char kInputStream[] = "input_video";
constexpr char kOutputStream[] = "output_video";
constexpr char kWindowName[] = "MediaPipe";

ABSL_FLAG(std::string, calculator_graph_config_file, "",
          "Name of file containing text format CalculatorGraphConfig proto.");
ABSL_FLAG(std::string, input_video_path, "",
          "Full path of video to load. "
          "If not provided, attempt to use a webcam.");
ABSL_FLAG(std::string, output_video_path, "",
          "Full path of where to save result (.mp4 only). "
          "If not provided, show result in a window.");

CalculatorGraphConfig BuildGraph() {
    Graph graph;

    string = R"
    input_stream: "float_value"
    input_stream: "int_value"
    input_stream: "bool_value"

    output_stream: "passed_float_value"
    output_stream: "passed_int_value"
    output_stream: "passed_bool_value"

    node {
            calculator: "PassThroughCalculator"
            input_stream: "float_value"
            input_stream: "int_value"
            input_stream: "bool_value"
# The order must be the same as for inputs (or you can use explicit indexes)
            output_stream: "passed_float_value"
            output_stream: "passed_int_value"
            output_stream: "passed_bool_value"
    }"

    // Graph inputs.
    Stream<std::vector<Tensor>>
}

Status RunMPPGraph() {
    std::string calculator_graph_config_contents;
    MP_RETURN_IF_ERROR(file::GetContents(absl::GetFlag(FLAGS_calculator_graph_config_file), &calculator_graph_config_contents));
    ABSL_LOG(INFO) << "Get calculator graph config contents: " << calculator_graph_config_contents;
    CalculatorGraphConfig config = ParseTextProtoOrDie<CalculatorGraphConfig>(calculator_graph_config_contents);

    ABSL_LOG(INFO) << "Initialize the calculator graph.";
    CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config));

    ABSL_LOG(INFO) << "Initialize the camera or load the video";
    cv::VideoCapture capture;
    const bool load_video = !absl::GetFlag(FLAGS_input_video_path).empty();
    if (load_video) {
        capture.open(absl::GetFlag(FLAGS_input_video_path));
    } else {
        // open first one
        capture.open(0);
    }
    RET_CHECK(capture.isOpened());

    cv::VideoWriter writer;
    const bool save_video = !absl::GetFlag(FLAGS_output_video_path).empty();
    if (!save_video) {
        cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
#if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
        capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        capture.set(cv::CAP_PROP_FPS, 30);
#endif
    }

    ABSL_LOG(INFO) << "Start running the calculator graph.";
    // Assuming output steam name is kOutputStream
    MP_ASSIGN_OR_RETURN(OutputStreamPoller poller, graph.AddOutputStreamPoller(kOutputStream));
    MP_RETURN_IF_ERROR(graph.StartRun({}));

    ABSL_LOG(INFO) << "Start grabbing and processing frames.";
    bool grab_frames = true;
    while (grab_frames) {
        // Capture opencv camera or video frame.
        cv::Mat camera_frame_raw;
        capture >> camera_frame_raw;
        if (camera_frame_raw.empty()) {
            if (!load_video) {
                ABSL_LOG(INFO) << "Ignore empty frame from camera.";
                continue;
            }
            ABSL_LOG(INFO) << "Empty frame, end of video reached.";
            break;
        }
        cv::Mat camera_frame;
        cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
        if (!load_video) {
            // assuming selfie mode
            cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);
        }

        // Wrap Mat into an ImageFrame
        auto input_frame = absl::make_unique<ImageFrame>(
                ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
                ImageFrame::kDefaultAlignmentBoundary);
        cv::Mat input_frame_mat = formats::MatView(input_frame.get());
        camera_frame.copyTo(input_frame_mat);

        // Send image packet into the graph
        size_t frame_timestamp_us = (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;
        // assuming name of graph input is kInputStream
        MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(kInputStream, Adopt(input_frame.release()).At(Timestamp(frame_timestamp_us))));

        // Get the graph result packet, or stop if that fails.
        Packet packet;
        if (!poller.Next(&packet)) break;
        auto& output_frame = packet.Get<ImageFrame>();

        // Convert back to opencv for display or saving.
        cv::Mat output_frame_mat = formats::MatView(&output_frame);
        cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);
        if (save_video) {
            if (!writer.isOpened()) {
                ABSL_LOG(INFO) << "Prepare video writer.";
                writer.open(absl::GetFlag(FLAGS_output_video_path), fourcc('a', 'v', 'c', '1'), // .mp4
                            capture.get(cv::CAP_PROP_FPS), output_frame_mat.size());
                RET_CHECK(writer.isOpened());
            }
            writer.write(output_frame_mat);
        } else {
            cv::imshow(kWindowName, output_frame_mat);
            // Press ESC key to exit.
            const int pressed_key = cv::waitKey(5);
            if (pressed_key == 27) grab_frames = false;
        }
    }

    ABSL_LOG(INFO) << "Shutting down.";
    if (writer.isOpened()) writer.release();
    MP_RETURN_IF_ERROR(graph.CloseInputStream(kInputStream));
    return graph.WaitUntilDone();
}

//==============================================================================
int main(int argc, char** argv){
    google::InitGoogleLogging(argv[0]);
    absl::ParseCommandLine(argc, argv);
    Status run_status = RunMPPGraph();
    if (!run_status.ok()) {
        ABSL_LOG(ERROR) << "Failed to run the graph: " << run_status.message();
        return EXIT_FAILURE;
    } else {
        ABSL_LOG(INFO) << "Success!";
    }
    return EXIT_SUCCESS;
}