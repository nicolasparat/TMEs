package datacloud.hadoop.noodle;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

/**
 * Input lines have the format: "DD_MM_YYYY_HH_MM_SS clientIp keyword1+keyword2+..."
 */
public class NoodleByMonthMapper extends Mapper<LongWritable, Text, TimeSlotWithMonth, Text> {

  public static final int SLOT_SIZE_MINUTES = 30;

  public void map(LongWritable key, Text value, Context context)
      throws IOException, InterruptedException {
    // TODO: parse the line, build a TimeSlotWithMonth from its month/hour/minute,
    // and emit (slot, keywords).
  }
}
