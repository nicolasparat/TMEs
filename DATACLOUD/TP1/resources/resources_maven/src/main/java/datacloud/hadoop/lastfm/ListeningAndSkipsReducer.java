package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

/** Job 2 reducer: sums listening and skip counts per track. */
public class ListeningAndSkipsReducer extends Reducer<Text, CoupleIntWritable, Text,
    CoupleIntWritable> {

  @Override
  protected void reduce(Text track, Iterable<CoupleIntWritable> couples, Context context)
      throws IOException, InterruptedException {
    // TODO: sum both components of the incoming couples and emit the totals.
  }
}
