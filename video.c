/*
 * Copyright (c) 2001 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * video decoding with libavcodec API example
 *
 * @example decode_video.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>

#include "video.h"
#include "xmalloc.h"

#define INBUF_SIZE 4096

static uint8_t *video_dst_data[4] = {NULL};
static int      video_dst_linesize[4];
static int video_dst_bufsize;
static int refcount = 0;
static int video_frame_count = 0;
static AVFormatContext *fmt_ctx = NULL;
static AVPacket pkt;
static int video_stream_idx = -1;
static AVStream *video_stream = NULL;
static AVCodecContext *video_dec_ctx = NULL;
static int width, height, pix_fmt;

static void video_info(AVFormatContext *fmt_ctx, AVCodecContext *codec_ctx)
{
  if (fmt_ctx != NULL) {
    fprintf(stderr, "Duration [secs]: %f\n", (float)fmt_ctx->duration/AV_TIME_BASE);
    fprintf(stderr, "Bitrate [kb/s] : %f\n", (float)fmt_ctx->bit_rate/1024.0f);
  }
  if (codec_ctx != NULL) {
    fprintf(stderr, "Width          : %d\n", codec_ctx->width);
    fprintf(stderr, "Height         : %d\n", codec_ctx->height);
    fprintf(stderr, "Format         : %d\n", codec_ctx->pix_fmt);
    fprintf(stderr, "Codec          : %d\n", codec_ctx->codec_id);
  }
}

static int registered =0 ;

static int decode_packet(int *got_frame, int cached, FILE *output_file, AVFrame *frame)
{
    int ret = 0;
    int decoded = pkt.size;

    *got_frame = 0;

    if (pkt.stream_index == video_stream_idx) {
        /* decode video frame */
        //printf("video_dec_ctx=%p frame=%p got_frame=%d pkt=%p\n", video_dec_ctx, frame, *got_frame, &pkt);
        ret = avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error decoding video frame (%s)\n", av_err2str(ret));
            return ret;
        }

        if (*got_frame) {

            if (frame->width != width || frame->height != height ||
                frame->format != pix_fmt) {
                /* To handle this change, one could call av_image_alloc again and
                 * decode the following frames into another rawvideo file. */
                fprintf(stderr, "Error: Width, height and pixel format have to be "
                        "constant in a rawvideo file, but the width, height or "
                        "pixel format of the input video changed:\n"
                        "old: width = %d, height = %d, format = %s\n"
                        "new: width = %d, height = %d, format = %s\n",
                        width, height, av_get_pix_fmt_name(pix_fmt),
                        frame->width, frame->height,
                        av_get_pix_fmt_name(frame->format));
                return -1;
            }

            printf("video_frame%s n:%d coded_n:%d\n",
                   cached ? "(cached)" : "",
                   video_frame_count++, frame->coded_picture_number);

            /* copy decoded frame to destination buffer:
             * this is required since rawvideo expects non aligned data */
            av_image_copy(video_dst_data, video_dst_linesize,
                          (const uint8_t **)(frame->data), frame->linesize,
                          pix_fmt, width, height);
            //if (pix_fmt!=PIX_FMT_YUV420P) {
            //  fprintf(stderr, "Unknown pixel format: %d\n", pix_fmt);
            //  exit(-1);
            //}
            unsigned ch = 0;
            u_int8_t *src = video_dst_data[0];
            int total_size = width * height;
            int line_size = video_dst_linesize[0];
            int line_size2 = video_dst_linesize[0] / 2;

            u_int8_t *dest_copy = xmalloc(3 * width * height / 2);
            for(int y=0; y<height; y++) {
              for (int x=0; x<width; x++) {
                 int Y_offset = x+y*line_size;
                 int U_offset = (y/2) * line_size2 + (x/2) + total_size;
                 int V_offset = (y/2) * line_size2 + (x/2) + total_size + total_size/4;
                 float Y = src[Y_offset];
                 float U = src[U_offset];
                 float V = src[V_offset];

                 float rTmp = Y + (1.370705 * (V-128));
                 float gTmp = Y - (0.698001 * (V-128)) - (0.337633 * (U-128));
                 float bTmp = Y + (1.732446 * (U-128));
                 float R = clamp(rTmp, 0, 255);
                 float G = clamp(gTmp, 0, 255);
                 float B = clamp(bTmp, 0, 255);
                 float Iv = 0.3f * R + 0.6f*G + 0.1f*G;
                 R = G = B = Iv;

                 Y = 0.299f * R + 0.587f * G + 0.114f * B;
                 U = 128.0f - 0.169f * R - 0.331f * G + 0.499 * B;
                 V = 128.0f + 0.499f * R - 0.418f * G - 0.0813f * B;
                 
                 Y = clamp(Y, 0, 255);
                 U = clamp(U, 0, 255);
                 V = clamp(V, 0, 255);

                 dest_copy[Y_offset] = (u_int8_t) Y;
                 dest_copy[U_offset] = (u_int8_t) U;
                 dest_copy[V_offset] = (u_int8_t) V;
              }
            }
            /* write to rawvideo file */
            //fwrite(video_dst_data[0], 1, video_dst_bufsize, output_file);
            fwrite(dest_copy, 1, video_dst_bufsize, output_file);
            free(dest_copy);
        }
    }

    /* If we use frame reference counting, we own the data and need
     * to de-reference it when we don't use it anymore */
    if (*got_frame && refcount)
        av_frame_unref(frame);

    return decoded;
}



