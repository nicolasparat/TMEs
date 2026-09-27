package datacloud.hadoop.lastfm;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

/** A pair of ints, used as job 2's output value: (numListening, numSkips). */
public class CoupleIntWritable implements Writable {

  private int v1, v2;

  public CoupleIntWritable(int v1, int v2) {
    this.v1 = v1;
    this.v2 = v2;
  }

  public CoupleIntWritable() {
  }

  public int v1() {
    return v1;
  }

  public int v2() {
    return v2;
  }

  @Override
  public void readFields(DataInput in) throws IOException {
    v1 = in.readInt();
    v2 = in.readInt();
  }

  @Override
  public void write(DataOutput out) throws IOException {
    out.writeInt(v1);
    out.writeInt(v2);
  }
}
