package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

/** Job 3 reducer: sums the two mappers' triples componentwise. */
public class MergeReducer extends Reducer<Text, TripleIntWritable, Text, Text> {

  @Override
  protected void reduce(Text track, Iterable<TripleIntWritable> triples, Context context)
      throws IOException, InterruptedException {
    // TODO: sum numListener/numListening/numSkips across all triples and emit
    // "<numListener> <numListening> <numSkips>".
  }
}
