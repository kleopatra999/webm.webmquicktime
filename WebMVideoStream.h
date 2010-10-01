// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.


#ifndef _WEBM_VIDEO_STREAM_H_
#define _WEBM_VIDEO_STREAM_H_ 1
//helper functions
ComponentResult openDecompressionSession(VideoStreamPtr si);
ComponentResult openCompressionSession(WebMExportGlobalsPtr globals, VideoStreamPtr si);

ComponentResult compressNextFrame(WebMExportGlobalsPtr globals, VideoStreamPtr si);
ComponentResult initVideoStream(VideoStreamPtr vs);
ComponentResult startPass(VideoStreamPtr vs,int pass);
ComponentResult endPass(VideoStreamPtr vs);
#endif