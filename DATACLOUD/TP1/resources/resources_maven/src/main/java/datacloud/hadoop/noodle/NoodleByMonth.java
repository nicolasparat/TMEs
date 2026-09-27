package datacloud.hadoop.noodle;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class NoodleByMonth {

  public static void main(String[] args) throws IOException, ClassNotFoundException,
      InterruptedException {
    Configuration conf = new Configuration();
    conf.setBoolean("mapreduce.map.speculative", false);
    conf.setBoolean("mapreduce.reduce.speculative", false);
    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    if (otherArgs.length != 2) {
      System.err.println("Usage: NoodleByMonth <in> <out>");
      System.exit(2);
    }
    Job job = Job.getInstance(conf, "NoodleByMonth");
    job.setJarByClass(NoodleByMonth.class);
    job.setMapperClass(NoodleByMonthMapper.class);
    job.setReducerClass(NoodleByMonthReducer.class);
    job.setMapOutputKeyClass(TimeSlotWithMonth.class);
    job.setMapOutputValueClass(Text.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
    job.setInputFormatClass(TextInputFormat.class);
    job.setOutputFormatClass(TextOutputFormat.class);
    // TODO: set the partitioner class and the number of reduce tasks (one per month).

    FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
    final Path outDir = new Path(otherArgs[1]);
    FileOutputFormat.setOutputPath(job, outDir);
    final FileSystem fs = FileSystem.get(conf);
    if (fs.exists(outDir)) {
      fs.delete(outDir, true);
    }

    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
