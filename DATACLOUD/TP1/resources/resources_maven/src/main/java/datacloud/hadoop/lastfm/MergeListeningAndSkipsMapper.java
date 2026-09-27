package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

/** Job 3, reads job 2's output (trackId -> (numListening, numSkips)). */
public class MergeListeningAndSkipsMapper
    extends Mapper<Text, CoupleIntWritable, Text, TripleIntWritable> {

  @Override
  protected void map(Text track, CoupleIntWritable listeningAndSkips, Context context)
      throws IOException, InterruptedException {
    // TODO: emit (track, TripleIntWritable(0, numListening, numSkips)).
  }
}
