package datacloud.hadoop.util;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.SequenceFile.CompressionType;
import org.apache.hadoop.io.SequenceFile.Writer;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.util.GenericOptionsParser;

public class SequenceFileCreator {

  public static void main(String[] args) throws IOException {
    Configuration conf = new Configuration();

    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
    if (otherArgs.length != 2) {
      System.err.println("Usage: SequenceFileCreator <hdfs_dest_path> <num_entries>");
      System.exit(2);
    }
    String pathDest = otherArgs[0];
    int numEntries = Integer.parseInt(otherArgs[1]);
    final Path file = new Path(pathDest);
    final SequenceFile.Writer writer = SequenceFile.createWriter(conf, Writer.file(file),
        Writer.keyClass(LongWritable.class), Writer.valueClass(Text.class),
        Writer.compression(CompressionType.NONE));

    try {
      for (int i = 0; i < numEntries; i++) {
        final LongWritable key = new LongWritable((i % 4));
        final Text value = new Text("Value " + i);
        writer.append(key, value);
      }
    } finally {
      writer.close();
    }
  }
}
