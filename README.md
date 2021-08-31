# GoPro GPMF format Reader/Parser/Writer
[GPMF structure](https://github.com/gopro/gpmf-parser#gpmf-introduction) (link to GPMF structure document)

## - Finding the metadata track that has the GPMF data
Find the track's hdlr atom and check if the componentSubType = "meta". There can be several tracks with meta data. GPMF is meta data.
* __/moov/trak/mdia/hdlr__

From that same track find the stsd atom. It will contain a sample description table where the entry should contain dataFormat = 'gpmd'
* __/moov/trak/mdia/minf/stbl/stsd__

## - Sample retrieval proces from the GPMF track
Samples are read from the track in chunks of samples. The MP4 atom containing the chunk offsets table for that track that points into the MP4 file where those chunks start. One of the following 2 chunk offsets tables will be present depending on the size of the video. We will call a chunk from now on a __payload__.
* __/moov/trak/mdia/minf/stbl/stco__ (32 bit values)
* __/moov/trak/mdia/minf/stbl/co64__ (64 bit values)

There is also an MP4 chunk size table with the same amount of entries as the chunk offset table. You could retrieve that also of you want to. But with good coding it's not really needed. And ... I don't need it :)
* __/moov/trak/mdia/minf/stbl/stsz__

## - Handling if the GPMF payload

Check this link for a full [GPMF data structure reference](https://github.com/gopro/gpmf-parser#gpmf-introduction). The GPMF payload exists of klv blocks that are nested with each other to get all the needed meta data samples. Each data type has a stream (STRM) that includes a table of the samples for that payload, the scaling factor, the units type and more data. The values are presented in itegers that need to be devided by the scalar integer value to get a precise float value.

## - Timing of the GPMF payload

In progress ...

