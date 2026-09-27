package datacloud.hadoop.noodle;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.WritableComparable;

/**
 * A time slot of a given size (in minutes) within a day, tagged with a month.
 * Two slots compare equal (and therefore reach the same reducer) iff they cover the same
 * slot-of-day, regardless of which day within the month they came from.
 */
public class TimeSlotWithMonth implements WritableComparable<TimeSlotWithMonth> {

  private int slotSize; // in minutes
  private int minuteOfDay;
  private int month;

  public TimeSlotWithMonth(int month, int slotSize, int hour, int minute) {
    if (hour > 23 || hour < 0 || minute > 59 || minute < 0 || month > 12 || month < 1) {
      throw new IllegalArgumentException();
    }
    this.slotSize = slotSize;
    this.minuteOfDay = hour * 60 + minute;
    this.month = month;
  }

  public TimeSlotWithMonth() {
  }

  public int getSlotId() {
    return minuteOfDay / slotSize;
  }

  public int getMonth() {
    return month;
  }

  @Override
  public String toString() {
    int id = getSlotId();
    int hourFrom = (id * slotSize) / 60;
    int minuteFrom = (id * slotSize) % 60;
    int hourTo = ((id + 1) * slotSize) / 60;
    int minuteTo = ((id + 1) * slotSize) % 60;
    return "between " + hourFrom + "h" + minuteFrom + " and " + hourTo + "h" + minuteTo;
  }

  @Override
  public void readFields(DataInput in) throws IOException {
    this.minuteOfDay = in.readInt();
    this.slotSize = in.readInt();
    this.month = in.readInt();
  }

  @Override
  public void write(DataOutput out) throws IOException {
    out.writeInt(minuteOfDay);
    out.writeInt(slotSize);
    out.writeInt(month);
  }

  @Override
  public int compareTo(TimeSlotWithMonth other) {
    return this.getSlotId() - other.getSlotId();
  }
}
