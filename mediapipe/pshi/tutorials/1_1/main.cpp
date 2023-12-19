#include <iostream>
#include <string>

// Mediapipe headers we will be using
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

// Running all mediapipe stuff inside a functions which returns mediapipe::Status
// is convenient, as you can use MP macroses like MP_RETURN_IF_ERROR
// mediapipe::Status is absl::Status from abseil under the hood
mediapipe::Status run() {
    using namespace std;
    // First, we have to create MP graph as protobuf text format
    // Here we run stuff through 2 copies of PassThroughCalculator, which
    // does nothing (just lets all packets through)
    string protoG = R"(
    input_stream: "in"
    output_stream: "out"
    node {
        input_stream: "in"
        output_stream: "out1"
        calculator: "PassThroughCalculator"
    }
    node {
        input_stream: "out1"
        output_stream: "out"
        calculator: "PassThroughCalculator"
    }
    )";

    // Next parse this string into a protobuf CalculatorGraphConfig object
    // Note that ParseTextProtoOrDie crashes program on error
    mediapipe::CalculatorGraphConfig config = mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(protoG);
    // Create MP Graph and Initialize it with config
    mediapipe::CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config));
    // This macros returns the status if not OK
    // It is roughly equivalent to
    // {
    //      mediapipe::Status status =  graph.Initialize(config);
    //      if (! sttaus.ok()) return status;
    // }

    // How do you recieve output packages from the graph (stream "out")?
    // Google example uses poller (synchronous logic)
    // I prefer observer (a callback, asynchronous logic)
    // Let's define our callback
    auto cb = [](const mediapipe::Packet &packet)->mediapipe::Status{
        // packet.Get<double>() extracts double number from a packet
        // Get() Exits program if the packet is not double
        // If you want to play it safe, check each packet with methods like
        // IsEmpty() and ValidateAsType<double>()
        cout << "ISEMPTY = " << packet.IsEmpty() << ", VALIDATE = " << packet.ValidateAsType<double>() << endl;
        // We also print the timestamp
        cout << packet.Timestamp() << ": RECEIVED PACKET " << packet.Get<double>() << endl;
        // It is vital to always return OK status, unless you want to stop the graph execution
        return mediapipe::OkStatus();
    };

    // Attach this callback as observer to the graph output stream out
    MP_RETURN_IF_ERROR(graph.ObserveOutputStream("out", cb));

    // Run the graph, it usually starts in parallel threads and waits for input data
    MP_RETURN_IF_ERROR(graph.StartRun({}));

    // Send input packers to the graph, stream "in"
    for (int i=0; i<13; ++i) {
        // Each packet must have a timestamp
        // MP timestamp must go in strictly ascending order
        // They don't have to be true "timestamps since epoch"
        mediapipe::Timestamp ts(i);
        // mediapipe::MakePacket creates a packet (copies or moves data), use mediapipe::Adapt() instead to avoid copy/move
        mediapipe::Packet packet = mediapipe::MakePacket<double>(i*0.1).At(ts);
        // Send this packet to the stream in
        MP_RETURN_IF_ERROR(graph.AddPacketToInputStream("in", packet));
    }

    // Close the input stream: signal MP that no more packets will be sent to "in"
    graph.CloseInputStream("in");

    // Wait for the graph to finish, joins any threads created for this graph
    MP_RETURN_IF_ERROR(graph.WaitUntilDone());
    // If we got this far, we are doing fine
    return mediapipe::OkStatus();
    // These two lines can be replaced by (in simplest cases only, thus I avoid this syntax)
    // return graph.WaitUntilDone();
}

// ===========
int main() {
    using namespace std;
    cout << "Example 1.1 : Simplest mediapipe pipeline ..." << endl;
    // Call run(), which return a status
    mediapipe::Status status = run();
    cout << "status = " << status << endl;
    cout << "status.ok() = " << status.ok() << endl;
    return 0;
}
