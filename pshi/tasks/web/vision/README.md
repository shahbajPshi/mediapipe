# MediaPipe Tasks Vision Package

This package contains the vision tasks for MediaPipe.

## Pose Landmarker

The MediaPipe Pose Landmarker task lets you detect the landmarks of body poses
in an image. You can use this Task to localize key points of a pose and render
visual effects over the body.

```
const vision = await FilesetResolver.forVisionTasks(
    "https://cdn.jsdelivr.net/npm/@mediapipe/tasks-vision/wasm"
);
const poseLandmarker = await PoseLandmarker.createFromModelPath(vision,
    "https://storage.googleapis.com/mediapipe-models/pose_landmarker/pose_landmarker_lite/float16/1/pose_landmarker_lite.task
);
const image = document.getElementById("image") as HTMLImageElement;
const landmarks = poseLandmarker.detect(image);
```

For more information, refer to the [Pose Landmarker](https://developers.google.com/mediapipe/solutions/vision/pose_landmarker/web_js) documentation.
