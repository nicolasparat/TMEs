package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

/** Job 3, reads job 1's output (trackId -> numListener). */
public class MergeListenerMapper extends Mapper<Text, IntWritable, Text, TripleIntWritable> {

  @Override
  protected void map(Text track, IntWritable numListener, Context context)
      throws IOException, InterruptedException {
    // TODO: emit (track, TripleIntWritable(numListener, 0, 0)).
  }
}
