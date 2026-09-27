package datacloud.hadoop.lastfm;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

/**
 * Orchestrates the 3-job Last.fm pipeline: job 1 and job 2 run independently against the raw
 * input and write intermediate SequenceFile output; job 3 must read both intermediate outputs
 * through two different mappers (via MultipleInputs) and merge them -- see the lab text for
 * how to use MultipleInputs.addInputPath.
 */
public class LastFmStats {

  public static void main(String[] args) throws IOException, ClassNotFoundException,
      InterruptedException {
    Configuration conf = new Configuration();
    conf.setBoolean("mapreduce.map.speculative", false);
    conf.setBoolean("mapreduce.reduce.speculative", false);
    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    if (otherArgs.length != 2) {
      System.err.println("Usage: LastFmStats <in> <out>");
      System.exit(2);
    }

    final FileSystem fs = FileSystem.get(conf);
    final Path outDir1 = new Path("/tmp-out-job1");
    final Path outDir2 = new Path("/tmp-out-job2");

    Job job1 = Job.getInstance(conf, "LastFmStats job1: distinct listeners per track");
    job1.setJarByClass(LastFmStats.class);
    job1.setMapperClass(NbListenerMapper.class);
    job1.setReducerClass(NbListenerReducer.class);
    job1.setMapOutputKeyClass(Text.class);
    job1.setMapOutputValueClass(Text.class);
    job1.setOutputKeyClass(Text.class);
    job1.setOutputValueClass(IntWritable.class);
    job1.setInputFormatClass(TextInputFormat.class);
    job1.setOutputFormatClass(SequenceFileOutputFormat.class);
    job1.setNumReduceTasks(3);
    FileInputFormat.addInputPath(job1, new Path(otherArgs[0]));
    FileOutputFormat.setOutputPath(job1, outDir1);
    if (fs.exists(outDir1)) {
      fs.delete(outDir1, true);
    }

    Job job2 = Job.getInstance(conf, "LastFmStats job2: listening and skip counts per track");
    job2.setJarByClass(LastFmStats.class);
    job2.setMapperClass(ListeningAndSkipsMapper.class);
    job2.setReducerClass(ListeningAndSkipsReducer.class);
    job2.setMapOutputKeyClass(Text.class);
    job2.setMapOutputValueClass(CoupleIntWritable.class);
    job2.setOutputKeyClass(Text.class);
    job2.setOutputValueClass(CoupleIntWritable.class);
    job2.setInputFormatClass(TextInputFormat.class);
    job2.setOutputFormatClass(SequenceFileOutputFormat.class);
    job2.setNumReduceTasks(3);
    FileInputFormat.addInputPath(job2, new Path(otherArgs[0]));
    FileOutputFormat.setOutputPath(job2, outDir2);
    if (fs.exists(outDir2)) {
      fs.delete(outDir2, true);
    }

    job1.waitForCompletion(true);
    job2.waitForCompletion(true);
    // Each intermediate output directory carries a _SUCCESS marker; MultipleInputs' input
    // format would otherwise try (and fail) to read it as a SequenceFile in job 3.
    fs.delete(new Path(outDir1, "_SUCCESS"), false);
    fs.delete(new Path(outDir2, "_SUCCESS"), false);

    // TODO: build job3. Use MultipleInputs.addInputPath(job3, outDir1,
    // SequenceFileInputFormat.class, MergeListenerMapper.class) and the equivalent call for
    // outDir2/MergeListeningAndSkipsMapper, set the reducer to MergeReducer, the map output
    // types to (Text, TripleIntWritable), the final output types to (Text, Text), a
    // TextOutputFormat, 1 reduce task, and the output path from otherArgs[1]. Then run it and
    // return its exit code via System.exit(...).
  }
}
