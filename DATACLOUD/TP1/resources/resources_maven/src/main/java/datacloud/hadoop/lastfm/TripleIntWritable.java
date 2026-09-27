package datacloud.hadoop.lastfm;

import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.IntWritable;

/**
 * A fixed-size array of 3 ints: (numListener, numListening, numSkips). Used by job 3 so that
 * both of its mappers (one per input job) can emit the same value shape, with the fields they
 * don't have padded to zero, ready to be summed componentwise by the reducer.
 */
public class TripleIntWritable extends ArrayWritable {

  public TripleIntWritable() {
    super(IntWritable.class);
  }

  public TripleIntWritable(int numListener, int numListening, int numSkips) {
    super(IntWritable.class);
    set(new IntWritable[] {
        new IntWritable(numListener), new IntWritable(numListening), new IntWritable(numSkips)
    });
  }

  public int numListener() {
    return ((IntWritable) get()[0]).get();
  }

  public int numListening() {
    return ((IntWritable) get()[1]).get();
  }

  public int numSkips() {
    return ((IntWritable) get()[2]).get();
  }
}
