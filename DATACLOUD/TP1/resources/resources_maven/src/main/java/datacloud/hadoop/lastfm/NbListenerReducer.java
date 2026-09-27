package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

/** Job 1 reducer: counts the distinct listeners of each track. */
public class NbListenerReducer extends Reducer<Text, Text, Text, IntWritable> {

  @Override
  protected void reduce(Text track, Iterable<Text> users, Context context)
      throws IOException, InterruptedException {
    // TODO: deduplicate the listened-to userIds and emit their count.
  }
}
