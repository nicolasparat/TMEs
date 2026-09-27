package datacloud.hadoop.noodle;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

/**
 * Should route every key to the reduce task dedicated to its month, so that each of the 12
 * reduce tasks produces exactly one output file for exactly one month.
 */
public class NoodleByMonthPartitioner extends Partitioner<TimeSlotWithMonth, Text> {

  @Override
  public int getPartition(TimeSlotWithMonth slot, Text value, int numReduceTasks) {
    // TODO: return the reduce task id (0-11) for this slot's month.
    return 0;
  }
}
