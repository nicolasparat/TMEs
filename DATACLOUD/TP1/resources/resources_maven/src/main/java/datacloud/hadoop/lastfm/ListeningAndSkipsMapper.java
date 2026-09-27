package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

/**
 * Job 2 mapper. Input lines have the format: "userId trackId localListening radioListening skip"
 */
public class ListeningAndSkipsMapper extends Mapper<LongWritable, Text, Text, CoupleIntWritable> {

  @Override
  protected void map(LongWritable key, Text line, Context context)
      throws IOException, InterruptedException {
    // TODO: emit (trackId, (localListening + radioListening, skip)) for every line.
  }
}
