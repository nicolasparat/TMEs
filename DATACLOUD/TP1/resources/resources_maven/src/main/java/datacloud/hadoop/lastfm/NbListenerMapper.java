package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

/**
 * Job 1 mapper. Input lines have the format: "userId trackId localListening radioListening skip"
 */
public class NbListenerMapper extends Mapper<LongWritable, Text, Text, Text> {

  @Override
  protected void map(LongWritable key, Text line, Context context)
      throws IOException, InterruptedException {
    // TODO: emit (trackId, userId) for every listen/radio event where the user listened at
    // least once, so the reducer can count distinct listeners per track.
  }
}
