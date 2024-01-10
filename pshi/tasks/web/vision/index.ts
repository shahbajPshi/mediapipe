/**
 * Copyright 2022 The MediaPipe Authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import {FilesetResolver as FilesetResolverImpl} from '../../web/core/fileset_resolver';
import {DrawingUtils as DrawingUtilsImpl} from './core/drawing_utils';
import {MPImage as MPImageImpl} from './core/image';
import {MPMask as MPMaskImpl} from './core/mask';
import {PoseLandmarker as PoseLandmarkerImpl} from './pose_landmarker/pose_landmarker';

// Declare the variables locally so that Rollup in OSS includes them explicitly
// as exports.
const DrawingUtils = DrawingUtilsImpl;
const FilesetResolver = FilesetResolverImpl;
const MPImage = MPImageImpl;
const MPMask = MPMaskImpl;
const PoseLandmarker = PoseLandmarkerImpl;

export {
  DrawingUtils,
  FilesetResolver,
  MPImage,
  MPMask,
  PoseLandmarker
};