static int open_codec_context(const char *src_filename, int *stream_idx,
                              AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret;
    AVStream *st;
    AVCodecContext *dec_ctx = NULL;
    AVCodec *dec = NULL;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), src_filename);
        return ret;
    } else {
        *stream_idx = ret;
        st = fmt_ctx->streams[*stream_idx];
        /* find decoder for the stream */
        dec_ctx = st->codec;
        dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }
    return 0;
}



void process_video(const char *filename, const char *video_dst_filename, filter_func_t filter_func) 
{
    int got_frame = 0;
    FILE *video_dst_file = fopen(video_dst_filename, "wb");
    if (!video_dst_file) {
            fprintf(stderr, "Could not open destination file %s\n", video_dst_filename);
            goto end;
    }

    AVFrame *frame;
    refcount = 0;
    video_frame_count = 0;
    video_stream_idx = -1;
    video_dec_ctx = NULL;
    fmt_ctx = NULL;
    width = height = -1;
    video_dst_data[0] = NULL;
    video_dst_data[1] = NULL;
    video_dst_data[2] = NULL;
    video_dst_data[3] = NULL;

    if(!registered) {
      /* register all formats and codecs */
      av_register_all();
      registered = 1;
    }

    /* open input file, and allocate format context */
    if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open source file %s\n", filename);
        exit(1);
    }
    /* retrieve stream information */
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }

    if (open_codec_context(filename, &video_stream_idx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0) {
        video_stream = fmt_ctx->streams[video_stream_idx];
        video_dec_ctx = video_stream->codec;
        /* allocate image where the decoded image will be put */
        int ret = av_image_alloc(video_dst_data, video_dst_linesize,
                             video_dec_ctx->width, video_dec_ctx->height,
                             video_dec_ctx->pix_fmt, 1);
        if (ret < 0) {
           fprintf(stderr, "Could not allocate raw video buffer\n");
           goto end;
        }
        fprintf(stderr, "Image buffer allocated: %d\n", ret);
        video_dst_bufsize = ret;
    }

    /* dump input information to stderr */
    av_dump_format(fmt_ctx, 0, filename, 0);
    video_info(fmt_ctx, video_dec_ctx);

    width = video_dec_ctx->width;
    height = video_dec_ctx->height;
    pix_fmt = video_dec_ctx->pix_fmt;
    frame = av_frame_alloc();
    printf("frame=%p\n", frame);
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        //ret = AVERROR(ENOMEM);
        goto end;
    }

    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    if (video_stream)
        printf("Demuxing video from file '%s' into '%s'\n", filename, video_dst_filename);

    /* read frames from the file */
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        AVPacket orig_pkt = pkt;
        do {
            //printf("frame5=%p\n", frame);
            int ret = decode_packet(&got_frame, 0, video_dst_file, frame);
            if (ret < 0)
                break;
            pkt.data += ret;
            pkt.size -= ret;
        } while (pkt.size > 0);
        av_packet_unref(&orig_pkt);
    }

    /* flush cached frames */
    pkt.data = NULL;
    pkt.size = 0;
    do {
        decode_packet(&got_frame, 1, video_dst_file, frame);
    } while (got_frame);

    printf("Demuxing succeeded.\n");

    if (video_stream) {
        printf("Play the output video file with the command:\n"
               "ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
               av_get_pix_fmt_name(pix_fmt), width, height,
               video_dst_filename);
    }

end:
    avcodec_free_context(&video_dec_ctx);
    //avformat_close_input(&fmt_ctx);
    if (video_dst_file)
        fclose(video_dst_file);
    av_frame_free(&frame);
    av_free(video_dst_data[0]);
}
