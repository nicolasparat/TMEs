package datacloud.hadoop.noodle;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class NoodleByMonthReducer extends Reducer<TimeSlotWithMonth, Text, Text, Text> {

  @Override
  public void reduce(TimeSlotWithMonth slot, Iterable<Text> requests, Context context)
      throws IOException, InterruptedException {
    // TODO: find the most frequent keyword across all requests in this slot, and count
    // the total number of requests, then emit (slot, "<bestKeyword> <numRequests>").
  }
}
